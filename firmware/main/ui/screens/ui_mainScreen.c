#include "flow_meter.h"
#include "ui_mainScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_cfg.h"
#include "ui.h"
#include "alarm_mgr.h"
#include "sensors_runtime.h" // sensors_runtime_get_debug (diagnostico dashboard)
#include <stdio.h>
#include <string.h>
#include <math.h>

// Preserve LVGL's local invalidation only when the visible text changes.
static void set_label_if_changed(lv_obj_t *label, const char *text)
{
    if (label && text && strcmp(lv_label_get_text(label), text) != 0)
        lv_label_set_text(label, text);
}

/* ============================================================
 *  Pantalla principal (mockups 3a/3b/4b/4c)
 *  Un único screen con 4 estados visuales:
 *   NORMAL / ADVERTENCIA / ALARMA / ALARMA-SILENCIADA
 * ============================================================ */

/* --- Constantes de escala ---
 * El fondo de escala del eje de flujo ahora viene de AppConfig
 * (sensors.flow_fullscale_lpm, calibrable sin recompilar); este define
 * queda solo como respaldo si la config trae un valor inválido. */
#define FLOW_AXIS_FULLSCALE_LPM   100.0f  /* respaldo (L/min) */
#define FLOW_HIGH_ZONE_FRAC       0.80f   /* inicio de zona "alta" (ámbar) en la barra */
#define PRESS_AXIS_FULLSCALE_KPA  1378.951f /* fixed 0..200 psi pressure sensor */

/* --- Globals del contrato (ui_bind / main.c) --- */
lv_obj_t *ui_mainScreen        = NULL;
lv_obj_t *ui_statusMainComm    = NULL;
lv_obj_t *ui_bluetoothStatusMain = NULL;
lv_obj_t *ui_wifiStatusMain    = NULL;
lv_obj_t *ui_ethernetStatusMain = NULL;
lv_obj_t *ui_cloudStatusBoxMain = NULL;
lv_obj_t *ui_cloudStatusMain   = NULL;
lv_obj_t *ui_cloudTxArrowMain  = NULL;
lv_obj_t *ui_alarmBtnMain      = NULL;

#define DATA_PULSE_GAP_MS      800
#define DATA_PULSE_HOLD_MS     180
#define DATA_ACTIVITY_TIMER_MS  80

static lv_obj_t *s_data_chip = NULL;
static lv_obj_t *s_data_dot = NULL;
static lv_obj_t *s_data_label = NULL;
static lv_timer_t *s_data_activity_timer = NULL;
static uint32_t s_data_pulse_tick = 0;
static uint32_t s_data_last_pulse_tick = 0;

/* Suavizado del VALOR MOSTRADO (EMA). El número refresca a 5 Hz sobre lecturas
 * instantáneas ruidosas (FS7 cerca de 0 salta 0.4–1.0 L/min); con 2 decimales se
 * vería nervioso. Alpha 0.25 = estable y sigue un cambio real en ~0.5–1 s. Solo
 * afecta la presentación; la máquina de alarmas usa el valor CRUDO (no se suaviza
 * para no enmascarar picos). Se reinicia (NAN) si no hay dato válido. */
#define DISP_EMA_ALPHA 0.25f
static float s_p_disp_ema = NAN;
static float s_f_disp_ema = NAN;
static bool s_data_seen = false;
static bool s_data_pulse_active = false;
static int64_t s_last_activity_sample_ts = -1;

/* --- Tarjeta de métrica (presión/flujo) --- */
typedef struct {
    lv_obj_t *card;
    lv_obj_t *title;
    lv_obj_t *icon;
    lv_obj_t *value;
    lv_obj_t *unit;
    lv_obj_t *bar;
    lv_obj_t *seg_lo, *seg_mid, *seg_hi;
    lv_obj_t *tick_lo, *tick_hi;
    lv_obj_t *marker;
    lv_obj_t *ax_left, *ax_mid, *ax_right;
    lv_obj_t *pill;
    lv_obj_t *pill_state;
    lv_obj_t *pill_mm;
    lv_obj_t *dbg;   /* linea de diagnostico TEMPORAL (atm / SFM / delta) */
} metric_card_t;

static metric_card_t s_press, s_flow;

/* --- Consumo del día: integral del flujo (∫ L/min · dt → m³) ---
 * Se integra entre timestamps de muestra (ts_ms): si ui_refresh repite la misma
 * muestra, dt=0 y no hay doble conteo. Vive en RAM (se pierde al reiniciar). */
static char    s_clock_prev[8] = "";

/* --- Cabecera / banner / consumo / gas --- */
static lv_obj_t *s_brand_lbl, *s_sub_lbl, *s_clock_lbl, *s_date_lbl;
static lv_obj_t *s_banner, *s_banner_icon, *s_banner_txt, *s_banner_sub, *s_banner_right, *s_banner_right_ic, *s_banner_right_tx;
static lv_obj_t *s_consumo_val, *s_consumo_badge, *s_consumo_badge_tx;
static lv_obj_t *s_consumo_cap = NULL;   /* caption "CONSUMO" (retraducible) */
static lv_obj_t *s_consumo_sub = NULL;   /* subtítulo "· desde 00:00 ..." */
static lv_obj_t *s_gas_banner, *s_gas_lbl;
static lv_obj_t *s_menu_pop = NULL;
static lv_obj_t *s_alarm_overlay = NULL;
static sensor_sample_t s_alarm_last;
static bool s_alarm_have_last = false;
static alarm_clinical_state_t s_alarm_state = ALARM_STATE_NORMAL;
static bool s_alarm_muted = false;
static uint32_t s_alarm_faults = 0;


/* ---------- helpers ---------- */
static void set_bg(lv_obj_t *o, uint32_t hex)
{ lv_obj_set_style_bg_color(o, ui_col(hex), LV_PART_MAIN | LV_STATE_DEFAULT); }
static void set_border(lv_obj_t *o, uint32_t hex)
{ lv_obj_set_style_border_color(o, ui_col(hex), LV_PART_MAIN | LV_STATE_DEFAULT); }
static void set_txt_color(lv_obj_t *o, uint32_t hex)
{ lv_obj_set_style_text_color(o, ui_col(hex), LV_PART_MAIN | LV_STATE_DEFAULT); }

static float clampf(float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); }

static void apply_data_activity_visual(void)
{
    if (!s_data_chip || !lv_obj_is_valid(s_data_chip)) return;

    uint32_t color = s_data_pulse_active ? UI_C_TEAL : UI_C_TEXT_MUTED;
    lv_obj_set_style_border_color(s_data_chip, ui_col(color), 0);
    lv_obj_set_style_bg_color(s_data_dot, ui_col(color), 0);
    lv_obj_set_style_bg_opa(s_data_dot,
                            s_data_pulse_active ? LV_OPA_COVER : LV_OPA_30, 0);
    lv_obj_set_style_text_color(s_data_label, ui_col(color), 0);
}

static void data_activity_timer_cb(lv_timer_t *t)
{
    (void)t;
    if (s_data_pulse_active &&
        lv_tick_elaps(s_data_pulse_tick) >= DATA_PULSE_HOLD_MS)
    {
        s_data_pulse_active = false;
        apply_data_activity_visual();
    }
}

/* Posiciona un overlay (marker/tick) dentro de la barra según una fracción 0..1 */
static void position_overlay(lv_obj_t *obj, lv_obj_t *bar, float frac, lv_coord_t obj_w)
{
    lv_coord_t w = lv_obj_get_content_width(bar);
    if (w <= 0) w = 150; /* fallback antes del primer layout */
    lv_coord_t x = (lv_coord_t)(clampf(frac, 0.f, 1.f) * (float)(w - obj_w));
    lv_obj_align(obj, LV_ALIGN_LEFT_MID, x, 0);
}

/* ---------- builder de la tarjeta de métrica ---------- */
static void build_metric_card(lv_obj_t *parent, metric_card_t *m,
                              const char *title, const char *icon_sym, const char *unit)
{
    m->card = ui_card(parent);
    lv_obj_set_flex_grow(m->card, 1);
    lv_obj_set_height(m->card, LV_PCT(100));
    lv_obj_set_flex_flow(m->card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(m->card, 6, 0);
    lv_obj_set_style_pad_row(m->card, 2, 0);

    /* fila título + icono */
    lv_obj_t *row = ui_box(m->card);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    m->title = ui_label(row, title, UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_style_text_letter_space(m->title, 1, 0);
    m->icon = ui_icon(row, icon_sym, UI_ICON_MD, UI_C_TEXT_MUTED);

    /* valor grande + unidad */
    lv_obj_t *vrow = ui_box(m->card);
    lv_obj_set_size(vrow, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(vrow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(vrow, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_column(vrow, 5, 0);
    m->value = ui_label(vrow, "--", UI_FONT_HUGE, UI_C_TEXT_STRONG);
    m->unit  = ui_label(vrow, unit, UI_FONT_LG, UI_C_TEXT_2);

    /* barra de rango (flex de 3 segmentos) */
    m->bar = ui_box(m->card);
    lv_obj_set_size(m->bar, LV_PCT(100), 13);
    lv_obj_set_style_radius(m->bar, 7, 0);
    lv_obj_set_style_clip_corner(m->bar, true, 0);
    set_bg(m->bar, UI_C_BORDER_SOFT);
    lv_obj_set_style_bg_opa(m->bar, LV_OPA_COVER, 0);
    lv_obj_set_flex_flow(m->bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(m->bar, 0, 0);

    m->seg_lo  = ui_box(m->bar); lv_obj_set_height(m->seg_lo, LV_PCT(100));
    m->seg_mid = ui_box(m->bar); lv_obj_set_height(m->seg_mid, LV_PCT(100));
    m->seg_hi  = ui_box(m->bar); lv_obj_set_height(m->seg_hi, LV_PCT(100));
    lv_obj_set_style_bg_opa(m->seg_lo, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(m->seg_mid, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_opa(m->seg_hi, LV_OPA_COVER, 0);

    /* ticks de umbral (overlay) */
    m->tick_lo = ui_box(m->bar);
    m->tick_hi = ui_box(m->bar);
    for (int i = 0; i < 2; i++) {
        lv_obj_t *t = i ? m->tick_hi : m->tick_lo;
        lv_obj_add_flag(t, LV_OBJ_FLAG_IGNORE_LAYOUT);
        lv_obj_set_size(t, 2, 13);
        set_bg(t, UI_C_OK_SOFT);
        lv_obj_set_style_bg_opa(t, LV_OPA_COVER, 0);
    }

    /* marcador (overlay) — cabe dentro de la barra (LVGL recorta hijos al padre) */
    m->marker = ui_box(m->bar);
    lv_obj_add_flag(m->marker, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(m->marker, 9, 13);
    lv_obj_set_style_radius(m->marker, 3, 0);
    set_bg(m->marker, UI_C_OK);
    lv_obj_set_style_bg_opa(m->marker, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(m->marker, ui_col(UI_C_CARD_BG), 0);
    lv_obj_set_style_border_width(m->marker, 2, 0);

    /* eje: 0 · centro · max */
    lv_obj_t *axrow = ui_box(m->card);
    lv_obj_set_size(axrow, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(axrow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(axrow, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    m->ax_left  = ui_label(axrow, "0", UI_FONT_XS, UI_C_TEXT_MUTED);
    m->ax_mid   = ui_label(axrow, "", UI_FONT_XS, UI_C_OK_DIM);
    m->ax_right = ui_label(axrow, "", UI_FONT_XS, UI_C_TEXT_MUTED);

    /* espaciador flexible: empuja la píldora al fondo */
    lv_obj_t *spacer = ui_box(m->card);
    lv_obj_set_width(spacer, LV_PCT(100));
    lv_obj_set_flex_grow(spacer, 1);

    /* pill de estado (empuja al fondo) */
    m->pill = ui_box(m->card);
    lv_obj_set_width(m->pill, LV_PCT(100));
    lv_obj_set_height(m->pill, LV_SIZE_CONTENT);
    lv_obj_set_style_margin_top(m->pill, 2, 0);
    lv_obj_set_flex_grow(m->pill, 0);
    lv_obj_set_style_radius(m->pill, UI_RADIUS_PILL, 0);
    lv_obj_set_style_border_width(m->pill, 1, 0);
    lv_obj_set_style_pad_hor(m->pill, 10, 0);
    lv_obj_set_style_pad_ver(m->pill, 3, 0);
    lv_obj_set_flex_flow(m->pill, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(m->pill, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    m->pill_state = ui_label(m->pill, _t("NORMAL"), UI_FONT_SM, UI_C_OK);
    lv_obj_set_style_text_letter_space(m->pill_state, 1, 0);
    m->pill_mm = ui_label(m->pill, "24H -- / --", UI_FONT_XS, UI_C_TEXT_MUTED);

    /* Linea de diagnostico TEMPORAL (se elimina en produccion): muestra los
     * valores crudos de los sensores de referencia para no leer los logs. */
    m->dbg = ui_label(m->card, "", UI_FONT_XS, UI_C_TEXT_MUTED);
    lv_obj_set_width(m->dbg, LV_PCT(100));
}

/* Actualiza una tarjeta con valor + zonas + estado */
typedef enum { CARD_OK = 0, CARD_WARN, CARD_ALARM } card_state_t;

/* Decimales del valor grande del dashboard (0 o 1), lo fija ui_main_update
 * desde cfg->sensors.decimals antes de refrescar las tarjetas. */
static int s_decimals = 0;

/* Decimales minimos por unidad (misma politica que el proyecto hermano: las
 * unidades "pequenas" tras convertir conservan resolucion). El ajuste global de
 * decimales (0/1) actua como base y estas subunidades lo elevan si hace falta. */
static int press_unit_min_dec(const char *u)
{
    if (u && strcmp(u, "mpa") == 0) return 3;  /* MPa: muy pequeño */
    if (u && strcmp(u, "bar") == 0) return 2;  /* bar: valores <10 */
    if (u && strcmp(u, "psi") == 0) return 2;  /* psi: gauge cerca de 0 -> evita "-0" */
    return 1; /* kpa y otros: al menos 1 decimal (antes 0 -> se veía entero) */
}
static int flow_unit_min_dec(const char *u)
{
    if (u && strcmp(u, "m3h") == 0) return 2;
    return 2; /* lpm/slpm/sccm: caudales pequeños (0.98) -> 2 decimales */
}

static void update_metric_card(metric_card_t *m, float value_disp, float frac,
                               float safe_lo_frac, float safe_hi_frac, bool low_zone,
                               bool show_lo, bool show_hi, int dec,
                               card_state_t st, const char *state_txt,
                               float mn_disp, float mx_disp)
{
    char buf[48];
    snprintf(buf, sizeof(buf), "%.*f", dec, value_disp);
    set_label_if_changed(m->value, buf);

    /* colores por estado */
    uint32_t accent = (st == CARD_ALARM) ? UI_C_ALARM_SOFT : (st == CARD_WARN ? UI_C_WARN : UI_C_OK);
    uint32_t valcol = (st == CARD_OK) ? UI_C_TEXT_STRONG : accent;
    set_txt_color(m->value, valcol);
    set_txt_color(m->title, (st == CARD_OK) ? UI_C_TEXT_3 : accent);
    set_txt_color(m->icon,  (st == CARD_OK) ? UI_C_TEXT_MUTED : accent);
    set_border(m->card, (st == CARD_OK) ? UI_C_BORDER : accent);
    lv_obj_set_style_border_opa(m->card, (st == CARD_OK) ? LV_OPA_60 : LV_OPA_COVER, 0);

    /* zonas: pct de cada segmento. Solo se dibuja la banda de un limite si ese
     * limite esta HABILITADO (show_lo/show_hi); si no, el segmento va a 0. */
    bool draw_lo = (show_lo && low_zone);
    int lo = draw_lo ? (int)(clampf(safe_lo_frac, 0.f, 1.f) * 100.f) : 0;
    int hi = show_hi ? (int)(clampf(1.f - safe_hi_frac, 0.f, 1.f) * 100.f) : 0;
    int mid = 100 - lo - hi; if (mid < 0) mid = 0;
    lv_obj_set_width(m->seg_lo, LV_PCT(lo));
    lv_obj_set_width(m->seg_mid, LV_PCT(mid));
    lv_obj_set_width(m->seg_hi, LV_PCT(hi));
    set_bg(m->seg_lo, UI_C_ALARM);   lv_obj_set_style_bg_opa(m->seg_lo, LV_OPA_20, 0);
    set_bg(m->seg_mid, UI_C_OK);     lv_obj_set_style_bg_opa(m->seg_mid, LV_OPA_30, 0);
    set_bg(m->seg_hi, low_zone ? UI_C_ALARM : UI_C_WARN);
    lv_obj_set_style_bg_opa(m->seg_hi, LV_OPA_20, 0);
    /* Ocultar del todo el segmento de un limite deshabilitado (no basta con
     * ancho 0%: garantizamos que no quede ninguna franja dibujada). */
    if (draw_lo) lv_obj_clear_flag(m->seg_lo, LV_OBJ_FLAG_HIDDEN);
    else         lv_obj_add_flag(m->seg_lo, LV_OBJ_FLAG_HIDDEN);
    if (show_hi) lv_obj_clear_flag(m->seg_hi, LV_OBJ_FLAG_HIDDEN);
    else         lv_obj_add_flag(m->seg_hi, LV_OBJ_FLAG_HIDDEN);

    /* marcador color + posición */
    set_bg(m->marker, accent);
    position_overlay(m->marker, m->bar, frac, 9);
    if (low_zone && show_lo) {
        lv_obj_clear_flag(m->tick_lo, LV_OBJ_FLAG_HIDDEN);
        position_overlay(m->tick_lo, m->bar, safe_lo_frac, 2);
    } else {
        lv_obj_add_flag(m->tick_lo, LV_OBJ_FLAG_HIDDEN);
    }
    if (show_hi) {
        lv_obj_clear_flag(m->tick_hi, LV_OBJ_FLAG_HIDDEN);
        position_overlay(m->tick_hi, m->bar, safe_hi_frac, 2);
        set_bg(m->tick_hi, low_zone ? UI_C_OK_SOFT : UI_C_WARN_SOFT);
    } else {
        lv_obj_add_flag(m->tick_hi, LV_OBJ_FLAG_HIDDEN);
    }

    /* pill de estado */
    uint32_t pill_bg = (st == CARD_ALARM) ? UI_C_ALARM_BG : (st == CARD_WARN ? UI_C_WARN_BG : UI_C_OK_BG);
    uint32_t pill_bd = (st == CARD_ALARM) ? UI_C_ALARM_BORDER : (st == CARD_WARN ? UI_C_WARN_BORDER : UI_C_OK_BORDER);
    set_bg(m->pill, pill_bg);
    set_border(m->pill, pill_bd);
    lv_obj_set_style_bg_opa(m->pill, LV_OPA_40, 0);
    set_label_if_changed(m->pill_state, state_txt);
    set_txt_color(m->pill_state, accent);
    snprintf(buf, sizeof(buf), "24H %.*f / %.*f", dec, mn_disp, dec, mx_disp);
    set_label_if_changed(m->pill_mm, buf);
}

/* ---------- conversiones de unidades ----------
 * bar era kPa/10 en el FW original: bug (1 bar = 100 kPa), corregido a /100.
 * Mantiene la firma (valor, unit) para usar el cfg que recibe ui_main_update. */
static float pressure_to_disp(float kpa, const char *unit)
{
    if (unit && strcmp(unit, "psi") == 0) return kpa * 0.145038f;
    if (unit && strcmp(unit, "bar") == 0) return kpa / 100.0f;
    if (unit && strcmp(unit, "mpa") == 0) return kpa / 1000.0f;
    return kpa; /* kpa */
}
static float flow_to_disp(float lpm, const char *unit)
{
    if (unit && strcmp(unit, "sccm") == 0) return lpm * 1000.0f;
    if (unit && strcmp(unit, "m3h") == 0)  return lpm * 0.06f;  /* 1 L/min = 0.06 m³/h */
    return lpm; /* lpm/slpm/nlpm */
}

/* ---------- callback de mute (tap en el banner) ---------- */
static void alarm_overlay_open(void);
static void banner_open_cb(lv_event_t *e) { (void)e; alarm_overlay_open(); }
static void menu_close(void) { if (s_menu_pop) { lv_obj_delete_async(s_menu_pop); s_menu_pop = NULL; } }
static void menu_scrim_cb(lv_event_t *e) { (void)e; menu_close(); }
static void menu_item_cb(lv_event_t *e) { int w=(int)(intptr_t)lv_event_get_user_data(e); menu_close(); if(w==0) ui_open_info_cb(e); else if(w==2) ui_open_config_pin_cb(e); else if(w==3) ui_open_config_ble_cb(e); }
static void menu_add_item(lv_obj_t *p,const char *sym,const char *txt,int w,bool en)
{
    lv_obj_t *r=ui_box(p); lv_obj_set_size(r,LV_PCT(100),38); lv_obj_set_flex_flow(r,LV_FLEX_FLOW_ROW); lv_obj_set_flex_align(r,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER); lv_obj_set_style_pad_hor(r,9,0); lv_obj_set_style_pad_column(r,9,0); lv_obj_set_style_radius(r,7,0); ui_label(r,sym,UI_FONT_SM,en?UI_C_TEXT_3:UI_C_TEXT_MUTED); ui_label(r,txt,UI_FONT_SM,en?UI_C_TEXT:UI_C_TEXT_MUTED); if(en){lv_obj_add_flag(r,LV_OBJ_FLAG_CLICKABLE);ui_press_feedback(r);lv_obj_add_event_cb(r,menu_item_cb,LV_EVENT_CLICKED,(void*)(intptr_t)w);}
}
static void menu_open_cb(lv_event_t *e)
{
    (void)e; if(s_menu_pop){menu_close();return;} s_menu_pop=lv_obj_create(lv_layer_top()); lv_obj_remove_style_all(s_menu_pop); lv_obj_set_size(s_menu_pop,LV_PCT(100),LV_PCT(100)); lv_obj_add_flag(s_menu_pop,LV_OBJ_FLAG_CLICKABLE); lv_obj_clear_flag(s_menu_pop,LV_OBJ_FLAG_SCROLLABLE); lv_obj_add_event_cb(s_menu_pop,menu_scrim_cb,LV_EVENT_CLICKED,NULL); lv_obj_t *p=ui_card(s_menu_pop); lv_obj_set_size(p,190,LV_SIZE_CONTENT); lv_obj_set_pos(p,278,46); lv_obj_set_style_pad_all(p,6,0); lv_obj_set_style_pad_row(p,2,0); lv_obj_set_style_shadow_width(p,24,0); lv_obj_set_style_shadow_opa(p,LV_OPA_40,0); lv_obj_set_flex_flow(p,LV_FLEX_FLOW_COLUMN); menu_add_item(p,LV_SYMBOL_EYE_OPEN,"Información",0,true); menu_add_item(p,LV_SYMBOL_FILE,"Logs",1,true); menu_add_item(p,LV_SYMBOL_SETTINGS,"Configuración",2,true); menu_add_item(p,LV_SYMBOL_BLUETOOTH,"Bluetooth",3,true);
}
static const char *alarm_condition(const AppConfig *c){if(s_alarm_faults)return "FALLA MODULO SENSORES";if(!s_alarm_have_last||!c)return "FALLA DE MEDICION";if(s_alarm_last.pressure_kpa<c->sensors.alarm_limits.pressure_min)return "PRESION BAJA";if(s_alarm_last.pressure_kpa>c->sensors.alarm_limits.pressure_max)return "PRESION ALTA";return "ALARMA DE FLUJO";}
static void alarm_overlay_close(void){if(s_alarm_overlay){lv_obj_delete_async(s_alarm_overlay);s_alarm_overlay=NULL;}}
static void alarm_overlay_close_cb(lv_event_t *e){(void)e;alarm_overlay_close();}
static void alarm_overlay_silence_cb(lv_event_t *e){(void)e;if(!s_alarm_muted)alarm_mgr_press_mute();alarm_overlay_close();}
static void alarm_overlay_open(void)
{
    if (s_alarm_overlay || s_alarm_state == ALARM_STATE_NORMAL) return;
    const AppConfig *c=appcfg_cache_peek(); bool crit=s_alarm_state==ALARM_STATE_ALERT; uint32_t ac=crit?UI_C_ALARM:UI_C_WARN;
    s_alarm_overlay=lv_obj_create(lv_layer_top()); lv_obj_set_size(s_alarm_overlay,LV_PCT(100),LV_PCT(100)); lv_obj_set_style_bg_color(s_alarm_overlay,ui_col(crit?0x210d10:0x211a08),0); lv_obj_set_style_bg_opa(s_alarm_overlay,LV_OPA_COVER,0);
    lv_obj_set_style_border_width(s_alarm_overlay,5,0); lv_obj_set_style_border_color(s_alarm_overlay,ui_col(ac),0); lv_obj_set_style_pad_all(s_alarm_overlay,14,0); lv_obj_set_flex_flow(s_alarm_overlay,LV_FLEX_FLOW_COLUMN); lv_obj_clear_flag(s_alarm_overlay,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *h=ui_box(s_alarm_overlay);lv_obj_set_size(h,LV_PCT(100),36);lv_obj_set_flex_flow(h,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(h,LV_FLEX_ALIGN_SPACE_BETWEEN,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_t *hl=ui_box(h);lv_obj_set_size(hl,LV_SIZE_CONTENT,36);lv_obj_set_flex_flow(hl,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(hl,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_column(hl,8,0);ui_icon(hl,UI_SYM_ALERT_TRIANGLE,UI_ICON_MD,ac);ui_label(hl,crit?"ALARMA":"ADVERTENCIA",UI_FONT_XL,0xffe3e3);lv_obj_t *bk=ui_icon_badge(h,UI_SYM_ARROW_LEFT,UI_ICON_SM,UI_C_TEXT,0x35191c,34);lv_obj_add_flag(bk,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(bk,alarm_overlay_close_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_t *ct=ui_box(s_alarm_overlay);lv_obj_set_width(ct,LV_PCT(100));lv_obj_set_flex_grow(ct,1);lv_obj_set_flex_flow(ct,LV_FLEX_FLOW_COLUMN);lv_obj_set_flex_align(ct,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_row(ct,5,0);ui_label(ct,alarm_condition(c),UI_FONT_XL,crit?UI_C_ALARM_SOFT:UI_C_WARN_SOFT);char val[24]="--",unit[12]="",lim[64]="";
    if(s_alarm_faults){snprintf(lim,sizeof(lim),"sin datos validos - revisar modulo y cable I2C");}else if(s_alarm_have_last&&c){bool p=s_alarm_last.pressure_kpa<c->sensors.alarm_limits.pressure_min||s_alarm_last.pressure_kpa>c->sensors.alarm_limits.pressure_max;if(p){float v=pressure_to_disp(s_alarm_last.pressure_kpa,c->sensors.pressure_unit);float t=pressure_to_disp(s_alarm_last.pressure_kpa<c->sensors.alarm_limits.pressure_min?c->sensors.alarm_limits.pressure_min:c->sensors.alarm_limits.pressure_max,c->sensors.pressure_unit);snprintf(val,sizeof(val),"%.0f",(double)v);snprintf(unit,sizeof(unit),"%s",c->sensors.pressure_unit);snprintf(lim,sizeof(lim),"valor %s %.0f %s",s_alarm_last.pressure_kpa<c->sensors.alarm_limits.pressure_min?"<":">",(double)t,c->sensors.pressure_unit);}else{snprintf(val,sizeof(val),"%.0f",(double)flow_to_disp(s_alarm_last.flow_lpm,c->sensors.flow_unit));snprintf(unit,sizeof(unit),"%s",c->sensors.flow_unit);snprintf(lim,sizeof(lim),"flujo por encima de lo habitual");}}
    lv_obj_t *vr=ui_box(ct);lv_obj_set_size(vr,LV_SIZE_CONTENT,LV_SIZE_CONTENT);lv_obj_set_flex_flow(vr,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(vr,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_END,LV_FLEX_ALIGN_END);lv_obj_set_style_pad_column(vr,7,0);ui_label(vr,val,UI_FONT_HUGE,UI_C_TEXT_STRONG);ui_label(vr,unit,UI_FONT_LG,UI_C_TEXT_2);ui_label(ct,lim,UI_FONT_SM,crit?UI_C_ALARM_SOFT:UI_C_WARN_DIM);
    lv_obj_t *s=ui_box(s_alarm_overlay);lv_obj_set_size(s,LV_PCT(100),48);ui_style_button(s,0xf1f3f5);lv_obj_add_flag(s,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(s,crit?alarm_overlay_silence_cb:alarm_overlay_close_cb,LV_EVENT_CLICKED,NULL);lv_obj_center(ui_label(s,(!crit||s_alarm_muted)?"Volver":"Silenciar alarma",UI_FONT_MD,0x2a1416));
}

/* ---------- construcción de la pantalla ---------- */
void ui_mainScreen_screen_init(void)
{
    if (s_data_activity_timer)
    {
        lv_timer_del(s_data_activity_timer);
        s_data_activity_timer = NULL;
    }
    s_data_seen = false;
    s_data_pulse_active = false;
    s_last_activity_sample_ts = -1;

    ui_mainScreen = ui_screen_base();
    lv_obj_set_flex_flow(ui_mainScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_ver(ui_mainScreen, 5, 0);
    lv_obj_set_style_pad_hor(ui_mainScreen, 12, 0);
    lv_obj_set_style_pad_row(ui_mainScreen, 4, 0);

    /* ===== Header ===== */
    lv_obj_t *hdr = ui_box(ui_mainScreen);
    lv_obj_set_size(hdr, LV_PCT(100), 36);
    lv_obj_set_flex_flow(hdr, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(hdr, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *brand = ui_box(hdr);
    lv_obj_set_size(brand, LV_SIZE_CONTENT, 36);
    lv_obj_set_flex_flow(brand, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(brand, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(brand, 9, 0);
    ui_icon_badge(brand, UI_SYM_ACTIVITY, UI_ICON_MD, 0x9fe1cb, UI_C_HEADER_ICONBG, 30);
    lv_obj_t *bcol = ui_box(brand);
    lv_obj_set_size(bcol, 116, 32);
    s_brand_lbl = ui_label(bcol, "Ubicacion", UI_FONT_SM, UI_C_TEXT);
    lv_obj_set_size(s_brand_lbl, 116, 16); lv_obj_set_pos(s_brand_lbl, 0, 0);
    s_sub_lbl   = ui_label(bcol, "Axira - FPM-200", UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_size(s_sub_lbl, 116, 16); lv_obj_set_pos(s_sub_lbl, 0, 16);

    /* fila de estado (iconos + reloj + settings) */
    ui_statusMainComm = ui_box(hdr);
    lv_obj_set_size(ui_statusMainComm, LV_SIZE_CONTENT, 36);
    lv_obj_set_flex_flow(ui_statusMainComm, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_statusMainComm, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(ui_statusMainComm, 7, 0);

    s_data_chip = ui_box(ui_statusMainComm);
    lv_obj_set_size(s_data_chip, 64, 28);
    lv_obj_set_style_radius(s_data_chip, UI_RADIUS_PILL, 0);
    lv_obj_set_style_bg_color(s_data_chip, ui_col(UI_C_CARD_BG), 0);
    lv_obj_set_style_bg_opa(s_data_chip, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(s_data_chip, 1, 0);
    lv_obj_set_style_pad_hor(s_data_chip, 7, 0);
    lv_obj_set_style_pad_ver(s_data_chip, 0, 0);
    lv_obj_set_style_pad_column(s_data_chip, 6, 0);
    lv_obj_set_flex_flow(s_data_chip, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_data_chip, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    s_data_dot = ui_box(s_data_chip);
    lv_obj_set_size(s_data_dot, 7, 7);
    lv_obj_set_style_radius(s_data_dot, LV_RADIUS_CIRCLE, 0);
    s_data_label = ui_label(s_data_chip, "DATOS", UI_FONT_XS, UI_C_TEXT_MUTED);
    apply_data_activity_visual();

    /* Iconos de red (bt/wifi/eth) a tamaño MD (20px Tabler) para que queden
     * proporcionales al icono de la nube (fa-cloud a 18px); antes iban a SM
     * (16px) y se veían más pequeños que el resto de la barra. */
    ui_bluetoothStatusMain = ui_icon(ui_statusMainComm, UI_SYM_BLUETOOTH, UI_ICON_MD, UI_C_BLUE);
    ui_wifiStatusMain      = ui_icon(ui_statusMainComm, UI_SYM_WIFI, UI_ICON_MD, UI_C_OK);
    ui_ethernetStatusMain  = ui_icon(ui_statusMainComm, UI_SYM_NETWORK, UI_ICON_MD, UI_C_TEXT_2);
    /* Icono compuesto: usa solo glifos ya presentes en las fuentes originales
     * del FPM. La caja conserva el ancho cuando aparece la flecha de TX. */
    ui_cloudStatusBoxMain = ui_box(ui_statusMainComm);
    lv_obj_set_size(ui_cloudStatusBoxMain, 26, 28);
    ui_cloudStatusMain = ui_label(ui_cloudStatusBoxMain, UI_SYM_CLOUD_SOLID,
                                  UI_FONT_LG, UI_C_ALARM);
    lv_obj_align(ui_cloudStatusMain, LV_ALIGN_BOTTOM_MID, 0, 1);
    ui_cloudTxArrowMain = ui_label(ui_cloudStatusBoxMain, UI_SYM_CLOUD_TX_ARROW,
                                   UI_FONT_XS, UI_C_TEAL);
    lv_obj_align(ui_cloudTxArrowMain, LV_ALIGN_TOP_MID, 0, -2);
    lv_obj_add_flag(ui_cloudTxArrowMain, LV_OBJ_FLAG_HIDDEN);
    lv_obj_t *sep = ui_box(ui_statusMainComm); lv_obj_set_size(sep, 1, 24); lv_obj_set_style_bg_color(sep, ui_col(UI_C_BORDER), 0); lv_obj_set_style_bg_opa(sep, LV_OPA_COVER, 0);
    lv_obj_t *dt = ui_box(ui_statusMainComm); lv_obj_set_size(dt,72,32);
    s_clock_lbl = ui_label(dt, "--:--", UI_FONT_XS, UI_C_TEXT); lv_obj_set_size(s_clock_lbl,72,16); lv_obj_set_pos(s_clock_lbl,0,0); lv_obj_set_style_text_align(s_clock_lbl,LV_TEXT_ALIGN_RIGHT,0);
    s_date_lbl  = ui_label(dt, "--/--/----", UI_FONT_XS, UI_C_TEXT_MUTED); lv_obj_set_size(s_date_lbl,72,16); lv_obj_set_pos(s_date_lbl,0,16); lv_obj_set_style_text_align(s_date_lbl,LV_TEXT_ALIGN_RIGHT,0);
    lv_obj_t *menu = ui_box(ui_statusMainComm); lv_obj_set_size(menu,30,30); lv_obj_set_style_bg_color(menu,lv_color_hex(UI_C_CARD_BG),0); lv_obj_set_style_bg_opa(menu,LV_OPA_COVER,0); lv_obj_set_style_border_width(menu,1,0); lv_obj_set_style_border_color(menu,ui_col(UI_C_BORDER),0); lv_obj_set_style_radius(menu,7,0); lv_obj_set_flex_flow(menu,LV_FLEX_FLOW_ROW); lv_obj_set_flex_align(menu,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER); ui_label(menu,LV_SYMBOL_LIST,UI_FONT_SM,UI_C_TEXT);
    lv_obj_add_flag(menu, LV_OBJ_FLAG_CLICKABLE); lv_obj_set_ext_click_area(menu, 7); lv_obj_add_event_cb(menu, menu_open_cb, LV_EVENT_CLICKED, NULL);

    /* ===== Banner de estado ===== */
    ui_alarmBtnMain = ui_box(ui_mainScreen);
    s_banner = ui_alarmBtnMain;
    lv_obj_set_size(s_banner, LV_PCT(100), 30);
    lv_obj_set_style_radius(s_banner, UI_RADIUS_SM, 0);
    lv_obj_set_style_border_width(s_banner, 1, 0);
    lv_obj_set_style_pad_hor(s_banner, 12, 0);
    lv_obj_set_style_pad_ver(s_banner, 0, 0);
    lv_obj_set_flex_flow(s_banner, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_banner, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(s_banner, 8, 0);
    lv_obj_add_flag(s_banner, LV_OBJ_FLAG_CLICKABLE);
    ui_press_push(s_banner, 248);   /* feedback "push" (sin tinte: conserva su color de estado) */
    lv_obj_add_event_cb(s_banner, banner_open_cb, LV_EVENT_CLICKED, NULL);
    s_banner_icon = ui_icon(s_banner, UI_SYM_CIRCLE_CHECK, UI_ICON_SM, UI_C_OK);
    s_banner_txt  = ui_label(s_banner, _t("SISTEMA NORMAL"), UI_FONT_SM, UI_C_OK_SOFT);
    lv_obj_set_style_text_letter_space(s_banner_txt, 1, 0);
    s_banner_sub  = ui_label(s_banner, _t("· sin alarmas"), UI_FONT_XS, UI_C_OK_DIM);
    /* pill derecho (para estado silenciado) */
    s_banner_right = ui_box(s_banner);
    lv_obj_set_size(s_banner_right, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_margin_left(s_banner_right, 6, 0);
    lv_obj_set_flex_flow(s_banner_right, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_banner_right, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(s_banner_right, 5, 0);
    lv_obj_set_flex_grow(s_banner_right, 1);
    s_banner_right_ic = ui_icon(s_banner_right, UI_SYM_BELL_OFF, UI_ICON_SM, UI_C_ALARM_SOFT);
    s_banner_right_tx = ui_label(s_banner_right, "", UI_FONT_XS, UI_C_ALARM_SOFT);
    lv_obj_add_flag(s_banner_right, LV_OBJ_FLAG_HIDDEN);

    /* ===== Tarjetas ===== */
    lv_obj_t *cards = ui_box(ui_mainScreen);
    lv_obj_set_size(cards, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(cards, 1);
    lv_obj_set_flex_flow(cards, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(cards, 10, 0);
    build_metric_card(cards, &s_press, _t("PRESIÓN"), UI_SYM_GAUGE, "psi");
    build_metric_card(cards, &s_flow,  _t("FLUJO"),   UI_SYM_WIND,  "SCCM");

    /* ===== Fila de consumo ===== */
    lv_obj_t *cons = ui_card(ui_mainScreen);
    lv_obj_set_size(cons, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_radius(cons, UI_RADIUS_TILE, 0);
    lv_obj_set_style_pad_hor(cons, 13, 0);
    lv_obj_set_style_pad_ver(cons, 3, 0);
    lv_obj_set_flex_flow(cons, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cons, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(cons, 7, 0);
    ui_icon(cons, UI_SYM_CHART_BAR, UI_ICON_SM, UI_C_TEXT_3);
    s_consumo_cap = ui_label(cons, _t("CONSUMO"), UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_style_text_letter_space(s_consumo_cap, 1, 0);
    s_consumo_val = ui_label(cons, "-- m³ hoy", UI_FONT_XL, UI_C_TEXT_STRONG);
    lv_obj_set_style_margin_left(s_consumo_val, 4, 0);
    /* Subtítulo como el mockup ("· desde 00:00 · acum. mes ...").
     * El acumulado mensual necesita un contador persistente (pendiente); por
     * ahora se muestra el origen del conteo diario. flex_grow empuja el badge. */
    s_consumo_sub = ui_label(cons, _t("· desde 00:00"), UI_FONT_XS, UI_C_TEXT_MUTED);
    lv_obj_set_style_margin_left(s_consumo_sub, 6, 0);
    lv_obj_set_flex_grow(s_consumo_sub, 1);
    s_consumo_badge = ui_pill(cons, "--", UI_FONT_XS, UI_C_OK_DIM, UI_C_OK_BG, UI_C_OK_BORDER);
    lv_obj_set_style_margin_left(s_consumo_badge, 8, 0);
    s_consumo_badge_tx = lv_obj_get_child(s_consumo_badge, 0);
    lv_obj_add_flag(s_consumo_badge, LV_OBJ_FLAG_HIDDEN); /* oculto hasta tener datos reales (consumo m³) */

    /* ===== Banner de gas ===== */
    s_gas_banner = ui_box(ui_mainScreen);
    lv_obj_set_size(s_gas_banner, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_radius(s_gas_banner, UI_RADIUS_SM, 0);
    set_bg(s_gas_banner, UI_C_GAS_O2);
    lv_obj_set_style_bg_opa(s_gas_banner, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_ver(s_gas_banner, 4, 0);
    lv_obj_set_flex_flow(s_gas_banner, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_gas_banner, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    s_gas_lbl = ui_label(s_gas_banner, _t("OXÍGENO"), UI_FONT_TITLE, 0xffffff);
    lv_obj_set_style_text_letter_space(s_gas_lbl, 3, 0);

    /* estado inicial */
    ui_main_apply_config(NULL);
    s_data_activity_timer = lv_timer_create(data_activity_timer_cb,
                                            DATA_ACTIVITY_TIMER_MS, NULL);
}

void ui_mainScreen_screen_destroy(void)
{
    if (s_data_activity_timer)
    {
        lv_timer_del(s_data_activity_timer);
        s_data_activity_timer = NULL;
    }
    if (ui_mainScreen) { lv_obj_del(ui_mainScreen); ui_mainScreen = NULL; }
    s_data_chip = s_data_dot = s_data_label = NULL;
    ui_cloudStatusBoxMain = ui_cloudStatusMain = ui_cloudTxArrowMain = NULL;
    s_data_seen = false;
    s_data_pulse_active = false;
    s_last_activity_sample_ts = -1;
}

/* ---------- API pública de binding ---------- */
void ui_main_set_clock(const char *hhmm)
{
    if (!hhmm) return;
    strncpy(s_clock_prev, hhmm, sizeof(s_clock_prev) - 1);
    s_clock_prev[sizeof(s_clock_prev) - 1] = '\0';
    if (s_clock_lbl) set_label_if_changed(s_clock_lbl, hhmm);
}
void ui_main_set_date(const char *date)
{
    if (s_date_lbl && date) set_label_if_changed(s_date_lbl, date);
}

void ui_main_signal_data_activity(void)
{
    if (!s_data_chip || !lv_obj_is_valid(s_data_chip)) return;

    uint32_t now = lv_tick_get();
    if (s_data_seen &&
        lv_tick_elaps(s_data_last_pulse_tick) < DATA_PULSE_GAP_MS)
        return;

    s_data_seen = true;
    s_data_last_pulse_tick = now;
    s_data_pulse_tick = now;
    s_data_pulse_active = true;
    apply_data_activity_visual();
}


/* Mapea gas_type -> etiqueta + color de banner usando el catálogo de gases y la
 * norma NFPA/ISO configurada (ui_cfg). dark = el texto debe ir oscuro (fondo claro). */
static void gas_label_color(const char *gas, const char **label, uint32_t *color, bool *dark)
{
    int idx = 0;
    for (int i = 0; i < ui_cfg_gas_count(); i++)
        if (gas && strcmp(gas, ui_cfg_gas_key(i)) == 0) { idx = i; break; }
    *label = ui_cfg_gas_name(idx);
    *color = ui_cfg_gas_color_at(idx, ui_cfg_color_is_iso(), dark);
}

void ui_main_apply_config(const AppConfig *cfg)
{
    if (!cfg) cfg = appcfg_cache_peek();
    if (!cfg) return;

    if (cfg->general.client[0]) set_label_if_changed(s_brand_lbl, cfg->general.client);
    if (cfg->general.model[0])  lv_label_set_text_fmt(s_sub_lbl, "Axira - %s", cfg->general.model);

    const char *glabel; uint32_t gcolor; bool gdark = false;
    gas_label_color(cfg->sensors.gas_type, &glabel, &gcolor, &gdark);
    set_label_if_changed(s_gas_lbl, glabel);
    set_bg(s_gas_banner, gcolor);
    set_txt_color(s_gas_lbl, gdark ? 0x14171c : 0xffffff);

    if (cfg->sensors.pressure_unit[0]) set_label_if_changed(s_press.unit, cfg->sensors.pressure_unit);
    if (cfg->sensors.flow_unit[0])     set_label_if_changed(s_flow.unit, cfg->sensors.flow_unit);

    /* Textos estáticos retraducibles (main persiste; el resto de pantallas se
     * recrean al abrirse y no lo necesitan). save_and_refresh() pasa por aquí. */
    if (s_press.title)  set_label_if_changed(s_press.title, _t("PRESIÓN"));
    if (s_flow.title)   set_label_if_changed(s_flow.title, _t("FLUJO"));
    if (s_consumo_cap)  set_label_if_changed(s_consumo_cap, _t("CONSUMO"));
    if (s_consumo_sub)  set_label_if_changed(s_consumo_sub, _t("· desde 00:00"));
}

/* Banner por estado */
static void set_banner(alarm_clinical_state_t st, bool muted)
{
    if (st == ALARM_STATE_NORMAL) {
        set_bg(s_banner, UI_C_OK_BG); set_border(s_banner, UI_C_OK_BORDER);
        lv_obj_set_style_bg_opa(s_banner, LV_OPA_COVER, 0);
        set_label_if_changed(s_banner_icon, UI_SYM_CIRCLE_CHECK); set_txt_color(s_banner_icon, UI_C_OK);
        set_label_if_changed(s_banner_txt, _t("SISTEMA NORMAL")); set_txt_color(s_banner_txt, UI_C_OK_SOFT);
        set_label_if_changed(s_banner_sub, _t("· sin alarmas")); set_txt_color(s_banner_sub, UI_C_OK_DIM);
        lv_obj_add_flag(s_banner_right, LV_OBJ_FLAG_HIDDEN);
    } else if (st == ALARM_STATE_WARNING) {
        set_bg(s_banner, UI_C_WARN_BG); set_border(s_banner, UI_C_WARN_BORDER);
        lv_obj_set_style_bg_opa(s_banner, LV_OPA_COVER, 0);
        set_label_if_changed(s_banner_icon, UI_SYM_ALERT_TRIANGLE); set_txt_color(s_banner_icon, UI_C_WARN);
        set_label_if_changed(s_banner_txt, _t("ADVERTENCIA")); set_txt_color(s_banner_txt, UI_C_WARN_SOFT);
        set_label_if_changed(s_banner_sub, _t("· vigilar")); set_txt_color(s_banner_sub, UI_C_WARN_DIM);
        lv_obj_add_flag(s_banner_right, LV_OBJ_FLAG_HIDDEN);
    } else { /* ALERT */
        set_bg(s_banner, UI_C_ALARM_BG); set_border(s_banner, UI_C_ALARM_BORDER);
        lv_obj_set_style_bg_opa(s_banner, LV_OPA_COVER, 0);
        set_label_if_changed(s_banner_icon, UI_SYM_ALERT_TRIANGLE_FILLED); set_txt_color(s_banner_icon, UI_C_ALARM_SOFT);
        set_label_if_changed(s_banner_txt, _t("ALARMA")); set_txt_color(s_banner_txt, 0xffd3d3);
        set_label_if_changed(s_banner_sub, _t("· revisar línea")); set_txt_color(s_banner_sub, UI_C_ALARM_SOFT);
        if (muted) {
            lv_obj_clear_flag(s_banner_right, LV_OBJ_FLAG_HIDDEN);
            set_label_if_changed(s_banner_right_ic, UI_SYM_BELL_OFF);
            set_label_if_changed(s_banner_right_tx, _t("silenciada"));
        } else {
            lv_obj_clear_flag(s_banner_right, LV_OBJ_FLAG_HIDDEN);
            set_label_if_changed(s_banner_right_ic, UI_SYM_BELL_RINGING);
            set_label_if_changed(s_banner_right_tx, _t("buzzer activo"));
        }
    }
}

void ui_main_update(const sensor_sample_t *last, bool have_last,
                    const sensor_sample_t *mn, const sensor_sample_t *mx, bool have_mm,
                    const AppConfig *cfg,
                    alarm_clinical_state_t state, bool muted)
{
    if (!cfg) cfg = appcfg_cache_peek();
    if (!cfg) return;
    s_decimals = (cfg->sensors.decimals > 0) ? 1 : 0;
    s_alarm_state = state;
    s_alarm_muted = muted;
    s_alarm_have_last = have_last;
    s_alarm_faults = alarm_mgr_get_sensor_faults();
    if (have_last) s_alarm_last = *last;

    /* ui_main_update ya corre bajo el lock del display. Un timestamp nuevo
     * equivale a una adquisicion completa de presion/flujo. */
    if (have_last && last && last->ts_ms != s_last_activity_sample_ts)
    {
        s_last_activity_sample_ts = last->ts_ms;
        ui_main_signal_data_activity();
    }

    set_banner(state, muted);

    if (have_last) {
        /* --- Presión --- */
        bool pmin_en = cfg->sensors.alarm_limits.pressure_min_enabled;
        bool pmax_en = cfg->sensors.alarm_limits.pressure_max_enabled;
        float p_min = cfg->sensors.alarm_limits.pressure_min;
        float p_max = cfg->sensors.alarm_limits.pressure_max;
        float axis_kpa = PRESS_AXIS_FULLSCALE_KPA;
        float p_disp = pressure_to_disp(last->pressure_kpa, cfg->sensors.pressure_unit);
        /* EMA para el número mostrado (el bar/frac sigue el valor crudo). */
        if (!isfinite(s_p_disp_ema)) s_p_disp_ema = p_disp;
        else s_p_disp_ema += DISP_EMA_ALPHA * (p_disp - s_p_disp_ema);
        float p_show = s_p_disp_ema;
        float p_frac = clampf(last->pressure_kpa / axis_kpa, 0.f, 1.f);
        float safe_lo = clampf(p_min / axis_kpa, 0.f, 1.f);
        float safe_hi = clampf(p_max / axis_kpa, 0.f, 1.f);

        /* La etiqueta se pinta de alarma y aparecen las bandas SOLO si el limite
         * correspondiente esta habilitado (#3). */
        card_state_t pst = CARD_OK; const char *ptx = _t("NORMAL");
        if (pmin_en && last->pressure_kpa < p_min) { pst = CARD_ALARM; ptx = _t("BAJA"); }
        else if (pmax_en && last->pressure_kpa > p_max) { pst = CARD_ALARM; ptx = _t("ALTA"); }

        const char *pu = cfg->sensors.pressure_unit;
        int pdec = s_decimals; if (press_unit_min_dec(pu) > pdec) pdec = press_unit_min_dec(pu);
        float pmn = have_mm ? pressure_to_disp(mn->pressure_kpa, pu) : p_disp;
        float pmx = have_mm ? pressure_to_disp(mx->pressure_kpa, pu) : p_disp;
        if (last->invalid_mask & SENSOR_INVALID_PRESSURE) { pst = CARD_WARN; ptx = _t("SIN DATO NUEVO"); }
        update_metric_card(&s_press, p_show, p_frac, safe_lo, safe_hi, true,
                           pmin_en, pmax_en, pdec, pst, ptx, pmn, pmx);

        char ab[24];
        snprintf(ab, sizeof(ab), "%.*f", pdec, pressure_to_disp(axis_kpa, pu));
        set_label_if_changed(s_press.ax_right, ab);
        /* Etiqueta central = rango seguro, mostrando solo los limites activos. */
        char pmb[48];
        double lo_d = (double)pressure_to_disp(p_min, pu);
        double hi_d = (double)pressure_to_disp(p_max, pu);
        if (pmin_en && pmax_en)
            snprintf(pmb, sizeof(pmb), "%.*f-%.*f %s", pdec, lo_d, pdec, hi_d, _t("seguro"));
        else if (pmax_en)
            snprintf(pmb, sizeof(pmb), "%s %.*f", _t("max"), pdec, hi_d);
        else if (pmin_en)
            snprintf(pmb, sizeof(pmb), "%s %.*f", _t("min"), pdec, lo_d);
        else
            snprintf(pmb, sizeof(pmb), "%s", _t("sin limites"));
        set_label_if_changed(s_press.ax_mid, pmb);

        /* --- Flujo --- */
        float f_disp = flow_to_disp(last->flow_lpm, cfg->sensors.flow_unit);
        if (!isfinite(s_f_disp_ema)) s_f_disp_ema = f_disp;
        else s_f_disp_ema += DISP_EMA_ALPHA * (f_disp - s_f_disp_ema);
        float f_show = s_f_disp_ema;
        float f_axis = (cfg->sensors.flow_fullscale_lpm > 0.f) ? cfg->sensors.flow_fullscale_lpm
                                                               : FLOW_AXIS_FULLSCALE_LPM;
        float f_frac = clampf(last->flow_lpm / f_axis, 0.f, 1.f);
        const char *fu = cfg->sensors.flow_unit;
        int fdec = s_decimals; if (flow_unit_min_dec(fu) > fdec) fdec = flow_unit_min_dec(fu);
        bool fhi_en = cfg->sensors.alarm_limits.flow_high_enabled;
        card_state_t fst = CARD_OK; const char *ftx = _t("NORMAL");
        if (fhi_en && last->flow_lpm > f_axis * FLOW_HIGH_ZONE_FRAC) { fst = CARD_WARN; ftx = _t("CONSUMO ALTO"); }
        float fmn = have_mm ? flow_to_disp(mn->flow_lpm, fu) : f_disp;
        float fmx = have_mm ? flow_to_disp(mx->flow_lpm, fu) : f_disp;
        if (last->invalid_mask & SENSOR_INVALID_FLOW) { fst = CARD_WARN; ftx = _t("SIN DATO NUEVO"); }
        update_metric_card(&s_flow, f_show, f_frac, 0.f, FLOW_HIGH_ZONE_FRAC, false,
                           false, fhi_en, fdec, fst, ftx, fmn, fmx);
        char fb[24];
        snprintf(fb, sizeof(fb), "%.*f", fdec, flow_to_disp(f_axis, fu));
        set_label_if_changed(s_flow.ax_right, fb);
        /* Etiqueta central = umbral de "alto" (solo si la alarma de flujo alto
         * esta habilitada). */
        char fmb[36];
        if (fhi_en)
            snprintf(fmb, sizeof(fmb), "%.*f %s",
                     fdec, (double)flow_to_disp(f_axis * FLOW_HIGH_ZONE_FRAC, fu), _t("alto"));
        else
            snprintf(fmb, sizeof(fmb), "%s", _t("sin limites"));
        set_label_if_changed(s_flow.ax_mid, fmb);

        /* --- Diagnóstico TEMPORAL en las tarjetas (se elimina en producción) ---
         * Presión: atmosférica del BMP280 (referencia del cero).
         * Flujo: referencia SFM3300 y su diferencia con el FS7. */
        {
            float dbg_atm = NAN, dbg_sfm = NAN, dbg_fs7 = NAN;
            sensors_runtime_get_debug(&dbg_atm, &dbg_sfm, &dbg_fs7);
            /* pu/fu ya declarados arriba en este mismo bloque (presión/flujo). */
            char dbgb[56];
            /* Atmosférica del BMP en la MISMA unidad que la presión de línea. */
            if (isfinite(dbg_atm))
                snprintf(dbgb, sizeof(dbgb), "atm %.2f %s (BMP)",
                         (double)pressure_to_disp(dbg_atm, pu), pu);
            else
                snprintf(dbgb, sizeof(dbgb), "atm -- (BMP)");
            set_label_if_changed(s_press.dbg, dbgb);

            /* SFM y su diferencia con el FS7, en la unidad de flujo configurada. */
            if (isfinite(dbg_sfm)) {
                float sfm_d = flow_to_disp(dbg_sfm, fu);
                if (isfinite(dbg_fs7))
                    snprintf(dbgb, sizeof(dbgb), "SFM %.1f  dif %+.1f %s",
                             (double)sfm_d, (double)flow_to_disp(dbg_sfm - dbg_fs7, fu), fu);
                else
                    snprintf(dbgb, sizeof(dbgb), "SFM %.1f %s", (double)sfm_d, fu);
            } else {
                snprintf(dbgb, sizeof(dbgb), "SFM -- %s", fu);
            }
            set_label_if_changed(s_flow.dbg, dbgb);
        }

        flow_meter_snapshot_t meter = flow_meter_get();
        double s_consumo_m3 = meter.volume_m3;
        if (s_consumo_badge) {
            if (meter.partial || !meter.dated) {
                set_label_if_changed(s_consumo_badge_tx, _t("PARCIAL"));
                lv_obj_remove_flag(s_consumo_badge, LV_OBJ_FLAG_HIDDEN);
            } else lv_obj_add_flag(s_consumo_badge, LV_OBJ_FLAG_HIDDEN);
        }
        if (s_consumo_val) {
            char cb[40];
            if (s_consumo_m3 < 0.1f)
                snprintf(cb, sizeof(cb), "%.0f %s", (double)(s_consumo_m3 * 1000.f), _t("L hoy"));
            else
                snprintf(cb, sizeof(cb), "%.2f %s", (double)s_consumo_m3, _t("m\xC2\xB3 hoy"));
            set_label_if_changed(s_consumo_val, cb);
        }
    }
}
