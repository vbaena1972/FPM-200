#include "fpm_ble_config.h"
#include <stdio.h>
#include "storage.h"
#include "sensors_runtime.h"
#include "cJSON.h"
#include "esp_log.h"
#include "esp_app_desc.h" // version real del firmware (PROJECT_VER)
#include "esp_netif.h"    // IP viva para el bloque lan de provisión
#include "esp_random.h"   // esp_fill_random para el token LAN
#include "nvs.h"          // token LAN persistente
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>

static const char *TAG_FPM = "fpm_ble_cfg";

/* ================= helpers de unidad (mismos factores que ui_cfg.c) ================= */
/* Internamente el FPM guarda presión en kPa y flujo en L/min. La app edita/muestra
 * en la unidad configurada; convertimos al leer (config_read) y al escribir. */

static float press_to_disp(float kpa, const char *u)
{
    if (!u) return kpa;
    if (strcmp(u, "psi") == 0) return kpa * 0.145038f;
    if (strcmp(u, "bar") == 0) return kpa / 100.0f;
    if (strcmp(u, "mpa") == 0) return kpa / 1000.0f;
    return kpa;
}
static float press_from_disp(float d, const char *u)
{
    if (!u) return d;
    if (strcmp(u, "psi") == 0) return d / 0.145038f;
    if (strcmp(u, "bar") == 0) return d * 100.0f;
    if (strcmp(u, "mpa") == 0) return d * 1000.0f;
    return d;
}
static float flow_to_disp(float lpm, const char *u)
{
    if (!u) return lpm;
    if (strcmp(u, "sccm") == 0) return lpm * 1000.0f;
    if (strcmp(u, "m3h") == 0)  return lpm * 0.06f;
    return lpm;
}
static float flow_from_disp(float d, const char *u)
{
    if (!u) return d;
    if (strcmp(u, "sccm") == 0) return d / 1000.0f;
    if (strcmp(u, "m3h") == 0)  return d / 0.06f;
    return d;
}

/* Unidad FPM (interna, minúscula) -> etiqueta que muestra la app. */
static const char *press_unit_app(const char *u)
{
    if (!u) return "kPa";
    if (strcmp(u, "kpa") == 0) return "kPa";
    if (strcmp(u, "mpa") == 0) return "MPa";
    if (strcmp(u, "psi") == 0) return "psi";
    if (strcmp(u, "bar") == 0) return "bar";
    return u;
}
static const char *flow_unit_app(const char *u)
{
    if (!u) return "L/min";
    if (strcmp(u, "lpm") == 0)  return "L/min";
    if (strcmp(u, "m3h") == 0)  return "m\xC2\xB3/h"; /* m³/h */
    if (strcmp(u, "sccm") == 0) return "CFM";
    return u;
}
/* Etiqueta de la app -> unidad FPM interna (minúscula). */
static void press_unit_fpm(const char *app, char *out, size_t cap)
{
    const char *r = "kpa";
    if (app) {
        if (strcasecmp(app, "psi") == 0) r = "psi";
        else if (strcasecmp(app, "bar") == 0) r = "bar";
        else if (strcasecmp(app, "mpa") == 0) r = "mpa";
        else if (strcasecmp(app, "kpa") == 0) r = "kpa";
    }
    snprintf(out, cap, "%s", r);
}
static void flow_unit_fpm(const char *app, char *out, size_t cap)
{
    const char *r = "lpm";
    if (app) {
        if (strcasecmp(app, "L/min") == 0) r = "lpm";
        else if (strstr(app, "m\xC2\xB3") || strcasecmp(app, "m3/h") == 0) r = "m3h";
        else if (strcasecmp(app, "CFM") == 0) r = "sccm";
    }
    snprintf(out, cap, "%s", r);
}

/* ================= gas (presión) <-> color ================= */
/* Colores GAS_* de ui_theme.h. dark_text=true cuando el fondo es claro (vacío/aire). */
typedef struct { const char *key; uint32_t color; bool dark; } gas_map_t;
static const gas_map_t GASES[] = {
    { "o2",      0x1d9e75, false },
    { "air_med", 0xe0a800, false },
    { "n2o",     0x2b6cb0, false },
    { "vac",     0xdfe3e8, true  },
};
#define GAS_N ((int)(sizeof(GASES)/sizeof(GASES[0])))

static void gas_to_color(const char *key, uint32_t *color, bool *dark)
{
    for (int i = 0; i < GAS_N; i++)
        if (key && strcmp(key, GASES[i].key) == 0) { *color = GASES[i].color; *dark = GASES[i].dark; return; }
    *color = 0x1d9e75; *dark = false; /* o2 por defecto */
}
static const char *color_to_gas(uint32_t color)
{
    for (int i = 0; i < GAS_N; i++)
        if ((color & 0xffffff) == GASES[i].color) return GASES[i].key;
    return "o2";
}

/* Token de acceso LAN (32 hex) persistente en NVS. Igual que MedGuard
 * (http_service): barrera de autenticación del servidor HTTP en la red. Se crea
 * una vez y se conserva; se entrega por BLE y se valida en cada request HTTP. */
void fpm_lan_token(char *out, size_t n)
{
    if (!out || !n) return;
    out[0] = '\0';
    nvs_handle_t h;
    if (nvs_open("httpmon", NVS_READWRITE, &h) != ESP_OK) return;
    char tok[33] = {0};
    size_t len = sizeof tok;
    if (nvs_get_str(h, "token", tok, &len) == ESP_OK && strlen(tok) == 32) {
        nvs_close(h);
        snprintf(out, n, "%s", tok);
        return;
    }
    uint8_t raw[16];
    esp_fill_random(raw, sizeof raw);
    static const char hex[] = "0123456789abcdef";
    for (int i = 0; i < 16; i++) {
        tok[i * 2] = hex[raw[i] >> 4];
        tok[i * 2 + 1] = hex[raw[i] & 0x0F];
    }
    tok[32] = '\0';
    if (nvs_set_str(h, "token", tok) == ESP_OK) nvs_commit(h);
    nvs_close(h);
    snprintf(out, n, "%s", tok);
}

/* ================= Info (característica 1001) ================= */
char *fpm_ble_info_json(void)
{
    const AppConfig *c = appcfg_cache_peek();
    cJSON *o = cJSON_CreateObject();
    /* "model" DEBE contener "axira" para que la app lo clasifique como Axira. */
    cJSON_AddStringToObject(o, "model", (c && c->general.model[0]) ? c->general.model : "Axira");
    cJSON_AddStringToObject(o, "name", (c && c->bt.legacy.name[0]) ? c->bt.legacy.name
                                       : (c && c->general.client[0]) ? c->general.client : "Axira");
    cJSON_AddStringToObject(o, "serial", c ? c->general.serial : "");
    cJSON_AddStringToObject(o, "thing_name", c ? c->general.serial : "");
    cJSON_AddStringToObject(o, "hw", c ? c->general.hw_version : "");
    /* Versión REAL del firmware flasheado (PROJECT_VER), no el campo de config. */
    const esp_app_desc_t *appdesc = esp_app_get_description();
    cJSON_AddStringToObject(o, "fw", appdesc ? appdesc->version : "");
    cJSON_AddStringToObject(o, "hostname", c ? c->eth.hostname : "");
    cJSON_AddNumberToObject(o, "channels", 2);   /* Axira: presión + flujo */
    /* La app lee `enabled_channel_count` (ble_provisioning_screen.dart); se manda
     * junto con `channels` por compatibilidad. */
    cJSON_AddNumberToObject(o, "enabled_channel_count", 2);
    /* Provisión de monitoreo LAN (igual que MedGuard): IP viva + puerto + token +
     * nombre mDNS, para que la app resuelva <serial>.local y consuma el HTTP de
     * solo lectura. La IP es la del netif por defecto (en FPM la red coexiste con
     * BLE, no hay teardown), o 0.0.0.0 si aún no hay enlace. */
    cJSON *lan = cJSON_AddObjectToObject(o, "lan");
    if (lan) {
        char ip[16] = "0.0.0.0";
        esp_netif_t *nif = esp_netif_get_default_netif();
        esp_netif_ip_info_t ii;
        if (nif && esp_netif_get_ip_info(nif, &ii) == ESP_OK && ii.ip.addr) {
            esp_ip4addr_ntoa(&ii.ip, ip, sizeof ip);
        }
        char tok[33];
        fpm_lan_token(tok, sizeof tok);
        cJSON_AddBoolToObject(lan, "enabled", true);
        cJSON_AddStringToObject(lan, "ip", ip);
        cJSON_AddNumberToObject(lan, "port", 80);
        cJSON_AddStringToObject(lan, "token", tok);
        if (c && c->general.serial[0]) {
            char host[64];
            snprintf(host, sizeof host, "%s.local", c->general.serial);
            cJSON_AddStringToObject(lan, "host", host);
            cJSON_AddStringToObject(lan, "id", c->general.serial);
            cJSON_AddStringToObject(lan, "mdns", "_fpm._tcp");
        }
    }
    char *s = cJSON_PrintUnformatted(o);
    cJSON_Delete(o);
    return s;
}

/* ================= config_read (esquema de la app) ================= */
static cJSON *netif_json(bool enabled, const char *ip_mode, const char *ip,
                         const char *mask, const char *gw, const char *dns)
{
    cJSON *n = cJSON_CreateObject();
    cJSON_AddBoolToObject(n, "enabled", enabled);
    cJSON_AddBoolToObject(n, "dhcp", !ip_mode || strcmp(ip_mode, "static") != 0);
    cJSON_AddStringToObject(n, "ip", ip ? ip : "");
    cJSON_AddStringToObject(n, "mask", mask ? mask : "");
    cJSON_AddStringToObject(n, "gateway", gw ? gw : "");
    cJSON_AddStringToObject(n, "dns", dns ? dns : "");
    return n;
}

/* Construye el objeto ChannelConfig (esquema app) para el canal id:
 * 1 = Presión (con gas + unidad), 2 = Flujo (unidad). Axira: sin calibración. */
static cJSON *build_channel_obj(const AppConfig *c, int id)
{
    cJSON *ch = cJSON_CreateObject();
    cJSON_AddNumberToObject(ch, "id", id);
    cJSON_AddBoolToObject(ch, "enabled", true);
    cJSON_AddStringToObject(ch, "location", c->general.info_text);
    cJSON *sc = cJSON_AddObjectToObject(ch, "scale");
    cJSON *am = cJSON_AddObjectToObject(ch, "alarm");
    cJSON *cal = cJSON_AddObjectToObject(ch, "cal");   /* Axira: fija */
    cJSON_AddNumberToObject(cal, "offset_ma", 0);
    cJSON_AddNumberToObject(cal, "gain", 1);
    cJSON_AddStringToObject(cal, "date", "");
    cJSON_AddNumberToObject(am, "hysteresis", 1);
    cJSON_AddNumberToObject(am, "delay_s", 0);
    cJSON_AddNumberToObject(am, "priority", 0);

    if (id == 1) { /* Presión */
        uint32_t gcol; bool gdark; gas_to_color(c->sensors.gas_type, &gcol, &gdark);
        bool p_dec = strcmp(c->sensors.pressure_unit, "bar") == 0 || strcmp(c->sensors.pressure_unit, "mpa") == 0;
        cJSON_AddStringToObject(ch, "name", "Presi\xC3\xB3n");
        cJSON_AddNumberToObject(ch, "sensor_type", 0);
        cJSON_AddStringToObject(ch, "unit", press_unit_app(c->sensors.pressure_unit));
        cJSON_AddNumberToObject(ch, "gas_color", gcol);
        cJSON_AddBoolToObject(ch, "dark_text", gdark);
        cJSON_AddNumberToObject(ch, "decimals", p_dec ? 1 : 0);
        cJSON_AddNumberToObject(sc, "ma4", 0);
        cJSON_AddNumberToObject(sc, "ma20", 0);
        cJSON_AddBoolToObject(am, "lo_enabled", c->sensors.alarm_limits.pressure_min_enabled);
        cJSON_AddNumberToObject(am, "lo_limit", press_to_disp(c->sensors.alarm_limits.pressure_min, c->sensors.pressure_unit));
        cJSON_AddBoolToObject(am, "hi_enabled", c->sensors.alarm_limits.pressure_max_enabled);
        cJSON_AddNumberToObject(am, "hi_limit", press_to_disp(c->sensors.alarm_limits.pressure_max, c->sensors.pressure_unit));
    } else { /* Flujo */
        cJSON_AddStringToObject(ch, "name", "Flujo");
        cJSON_AddNumberToObject(ch, "sensor_type", 4);
        cJSON_AddStringToObject(ch, "unit", flow_unit_app(c->sensors.flow_unit));
        cJSON_AddNumberToObject(ch, "gas_color", 0x0f6e56);
        cJSON_AddBoolToObject(ch, "dark_text", false);
        cJSON_AddNumberToObject(ch, "decimals", 0);
        cJSON_AddNumberToObject(sc, "ma4", 0);
        cJSON_AddNumberToObject(sc, "ma20", c->sensors.flow_fullscale_lpm);
        cJSON_AddBoolToObject(am, "lo_enabled", false);
        cJSON_AddNumberToObject(am, "lo_limit", 0);
        cJSON_AddBoolToObject(am, "hi_enabled", c->sensors.alarm_limits.flow_high_enabled);
        cJSON_AddNumberToObject(am, "hi_limit", flow_to_disp(c->sensors.alarm_limits.flow_high_limit, c->sensors.flow_unit));
    }
    return ch;
}

/* Igual que build_channel_obj pero en esquema PLANO (BleChannelConfig.fromJson de
 * la app): scale/alarma a nivel RAÍZ (range_min/range_max, lo_* / hi_*, hysteresis,
 * delay_s, priority) y solo `cal` anidado. Es el payload de la característica de
 * canal dedicada (1006). La escala se emite válida (min<max) usando los fondos de
 * escala configurables para que la validación de la app (rangeMin<rangeMax) pase. */
static cJSON *build_channel_flat(const AppConfig *c, int id)
{
    cJSON *ch = cJSON_CreateObject();
    cJSON_AddNumberToObject(ch, "id", id);
    cJSON_AddBoolToObject(ch, "enabled", true);
    cJSON_AddStringToObject(ch, "location", c->general.info_text);
    cJSON *cal = cJSON_AddObjectToObject(ch, "cal");   /* Axira: fija */
    cJSON_AddNumberToObject(cal, "offset_ma", 0);
    cJSON_AddNumberToObject(cal, "gain", 1);
    cJSON_AddStringToObject(cal, "date", "");
    cJSON_AddNumberToObject(ch, "hysteresis", 1);
    cJSON_AddNumberToObject(ch, "delay_s", 0);
    cJSON_AddNumberToObject(ch, "priority", 0);

    if (id == 1) { /* Presión */
        uint32_t gcol; bool gdark; gas_to_color(c->sensors.gas_type, &gcol, &gdark);
        bool p_dec = strcmp(c->sensors.pressure_unit, "bar") == 0 || strcmp(c->sensors.pressure_unit, "mpa") == 0;
        cJSON_AddStringToObject(ch, "name", "Presi\xC3\xB3n");
        cJSON_AddNumberToObject(ch, "sensor_type", 0);
        cJSON_AddStringToObject(ch, "unit", press_unit_app(c->sensors.pressure_unit));
        cJSON_AddNumberToObject(ch, "gas_color", gcol);
        cJSON_AddBoolToObject(ch, "dark_text", gdark);
        cJSON_AddNumberToObject(ch, "decimals", p_dec ? 1 : 0);
        cJSON_AddNumberToObject(ch, "range_min", 0);
        cJSON_AddNumberToObject(ch, "range_max", press_to_disp(c->sensors.pressure_fullscale_kpa, c->sensors.pressure_unit));
        cJSON_AddBoolToObject(ch, "lo_enabled", c->sensors.alarm_limits.pressure_min_enabled);
        cJSON_AddNumberToObject(ch, "lo_limit", press_to_disp(c->sensors.alarm_limits.pressure_min, c->sensors.pressure_unit));
        cJSON_AddBoolToObject(ch, "hi_enabled", c->sensors.alarm_limits.pressure_max_enabled);
        cJSON_AddNumberToObject(ch, "hi_limit", press_to_disp(c->sensors.alarm_limits.pressure_max, c->sensors.pressure_unit));
    } else { /* Flujo */
        cJSON_AddStringToObject(ch, "name", "Flujo");
        cJSON_AddNumberToObject(ch, "sensor_type", 4);
        cJSON_AddStringToObject(ch, "unit", flow_unit_app(c->sensors.flow_unit));
        cJSON_AddNumberToObject(ch, "gas_color", 0x0f6e56);
        cJSON_AddBoolToObject(ch, "dark_text", false);
        cJSON_AddNumberToObject(ch, "decimals", 0);
        cJSON_AddNumberToObject(ch, "range_min", 0);
        cJSON_AddNumberToObject(ch, "range_max", flow_to_disp(c->sensors.flow_fullscale_lpm, c->sensors.flow_unit));
        cJSON_AddBoolToObject(ch, "lo_enabled", false);
        cJSON_AddNumberToObject(ch, "lo_limit", 0);
        cJSON_AddBoolToObject(ch, "hi_enabled", c->sensors.alarm_limits.flow_high_enabled);
        cJSON_AddNumberToObject(ch, "hi_limit", flow_to_disp(c->sensors.alarm_limits.flow_high_limit, c->sensors.flow_unit));
    }
    return ch;
}

char *fpm_ble_config_read_json(bool redact)
{
    const AppConfig *c = appcfg_cache_peek();
    if (!c) return NULL;
    cJSON *r = cJSON_CreateObject();
    cJSON_AddNumberToObject(r, "ver", 3);

    /* display */
    cJSON *disp = cJSON_AddObjectToObject(r, "display");
    cJSON_AddNumberToObject(disp, "brightness", c->general.brightness);
    cJSON_AddNumberToObject(disp, "dim_minutes", c->general.dim_minutes);
    cJSON_AddNumberToObject(disp, "session_timeout_minutes", 5);
    cJSON_AddNumberToObject(disp, "theme", strcmp(c->general.theme, "light") == 0 ? 1 : 0);
    cJSON_AddStringToObject(disp, "language", c->general.lang);

    /* time (el FPM solo guarda timezone; el resto va por defecto) */
    cJSON *tm = cJSON_AddObjectToObject(r, "time");
    cJSON_AddBoolToObject(tm, "manual", false);
    cJSON_AddBoolToObject(tm, "dst", false);
    cJSON_AddNumberToObject(tm, "hour_format", 0);
    cJSON_AddNumberToObject(tm, "date_format", 0);
    cJSON_AddStringToObject(tm, "timezone", c->general.timezone);
    cJSON_AddStringToObject(tm, "ntp_server", "");

    /* alarms (el FPM no tiene relés → por defecto) */
    cJSON *al = cJSON_AddObjectToObject(r, "alarms");
    cJSON_AddNumberToObject(al, "volume", c->general.alarm.volume);
    cJSON_AddNumberToObject(al, "reannounce_minutes", c->general.alarm.reannounce_minutes);
    cJSON_AddNumberToObject(al, "max_silence_minutes", c->general.alarm.max_silence_minutes);
    cJSON_AddBoolToObject(al, "relay1_enabled", false);
    cJSON_AddNumberToObject(al, "relay1_condition", 0);
    cJSON_AddNumberToObject(al, "relay1_mode", 0);
    cJSON_AddBoolToObject(al, "relay2_enabled", false);
    cJSON_AddNumberToObject(al, "relay2_condition", 0);
    cJSON_AddNumberToObject(al, "relay2_mode", 0);

    /* network */
    cJSON *net = cJSON_AddObjectToObject(r, "network");
    cJSON_AddItemToObject(net, "ethernet", netif_json(c->eth.enabled, c->eth.ip_mode,
                          c->eth.ip, c->eth.mask, c->eth.gw, c->eth.dns1));
    cJSON_AddItemToObject(net, "wifi", netif_json(c->wifi.enabled, c->wifi.ip_mode,
                          c->wifi.ip, c->wifi.mask, c->wifi.gw, c->wifi.dns1));
    cJSON_AddStringToObject(net, "wifi_ssid", c->wifi.ssid);
    cJSON_AddStringToObject(net, "wifi_password", redact ? "" : c->wifi.password);
    cJSON_AddStringToObject(net, "hostname", c->eth.hostname);

    /* modbus (el FPM no lo tiene) */
    cJSON *mb = cJSON_AddObjectToObject(r, "modbus");
    cJSON_AddBoolToObject(mb, "enabled", false);
    cJSON_AddNumberToObject(mb, "transport", 0);
    cJSON_AddNumberToObject(mb, "tcp_port", 502);
    cJSON_AddNumberToObject(mb, "unit_id", 1);
    cJSON_AddNumberToObject(mb, "slave_id", 1);
    cJSON_AddNumberToObject(mb, "baud", 9600);
    cJSON_AddNumberToObject(mb, "parity", 0);
    cJSON_AddNumberToObject(mb, "data_bits", 8);
    cJSON_AddNumberToObject(mb, "stop_bits", 1);

    /* cloud (los certs/clave nunca viajan por BLE) */
    cJSON *cl = cJSON_AddObjectToObject(r, "cloud");
    cJSON_AddBoolToObject(cl, "enabled", c->cloud.enabled);
    cJSON_AddStringToObject(cl, "endpoint", c->cloud.broker_url);
    cJSON_AddNumberToObject(cl, "port", 8883);
    cJSON_AddStringToObject(cl, "thing_name", c->general.serial);
    cJSON_AddStringToObject(cl, "client_id", c->general.serial);
    cJSON_AddStringToObject(cl, "topic_base", c->cloud.topic_base);
    cJSON_AddNumberToObject(cl, "qos", c->cloud.qos);
    cJSON_AddNumberToObject(cl, "keepalive_s", c->cloud.keepalive);
    cJSON_AddStringToObject(cl, "root_ca", "");     /* certs solo por microSD */
    cJSON_AddStringToObject(cl, "device_cert", "");
    cJSON_AddStringToObject(cl, "private_key", "");

    /* bluetooth */
    cJSON *bt = cJSON_AddObjectToObject(r, "bluetooth");
    cJSON_AddBoolToObject(bt, "enabled", c->bt.enabled);
    cJSON_AddStringToObject(bt, "name", c->bt.legacy.name);
    cJSON_AddNumberToObject(bt, "tx_power", c->bt.tx_power);

    /* ota */
    cJSON *ota = cJSON_AddObjectToObject(r, "ota");
    cJSON_AddNumberToObject(ota, "channel", 0);
    cJSON_AddNumberToObject(ota, "source", 0);
    cJSON_AddStringToObject(ota, "url", c->cloud.ota_url);

    /* users (PIN redactado) */
    cJSON *users = cJSON_AddArrayToObject(r, "users");
    for (int i = 0; i < c->general.users_count && i < APP_MAX_USERS; i++) {
        const app_user_t *u = &c->general.users[i];
        cJSON *uo = cJSON_CreateObject();
        cJSON_AddStringToObject(uo, "name", u->name);
        cJSON_AddStringToObject(uo, "pin", redact ? "" : u->pin);
        cJSON_AddNumberToObject(uo, "role", u->role);
        cJSON_AddBoolToObject(uo, "locked", u->locked);
        cJSON_AddStringToObject(uo, "last", u->last);
        cJSON_AddItemToArray(users, uo);
    }
    cJSON *adm = cJSON_AddObjectToObject(r, "admin");
    cJSON_AddStringToObject(adm, "name", c->general.factory.name);
    cJSON_AddStringToObject(adm, "pin", redact ? "" : c->general.factory.pin);
    cJSON_AddNumberToObject(adm, "role", c->general.factory.role);
    cJSON_AddBoolToObject(adm, "locked", c->general.factory.locked);
    cJSON_AddStringToObject(adm, "last", c->general.factory.last);

    /* channels: Axira = 2 sensores FIJOS (sin calibración) */
    cJSON *chs = cJSON_AddArrayToObject(r, "channels");
    cJSON_AddItemToArray(chs, build_channel_obj(c, 1));
    cJSON_AddItemToArray(chs, build_channel_obj(c, 2));

    char *s = cJSON_PrintUnformatted(r);
    cJSON_Delete(r);
    return s;
}

/* ================= set_config (merge parcial) ================= */
static void cpy(char *d, size_t n, const char *s) { snprintf(d, n, "%s", s ? s : ""); }
static const char *jstr(const cJSON *o, const char *k, const char *def)
{
    const cJSON *v = cJSON_GetObjectItemCaseSensitive(o, k);
    return (cJSON_IsString(v) && v->valuestring) ? v->valuestring : def;
}
static double jnum(const cJSON *o, const char *k, double def)
{
    const cJSON *v = cJSON_GetObjectItemCaseSensitive(o, k);
    return cJSON_IsNumber(v) ? v->valuedouble : def;
}
static int jbool(const cJSON *o, const char *k, int def)
{
    const cJSON *v = cJSON_GetObjectItemCaseSensitive(o, k);
    return cJSON_IsBool(v) ? cJSON_IsTrue(v) : def;
}

static void apply_netif(const cJSON *n, bool *en, char *ip_mode, size_t im_cap,
                        char *ip, char *mask, char *gw, char *dns1, size_t ip_cap)
{
    if (!n) return;
    *en = jbool(n, "enabled", *en);
    if (cJSON_HasObjectItem(n, "dhcp")) cpy(ip_mode, im_cap, jbool(n, "dhcp", 1) ? "dhcp" : "static");
    cpy(ip,   ip_cap, jstr(n, "ip", ip));
    cpy(mask, ip_cap, jstr(n, "mask", mask));
    cpy(gw,   ip_cap, jstr(n, "gateway", gw));
    cpy(dns1, ip_cap, jstr(n, "dns", dns1));
}

/* Aplica un ChannelConfig al AppConfig: id 1 = Presión (unidad, gas, límites),
 * id 2 = Flujo (unidad, límite alto). Ignora scale/cal (Axira fija).
 * Acepta AMBOS esquemas de la app:
 *   - ANIDADO (documento de config completa, ChannelConfig.toJson): alarma en "alarm".
 *   - PLANO (característica de canal dedicada, BleChannelConfig.toJson): alarma en la raíz.
 * Por eso la fuente de los campos de alarma es el objeto "alarm" si existe, o el
 * propio objeto del canal cuando vienen a nivel raíz. */
static void apply_channel_obj(AppConfig *c, const cJSON *ch)
{
    int id = (int)jnum(ch, "id", 0);
    const cJSON *nested = cJSON_GetObjectItemCaseSensitive(ch, "alarm");
    const cJSON *am = nested ? nested : ch;
    if (id == 1) { /* Presión */
        char u[8]; press_unit_fpm(jstr(ch, "unit", c->sensors.pressure_unit), u, sizeof(u));
        cpy(c->sensors.pressure_unit, sizeof(c->sensors.pressure_unit), u);
        if (cJSON_HasObjectItem(ch, "gas_color"))
            cpy(c->sensors.gas_type, sizeof(c->sensors.gas_type),
                color_to_gas((uint32_t)jnum(ch, "gas_color", 0x1d9e75)));
        if (am) {
            c->sensors.alarm_limits.pressure_min_enabled = jbool(am, "lo_enabled", c->sensors.alarm_limits.pressure_min_enabled);
            c->sensors.alarm_limits.pressure_max_enabled = jbool(am, "hi_enabled", c->sensors.alarm_limits.pressure_max_enabled);
            c->sensors.alarm_limits.pressure_min = press_from_disp((float)jnum(am, "lo_limit", press_to_disp(c->sensors.alarm_limits.pressure_min, u)), u);
            c->sensors.alarm_limits.pressure_max = press_from_disp((float)jnum(am, "hi_limit", press_to_disp(c->sensors.alarm_limits.pressure_max, u)), u);
        }
    } else if (id == 2) { /* Flujo */
        char u[8]; flow_unit_fpm(jstr(ch, "unit", c->sensors.flow_unit), u, sizeof(u));
        cpy(c->sensors.flow_unit, sizeof(c->sensors.flow_unit), u);
        if (am) {
            c->sensors.alarm_limits.flow_high_enabled = jbool(am, "hi_enabled", c->sensors.alarm_limits.flow_high_enabled);
            c->sensors.alarm_limits.flow_high_limit = flow_from_disp((float)jnum(am, "hi_limit", flow_to_disp(c->sensors.alarm_limits.flow_high_limit, u)), u);
        }
    }
}

/* R1 (concurrencia): en vez de mutar el snapshot vivo en sitio —que la tarea LVGL
 * puede leer a medio escribir (torn read)—, los aplicadores trabajan sobre una
 * COPIA en heap y solo al final persisten + recargan el snapshot de una vez
 * (appcfg_cache_reload hace un único memcpy). Reduce la ventana de carrera al
 * memcpy de reload. El usuario debe liberar con free(). */
static AppConfig *cfg_dup(void)
{
    AppConfig *c = malloc(sizeof(AppConfig));
    if (!c) return NULL;
    if (appcfg_cache_get(c) != ESP_OK) { free(c); return NULL; }
    return c;
}

bool fpm_ble_config_apply_json(const char *json)
{
    cJSON *r = cJSON_Parse(json);
    if (!r) return false;
    AppConfig *c = cfg_dup();
    if (!c) { cJSON_Delete(r); return false; }

    /* --- Comandos de presión (opción B: tara / modo gauge) ---
     * La app envía esto al comisionar, con la LÍNEA VENTEADA (a la atmósfera):
     *   {"pressure_tare": true}                 -> captura el cero manométrico
     *   {"pressure_mode": "gauge"|"absolute"}   -> cambia el modo sin re-tarar
     * Se procesan antes del resto de la config; no dependen del AppConfig. */
    if (jbool(r, "pressure_tare", false)) {
        esp_err_t terr = sensors_runtime_tare_pressure();
        ESP_LOGI(TAG_FPM, "Comando BLE 'pressure_tare' -> %s", esp_err_to_name(terr));
    }
    const char *pmode = jstr(r, "pressure_mode", NULL);
    if (pmode) {
        bool gauge = (strcasecmp(pmode, "gauge") == 0);
        esp_err_t merr = sensors_runtime_set_pressure_gauge(gauge);
        ESP_LOGI(TAG_FPM, "Comando BLE 'pressure_mode'=%s -> %s", pmode, esp_err_to_name(merr));
    }

    /* --- Comandos de flujo (FS7): auto-cero + calibración de 1 punto ---
     *   {"flow_tare": true}         -> con la LÍNEA SIN FLUJO, fija el cero (u0)
     *   {"flow_cal_slm": <caudal>}  -> con un CAUDAL CONOCIDO estable (del patrón
     *                                  SFM3300), ajusta flow_scale para coincidir.
     * Flujo típico: tare primero, luego cal_slm a un caudal estable. */
    if (jbool(r, "flow_tare", false)) {
        esp_err_t ferr = sensors_runtime_tare_flow();
        ESP_LOGI(TAG_FPM, "Comando BLE 'flow_tare' -> %s", esp_err_to_name(ferr));
    }
    const cJSON *fcal = cJSON_GetObjectItemCaseSensitive(r, "flow_cal_slm");
    if (cJSON_IsNumber(fcal)) {
        esp_err_t cerr = sensors_runtime_cal_flow_point((float)fcal->valuedouble);
        ESP_LOGI(TAG_FPM, "Comando BLE 'flow_cal_slm'=%.2f -> %s",
                 fcal->valuedouble, esp_err_to_name(cerr));
    }

    const cJSON *disp = cJSON_GetObjectItemCaseSensitive(r, "display");
    if (disp) {
        c->general.brightness = (int)jnum(disp, "brightness", c->general.brightness);
        c->general.dim_minutes = (int)jnum(disp, "dim_minutes", c->general.dim_minutes);
        if (cJSON_HasObjectItem(disp, "theme"))
            cpy(c->general.theme, sizeof(c->general.theme), jnum(disp, "theme", 0) ? "light" : "dark");
        cpy(c->general.lang, sizeof(c->general.lang), jstr(disp, "language", c->general.lang));
    }

    const cJSON *tm = cJSON_GetObjectItemCaseSensitive(r, "time");
    if (tm) cpy(c->general.timezone, sizeof(c->general.timezone), jstr(tm, "timezone", c->general.timezone));

    const cJSON *al = cJSON_GetObjectItemCaseSensitive(r, "alarms");
    if (al) {
        c->general.alarm.volume = (int)jnum(al, "volume", c->general.alarm.volume);
        c->general.alarm.reannounce_minutes = (int)jnum(al, "reannounce_minutes", c->general.alarm.reannounce_minutes);
        c->general.alarm.max_silence_minutes = (int)jnum(al, "max_silence_minutes", c->general.alarm.max_silence_minutes);
    }

    const cJSON *net = cJSON_GetObjectItemCaseSensitive(r, "network");
    if (net) {
        apply_netif(cJSON_GetObjectItemCaseSensitive(net, "ethernet"), &c->eth.enabled,
                    c->eth.ip_mode, sizeof(c->eth.ip_mode), c->eth.ip, c->eth.mask, c->eth.gw, c->eth.dns1, sizeof(c->eth.ip));
        apply_netif(cJSON_GetObjectItemCaseSensitive(net, "wifi"), &c->wifi.enabled,
                    c->wifi.ip_mode, sizeof(c->wifi.ip_mode), c->wifi.ip, c->wifi.mask, c->wifi.gw, c->wifi.dns1, sizeof(c->wifi.ip));
        cpy(c->wifi.ssid, sizeof(c->wifi.ssid), jstr(net, "wifi_ssid", c->wifi.ssid));
        /* la password solo se toca si viene NO vacía (el read la redacta) */
        const char *pw = jstr(net, "wifi_password", "");
        if (pw && pw[0]) cpy(c->wifi.password, sizeof(c->wifi.password), pw);
        cpy(c->eth.hostname, sizeof(c->eth.hostname), jstr(net, "hostname", c->eth.hostname));
    }

    const cJSON *cl = cJSON_GetObjectItemCaseSensitive(r, "cloud");
    if (cl) {
        c->cloud.enabled = jbool(cl, "enabled", c->cloud.enabled);
        cpy(c->cloud.broker_url, sizeof(c->cloud.broker_url), jstr(cl, "endpoint", c->cloud.broker_url));
        cpy(c->cloud.topic_base, sizeof(c->cloud.topic_base), jstr(cl, "topic_base", c->cloud.topic_base));
        c->cloud.qos = (int)jnum(cl, "qos", c->cloud.qos);
        c->cloud.keepalive = (int)jnum(cl, "keepalive_s", c->cloud.keepalive);
    }

    const cJSON *bt = cJSON_GetObjectItemCaseSensitive(r, "bluetooth");
    if (bt) {
        c->bt.enabled = jbool(bt, "enabled", c->bt.enabled);
        cpy(c->bt.legacy.name, sizeof(c->bt.legacy.name), jstr(bt, "name", c->bt.legacy.name));
        c->bt.tx_power = (app_bt_tx_power_t)(int)jnum(bt, "tx_power", c->bt.tx_power);
    }

    const cJSON *ota = cJSON_GetObjectItemCaseSensitive(r, "ota");
    if (ota) cpy(c->cloud.ota_url, sizeof(c->cloud.ota_url), jstr(ota, "url", c->cloud.ota_url));

    /* channels: solo Presión (id 1) y Flujo (id 2). Gas/unidad/límites; sin cal. */
    const cJSON *chs = cJSON_GetObjectItemCaseSensitive(r, "channels");
    if (cJSON_IsArray(chs)) {
        const cJSON *ch;
        cJSON_ArrayForEach(ch, chs) apply_channel_obj(c, ch);
    }

    cJSON_Delete(r);
    (void)appcfg_save(c);       /* persiste NVS */
    (void)appcfg_cache_reload(); /* refresca snapshot (dashboard/alarmas lo ven) */
    free(c);
    return true;
}

/* ================= características dedicadas ================= */

char *fpm_ble_wifi_read_json(void)
{
    const AppConfig *c = appcfg_cache_peek();
    if (!c) return NULL;
    cJSON *o = cJSON_CreateObject();
    cJSON_AddBoolToObject(o, "enabled", c->wifi.enabled);
    cJSON_AddBoolToObject(o, "ethernet_enabled", c->eth.enabled);
    cJSON_AddBoolToObject(o, "dhcp", strcmp(c->wifi.ip_mode, "static") != 0);
    cJSON_AddStringToObject(o, "ssid", c->wifi.ssid);
    cJSON_AddStringToObject(o, "hostname", c->eth.hostname);
    cJSON_AddStringToObject(o, "ip", c->wifi.ip);
    cJSON_AddStringToObject(o, "mask", c->wifi.mask);
    cJSON_AddStringToObject(o, "gateway", c->wifi.gw);
    cJSON_AddStringToObject(o, "dns", c->wifi.dns1);
    char *s = cJSON_PrintUnformatted(o);
    cJSON_Delete(o);
    return s;
}

bool fpm_ble_wifi_apply_json(const char *json)
{
    cJSON *o = cJSON_Parse(json);
    if (!o) return false;
    AppConfig *c = cfg_dup();
    if (!c) { cJSON_Delete(o); return false; }
    c->wifi.enabled = jbool(o, "enabled", c->wifi.enabled);
    c->eth.enabled  = jbool(o, "ethernet_enabled", c->eth.enabled);
    if (cJSON_HasObjectItem(o, "dhcp"))
        cpy(c->wifi.ip_mode, sizeof(c->wifi.ip_mode), jbool(o, "dhcp", 1) ? "dhcp" : "static");
    cpy(c->wifi.ssid, sizeof(c->wifi.ssid), jstr(o, "ssid", c->wifi.ssid));
    cpy(c->eth.hostname, sizeof(c->eth.hostname), jstr(o, "hostname", c->eth.hostname));
    const char *pw = jstr(o, "password", "");
    if (pw && pw[0]) cpy(c->wifi.password, sizeof(c->wifi.password), pw);
    if (cJSON_HasObjectItem(o, "ip"))      cpy(c->wifi.ip,   sizeof(c->wifi.ip),   jstr(o, "ip", c->wifi.ip));
    if (cJSON_HasObjectItem(o, "mask"))    cpy(c->wifi.mask, sizeof(c->wifi.mask), jstr(o, "mask", c->wifi.mask));
    if (cJSON_HasObjectItem(o, "gateway")) cpy(c->wifi.gw,   sizeof(c->wifi.gw),   jstr(o, "gateway", c->wifi.gw));
    if (cJSON_HasObjectItem(o, "dns"))     cpy(c->wifi.dns1, sizeof(c->wifi.dns1), jstr(o, "dns", c->wifi.dns1));
    cJSON_Delete(o);
    (void)appcfg_save(c);
    (void)appcfg_cache_reload();
    free(c);
    return true;
}

char *fpm_ble_cloud_read_json(void)
{
    const AppConfig *c = appcfg_cache_peek();
    if (!c) return NULL;
    cJSON *o = cJSON_CreateObject();
    cJSON_AddBoolToObject(o, "enabled", c->cloud.enabled);
    cJSON_AddStringToObject(o, "endpoint", c->cloud.broker_url);
    cJSON_AddNumberToObject(o, "port", 8883);
    cJSON_AddStringToObject(o, "thing_name", c->general.serial);
    cJSON_AddStringToObject(o, "client_id", c->general.serial);
    cJSON_AddStringToObject(o, "topic_base", c->cloud.topic_base);
    cJSON_AddNumberToObject(o, "qos", c->cloud.qos);
    cJSON_AddNumberToObject(o, "keepalive_s", c->cloud.keepalive);
    char *s = cJSON_PrintUnformatted(o);
    cJSON_Delete(o);
    return s;
}

bool fpm_ble_cloud_apply_json(const char *json)
{
    cJSON *o = cJSON_Parse(json);
    if (!o) return false;
    AppConfig *c = cfg_dup();
    if (!c) { cJSON_Delete(o); return false; }
    /* escritura parcial (la app manda enabled/endpoint/port y luego thing/client/topic) */
    if (cJSON_HasObjectItem(o, "enabled"))    c->cloud.enabled = jbool(o, "enabled", c->cloud.enabled);
    if (cJSON_HasObjectItem(o, "endpoint"))   cpy(c->cloud.broker_url, sizeof(c->cloud.broker_url), jstr(o, "endpoint", c->cloud.broker_url));
    if (cJSON_HasObjectItem(o, "topic_base")) cpy(c->cloud.topic_base, sizeof(c->cloud.topic_base), jstr(o, "topic_base", c->cloud.topic_base));
    if (cJSON_HasObjectItem(o, "qos"))        c->cloud.qos = (int)jnum(o, "qos", c->cloud.qos);
    if (cJSON_HasObjectItem(o, "keepalive_s"))c->cloud.keepalive = (int)jnum(o, "keepalive_s", c->cloud.keepalive);
    cJSON_Delete(o);
    (void)appcfg_save(c);
    (void)appcfg_cache_reload();
    free(c);
    return true;
}

char *fpm_ble_channel_read_json(int sel)
{
    const AppConfig *c = appcfg_cache_peek();
    if (!c) return NULL;
    cJSON *ch = build_channel_flat(c, sel == 1 ? 2 : 1);   /* sel 0=Presión(id1), 1=Flujo(id2) */
    char *s = cJSON_PrintUnformatted(ch);
    cJSON_Delete(ch);
    return s;
}

bool fpm_ble_channel_apply_json(const char *json)
{
    cJSON *o = cJSON_Parse(json);
    if (!o) return false;
    AppConfig *c = cfg_dup();
    if (!c) { cJSON_Delete(o); return false; }
    apply_channel_obj(c, o);
    cJSON_Delete(o);
    (void)appcfg_save(c);
    (void)appcfg_cache_reload();
    free(c);
    return true;
}

char *fpm_ble_status_read_json(int sel)
{
    /* Axira no calibra; estado mínimo para el flujo de la app. */
    cJSON *o = cJSON_CreateObject();
    cJSON_AddNumberToObject(o, "sel_channel", sel + 1);
    cJSON_AddBoolToObject(o, "ch_enabled", true);
    cJSON_AddNumberToObject(o, "ch_state", 0);
    cJSON_AddNumberToObject(o, "raw_ma", 0);
    char *s = cJSON_PrintUnformatted(o);
    cJSON_Delete(o);
    return s;
}

/* Cuentas LISTADAS (técnicos/administradores) en general.users[]. El fabricante
 * (general.factory) es aparte y no se toca por BLE. Roles: 1=TÉCNICO, 2=ADMIN. */
bool fpm_ble_user_op_json(const char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (!root) return false;
    AppConfig *c = cfg_dup();
    if (!c) { cJSON_Delete(root); return false; }

    const char *op = jstr(root, "op", "");
    int index = (int)jnum(root, "index", -1);
    bool ok = false;

    if (strcmp(op, "user_remove") == 0) {
        if (index >= 0 && index < c->general.users_count) {
            int admins = 0;
            for (int i = 0; i < c->general.users_count; i++)
                if (c->general.users[i].role >= APP_ROLE_ADMIN) admins++;
            bool is_admin = c->general.users[index].role >= APP_ROLE_ADMIN;
            if (!(is_admin && admins <= 1)) {   /* no dejar el equipo sin admin */
                for (int i = index; i < c->general.users_count - 1; i++)
                    c->general.users[i] = c->general.users[i + 1];
                c->general.users_count--;
                ok = true;
            }
        }
    } else { /* user_upsert */
        const char *name = jstr(root, "name", "");
        int role = (int)jnum(root, "role", APP_ROLE_TECH);
        bool locked = jbool(root, "locked", 0);
        const char *pin = jstr(root, "pin", "");
        if (role < APP_ROLE_TECH) role = APP_ROLE_TECH;
        if (role > APP_ROLE_ADMIN) role = APP_ROLE_ADMIN;   /* listados: técnico/admin */
        if (index >= 0 && index < c->general.users_count) {          /* editar */
            app_user_t *u = &c->general.users[index];
            cpy(u->name, sizeof(u->name), name);
            u->role = (app_user_role_t)role;
            u->locked = locked;
            if (pin[0]) cpy(u->pin, sizeof(u->pin), pin);           /* preserva si vacío */
            ok = true;
        } else if (index == c->general.users_count &&
                   c->general.users_count < APP_MAX_USERS && pin[0]) { /* alta (PIN oblig.) */
            app_user_t *u = &c->general.users[c->general.users_count];
            memset(u, 0, sizeof(*u));
            cpy(u->name, sizeof(u->name), name);
            cpy(u->pin, sizeof(u->pin), pin);
            u->role = (app_user_role_t)role;
            u->locked = locked;
            cpy(u->last, sizeof(u->last), "nunca");
            c->general.users_count++;
            ok = true;
        }
    }

    cJSON_Delete(root);
    if (ok) { (void)appcfg_save(c); (void)appcfg_cache_reload(); }
    free(c);
    return ok;
}

/* Callback de ajuste de reloj. Se registra desde main (que sí depende de
 * network_core/time_mgr) para evitar una dependencia circular
 * transport_ble <-> network_core. */
static fpm_ble_clock_cb_t s_clock_cb = NULL;
void fpm_ble_config_set_clock_cb(fpm_ble_clock_cb_t cb) { s_clock_cb = cb; }

/* Ajuste de reloj por BLE (op de control `set_clock`). La app manda componentes
 * de hora LOCAL: {op,year,month,day,hour,minute} (segundos opcionales, precisión
 * al minuto como la HMI). Delega en el callback registrado (main -> time_mgr).
 * Devuelve false si faltan campos, quedan fuera de rango o no hay callback. */
bool fpm_ble_set_clock_json(const char *json)
{
    cJSON *o = cJSON_Parse(json);
    if (!o) return false;
    int year   = (int)jnum(o, "year", 0);
    int month  = (int)jnum(o, "month", 0);
    int day    = (int)jnum(o, "day", 0);
    int hour   = (int)jnum(o, "hour", -1);
    int minute = (int)jnum(o, "minute", -1);
    int second = (int)jnum(o, "second", 0);
    cJSON_Delete(o);

    if (year < 2024 || year > 2099) return false;
    if (month < 1  || month > 12)   return false;
    if (day < 1    || day > 31)     return false;
    if (hour < 0   || hour > 23)    return false;
    if (minute < 0 || minute > 59)  return false;
    if (second < 0 || second > 59)  return false;
    if (!s_clock_cb) return false;

    return s_clock_cb(year, month, day, hour, minute, second);
}
