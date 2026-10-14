/*
 * sim_backend.c — backend simulado del FPM-200 para el simulador de PC.
 * AppConfig de demostracion + implementaciones vacias de las funciones del firmware (componentes y
 * ESP-IDF) que la HMI llama. Si al enlazar falta un simbolo, agregarlo aqui.
 */
#include <stdio.h>
#include <string.h>

#include "storage.h"
#include "esp_err.h"
#include "esp_app_desc.h"
#include "nvs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "alarm_mgr.h"
#include "cert_store.h"
#include "flow_meter.h"
#include "ota_update.h"
#include "sensors_runtime.h"
#include "transport_mqtt.h"
#include "ui_statusbar_controller.h"

static AppConfig s_cfg;
static const char s_tecnico[] = {'T',(char)0xC3,(char)0xA9,'c','n','i','c','o',0};
static bool s_init = false;

static void ensure_cfg(void)
{
    if (s_init) return;
    memset(&s_cfg, 0, sizeof(s_cfg));

    strcpy(s_cfg.general.info_text, "Axira Equipos");
    strcpy(s_cfg.general.client,    "UCI Torre A");
    strcpy(s_cfg.general.model,     "FPM-200");
    strcpy(s_cfg.general.serial,    "AX-4200-01847");
    strcpy(s_cfg.general.hw_version, "1.1");
    strcpy(s_cfg.general.fw_version, "1.8.0");
    strcpy(s_cfg.general.lang,      "es");
    strcpy(s_cfg.general.timezone,  "America/Bogota");
    s_cfg.general.brightness = 80;
    strcpy(s_cfg.general.theme, "dark"); s_cfg.general.dim_minutes = 5;
    s_cfg.general.alarm.tone_warn = true; s_cfg.general.alarm.tone_alert = true;
    s_cfg.general.alarm.warn_timeout_s = 15; s_cfg.general.alarm.alert_timeout_s = 120;
    s_cfg.general.alarm.volume=80; s_cfg.general.alarm.reannounce_minutes=15; s_cfg.general.alarm.max_silence_minutes=2;
    strcpy(s_cfg.general.admin.pass, "1234");
    s_cfg.general.users_count = 2;
    strcpy(s_cfg.general.users[0].name, s_tecnico); strcpy(s_cfg.general.users[0].pin, "4321"); s_cfg.general.users[0].role = APP_ROLE_TECH; strcpy(s_cfg.general.users[0].last, "nunca");
    strcpy(s_cfg.general.users[1].name, "Administrador"); strcpy(s_cfg.general.users[1].pin, "1234"); s_cfg.general.users[1].role = APP_ROLE_ADMIN; strcpy(s_cfg.general.users[1].last, "nunca");
    strcpy(s_cfg.general.factory.name, "Fabricante"); strcpy(s_cfg.general.factory.pin, "axira-2026"); s_cfg.general.factory.role = APP_ROLE_FACTORY; s_cfg.general.factory.locked = true;
    strcpy(s_cfg.general.admin.user, "admin");

    strcpy(s_cfg.sensors.pressure_unit, "psi");
    strcpy(s_cfg.sensors.flow_unit,     "lpm");
    strcpy(s_cfg.sensors.gas_type,      "o2");
    strcpy(s_cfg.sensors.color_code,    "nfpa");
    s_cfg.sensors.flow_fullscale_lpm = 100.0f;
    strcpy(s_cfg.sensors.cal.last_cal_date,     "12 mar 2026");
    strcpy(s_cfg.sensors.cal.next_service_date, "08 sep 2026");

    /* Limites en kPa (coherentes con pressure_kpa que inyecta main.c): 500 psi = 3447 kPa */
    s_cfg.sensors.alarm_limits.pressure_min_enabled = true;
    s_cfg.sensors.alarm_limits.pressure_max_enabled = true;
    s_cfg.sensors.alarm_limits.flow_delta_enabled = true; s_cfg.sensors.alarm_limits.flow_high_enabled = false;
    s_cfg.sensors.alarm_limits.pressure_min = 344.7f;
    s_cfg.sensors.alarm_limits.pressure_max = 999.7f;
    s_cfg.sensors.alarm_limits.flow_delta_threshold = 20.0f;
    s_cfg.sensors.alarm_limits.flow_high_limit = 90.0f;
    s_cfg.sensors.alarm_limits.flow_delta_window_ms = 3000;

    s_cfg.wifi.enabled = true;
    strcpy(s_cfg.wifi.ssid, "Hospital-BIOMED-5G");
    strcpy(s_cfg.wifi.password, "biomed2026");
    strcpy(s_cfg.wifi.ip_mode, "dhcp");
    strcpy(s_cfg.wifi.ip, "10.4.12.87");
    strcpy(s_cfg.wifi.mask, "255.255.255.0");
    strcpy(s_cfg.wifi.gw, "10.4.12.1");
    strcpy(s_cfg.wifi.dns1, "10.4.12.1");

    s_cfg.eth.enabled = false;
    strcpy(s_cfg.eth.hostname, "AX-0001-Eth");
    strcpy(s_cfg.eth.ip_mode, "dhcp");

    s_cfg.bt.enabled = true;
    s_cfg.bt.advertise = true;
    s_cfg.bt.tx_power = ALTO;
    s_cfg.bt.mesh.enabled = false;
    s_cfg.bt.mesh.provisioned = false;
    s_cfg.bt.mesh.ttl = 5;
    s_cfg.bt.mesh.relay = false;
    strcpy(s_cfg.bt.mesh.net_key, "00112233445566778899AABBCCDDEEFF");
    strcpy(s_cfg.bt.mesh.app_key, "FFEEDDCCBBAA99887766554433221100");
    strcpy(s_cfg.bt.mesh.dev_uuid, "123e4567-e89b-12d3-a456-426614174000");
    s_cfg.bt.mesh.unicast_addr = 0x0002;
    strcpy(s_cfg.bt.legacy.name, "MPF-4200-1847");
    strcpy(s_cfg.bt.legacy.pin, "0000");
    s_cfg.bt.legacy.sec_mode = APP_BT_SEC_PASSKEY;

    s_cfg.cloud.enabled = true;
    strcpy(s_cfg.cloud.type, "mqtt");
    strcpy(s_cfg.cloud.broker_url, "broker.axira.io:8883");
    strcpy(s_cfg.cloud.topic_base, "axira");
    s_cfg.cloud.qos = 1;
    s_cfg.cloud.keepalive = 60;

    s_init = true;
}

AppConfig *appcfg_cache_peek(void)
{
    ensure_cfg();
    return &s_cfg;
}

/* ---------------------------------------------------------------- storage (config en RAM) */

void appcfg_defaults(AppConfig *c)
{
    s_init = false;
    ensure_cfg();
    *c = s_cfg;
}
esp_err_t appcfg_load(AppConfig *out) { ensure_cfg(); *out = s_cfg; return ESP_OK; }
esp_err_t appcfg_save(const AppConfig *in) { s_cfg = *in; s_init = true; return ESP_OK; }
esp_err_t appcfg_cache_get(AppConfig *out) { return appcfg_load(out); }
esp_err_t appcfg_cache_reload(void) { ensure_cfg(); return ESP_OK; }
esp_err_t cert_store_erase_all(void) { return ESP_OK; }

/* ---------------------------------------------------------------- sensores y alarmas (demo) */

uint32_t alarm_mgr_get_sensor_faults(void) { return 0; }
void alarm_mgr_press_mute(void) {}
void alarm_mgr_set_audio_inhibit_noncritical(bool inhibit) { (void)inhibit; }
void alarm_mgr_test_buzzer(void) {}
bool sensors_runtime_flow_warming(void) { return false; }
uint32_t sensors_runtime_flow_warming_elapsed_ms(void) { return 0; }
void sensors_runtime_get_debug(float *atm_kpa, float *sfm_slm, float *fs7_lpm)
{
    if (atm_kpa) *atm_kpa = 85.2f;
    if (sfm_slm) *sfm_slm = 0.0f;
    if (fs7_lpm) *fs7_lpm = 59.0f;
}
flow_meter_snapshot_t flow_meter_get(void)
{
    flow_meter_snapshot_t s = { .volume_m3 = 12.84, .missing_ms = 0, .partial = false, .dated = true };
    return s;
}

/* ---------------------------------------------------------------- red, OTA, barra de estado */

esp_err_t ota_update_check_url(const char *base, int channel, char *version_out, size_t version_size,
                               bool *newer)
{
    (void)base; (void)channel;
    if (version_out && version_size) snprintf(version_out, version_size, "(sim)");
    if (newer) *newer = false;
    return ESP_ERR_NOT_SUPPORTED;
}
esp_err_t ota_update_from_url(const char *base, int channel, ota_update_progress_cb_t cb, void *user,
                              char *version_out, size_t version_size)
{
    (void)base; (void)channel; (void)cb; (void)user; (void)version_out; (void)version_size;
    return ESP_ERR_NOT_SUPPORTED;
}
void transport_mqtt_on_time_ready(void) {}
void transport_mqtt_stop(void) {}
void ui_statusbar_request_refresh(void) {}
void ui_statusbar_set_enabled(bool wifi, bool eth, bool bt, bool cloud)
{
    (void)wifi; (void)eth; (void)bt; (void)cloud;
}

/* ---------------------------------------------------------------- ESP-IDF */

const char *esp_err_to_name(esp_err_t code)
{
    return code == ESP_OK ? "ESP_OK" : "ESP_ERR (sim)";
}

const esp_app_desc_t *esp_app_get_description(void)
{
    static const esp_app_desc_t d = { .version = "sim", .project_name = "fpm200" };
    return &d;
}

void esp_restart(void)
{
    printf("esp_restart() (sim): se ignora\n");
}

esp_err_t nvs_open(const char *ns, nvs_open_mode_t mode, nvs_handle_t *out)
{
    (void)ns; (void)mode;
    *out = 1;
    return ESP_OK;
}
esp_err_t nvs_erase_all(nvs_handle_t h) { (void)h; return ESP_OK; }
esp_err_t nvs_commit(nvs_handle_t h) { (void)h; return ESP_OK; }
void nvs_close(nvs_handle_t h) { (void)h; }

/* Tareas: en el sim no hay FreeRTOS; una tarea que la HMI cree no se ejecuta. */
void vTaskDelay(TickType_t ticks) { (void)ticks; }
void vTaskDelete(TaskHandle_t t) { (void)t; }
BaseType_t xTaskCreatePinnedToCore(TaskFunction_t fn, const char *name, uint32_t stack, void *arg,
                                   UBaseType_t prio, TaskHandle_t *out, BaseType_t core)
{
    (void)fn; (void)stack; (void)arg; (void)prio; (void)core;
    printf("xTaskCreatePinnedToCore(%s) (sim): no se ejecuta\n", name);
    if (out) *out = NULL;
    return 0;
}
