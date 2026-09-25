#include "ui_statusbar_controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "ui.h"               /* extern iconos de estado + tema/iconos */
#include "wifi_mgr.h"
#include "eth_mgr.h"
#include "transport_ble.h"
#include "ui_wifi_main_icon.h"
#include "esp_log.h"
#include <stdbool.h>
#include <stdatomic.h>

static const char *TAG = "ui_statusbar";

/* Visibilidad (A/B) según configuración */
static bool s_vis_wifi = false, s_vis_eth = false, s_vis_bt = false, s_vis_cloud = false;
// Repaint only on a real change (or when forced by set_enabled/request_refresh).
// Re-applying the same styles every 500 ms made LVGL redraw the status bar.
static bool s_force_repaint = true;

static lv_timer_t *s_watch_timer = NULL;
static lv_timer_t *s_activity_timer = NULL;

#define CLOUD_PULSE_HOLD_MS 650
#define ACTIVITY_TIMER_MS    80

/* mqtt_event() corre fuera de LVGL: solo escribe este flag atomico. */
static atomic_bool s_cloud_activity_pending = ATOMIC_VAR_INIT(false);
static bool s_cloud_pulse_active = false;
static uint32_t s_cloud_pulse_tick = 0;

/* estados previos para no repintar innecesariamente */
static bool s_prev_eth_link = false;
static bool s_prev_wifi_link = false;
static int  s_prev_wifi_rssi = 0;
static bool s_prev_ble_conn = false;
static bool s_prev_ble_adv = false;
static bool s_prev_cloud_conn = false;

static inline void set_visible(lv_obj_t *o, bool on)
{
    if (!o) return;
    if (on) lv_obj_clear_flag(o, LV_OBJ_FLAG_HIDDEN);
    else    lv_obj_add_flag(o, LV_OBJ_FLAG_HIDDEN);
}

static inline void set_glyph(lv_obj_t *o, const char *sym, uint32_t color)
{
    if (!o || !lv_obj_is_valid(o)) return;
    lv_label_set_text(o, sym);
    lv_obj_set_style_text_color(o, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void apply_cloud_visual(bool connected)
{
    if (!ui_cloudStatusMain || !lv_obj_is_valid(ui_cloudStatusMain)) return;

    bool transmitting = connected && s_cloud_pulse_active;
    /* Igual que el proyecto hermano (MedGuard): un SOLO glifo — fa-cloud
     * (U+F0C2) en reposo y fa-cloud-upload-alt (U+F382, nube con flecha) al
     * transmitir. F382 SI esta compilado en mg_font_18 (unicode_list, offset
     * 0xD36F), asi que renderiza como nube-con-flecha, no el chevron superpuesto. */
    set_glyph(ui_cloudStatusMain, transmitting ? UI_SYM_CLOUD_UP : UI_SYM_CLOUD_SOLID,
              transmitting ? UI_C_TEAL : (connected ? UI_C_INFO : UI_C_ALARM));

    /* La flecha chevron superpuesta queda obsoleta: siempre oculta. */
    if (ui_cloudTxArrowMain && lv_obj_is_valid(ui_cloudTxArrowMain))
        set_visible(ui_cloudTxArrowMain, false);
}

static void activity_timer_cb(lv_timer_t *t)
{
    (void)t;
    bool repaint = false;

    if (atomic_exchange_explicit(&s_cloud_activity_pending, false,
                                 memory_order_acq_rel))
    {
        s_cloud_pulse_tick = lv_tick_get();
        s_cloud_pulse_active = true;
        repaint = true;
    }

    if (s_cloud_pulse_active &&
        lv_tick_elaps(s_cloud_pulse_tick) >= CLOUD_PULSE_HOLD_MS)
    {
        s_cloud_pulse_active = false;
        repaint = true;
    }

    if (repaint && s_vis_cloud)
        apply_cloud_visual(s_prev_cloud_conn);
}

static void statusbar_timer_cb(lv_timer_t *t)
{
    (void)t;

    wifi_netinfo_t wi = {0};
    (void)wifi_mgr_get_netinfo(&wi);
    bool eth = eth_mgr_is_up();
    bool ble = transport_ble_is_connected();
    bool ble_adv = transport_ble_is_advertising();
    extern bool cloud_mgr_connected(void);
    bool cloud = cloud_mgr_connected();

    bool changed =
        (eth != s_prev_eth_link) ||
        (wi.connected != s_prev_wifi_link) ||
        (ui_wifi_rssi_to_bars(wi.rssi_dbm) != ui_wifi_rssi_to_bars(s_prev_wifi_rssi)) ||
        (ble != s_prev_ble_conn) ||
        (ble_adv != s_prev_ble_adv) ||
        (cloud != s_prev_cloud_conn);

    if (!changed && !s_force_repaint)
        return;
    s_force_repaint = false;

    s_prev_eth_link = eth;
    s_prev_wifi_link = wi.connected;
    s_prev_wifi_rssi = wi.rssi_dbm;
    s_prev_ble_conn = ble;
    s_prev_ble_adv = ble_adv;
    s_prev_cloud_conn = cloud;
    if (!cloud) s_cloud_pulse_active = false;

    /* === BLUETOOTH ===
     * Visible si está configurado, anunciando o conectado (estado vivo). Verde =
     * conectado, azul = anunciando, atenuado = inactivo. */
    bool bt_show = s_vis_bt || ble_adv || ble;
    set_visible(ui_bluetoothStatusMain, bt_show);
    if (bt_show)
        set_glyph(ui_bluetoothStatusMain, UI_SYM_BLUETOOTH,
                  ble ? UI_C_OK : (ble_adv ? UI_C_BLUE : UI_C_TEXT_MUTED));

    /* === ETHERNET === */
    set_visible(ui_ethernetStatusMain, s_vis_eth);
    if (s_vis_eth)
        set_glyph(ui_ethernetStatusMain, UI_SYM_NETWORK, eth ? UI_C_OK : UI_C_TEXT_MUTED);

    /* === WIFI (prioridad Ethernet: si hay cable, wifi atenuado) === */
    set_visible(ui_wifiStatusMain, s_vis_wifi);
    if (s_vis_wifi)
    {
        if (eth || !wi.connected)
            set_glyph(ui_wifiStatusMain, UI_SYM_WIFI, UI_C_TEXT_MUTED);
        else
        {
            int bars = ui_wifi_rssi_to_bars(wi.rssi_dbm);
            uint32_t c = (bars >= 2) ? UI_C_OK : UI_C_WARN;
            set_glyph(ui_wifiStatusMain, UI_SYM_WIFI, c);
        }
    }

    /* === CLOUD === */
    if (ui_cloudStatusBoxMain && lv_obj_is_valid(ui_cloudStatusBoxMain))
    {
        set_visible(ui_cloudStatusBoxMain, s_vis_cloud);
        if (s_vis_cloud)
            apply_cloud_visual(cloud);
    }

    if (ui_statusMainComm && lv_obj_is_valid(ui_statusMainComm))
        lv_obj_invalidate(ui_statusMainComm);
}

void ui_statusbar_request_refresh(void)
{
    s_force_repaint = true;
    if (s_watch_timer) lv_timer_ready(s_watch_timer);
}

void ui_statusbar_signal_cloud_activity(void)
{
    atomic_store_explicit(&s_cloud_activity_pending, true, memory_order_release);
}

void ui_statusbar_controller_init(void)
{
    if (!s_watch_timer)
        s_watch_timer = lv_timer_create(statusbar_timer_cb, 500, NULL);
    if (!s_activity_timer)
        s_activity_timer = lv_timer_create(activity_timer_cb, ACTIVITY_TIMER_MS, NULL);
    ESP_LOGI(TAG, "statusbar controller iniciado");
    ui_statusbar_request_refresh();
}

void ui_statusbar_controller_deinit(void)
{
    if (s_watch_timer)
    {
        lv_timer_del(s_watch_timer);
        s_watch_timer = NULL;
    }
    if (s_activity_timer)
    {
        lv_timer_del(s_activity_timer);
        s_activity_timer = NULL;
    }
    atomic_store_explicit(&s_cloud_activity_pending, false, memory_order_release);
    s_cloud_pulse_active = false;
    s_vis_wifi = s_vis_eth = s_vis_bt = s_vis_cloud = false;
}

void ui_statusbar_set_enabled(bool wifi, bool eth, bool bt, bool cloud)
{
    s_vis_wifi = wifi;
    s_vis_eth = eth;
    s_vis_bt = bt;
    s_vis_cloud = cloud;
    ui_statusbar_request_refresh();
}
