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
static lv_obj_t *s_consumo_val;
static lv_obj_t *s_consumo_cap = NULL;   /* caption "HOY" (retraducible) */
static lv_obj_t *s_consumo_sub = NULL;   /* "desde HH:MM" (inicio real del conteo de hoy) */
static lv_obj_t *s_consumo_miss = NULL;  /* "N min sin medir" (antes la pildora PARCIAL) */
static lv_obj_t *s_total_cap = NULL, *s_total_val = NULL;   /* totalizador (1.6.8) */
static lv_obj_t *s_consumo_title = NULL; /* "CONSUMO" sobre "TOTAL" (1.6.9) */
static lv_obj_t *s_gas_banner, *s_gas_lbl;
static lv_obj_t *s_menu_pop = NULL;
static lv_obj_t *s_alarm_overlay = NULL;
static sensor_sample_t s_alarm_last;
static bool s_alarm_have_last = false;
static alarm_clinical_state_t s_alarm_state = ALARM_STATE_NORMAL;
static bool s_alarm_muted = false;
static uint32_t s_alarm_faults = 0;
static uint32_t s_alarm_conds = 0;    /* 1.6.8: condiciones activas (alarm_mgr) */
static uint32_t s_alarm_crit = 0;     /* ... de esas, las de banda critica */
/* Overlay de alarmas: firma con la que se armo + valores en vivo por condicion. */
#define OV_MAX 5
static uint32_t s_ov_sig = 0;
static int s_ov_n = 0;
static uint32_t s_ov_bit[OV_MAX];
static lv_obj_t *s_ov_val[OV_MAX];


/* ---------- helpers ---------- */
/* Setters "si cambio": en LVGL 9 cada lv_obj_set_style_* invalida el objeto
 * aunque el valor sea el mismo. ui_main_update corre cada 200 ms y re-aplicaba
 * decenas de estilos iguales -> banner y tarjetas se redibujaban siempre
 * (taskLVGL ~42 % CPU). Comparar primero evita esos redibujados. */
static void set_bg(lv_obj_t *o, uint32_t hex)
{
    lv_color_t c = ui_col(hex);
    if (!lv_color_eq(lv_obj_get_style_bg_color(o, LV_PART_MAIN), c))
        lv_obj_set_style_bg_color(o, c, LV_PART_MAIN | LV_STATE_DEFAULT);
}
static void set_border(lv_obj_t *o, uint32_t hex)
{
    lv_color_t c = ui_col(hex);
    if (!lv_color_eq(lv_obj_get_style_border_color(o, LV_PART_MAIN), c))
        lv_obj_set_style_border_color(o, c, LV_PART_MAIN | LV_STATE_DEFAULT);
}
static void set_txt_color(lv_obj_t *o, uint32_t hex)
{
    lv_color_t c = ui_col(hex);
    if (!lv_color_eq(lv_obj_get_style_text_color(o, LV_PART_MAIN), c))
        lv_obj_set_style_text_color(o, c, LV_PART_MAIN | LV_STATE_DEFAULT);
}
static void set_bg_opa_if(lv_obj_t *o, lv_opa_t opa)
{
    if (lv_obj_get_style_bg_opa(o, LV_PART_MAIN) != opa)
        lv_obj_set_style_bg_opa(o, opa, 0);
}
static void set_border_opa_if(lv_obj_t *o, lv_opa_t opa)
{
    if (lv_obj_get_style_border_opa(o, LV_PART_MAIN) != opa)
        lv_obj_set_style_border_opa(o, opa, 0);
}
static void set_hidden(lv_obj_t *o, bool hidden)
{
    if (lv_obj_has_flag(o, LV_OBJ_FLAG_HIDDEN) == hidden) return;
    if (hidden) lv_obj_add_flag(o, LV_OBJ_FLAG_HIDDEN);
    else        lv_obj_remove_flag(o, LV_OBJ_FLAG_HIDDEN);
}

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
    /* lv_obj_align re-escribe el estilo align siempre (invalida); una vez alineado
     * basta mover la posicion, y lv_obj_set_pos ya ignora valores iguales. */
    if (lv_obj_get_style_align(obj, LV_PART_MAIN) != LV_ALIGN_LEFT_MID)
        lv_obj_align(obj, LV_ALIGN_LEFT_MID, x, 0);
    else
        lv_obj_set_pos(obj, x, 0);
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
    if (isfinite(value_disp)) snprintf(buf, sizeof(buf), "%.*f", dec, value_disp);
    else snprintf(buf, sizeof(buf), "--");   /* sin valor valido (p. ej. FS7 calentando) */
    set_label_if_changed(m->value, buf);

    /* colores por estado */
    uint32_t accent = (st == CARD_ALARM) ? UI_C_ALARM_SOFT : (st == CARD_WARN ? UI_C_WARN : UI_C_OK);
    uint32_t valcol = (st == CARD_OK) ? UI_C_TEXT_STRONG : accent;
    set_txt_color(m->value, valcol);
    set_txt_color(m->title, (st == CARD_OK) ? UI_C_TEXT_3 : accent);
    set_txt_color(m->icon,  (st == CARD_OK) ? UI_C_TEXT_MUTED : accent);
    set_border(m->card, (st == CARD_OK) ? UI_C_BORDER : accent);
    set_border_opa_if(m->card, (st == CARD_OK) ? LV_OPA_60 : LV_OPA_COVER);

    /* zonas: pct de cada segmento. Solo se dibuja la banda de un limite si ese
     * limite esta HABILITADO (show_lo/show_hi); si no, el segmento va a 0. */
    bool draw_lo = (show_lo && low_zone);
    int lo = draw_lo ? (int)(clampf(safe_lo_frac, 0.f, 1.f) * 100.f) : 0;
    int hi = show_hi ? (int)(clampf(1.f - safe_hi_frac, 0.f, 1.f) * 100.f) : 0;
    int mid = 100 - lo - hi; if (mid < 0) mid = 0;
    lv_obj_set_width(m->seg_lo, LV_PCT(lo));
    lv_obj_set_width(m->seg_mid, LV_PCT(mid));
    lv_obj_set_width(m->seg_hi, LV_PCT(hi));
    set_bg(m->seg_lo, UI_C_ALARM);   set_bg_opa_if(m->seg_lo, LV_OPA_20);
    set_bg(m->seg_mid, UI_C_OK);     set_bg_opa_if(m->seg_mid, LV_OPA_30);
    set_bg(m->seg_hi, low_zone ? UI_C_ALARM : UI_C_WARN);
    set_bg_opa_if(m->seg_hi, LV_OPA_20);
    /* Ocultar del todo el segmento de un limite deshabilitado (no basta con
     * ancho 0%: garantizamos que no quede ninguna franja dibujada). */
    if (draw_lo) set_hidden(m->seg_lo, false);
    else         set_hidden(m->seg_lo, true);
    if (show_hi) set_hidden(m->seg_hi, false);
    else         set_hidden(m->seg_hi, true);

    /* marcador color + posición */
    set_bg(m->marker, accent);
    position_overlay(m->marker, m->bar, frac, 9);
    if (low_zone && show_lo) {
        set_hidden(m->tick_lo, false);
        position_overlay(m->tick_lo, m->bar, safe_lo_frac, 2);
    } else {
        set_hidden(m->tick_lo, true);
    }
    if (show_hi) {
        set_hidden(m->tick_hi, false);
        position_overlay(m->tick_hi, m->bar, safe_hi_frac, 2);
        set_bg(m->tick_hi, low_zone ? UI_C_OK_SOFT : UI_C_WARN_SOFT);
    } else {
        set_hidden(m->tick_hi, true);
    }

    /* pill de estado */
    uint32_t pill_bg = (st == CARD_ALARM) ? UI_C_ALARM_BG : (st == CARD_WARN ? UI_C_WARN_BG : UI_C_OK_BG);
    uint32_t pill_bd = (st == CARD_ALARM) ? UI_C_ALARM_BORDER : (st == CARD_WARN ? UI_C_WARN_BORDER : UI_C_OK_BORDER);
    set_bg(m->pill, pill_bg);
    set_border(m->pill, pill_bd);
    set_bg_opa_if(m->pill, LV_OPA_40);
    set_label_if_changed(m->pill_state, state_txt);
    set_txt_color(m->pill_state, accent);
    /* 1.6.8: min/max de las ultimas 24 h (NAN = aun sin dato valido -> "--"). */
    char mn_s[16] = "--", mx_s[16] = "--";
    if (isfinite(mn_disp)) snprintf(mn_s, sizeof(mn_s), "%.*f", dec, mn_disp);
    if (isfinite(mx_disp)) snprintf(mx_s, sizeof(mx_s), "%.*f", dec, mx_disp);
    snprintf(buf, sizeof(buf), "24H %s / %s", mn_s, mx_s);
    set_label_if_changed(m->pill_mm, buf);

    /* 1.5.33: con un estado largo ("ESTABILIZANDO", "SIN DATO NUEVO") los dos
     * textos no caben y el flex los SUPERPONIA. Si no caben, se oculta el 24H:
     * el estado manda. Antes del primer layout (ancho 0) no se decide. */
    lv_coord_t avail = lv_obj_get_content_width(m->pill);
    if (avail > 0) {
        lv_point_t st_sz, mm_sz;
        lv_text_get_size(&st_sz, state_txt, lv_obj_get_style_text_font(m->pill_state, LV_PART_MAIN),
                         lv_obj_get_style_text_letter_space(m->pill_state, LV_PART_MAIN), 0,
                         LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        lv_text_get_size(&mm_sz, buf, lv_obj_get_style_text_font(m->pill_mm, LV_PART_MAIN),
                         lv_obj_get_style_text_letter_space(m->pill_mm, LV_PART_MAIN), 0,
                         LV_COORD_MAX, LV_TEXT_FLAG_NONE);
        set_hidden(m->pill_mm, st_sz.x + mm_sz.x + 8 > avail);
    }
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
/* ---------- condiciones de alarma (1.6.8: pueden ser varias a la vez) ---------- */
/* Orden de presentacion: presion, flujo, sensores. */
static const uint32_t COND_ORDER[OV_MAX] = { ALARM_COND_P_LOW, ALARM_COND_P_HIGH, ALARM_COND_FLOW_HIGH,
                                             ALARM_COND_FLOW_DELTA, ALARM_COND_SENSOR };
static int cond_count(uint32_t c) { int n = 0; for (; c; c >>= 1) n += (int)(c & 1u); return n; }
static const char *cond_title(uint32_t bit)
{
    switch (bit) {
    case ALARM_COND_P_LOW:      return _t("PRESIÓN BAJA");
    case ALARM_COND_P_HIGH:     return _t("PRESIÓN ALTA");
    case ALARM_COND_FLOW_HIGH:  return _t("FLUJO ALTO");
    case ALARM_COND_FLOW_DELTA: return _t("VARIACIÓN DE FLUJO");
    default:                    return _t("FALLA DE SENSORES");
    }
}
/* Subtitulo corto del banner para UNA condicion (la causa, no un generico "vigilar"). */
static const char *cond_banner(uint32_t bit)
{
    switch (bit) {
    case ALARM_COND_P_LOW:      return _t("· presión baja");
    case ALARM_COND_P_HIGH:     return _t("· presión alta");
    case ALARM_COND_SENSOR:     return _t("· falla de sensores");
    default:                    return _t("· flujo anormal");
    }
}
/* Subtitulo del banner: la causa si es una; con varias, "· presión + flujo". */
static const char *banner_cause(void)
{
    static char buf[64];
    uint32_t c = s_alarm_conds;
    int n = cond_count(c);
    if (n == 0) return s_alarm_have_last ? _t("· revisar línea") : _t("· sin medición");
    if (n == 1) return cond_banner(c);
    const char *w[3]; int k = 0;
    if (c & (ALARM_COND_P_LOW | ALARM_COND_P_HIGH))         w[k++] = _t("presión");
    if (c & (ALARM_COND_FLOW_HIGH | ALARM_COND_FLOW_DELTA)) w[k++] = _t("flujo");
    if (c & ALARM_COND_SENSOR)                              w[k++] = _t("sensores");
    size_t used = (size_t)snprintf(buf, sizeof(buf), "·");
    for (int i = 0; i < k && used < sizeof(buf); i++)
        used += (size_t)snprintf(buf + used, sizeof(buf) - used, "%s%s", i ? " + " : " ", w[i]);
    return buf;
}
/* Valor actual y texto de limite de una condicion, en las unidades configuradas. */
static void cond_detail(uint32_t bit, const AppConfig *c, char *val, size_t vn,
                        char *unit, size_t un, char *lim, size_t ln)
{
    snprintf(val, vn, "--"); unit[0] = '\0'; lim[0] = '\0';
    if (bit == ALARM_COND_SENSOR) {
        snprintf(lim, ln, "%s", _t("sin datos válidos: revisar módulo y cable I2C"));
        return;
    }
    if (!s_alarm_have_last || !c) return;
    const char *pu = c->sensors.pressure_unit, *fu = c->sensors.flow_unit;
    if (bit == ALARM_COND_P_LOW || bit == ALARM_COND_P_HIGH) {
        int dec = press_unit_min_dec(pu); if (s_decimals > dec) dec = s_decimals;
        bool lo = bit == ALARM_COND_P_LOW;
        float lim_kpa = lo ? c->sensors.alarm_limits.pressure_min : c->sensors.alarm_limits.pressure_max;
        snprintf(val, vn, "%.*f", dec, (double)pressure_to_disp(s_alarm_last.pressure_kpa, pu));
        snprintf(unit, un, "%s", pu);
        snprintf(lim, ln, "%s %.*f %s", lo ? _t("mínimo") : _t("máximo"), dec,
                 (double)pressure_to_disp(lim_kpa, pu), pu);
        return;
    }
    int dec = flow_unit_min_dec(fu); if (s_decimals > dec) dec = s_decimals;
    snprintf(val, vn, "%.*f", dec, (double)flow_to_disp(s_alarm_last.flow_lpm, fu));
    snprintf(unit, un, "%s", fu);
    if (bit == ALARM_COND_FLOW_HIGH)
        snprintf(lim, ln, "%s %.*f %s", _t("límite"), dec,
                 (double)flow_to_disp(c->sensors.alarm_limits.flow_high_limit, fu), fu);
    else
        snprintf(lim, ln, "%s", _t("cambio brusco de flujo: posible fuga"));
}
static uint32_t overlay_signature(void)
{
    return s_alarm_conds | (s_alarm_crit << 8) | ((uint32_t)s_alarm_state << 16) |
           ((uint32_t)s_alarm_muted << 20);
}
static void alarm_overlay_close(void)
{
    if (s_alarm_overlay) { lv_obj_delete_async(s_alarm_overlay); s_alarm_overlay = NULL; }
    s_ov_n = 0;
}
static void alarm_overlay_close_cb(lv_event_t *e){(void)e;alarm_overlay_close();}
static void alarm_overlay_silence_cb(lv_event_t *e){(void)e;if(!s_alarm_muted)alarm_mgr_press_mute();alarm_overlay_close();}
/* Fila de la lista (2 o mas condiciones): punto de severidad, nombre + limite, valor en vivo. */
static void overlay_row(lv_obj_t *list, uint32_t bit, const AppConfig *c)
{
    bool crit = (s_alarm_crit & bit) != 0;
    char val[24], unit[12], lim[64];
    cond_detail(bit, c, val, sizeof val, unit, sizeof unit, lim, sizeof lim);
    lv_obj_t *r = ui_box(list);
    lv_obj_set_size(r, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(r, ui_col(0x000000), 0);
    lv_obj_set_style_bg_opa(r, LV_OPA_30, 0);
    lv_obj_set_style_radius(r, 8, 0);
    lv_obj_set_style_border_width(r, 1, 0);
    lv_obj_set_style_border_color(r, ui_col(crit ? UI_C_ALARM : UI_C_WARN), 0);
    lv_obj_set_style_pad_hor(r, 10, 0);
    lv_obj_set_style_pad_ver(r, 4, 0);
    lv_obj_set_style_pad_column(r, 9, 0);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *dot = ui_box(r);
    lv_obj_set_size(dot, 10, 10);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, ui_col(crit ? UI_C_ALARM : UI_C_WARN), 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
    lv_obj_t *col = ui_box(r);
    lv_obj_set_height(col, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(col, 1);
    lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
    ui_label(col, cond_title(bit), UI_FONT_MD, crit ? UI_C_ALARM_SOFT : UI_C_WARN_SOFT);
    ui_label(col, lim, UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_t *v = ui_label(r, val, UI_FONT_LG, UI_C_TEXT_STRONG);
    ui_label(r, unit, UI_FONT_XS, UI_C_TEXT_2);
    if (s_ov_n < OV_MAX) { s_ov_bit[s_ov_n] = bit; s_ov_val[s_ov_n] = v; s_ov_n++; }
}
static void alarm_overlay_open(void)
{
    if (s_alarm_overlay || s_alarm_state == ALARM_STATE_NORMAL) return;
    const AppConfig *c=appcfg_cache_peek(); bool crit=s_alarm_state==ALARM_STATE_ALERT; uint32_t ac=crit?UI_C_ALARM:UI_C_WARN;
    const int n = cond_count(s_alarm_conds);
    s_ov_sig = overlay_signature();
    s_ov_n = 0;
    s_alarm_overlay=lv_obj_create(lv_layer_top()); lv_obj_set_size(s_alarm_overlay,LV_PCT(100),LV_PCT(100)); lv_obj_set_style_bg_color(s_alarm_overlay,ui_col(crit?0x210d10:0x211a08),0); set_bg_opa_if(s_alarm_overlay, LV_OPA_COVER);
    lv_obj_set_style_border_width(s_alarm_overlay,5,0); lv_obj_set_style_border_color(s_alarm_overlay,ui_col(ac),0); lv_obj_set_style_pad_all(s_alarm_overlay,14,0); lv_obj_set_style_pad_row(s_alarm_overlay,6,0); lv_obj_set_flex_flow(s_alarm_overlay,LV_FLEX_FLOW_COLUMN); lv_obj_clear_flag(s_alarm_overlay,LV_OBJ_FLAG_SCROLLABLE);
    /* Encabezado: "ALARMA"/"ADVERTENCIA", o "N ALARMAS"/"N ADVERTENCIAS" si hay varias. */
    char head[32];
    if (n >= 2) snprintf(head, sizeof head, "%d %s", n, crit ? _t("ALARMAS") : _t("ADVERTENCIAS"));
    else snprintf(head, sizeof head, "%s", crit ? _t("ALARMA") : _t("ADVERTENCIA"));
    lv_obj_t *h=ui_box(s_alarm_overlay);lv_obj_set_size(h,LV_PCT(100),36);lv_obj_set_flex_flow(h,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(h,LV_FLEX_ALIGN_SPACE_BETWEEN,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_t *hl=ui_box(h);lv_obj_set_size(hl,LV_SIZE_CONTENT,36);lv_obj_set_flex_flow(hl,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(hl,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_column(hl,8,0);ui_icon(hl,UI_SYM_ALERT_TRIANGLE,UI_ICON_MD,ac);ui_label(hl,head,UI_FONT_XL,0xffe3e3);lv_obj_t *bk=ui_icon_badge(h,UI_SYM_ARROW_LEFT,UI_ICON_SM,UI_C_TEXT,0x35191c,34);lv_obj_add_flag(bk,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(bk,alarm_overlay_close_cb,LV_EVENT_CLICKED,NULL);
    lv_obj_t *ct=ui_box(s_alarm_overlay);lv_obj_set_width(ct,LV_PCT(100));lv_obj_set_flex_grow(ct,1);lv_obj_set_flex_flow(ct,LV_FLEX_FLOW_COLUMN);lv_obj_clear_flag(ct,LV_OBJ_FLAG_SCROLLABLE);
    if (n >= 2) {
        /* Varias condiciones: lista (como "Alarmas activas" de MedGuard). */
        lv_obj_set_flex_align(ct,LV_FLEX_ALIGN_START,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_row(ct,5,0);
        for (int i = 0; i < OV_MAX; i++)
            if (s_alarm_conds & COND_ORDER[i]) overlay_row(ct, COND_ORDER[i], c);
    } else {
        /* Una condicion: nombre, valor grande en vivo y limite. */
        uint32_t bit = s_alarm_conds ? s_alarm_conds : ALARM_COND_SENSOR;
        char val[24], unit[12], lim[64];
        cond_detail(bit, c, val, sizeof val, unit, sizeof unit, lim, sizeof lim);
        lv_obj_set_flex_align(ct,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_row(ct,5,0);
        ui_label(ct,s_alarm_conds?cond_title(bit):_t("FALLA DE MEDICIÓN"),UI_FONT_XL,crit?UI_C_ALARM_SOFT:UI_C_WARN_SOFT);
        lv_obj_t *vr=ui_box(ct);lv_obj_set_size(vr,LV_SIZE_CONTENT,LV_SIZE_CONTENT);lv_obj_set_flex_flow(vr,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(vr,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_END,LV_FLEX_ALIGN_END);lv_obj_set_style_pad_column(vr,7,0);
        lv_obj_t *v=ui_label(vr,val,UI_FONT_HUGE,UI_C_TEXT_STRONG);ui_label(vr,unit,UI_FONT_LG,UI_C_TEXT_2);ui_label(ct,lim,UI_FONT_SM,crit?UI_C_ALARM_SOFT:UI_C_WARN_DIM);
        if (s_alarm_conds) { s_ov_bit[0] = bit; s_ov_val[0] = v; s_ov_n = 1; }
    }
    lv_obj_t *s=ui_box(s_alarm_overlay);lv_obj_set_size(s,LV_PCT(100),44);ui_style_button(s,0xf1f3f5);lv_obj_add_flag(s,LV_OBJ_FLAG_CLICKABLE);/* ADVERTENCIA tambien se silencia (alarm_mgr: por reannounce_minutes; si escala a ALARMA el silencio se rompe) */
    lv_obj_add_event_cb(s,s_alarm_muted?alarm_overlay_close_cb:alarm_overlay_silence_cb,LV_EVENT_CLICKED,NULL);lv_obj_center(ui_label(s,s_alarm_muted?_t("Volver"):(crit?_t("Silenciar alarma"):_t("Silenciar advertencia")),UI_FONT_MD,0x2a1416));
}
/* Llamada desde ui_main_update: valores en vivo y reconstruccion si cambiaron las
 * condiciones (aparece una nueva, escala, se silencia). Sin alarma, se cierra. */
static void alarm_overlay_refresh(const AppConfig *c)
{
    if (!s_alarm_overlay) return;
    if (s_alarm_state == ALARM_STATE_NORMAL) { alarm_overlay_close(); return; }
    if (overlay_signature() != s_ov_sig) { alarm_overlay_close(); alarm_overlay_open(); return; }
    for (int i = 0; i < s_ov_n; i++) {
        char val[24], unit[12], lim[64];
        cond_detail(s_ov_bit[i], c, val, sizeof val, unit, sizeof unit, lim, sizeof lim);
        set_label_if_changed(s_ov_val[i], val);
    }
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
    set_bg_opa_if(s_data_chip, LV_OPA_COVER);
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
    set_hidden(ui_cloudTxArrowMain, true);
    lv_obj_t *sep = ui_box(ui_statusMainComm); lv_obj_set_size(sep, 1, 24); lv_obj_set_style_bg_color(sep, ui_col(UI_C_BORDER), 0); set_bg_opa_if(sep, LV_OPA_COVER);
    lv_obj_t *dt = ui_box(ui_statusMainComm); lv_obj_set_size(dt,72,32);
    s_clock_lbl = ui_label(dt, "--:--", UI_FONT_XS, UI_C_TEXT); lv_obj_set_size(s_clock_lbl,72,16); lv_obj_set_pos(s_clock_lbl,0,0); lv_obj_set_style_text_align(s_clock_lbl,LV_TEXT_ALIGN_RIGHT,0);
    s_date_lbl  = ui_label(dt, "--/--/----", UI_FONT_XS, UI_C_TEXT_MUTED); lv_obj_set_size(s_date_lbl,72,16); lv_obj_set_pos(s_date_lbl,0,16); lv_obj_set_style_text_align(s_date_lbl,LV_TEXT_ALIGN_RIGHT,0);
    lv_obj_t *menu = ui_box(ui_statusMainComm); lv_obj_set_size(menu,30,30); lv_obj_set_style_bg_color(menu,lv_color_hex(UI_C_CARD_BG),0); set_bg_opa_if(menu, LV_OPA_COVER); lv_obj_set_style_border_width(menu,1,0); lv_obj_set_style_border_color(menu,ui_col(UI_C_BORDER),0); lv_obj_set_style_radius(menu,7,0); lv_obj_set_flex_flow(menu,LV_FLEX_FLOW_ROW); lv_obj_set_flex_align(menu,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER); ui_label(menu,LV_SYMBOL_LIST,UI_FONT_SM,UI_C_TEXT);
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
    set_hidden(s_banner_right, true);

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
    lv_obj_set_style_pad_column(cons, 8, 0);
    /* 1.6.9 (pedido del usuario): "CONSUMO TOTAL" primero y grande (totalizador de vida,
     * persistente, ningun restablecer lo borra) y a continuacion, mas pequeno, el de HOY
     * (se reinicia a las 00:00; guardado cada 2 min) con "desde HH:MM" y "N min sin
     * medir" (antes la pildora PARCIAL). Todo seguido, sin hueco en el medio. */
    ui_icon(cons, UI_SYM_CHART_BAR, UI_ICON_SM, UI_C_TEXT_3);
    lv_obj_t *capcol = ui_box(cons);
    lv_obj_set_size(capcol, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(capcol, LV_FLEX_FLOW_COLUMN);
    s_consumo_title = ui_label(capcol, _t("CONSUMO"), UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_style_text_letter_space(s_consumo_title, 1, 0);
    s_total_cap = ui_label(capcol, _t("TOTAL"), UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_style_text_letter_space(s_total_cap, 1, 0);
    s_total_val = ui_label(cons, "--", UI_FONT_XXL, UI_C_TEXT_STRONG);
    lv_obj_t *today = ui_box(cons);
    lv_obj_set_size(today, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(today, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_margin_left(today, 8, 0);
    lv_obj_t *trow = ui_box(today);
    lv_obj_set_size(trow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(trow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(trow, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_column(trow, 5, 0);
    s_consumo_cap = ui_label(trow, _t("HOY"), UI_FONT_XS, UI_C_TEXT_3);
    lv_obj_set_style_text_letter_space(s_consumo_cap, 1, 0);
    s_consumo_val = ui_label(trow, "--", UI_FONT_MD, UI_C_TEXT_2);
    lv_obj_t *srow = ui_box(today);
    lv_obj_set_size(srow, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(srow, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(srow, 6, 0);
    s_consumo_sub = ui_label(srow, _t("sin hora"), UI_FONT_XS, UI_C_TEXT_MUTED);
    s_consumo_miss = ui_label(srow, "", UI_FONT_XS, UI_C_WARN_DIM);
    set_hidden(s_consumo_miss, true);

    /* ===== Banner de gas ===== */
    s_gas_banner = ui_box(ui_mainScreen);
    lv_obj_set_size(s_gas_banner, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_radius(s_gas_banner, UI_RADIUS_SM, 0);
    set_bg(s_gas_banner, UI_C_GAS_O2);
    set_bg_opa_if(s_gas_banner, LV_OPA_COVER);
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
    snprintf(s_clock_prev, sizeof(s_clock_prev), "%s", hhmm);
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
    if (s_consumo_cap)  set_label_if_changed(s_consumo_cap, _t("HOY"));
    if (s_total_cap)    set_label_if_changed(s_total_cap, _t("TOTAL"));
    if (s_consumo_title) set_label_if_changed(s_consumo_title, _t("CONSUMO"));
}

/* Banner por estado */
static void set_banner(alarm_clinical_state_t st, bool muted, const AppConfig *cfg)
{
    /* 1.6.8: con 2 o mas condiciones el titulo las cuenta ("2 ADVERTENCIAS",
     * "2 ALARMAS" si alguna es critica) y el subtitulo nombra las causas. */
    const int n = cond_count(s_alarm_conds);
    char title[32];
    if (st != ALARM_STATE_NORMAL && n >= 2)
        snprintf(title, sizeof title, "%d %s", n,
                 st == ALARM_STATE_ALERT ? _t("ALARMAS") : _t("ADVERTENCIAS"));
    else
        snprintf(title, sizeof title, "%s", st == ALARM_STATE_ALERT ? _t("ALARMA") : _t("ADVERTENCIA"));
    /* Pildora derecha: "silenciada" o "buzzer activo" (ADVERTENCIA y ALARMA por igual),
     * salvo que el tono de ese nivel este deshabilitado en la config. */
    const bool tone_on = st == ALARM_STATE_ALERT ? cfg->general.alarm.tone_alert
                                                 : cfg->general.alarm.tone_warn;
    if (st == ALARM_STATE_NORMAL) {
        set_bg(s_banner, UI_C_OK_BG); set_border(s_banner, UI_C_OK_BORDER);
        set_bg_opa_if(s_banner, LV_OPA_COVER);
        set_label_if_changed(s_banner_icon, UI_SYM_CIRCLE_CHECK); set_txt_color(s_banner_icon, UI_C_OK);
        set_label_if_changed(s_banner_txt, _t("SISTEMA NORMAL")); set_txt_color(s_banner_txt, UI_C_OK_SOFT);
        set_label_if_changed(s_banner_sub, _t("· sin alarmas")); set_txt_color(s_banner_sub, UI_C_OK_DIM);
        set_hidden(s_banner_right, true);
    } else if (st == ALARM_STATE_WARNING) {
        set_bg(s_banner, UI_C_WARN_BG); set_border(s_banner, UI_C_WARN_BORDER);
        set_bg_opa_if(s_banner, LV_OPA_COVER);
        set_label_if_changed(s_banner_icon, UI_SYM_ALERT_TRIANGLE); set_txt_color(s_banner_icon, UI_C_WARN);
        set_label_if_changed(s_banner_txt, title); set_txt_color(s_banner_txt, UI_C_WARN_SOFT);
        set_label_if_changed(s_banner_sub, banner_cause()); set_txt_color(s_banner_sub, UI_C_WARN_DIM);
        set_txt_color(s_banner_right_ic, UI_C_WARN_SOFT); set_txt_color(s_banner_right_tx, UI_C_WARN_SOFT);
    } else { /* ALERT */
        set_bg(s_banner, UI_C_ALARM_BG); set_border(s_banner, UI_C_ALARM_BORDER);
        set_bg_opa_if(s_banner, LV_OPA_COVER);
        set_label_if_changed(s_banner_icon, UI_SYM_ALERT_TRIANGLE_FILLED); set_txt_color(s_banner_icon, UI_C_ALARM_SOFT);
        set_label_if_changed(s_banner_txt, title); set_txt_color(s_banner_txt, 0xffd3d3);
        set_label_if_changed(s_banner_sub, banner_cause()); set_txt_color(s_banner_sub, UI_C_ALARM_SOFT);
        set_txt_color(s_banner_right_ic, UI_C_ALARM_SOFT); set_txt_color(s_banner_right_tx, UI_C_ALARM_SOFT);
    }
    if (st != ALARM_STATE_NORMAL) {
        set_hidden(s_banner_right, !muted && !tone_on);
        set_label_if_changed(s_banner_right_ic, muted ? UI_SYM_BELL_OFF : UI_SYM_BELL_RINGING);
        set_label_if_changed(s_banner_right_tx, muted ? _t("silenciada") : _t("buzzer activo"));
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
    s_alarm_conds = state == ALARM_STATE_NORMAL ? 0 : alarm_mgr_get_active_conditions(&s_alarm_crit);
    if (!s_alarm_conds) s_alarm_crit = 0;
    if (have_last) s_alarm_last = *last;

    /* ui_main_update ya corre bajo el lock del display. Un timestamp nuevo
     * equivale a una adquisicion completa de presion/flujo. */
    if (have_last && last && last->ts_ms != s_last_activity_sample_ts)
    {
        s_last_activity_sample_ts = last->ts_ms;
        ui_main_signal_data_activity();
    }

    set_banner(state, muted, cfg);
    alarm_overlay_refresh(cfg);

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
        /* 1.6.8: ambar = ADVERTENCIA, rojo = ALARMA (> 15 kPa mas alla del limite,
         * misma banda que alarm_mgr). Antes todo cruce se pintaba rojo. */
        card_state_t pst = CARD_OK; const char *ptx = _t("NORMAL");
        if (pmin_en && last->pressure_kpa < p_min) {
            pst = last->pressure_kpa < p_min - 15.0f ? CARD_ALARM : CARD_WARN; ptx = _t("BAJA");
        } else if (pmax_en && last->pressure_kpa > p_max) {
            pst = last->pressure_kpa > p_max + 15.0f ? CARD_ALARM : CARD_WARN; ptx = _t("ALTA");
        }

        const char *pu = cfg->sensors.pressure_unit;
        int pdec = s_decimals; if (press_unit_min_dec(pu) > pdec) pdec = press_unit_min_dec(pu);
        float pmn = have_mm ? pressure_to_disp(mn->pressure_kpa, pu) : NAN;
        float pmx = have_mm ? pressure_to_disp(mx->pressure_kpa, pu) : NAN;
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
        /* 1.6.8: la pildora sigue a las condiciones de alarm_mgr (flujo alto con su
         * limite real y la variacion brusca); antes miraba el 80 % del eje. */
        card_state_t fst = CARD_OK; const char *ftx = _t("NORMAL");
        if (s_alarm_conds & ALARM_COND_FLOW_HIGH)       { fst = CARD_WARN; ftx = _t("FLUJO ALTO"); }
        else if (s_alarm_conds & ALARM_COND_FLOW_DELTA) { fst = CARD_WARN; ftx = _t("VARIACIÓN"); }
        float fmn = have_mm ? flow_to_disp(mn->flow_lpm, fu) : NAN;
        float fmx = have_mm ? flow_to_disp(mx->flow_lpm, fu) : NAN;
        /* 1.5.33: durante el calentamiento del FS7 el valor grande es "--" (no
         * 0.00, que parece un flujo real) y la etiqueta central muestra el
         * progreso. La EMA se reinicia para arrancar del primer valor valido. */
        const bool f_warming = (last->invalid_mask & SENSOR_INVALID_FLOW) &&
                               sensors_runtime_flow_warming();
        if (f_warming) { s_f_disp_ema = NAN; f_show = NAN; }
        if (last->invalid_mask & SENSOR_INVALID_FLOW) {
            fst = CARD_WARN;
            ftx = f_warming ? _t("ESTABILIZANDO") : _t("SIN DATO NUEVO");
        }
        /* 1.6.11: la zona alta y la etiqueta usan el LIMITE configurado de flujo alto
         * (antes un 80 % fijo del eje: el "80.00 alto" no salia de la config). */
        const float f_hi_lim = cfg->sensors.alarm_limits.flow_high_limit;
        const float f_hi_frac = clampf(f_hi_lim / f_axis, 0.f, 1.f);
        update_metric_card(&s_flow, f_show, f_frac, 0.f, f_hi_frac, false,
                           false, fhi_en, fdec, fst, ftx, fmn, fmx);
        char fb[24];
        snprintf(fb, sizeof(fb), "%.*f", fdec, flow_to_disp(f_axis, fu));
        set_label_if_changed(s_flow.ax_right, fb);
        /* Etiqueta central = umbral de "alto" (solo si la alarma de flujo alto
         * esta habilitada), o el progreso del calentamiento. */
        char fmb[36];
        if (f_warming)
            snprintf(fmb, sizeof(fmb), "%s %u s", _t("calentando"),
                     (unsigned)(sensors_runtime_flow_warming_elapsed_ms() / 1000));
        else if (fhi_en)
            snprintf(fmb, sizeof(fmb), "%.*f %s",
                     fdec, (double)flow_to_disp(f_hi_lim, fu), _t("alto"));
        else
            snprintf(fmb, sizeof(fmb), "%s", _t("sin limites"));
        set_label_if_changed(s_flow.ax_mid, fmb);
        set_txt_color(s_flow.ax_mid, f_warming ? UI_C_WARN : UI_C_OK_DIM);

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

    }

    /* --- Consumo: HOY + TOTAL (1.6.8) --- */
    flow_meter_snapshot_t meter = flow_meter_get();
    char cb[40];
    if (s_consumo_val) {
        if (meter.volume_m3 < 0.1)
            snprintf(cb, sizeof(cb), "%.1f L", meter.volume_m3 * 1000.0);
        else
            snprintf(cb, sizeof(cb), "%.2f m\xC2\xB3", meter.volume_m3);
        set_label_if_changed(s_consumo_val, cb);
    }
    if (s_total_val) {
        double t = meter.total_m3;
        if (t < 1.0)          snprintf(cb, sizeof(cb), "%.1f L", t * 1000.0);
        else if (t < 1000.0)  snprintf(cb, sizeof(cb), "%.2f m\xC2\xB3", t);
        else if (t < 1e5)     snprintf(cb, sizeof(cb), "%.1f m\xC2\xB3", t);
        else                  snprintf(cb, sizeof(cb), "%.0f m\xC2\xB3", t);
        set_label_if_changed(s_total_val, cb);
    }
    if (s_consumo_sub) {
        /* Periodo del conteo de hoy: "00:00–HH:MM" (inicio real -> ahora, avanza con el
         * reloj; 1.6.10, antes "desde 00:00" fijo parecia un reloj detenido). El inicio
         * es 00:00, o la hora de arranque si el equipo arranco hoy sin un guardado del
         * mismo dia (corte que cruzo la medianoche). */
        const bool clock_ok = s_clock_prev[0] >= '0' && s_clock_prev[0] <= '9';
        if (meter.since_min >= 0 && clock_ok)
            snprintf(cb, sizeof(cb), "%02d:%02d\xE2\x80\x93%s", meter.since_min / 60, meter.since_min % 60,
                     s_clock_prev);
        else if (meter.since_min >= 0)
            snprintf(cb, sizeof(cb), "%s %02d:%02d", _t("desde"), meter.since_min / 60, meter.since_min % 60);
        else
            snprintf(cb, sizeof(cb), "%s", _t("sin hora"));
        set_label_if_changed(s_consumo_sub, cb);
    }
    if (s_consumo_miss) {
        /* Tiempo de hoy sin medicion (calentamiento del FS7, reinicios, cortes de
         * energia). Bajo 1 min no se muestra. */
        unsigned m = (unsigned)(meter.missing_ms / 60000);
        if (m >= 60) snprintf(cb, sizeof(cb), "· %u h %u min %s", m / 60, m % 60, _t("sin medir"));
        else         snprintf(cb, sizeof(cb), "· %u min %s", m, _t("sin medir"));
        set_label_if_changed(s_consumo_miss, cb);
        set_hidden(s_consumo_miss, m == 0);
    }
}
