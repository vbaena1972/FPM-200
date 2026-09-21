#include "ui_bleAppScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_nav.h"
#include "storage.h"
#include <stdio.h>
#include <string.h>
#ifdef ESP_PLATFORM
#include "config_mode.h"
#include "transport_ble.h"
#include "esp_system.h"   // esp_restart
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

/* Ventana de anuncio: si nadie se conecta en este tiempo, se apaga el BLE y se
 * vuelve al dashboard (patrón del hermano). Si una app SE CONECTA y luego se
 * desconecta, también se cierra la pantalla y se vuelve al dashboard. */
#define BLEAPP_TIMEOUT_S  120

lv_obj_t *ui_bleAppScreen = NULL;

/* Estado vivo de la pantalla (se re-crea en cada open, así que se resetea). */
static lv_timer_t *s_tick        = NULL;
static lv_obj_t   *s_badge_box   = NULL;   /* badge BT grande (arriba derecha) */
static lv_obj_t   *s_badge_ic    = NULL;
static lv_obj_t   *s_status_ic   = NULL;   /* icono del pill de estado */
static lv_obj_t   *s_status_tx   = NULL;   /* texto del pill de estado */
static lv_obj_t   *s_count_tx    = NULL;   /* cuenta atrás / "conectado" */
static char        s_bname[32]   = "AXIRA";
static int         s_secs_left   = BLEAPP_TIMEOUT_S;
static bool        s_connected   = false;
static bool        s_ever_conn   = false;  /* ¿llegó a conectarse una app? */
static bool        s_leaving     = false;

static bool bleapp_is_connected(void)
{
#ifdef ESP_PLATFORM
    return transport_ble_is_connected();
#else
    return false;
#endif
}

/* PIN de 6 dígitos derivado del serial para el QR de emparejamiento.
 * El BLE es Just Works (sin passkey), así que este PIN es solo una guía visual
 * y para satisfacer el validador del QR en la app (exige exactamente 6 dígitos). */
static void bleapp_pair_pin(const AppConfig *c, char out[7])
{
    char digits[16];
    int n = 0;
    const char *s = (c && c->general.serial[0]) ? c->general.serial : "";
    for (; *s && n < 15; s++)
        if (*s >= '0' && *s <= '9') digits[n++] = *s;
    if (n == 0) { strcpy(out, "000000"); return; }
    int take = n < 6 ? n : 6;
    memset(out, '0', (size_t)(6 - take));       /* pad a la izquierda con ceros */
    memcpy(out + (6 - take), digits + (n - take), (size_t)take);
    out[6] = '\0';
}

/* Navegar al dashboard fuera del contexto del timer (no borrar el timer dentro de
 * su propio callback). Descarga esta pantalla -> config_mode_exit (apaga BLE). */
static void bleapp_goroot_async(void *arg) { (void)arg; ui_nav_show_root(); }
static void bleapp_leave_to_dashboard(void)
{
    if (s_leaving) return;
    s_leaving = true;
    if (s_tick) lv_timer_pause(s_tick);
    lv_async_call(bleapp_goroot_async, NULL);
}

static void bleapp_off_cb(lv_event_t *e)
{
    (void)e;
#ifdef ESP_PLATFORM
    /* "Apagar Bluetooth": PERSISTE bt.enabled=false y REINICIA el equipo. Igual
     * criterio que MedGuard (y que la pantalla Bluetooth LE). Antes este botón
     * solo volvía al dashboard (ui_nav_show_root) -> config_mode_exit detenía el
     * advertising, pero NimBLE no liberaba su RAM interna y la red/AWS no
     * reconectaban limpio: el BT quedaba "activo" hasta reiniciar. Al dejar
     * bt.enabled=false en NVS y reiniciar, el BT no vuelve a subir y la red/AWS
     * arrancan limpias. */
    AppConfig *cfg = appcfg_cache_peek();
    if (cfg) {
        cfg->bt.enabled   = false;
        cfg->bt.advertise = false;
        (void)appcfg_save(cfg);
        transport_ble_set_enabled(false);
        transport_ble_stop_adv();
    }
    ESP_LOGW("bleapp", "Apagar Bluetooth -> bt.enabled=false guardado; reiniciando "
                       "para liberar RAM y restaurar red/AWS");
    s_leaving = true;
    if (s_tick) lv_timer_pause(s_tick);
    vTaskDelay(pdMS_TO_TICKS(150));   /* deja asentar la escritura NVS */
    esp_restart();
#else
    ui_nav_show_root();   /* simulador: solo volver al dashboard */
#endif
}

/* Refleja el estado del enlace en el badge grande (arriba dcha) + el pill verde.
 * Conectado = cyan estándar (UI_C_TEAL) relleno. Desconectado = contorno tenue. */
static void bleapp_set_connected(bool conn)
{
    if (conn == s_connected && s_badge_box) return;
    s_connected = conn;

    if (s_badge_box) {
        lv_obj_set_style_bg_color(s_badge_box, ui_col(conn ? UI_C_TEAL : UI_C_CARD_BG2), 0);
        lv_obj_set_style_bg_opa(s_badge_box, conn ? LV_OPA_COVER : LV_OPA_40, 0);
        lv_obj_set_style_border_color(s_badge_box, ui_col(conn ? UI_C_TEAL : UI_C_BORDER), 0);
    }
    if (s_badge_ic)
        lv_obj_set_style_text_color(s_badge_ic, ui_col(conn ? 0xffffff : UI_C_TEXT_MUTED), 0);

    if (conn) {
        if (s_status_ic) { lv_label_set_text(s_status_ic, UI_SYM_BLUETOOTH);
                           lv_obj_set_style_text_color(s_status_ic, ui_col(UI_C_TEAL), 0); }
        if (s_status_tx) { lv_label_set_text(s_status_tx, _t("App conectada · configurando"));
                           lv_obj_set_style_text_color(s_status_tx, ui_col(UI_C_TEAL), 0); }
        if (s_count_tx)  { lv_label_set_text(s_count_tx, _t("Conectado — no se apagará solo")); }
    } else {
        char t[96];
        if (s_status_ic) { lv_label_set_text(s_status_ic, UI_SYM_RADAR_2);
                           lv_obj_set_style_text_color(s_status_ic, ui_col(UI_C_OK_SOFT), 0); }
        snprintf(t, sizeof(t), "Anunciando por BLE · visible como %s", s_bname);
        if (s_status_tx) { lv_label_set_text(s_status_tx, t);
                           lv_obj_set_style_text_color(s_status_tx, ui_col(UI_C_OK_SOFT), 0); }
    }
}

static void bleapp_tick_cb(lv_timer_t *t)
{
    (void)t;
    if (s_leaving) return;

    bool conn = bleapp_is_connected();
    bleapp_set_connected(conn);

    if (conn) { s_ever_conn = true; return; }   /* conectado: sin cuenta atrás */

    /* La app se conectó y luego se desconectó -> cerrar, apagar BLE y dashboard. */
    if (s_ever_conn) { bleapp_leave_to_dashboard(); return; }

    /* Nunca se conectó: cuenta atrás de auto-apagado. */
    if (s_secs_left > 0) s_secs_left--;
    if (s_count_tx) {
        char b[56];
        snprintf(b, sizeof(b), "Se apaga en %d s si no te conectas", s_secs_left);
        lv_label_set_text(s_count_tx, b);
    }
    if (s_secs_left <= 0) bleapp_leave_to_dashboard();
}

static void bleapp_loaded_cb(lv_event_t *e)
{
    (void)e;
#ifdef ESP_PLATFORM
    config_mode_enter();
#endif
    s_secs_left = BLEAPP_TIMEOUT_S;
    s_connected = false;
    s_ever_conn = false;
    s_leaving   = false;
    if (!s_tick) s_tick = lv_timer_create(bleapp_tick_cb, 1000, NULL);
    else         lv_timer_resume(s_tick);
}
static void bleapp_unloaded_cb(lv_event_t *e)
{
    (void)e;
    if (s_tick) { lv_timer_delete(s_tick); s_tick = NULL; }
#ifdef ESP_PLATFORM
    config_mode_exit();
#endif
}

void ui_bleAppScreen_screen_init(void)
{
    /* Identidad del equipo para la etiqueta y el QR (nombre BLE + PIN guía). */
    const AppConfig *cfg = appcfg_cache_peek();
    const char *bname = (cfg && cfg->bt.legacy.name[0]) ? cfg->bt.legacy.name : "AXIRA";
    snprintf(s_bname, sizeof(s_bname), "%s", bname);
    char pin[7];
    bleapp_pair_pin(cfg, pin);

    /* Pantalla 480x320 (landscape). Columna: [header] + [body]. */
    ui_bleAppScreen = ui_screen_base();
    lv_obj_set_flex_flow(ui_bleAppScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_hor(ui_bleAppScreen, 14, 0);
    lv_obj_set_style_pad_ver(ui_bleAppScreen, 10, 0);
    lv_obj_set_style_pad_row(ui_bleAppScreen, 8, 0);
    lv_obj_set_flex_align(ui_bleAppScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    /* ---- Header: [volver] [título] .... [badge BT grande derecha] ---- */
    lv_obj_t *hdr = ui_box(ui_bleAppScreen);
    lv_obj_set_size(hdr, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(hdr, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(hdr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(hdr, 10, 0);

    lv_obj_t *back = ui_icon_badge(hdr, UI_SYM_ARROW_LEFT, UI_ICON_SM, 0xcfd3d9, UI_C_CARD_BG, 32);
    lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_ext_click_area(back, 6);
    lv_obj_add_event_cb(back, ui_nav_back_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *ttl = ui_label(hdr, _t("Configurar por app"), UI_FONT_LG, UI_C_TEXT);
    lv_obj_set_flex_grow(ttl, 1);   /* empuja el badge BT a la derecha */

    /* Badge BT grande (arriba dcha): indicador claro conectado/desconectado. */
    s_badge_box = lv_obj_create(hdr);
    ui_kill_scroll(s_badge_box);
    lv_obj_set_size(s_badge_box, 46, 46);
    lv_obj_set_style_radius(s_badge_box, 12, 0);
    lv_obj_set_style_bg_color(s_badge_box, ui_col(UI_C_CARD_BG2), 0);
    lv_obj_set_style_bg_opa(s_badge_box, LV_OPA_40, 0);
    lv_obj_set_style_border_width(s_badge_box, 2, 0);
    lv_obj_set_style_border_color(s_badge_box, ui_col(UI_C_BORDER), 0);
    s_badge_ic = ui_icon(s_badge_box, UI_SYM_BLUETOOTH, UI_ICON_LG, UI_C_TEXT_MUTED);
    lv_obj_center(s_badge_ic);

    /* ---- Body: info+estado (izq) + QR (der) ---- */
    lv_obj_t *body = ui_box(ui_bleAppScreen);
    lv_obj_set_width(body, LV_PCT(100));
    lv_obj_set_flex_grow(body, 1);
    lv_obj_set_flex_flow(body, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(body, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_column(body, 16, 0);

    lv_obj_t *left = ui_box(body);
    lv_obj_set_flex_grow(left, 1);
    lv_obj_set_height(left, LV_PCT(100));
    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(left, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(left, 8, 0);

    lv_obj_t *desc = ui_label(left, _t("Wi-Fi, contraseñas, certificados AWS, IP y nombre del equipo se "
                              "escriben desde la app de servicio."), UI_FONT_SM, UI_C_TEXT_3);
    lv_obj_set_width(desc, LV_PCT(100));
    lv_label_set_long_mode(desc, LV_LABEL_LONG_WRAP);

    /* Pill de estado vivo (la "tarjeta verde"): Anunciando / Conectado. */
    lv_obj_t *adv = ui_box(left);
    lv_obj_set_size(adv, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(adv, ui_col(UI_C_OK_BG), 0);
    lv_obj_set_style_bg_opa(adv, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(adv, UI_RADIUS_SM, 0);
    lv_obj_set_style_border_width(adv, 1, 0);
    lv_obj_set_style_border_color(adv, ui_col(UI_C_OK_BORDER), 0);
    lv_obj_set_style_pad_hor(adv, 12, 0);
    lv_obj_set_style_pad_ver(adv, 8, 0);
    lv_obj_set_flex_flow(adv, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(adv, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(adv, 8, 0);
    s_status_ic = ui_icon(adv, UI_SYM_RADAR_2, UI_ICON_SM, UI_C_OK_SOFT);
    char advtxt[96];
    snprintf(advtxt, sizeof(advtxt), "Anunciando por BLE · visible como %s", bname);
    s_status_tx = ui_label(adv, advtxt, UI_FONT_SM, UI_C_OK_SOFT);
    lv_obj_set_flex_grow(s_status_tx, 1);
    lv_label_set_long_mode(s_status_tx, LV_LABEL_LONG_WRAP);

    /* Cuenta atrás de auto-apagado (se actualiza en el tick de 1 s). */
    s_count_tx = ui_label(left, "Se apaga en 120 s si no te conectas", UI_FONT_XS, UI_C_TEXT_MUTED);

    /* Espaciador que empuja el botón grande al fondo de la columna. */
    lv_obj_t *sp = ui_box(left);
    lv_obj_set_width(sp, LV_PCT(100));
    lv_obj_set_flex_grow(sp, 1);

    /* Botón GRANDE de apagado voluntario del Bluetooth (ancho completo). */
    lv_obj_t *off = ui_box(left);
    lv_obj_set_size(off, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_radius(off, UI_RADIUS_SM, 0);
    lv_obj_set_style_bg_color(off, ui_col(UI_C_ALARM_BG), 0);
    lv_obj_set_style_bg_opa(off, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(off, 1, 0);
    lv_obj_set_style_border_color(off, ui_col(UI_C_ALARM_BORDER), 0);
    lv_obj_set_style_pad_hor(off, 14, 0);
    lv_obj_set_style_pad_ver(off, 12, 0);
    lv_obj_set_flex_flow(off, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(off, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(off, 8, 0);
    lv_obj_add_flag(off, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(off, bleapp_off_cb, LV_EVENT_CLICKED, NULL);
    ui_icon(off, UI_SYM_BLUETOOTH, UI_ICON_MD, UI_C_ALARM_SOFT);
    ui_label(off, _t("Apagar Bluetooth"), UI_FONT_MD, UI_C_ALARM_SOFT);

    /* ---- derecha: QR ---- */
    lv_obj_t *right = ui_box(body);
    lv_obj_set_size(right, 132, LV_PCT(100));
    lv_obj_set_flex_flow(right, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(right, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(right, 8, 0);
    /* QR real de emparejamiento. La app (mobile_scanner) espera el formato
     * medguard://pair?name=<nombre BLE>&pin=<6 dígitos> (QrPairingData.parse). */
    lv_obj_t *qr = lv_qrcode_create(right);
    lv_qrcode_set_size(qr, 112);
    lv_qrcode_set_dark_color(qr, ui_col(0x0f1115));
    lv_qrcode_set_light_color(qr, ui_col(0xffffff));
    lv_obj_set_style_border_width(qr, 5, 0);
    lv_obj_set_style_border_color(qr, ui_col(0xffffff), 0);
    lv_obj_set_style_radius(qr, UI_RADIUS_TILE, 0);
    lv_obj_set_style_clip_corner(qr, true, 0);
    char uri[96];
    snprintf(uri, sizeof(uri), "medguard://pair?name=%s&pin=%s", bname, pin);
    lv_qrcode_update(qr, uri, strlen(uri));
    lv_obj_t *qcap = ui_label(right, _t("Escanea para emparejar"), UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_width(qcap, LV_PCT(100));
    lv_obj_set_style_text_align(qcap, LV_TEXT_ALIGN_CENTER, 0);

    /* Estado inicial del badge (desconectado). */
    s_connected = true; bleapp_set_connected(false);

    /* Entrar/salir del modo configuración al mostrar/ocultar esta pantalla. */
    lv_obj_add_event_cb(ui_bleAppScreen, bleapp_loaded_cb, LV_EVENT_SCREEN_LOADED, NULL);
    lv_obj_add_event_cb(ui_bleAppScreen, bleapp_unloaded_cb, LV_EVENT_SCREEN_UNLOADED, NULL);
}

void ui_bleAppScreen_screen_destroy(void)
{
    if (s_tick) { lv_timer_delete(s_tick); s_tick = NULL; }
    s_badge_box = s_badge_ic = s_status_ic = s_status_tx = s_count_tx = NULL;
    if (ui_bleAppScreen) { lv_obj_del(ui_bleAppScreen); ui_bleAppScreen = NULL; }
}
