#include "ui_connectivityScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui.h"
#include "storage.h"
#include <string.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "wifi_mgr.h"
#include "eth_mgr.h"
#include "transport_ble.h"
#endif

/* Conectividad (mockup 4g): grid 2x2 de tarjetas de conexión.
 * Datos reales: config desde AppConfig + estado en vivo de los managers
 * (RSSI/IP/enlace) bajo ESP_PLATFORM; el simulador usa solo la config. */

lv_obj_t *ui_connectivityScreen = NULL;

static int32_t s_col[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
static int32_t s_row[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };

typedef struct {
    bool wifi_on, wifi_conn, wifi_dhcp;
    char wifi_ssid[33], wifi_ip[16];
    int  wifi_rssi;
    bool eth_on, eth_up;
    char eth_ip[16];
    bool ble_on, ble_conn, ble_adv;
    char ble_name[32];
    bool cloud_on, cloud_conn;
    char broker[128];
} conn_status_t;

static void cpy(char *d, size_t n, const char *s) { strncpy(d, s ? s : "", n - 1); d[n - 1] = '\0'; }

static void gather(conn_status_t *s)
{
    memset(s, 0, sizeof(*s));
    const AppConfig *c = appcfg_cache_peek();
    if (c) {
        s->wifi_on   = c->wifi.enabled;
        s->wifi_dhcp = strcmp(c->wifi.ip_mode, "static") != 0;
        cpy(s->wifi_ssid, sizeof(s->wifi_ssid), c->wifi.ssid);
        if (!s->wifi_dhcp) cpy(s->wifi_ip, sizeof(s->wifi_ip), c->wifi.ip);
        s->eth_on = c->eth.enabled;
        if (strcmp(c->eth.ip_mode, "static") == 0) cpy(s->eth_ip, sizeof(s->eth_ip), c->eth.ip);
        s->ble_on  = c->bt.enabled;
        s->ble_adv = c->bt.advertise;
        cpy(s->ble_name, sizeof(s->ble_name), c->bt.legacy.name);
        s->cloud_on = c->cloud.enabled;
        cpy(s->broker, sizeof(s->broker), c->cloud.broker_url);
    }
#ifdef ESP_PLATFORM
    wifi_netinfo_t wi = {0};
    if (wifi_mgr_get_netinfo(&wi) == ESP_OK) {
        s->wifi_conn = wi.connected;
        s->wifi_rssi = wi.rssi_dbm;
        if (wi.ssid[0]) cpy(s->wifi_ssid, sizeof(s->wifi_ssid), wi.ssid);
        if (wi.ip[0])   cpy(s->wifi_ip,   sizeof(s->wifi_ip),   wi.ip);
    }
    s->eth_up   = eth_mgr_is_up();
    s->ble_conn = transport_ble_is_connected();
    extern bool cloud_mgr_connected(void);
    s->cloud_conn = cloud_mgr_connected();
#endif
}

/* RSSI (dBm) -> porcentaje aproximado para la barra (−90..−40 dBm). */
static int rssi_pct(int dbm)
{
    int p = (dbm + 90) * 100 / 50;
    return p < 0 ? 0 : p > 100 ? 100 : p;
}

static lv_obj_t *conn_card(lv_obj_t *grid, const char *sym, uint32_t icon_col, uint32_t border_col,
                           const char *title, const char *status, uint32_t status_col,
                           int col, int row, lv_event_cb_t cb)
{
    lv_obj_t *c = ui_card(grid);
    lv_obj_set_grid_cell(c, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);
    lv_obj_set_style_radius(c, UI_RADIUS_TILE, 0);
    lv_obj_set_style_border_color(c, ui_col(border_col), 0);
    lv_obj_set_style_border_opa(c, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(c, 10, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(c, 4, 0);
    lv_obj_add_flag(c, LV_OBJ_FLAG_CLICKABLE);
    if (cb) lv_obj_add_event_cb(c, cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *top = ui_box(c);
    lv_obj_set_size(top, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(top, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *tl = ui_box(top);
    lv_obj_set_size(tl, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(tl, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tl, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(tl, 8, 0);
    ui_icon(tl, sym, UI_ICON_MD, icon_col);
    ui_label(tl, title, UI_FONT_MD, UI_C_TEXT);
    ui_label(top, status, UI_FONT_XS, status_col);
    return c;
}

void ui_connectivityScreen_screen_init(void)
{
    conn_status_t st;
    gather(&st);

    ui_connectivityScreen = ui_screen_base();
    lv_obj_set_flex_flow(ui_connectivityScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ui_connectivityScreen, 8, 0);
    lv_obj_set_style_pad_row(ui_connectivityScreen, 8, 0);

    lv_obj_t *hdr = ui_nav_header(ui_connectivityScreen, _t("Conectividad"));
    bool telem = st.cloud_on && st.cloud_conn;
    ui_pill(hdr, telem ? _t("telemetría activa") : _t("telemetría inactiva"), UI_FONT_XS,
            telem ? UI_C_OK_DIM : UI_C_TEXT_MUTED,
            telem ? UI_C_OK_BG : UI_C_CARD_BG2,
            telem ? UI_C_OK_BORDER : UI_C_BORDER);

    lv_obj_t *grid = ui_box(ui_connectivityScreen);
    lv_obj_set_width(grid, LV_PCT(100));
    lv_obj_set_flex_grow(grid, 1);
    lv_obj_set_style_pad_column(grid, 8, 0);
    lv_obj_set_style_pad_row(grid, 8, 0);
    lv_obj_set_grid_dsc_array(grid, s_col, s_row);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);

    char buf[64];

    /* ---- Wi-Fi ---- */
    const char *wifi_status = !st.wifi_on ? _t("Desactivado")
                              : st.wifi_conn ? _t("Conectado") : _t("Desconectado");
    uint32_t wifi_col = !st.wifi_on ? UI_C_TEXT_MUTED : st.wifi_conn ? UI_C_OK : UI_C_WARN;
    lv_obj_t *wifi = conn_card(grid, UI_SYM_WIFI, wifi_col,
                               st.wifi_conn ? UI_C_OK_BORDER : UI_C_BORDER,
                               "Wi-Fi", wifi_status, wifi_col, 0, 0, ui_open_net_wifi_cb);
    ui_label(wifi, st.wifi_ssid[0] ? st.wifi_ssid : _t("Sin configurar"), UI_FONT_SM, 0xcfd3d9);
    if (st.wifi_on && st.wifi_conn) {
        lv_obj_t *sig = ui_box(wifi);
        lv_obj_set_size(sig, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(sig, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(sig, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_column(sig, 6, 0);
        lv_obj_t *bar = lv_bar_create(sig);
        lv_obj_set_flex_grow(bar, 1);
        lv_obj_set_height(bar, 5);
        lv_obj_set_style_bg_color(bar, ui_col(0x22262d), LV_PART_MAIN);
        lv_obj_set_style_bg_color(bar, ui_col(UI_C_OK), LV_PART_INDICATOR);
        lv_bar_set_value(bar, rssi_pct(st.wifi_rssi), LV_ANIM_OFF);
        snprintf(buf, sizeof(buf), "%d dBm", st.wifi_rssi);
        ui_label(sig, buf, UI_FONT_XS, UI_C_TEXT_MUTED);
        snprintf(buf, sizeof(buf), "IP %s · %s", st.wifi_ip[0] ? st.wifi_ip : "—",
                 st.wifi_dhcp ? "DHCP" : _t("Manual"));
        ui_label(wifi, buf, UI_FONT_XS, UI_C_TEXT_MUTED);
    } else {
        ui_label(wifi, st.wifi_dhcp ? "DHCP" : (st.wifi_ip[0] ? st.wifi_ip : _t("IP manual")),
                 UI_FONT_XS, UI_C_TEXT_MUTED);
    }

    /* ---- Nube · MQTT ---- */
    const char *cloud_status = !st.cloud_on ? _t("Desactivado")
                               : st.cloud_conn ? _t("Sincronizado") : _t("Sin conexión");
    uint32_t cloud_col = !st.cloud_on ? UI_C_TEXT_MUTED : st.cloud_conn ? UI_C_TEAL : UI_C_ALARM;
    lv_obj_t *cloud = conn_card(grid, UI_SYM_CLOUD_CHECK, cloud_col,
                                st.cloud_conn ? UI_C_TEAL : UI_C_BORDER,
                                "Nube · MQTT", cloud_status, cloud_col, 1, 0, ui_open_net_cloud_cb);
    ui_label(cloud, st.broker[0] ? st.broker : _t("Sin broker configurado"), UI_FONT_SM, 0xcfd3d9);
    lv_obj_t *sp1 = ui_box(cloud); lv_obj_set_flex_grow(sp1, 1); lv_obj_set_width(sp1, LV_PCT(100));
    ui_label(cloud, st.cloud_conn ? _t("Publicando telemetría · QoS 1")
                                  : _t("Reintentando conexión…"), UI_FONT_XS, UI_C_TEXT_MUTED);

    /* ---- Bluetooth LE ---- */
    const char *ble_status = !st.ble_on ? _t("Desactivado")
                             : st.ble_conn ? _t("Emparejado") : st.ble_adv ? _t("Anunciando")
                                                                           : _t("Inactivo");
    uint32_t ble_col = !st.ble_on ? UI_C_TEXT_MUTED : st.ble_conn ? UI_C_OK : UI_C_BLUE;
    lv_obj_t *ble = conn_card(grid, UI_SYM_BLUETOOTH, ble_col,
                              st.ble_on ? UI_C_BLUE : UI_C_BORDER,
                              "Bluetooth LE", ble_status, ble_col, 0, 1, ui_open_net_ble_cb);
    ui_label(ble, st.ble_name[0] ? st.ble_name : _t("Equipo sin nombre"), UI_FONT_SM, 0xcfd3d9);
    lv_obj_t *sp2 = ui_box(ble); lv_obj_set_flex_grow(sp2, 1); lv_obj_set_width(sp2, LV_PCT(100));
    ui_label(ble, st.ble_conn ? _t("App de servicio conectada")
                              : _t("Sin dispositivo emparejado"), UI_FONT_XS, UI_C_TEXT_MUTED);

    /* ---- Ethernet ---- */
    const char *eth_status = !st.eth_on ? _t("Desactivado")
                             : st.eth_up ? _t("Enlace activo") : _t("Sin cable");
    uint32_t eth_col = !st.eth_on ? UI_C_TEXT_MUTED : st.eth_up ? UI_C_OK : UI_C_TEXT_MUTED;
    lv_obj_t *eth = conn_card(grid, UI_SYM_NETWORK, eth_col,
                              st.eth_up ? UI_C_OK_BORDER : UI_C_BORDER,
                              "Ethernet", eth_status, eth_col, 1, 1, ui_open_net_eth_cb);
    ui_label(eth, _t("Respaldo cableado (failover)"), UI_FONT_SM, UI_C_TEXT_3);
    lv_obj_t *sp3 = ui_box(eth); lv_obj_set_flex_grow(sp3, 1); lv_obj_set_width(sp3, LV_PCT(100));
    ui_label(eth, st.eth_up ? (st.eth_ip[0] ? st.eth_ip : "RJ45 · 100 Mbps") : _t("Inactivo"),
             UI_FONT_XS, UI_C_TEXT_MUTED);
}

void ui_connectivityScreen_screen_destroy(void)
{
    if (ui_connectivityScreen) { lv_obj_del(ui_connectivityScreen); ui_connectivityScreen = NULL; }
}
