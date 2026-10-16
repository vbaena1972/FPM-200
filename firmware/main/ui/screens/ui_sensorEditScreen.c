#include "ui_sensorEditScreen.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_cfg.h"
#include "ui_i18n.h"
#include "ui_nav.h"
#include "ui.h"
#include <string.h>
#include <stdio.h>

lv_obj_t *ui_sensorEditScreen = NULL;

/* 1.6.13: los +/- de los limites ya NO reconstruyen la pantalla (antes cada toque
 * creaba una pantalla nueva: se perdia el scroll y los toques durante la
 * reconstruccion; ademas cada toque guardaba la config completa en NVS). Ahora se
 * actualiza solo el numero, el cambio rige al instante (cache en vivo) y la NVS se
 * escribe una vez, SAVE_DELAY_MS despues del ultimo toque o al salir. */
#define SAVE_DELAY_MS 1500
enum { V_PMIN, V_PMAX, V_FDELTA, V_FHIGH, V_COUNT };
static lv_obj_t *s_val_lbl[V_COUNT];
static lv_timer_t *s_save_timer;

static int vidx(ui_edit_target_t t)
{
    switch (t) {
    case UI_EDIT_PRES_MIN:   return V_PMIN;
    case UI_EDIT_PRES_MAX:   return V_PMAX;
    case UI_EDIT_FLOW_DELTA: return V_FDELTA;
    case UI_EDIT_FLOW_HIGH:  return V_FHIGH;
    default:                 return -1;
    }
}
static void value_text(int i, char *b, size_t n)
{
    char v[16];
    if (i == V_PMIN || i == V_PMAX) {
        ui_cfg_press_fmt(v, sizeof(v), ui_cfg_press_to_disp(i == V_PMAX ? ui_cfg_pressure_max()
                                                                          : ui_cfg_pressure_min()));
        snprintf(b, n, "%s %s", v, ui_cfg_pressure_unit());
    } else {
        ui_cfg_flow_fmt(v, sizeof(v), ui_cfg_flow_to_disp(ui_cfg_flow_limit(i == V_FHIGH)));
        snprintf(b, n, "%s %s", v, ui_cfg_flow_unit());
    }
}
static void refresh_values(void)
{
    char b[28];
    for (int i = 0; i < V_COUNT; i++) {
        if (!s_val_lbl[i]) continue;
        value_text(i, b, sizeof(b));
        if (strcmp(lv_label_get_text(s_val_lbl[i]), b) != 0) lv_label_set_text(s_val_lbl[i], b);
    }
}

static void save_timer_cb(lv_timer_t *t)
{
    (void)t;
    s_save_timer = NULL;          /* temporizador de una vuelta: LVGL lo borra */
    ui_cfg_save_now();
}
static void save_flush(void)
{
    if (!s_save_timer) return;
    lv_timer_delete(s_save_timer);
    s_save_timer = NULL;
    ui_cfg_save_now();
}
static void save_later(void)
{
    if (s_save_timer) { lv_timer_reset(s_save_timer); return; }
    s_save_timer = lv_timer_create(save_timer_cb, SAVE_DELAY_MS, NULL);
    lv_timer_set_repeat_count(s_save_timer, 1);
}

/* Al borrar la pantalla: guardar lo pendiente y soltar los labels (solo si es la
 * pantalla vigente: rebuild() crea la nueva antes de borrar la vieja). */
static void screen_del_cb(lv_event_t *e)
{
    save_flush();
    if (lv_event_get_target(e) == ui_sensorEditScreen || !ui_sensorEditScreen)
        memset(s_val_lbl, 0, sizeof(s_val_lbl));
}

/* Unidades/decimales/norma cambian todos los textos: se reconstruye, pero
 * conservando la posicion del scroll (antes volvia arriba). */
static void rebuild(void)
{
    save_flush();
    lv_obj_t *o = ui_sensorEditScreen;
    const int32_t y = o ? lv_obj_get_scroll_y(o) : 0;
    ui_sensorEditScreen = NULL;
    ui_sensorEditScreen_screen_init();
    lv_obj_update_layout(ui_sensorEditScreen);
    lv_obj_scroll_to_y(ui_sensorEditScreen, y, LV_ANIM_OFF);
    ui_nav_replace(ui_sensorEditScreen);
    lv_obj_delete_delayed(o, 250);
}

/* Tras editar un limite con el teclado (ui_confirmScreen): la pantalla sigue en la
 * pila; basta con actualizar los numeros en su lugar (conserva el scroll). */
void ui_sensorEditScreen_refresh(void)
{
    if (!ui_sensorEditScreen) return;
    refresh_values();
}

/* ---- unidades ---- */
static bool is_press_unit(const char *k)
{
    return !strcmp(k, "psi") || !strcmp(k, "bar") || !strcmp(k, "kpa") || !strcmp(k, "mpa");
}
static void unit_cb(lv_event_t *e)
{
    const char *k = lv_event_get_user_data(e);
    save_flush();
    if (is_press_unit(k)) ui_cfg_set_pressure_unit(k);
    else                  ui_cfg_set_flow_unit(k);
    rebuild();
}
static void seg(lv_obj_t *p, const char *t, bool on, const char *k)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_height(b, 28);
    ui_style_button(b, on ? UI_C_OK_BG : UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, t, UI_FONT_XS, on ? UI_C_OK : UI_C_TEXT_2));
    lv_obj_add_event_cb(b, unit_cb, LV_EVENT_CLICKED, (void *)k);
}

/* ---- decimales del dashboard (0 o 1) ---- */
static void dec_cb(lv_event_t *e)
{
    save_flush();
    ui_cfg_set_decimals((int)(intptr_t)lv_event_get_user_data(e));
    rebuild();
}
static void dec_seg(lv_obj_t *p, const char *t, bool on, int d)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_height(b, 28);
    ui_style_button(b, on ? UI_C_OK_BG : UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, t, UI_FONT_XS, on ? UI_C_OK : UI_C_TEXT_2));
    lv_obj_add_event_cb(b, dec_cb, LV_EVENT_CLICKED, (void *)(intptr_t)d);
}

/* ---- gas + norma ---- */
static void gas_cb(lv_event_t *e)
{
    lv_obj_t *dd = lv_event_get_target(e);
    save_flush();
    ui_cfg_set_gas(ui_cfg_gas_key((int)lv_dropdown_get_selected(dd)));
}
static void std_cb(lv_event_t *e)
{
    save_flush();
    ui_cfg_set_color_code((const char *)lv_event_get_user_data(e));
    rebuild();
}
static void std_seg(lv_obj_t *p, const char *t, bool on, const char *code)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_size(b, 60, 28);
    ui_style_button(b, on ? UI_C_OK_BG : UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, t, UI_FONT_XS, on ? UI_C_OK : UI_C_TEXT_2));
    lv_obj_add_event_cb(b, std_cb, LV_EVENT_CLICKED, (void *)code);
}

/* ---- límites (presión/flujo) ---- */
static void p_switch(lv_event_t *e)
{
    bool hi = (bool)(intptr_t)lv_event_get_user_data(e);
    save_flush();
    ui_cfg_set_pressure_limit_enabled(hi, lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
}
static void limit_edit(lv_event_t *e)
{
    save_flush();
    ui_edit_begin((ui_edit_target_t)(intptr_t)lv_event_get_user_data(e));
    ui_open_keypad_cb(e);
}
static void f_switch(lv_event_t *e)
{
    bool hi = (bool)(intptr_t)lv_event_get_user_data(e);
    save_flush();
    ui_cfg_set_flow_alarm(hi, lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED), ui_cfg_flow_limit(hi));
}
/* Un paso de +/- (toque corto, y repetido mientras se mantiene presionado). */
static void limit_step(lv_event_t *e)
{
    int c = (int)(intptr_t)lv_event_get_user_data(e);
    ui_edit_target_t t = (ui_edit_target_t)(c < 0 ? -c : c);
    ui_edit_begin(t);
    float step = (t == UI_EDIT_FLOW_HIGH ? 5.f : 1.f);
    float v = ui_edit_new() + (c > 0 ? step : -step);
    const float vmin = (t == UI_EDIT_FLOW_HIGH || t == UI_EDIT_FLOW_DELTA) ? 1.f : 0.f;
    if (v < vmin) v = vmin;     /* sin limites negativos (flujo >= 1, como ui_cfg_set_flow_alarm) */
    ui_edit_set_new(v);
    ui_edit_apply_unsaved();
    refresh_values();
    save_later();
}
static lv_obj_t *card(lv_obj_t *p, const char *t)
{
    lv_obj_t *c = ui_card(p);
    lv_obj_set_flex_grow(c, 1);
    lv_obj_set_height(c, 82);
    lv_obj_set_style_pad_all(c, 8, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(c, 4, 0);
    ui_label(c, t, UI_FONT_XS, UI_C_TEXT_2);
    return c;
}
static void mini(lv_obj_t *p, const char *t, int code)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_size(b, 36, 36);   /* 1.6.13: antes 30x30 */
    ui_style_button(b, UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, t, UI_FONT_LG, UI_C_TEXT));
    /* SHORT_CLICKED + LONG_PRESSED(+REPEAT): un toque = un paso; mantener = repetir
     * (CLICKED sumaria un paso extra al soltar tras mantener). */
    lv_obj_add_event_cb(b, limit_step, LV_EVENT_SHORT_CLICKED, (void *)(intptr_t)code);
    lv_obj_add_event_cb(b, limit_step, LV_EVENT_LONG_PRESSED, (void *)(intptr_t)code);
    lv_obj_add_event_cb(b, limit_step, LV_EVENT_LONG_PRESSED_REPEAT, (void *)(intptr_t)code);
}
static void value_button(lv_obj_t *p, ui_edit_target_t target, uint32_t color)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_height(b, 36);
    ui_style_button(b, UI_C_CARD_BG2);
    char txt[28];
    const int i = vidx(target);
    value_text(i, txt, sizeof(txt));
    lv_obj_t *l = ui_label(b, txt, UI_FONT_MD, color);
    lv_obj_center(l);
    if (i >= 0) s_val_lbl[i] = l;
    lv_obj_add_event_cb(b, limit_edit, LV_EVENT_CLICKED, (void *)(intptr_t)target);
}
static lv_obj_t *limit_row(lv_obj_t *c)
{
    lv_obj_t *r = ui_box(c);
    lv_obj_set_size(r, LV_PCT(100), 45);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(r, 5, 0);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return r;
}
static void pressure_card(lv_obj_t *p, const char *t, bool hi)
{
    ui_edit_target_t target = hi ? UI_EDIT_PRES_MAX : UI_EDIT_PRES_MIN;
    lv_obj_t *r = limit_row(card(p, t));
    lv_obj_t *s = lv_switch_create(r);
    lv_obj_set_size(s, 38, 20);
    lv_obj_set_ext_click_area(s, 8);
    if (ui_cfg_pressure_limit_enabled(hi)) lv_obj_add_state(s, LV_STATE_CHECKED);
    lv_obj_add_event_cb(s, p_switch, LV_EVENT_VALUE_CHANGED, (void *)(intptr_t)hi);
    mini(r, "-", -target);
    value_button(r, target, UI_C_ALARM);
    mini(r, "+", target);
}
static void flow_card(lv_obj_t *p, const char *t, bool hi)
{
    ui_edit_target_t target = hi ? UI_EDIT_FLOW_HIGH : UI_EDIT_FLOW_DELTA;
    lv_obj_t *r = limit_row(card(p, t));
    lv_obj_t *s = lv_switch_create(r);
    lv_obj_set_size(s, 38, 20);
    lv_obj_set_ext_click_area(s, 8);
    if (ui_cfg_flow_limit_enabled(hi)) lv_obj_add_state(s, LV_STATE_CHECKED);
    lv_obj_add_event_cb(s, f_switch, LV_EVENT_VALUE_CHANGED, (void *)(intptr_t)hi);
    mini(r, "-", -target);
    value_button(r, target, UI_C_WARN);
    mini(r, "+", target);
}

/* fila de segmentos con etiqueta a la izquierda */
static lv_obj_t *unit_row(lv_obj_t *parent, const char *label)
{
    lv_obj_t *row = ui_box(parent);
    lv_obj_set_size(row, LV_PCT(100), 30);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(row, 5, 0);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *l = ui_label(row, label, UI_FONT_XS, UI_C_TEXT_2);
    lv_obj_set_width(l, 52);
    return row;
}

void ui_sensorEditScreen_screen_init(void)
{
    const char *pu = ui_cfg_pressure_unit();
    const char *fu = ui_cfg_flow_unit();
    bool iso = ui_cfg_color_is_iso();

    ui_sensorEditScreen = ui_screen_base();
    memset(s_val_lbl, 0, sizeof(s_val_lbl));
    lv_obj_add_event_cb(ui_sensorEditScreen, screen_del_cb, LV_EVENT_DELETE, NULL);
    lv_obj_set_flex_flow(ui_sensorEditScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ui_sensorEditScreen, 8, 0);
    lv_obj_set_style_pad_row(ui_sensorEditScreen, 6, 0);
    /* Ahora hay unidades completas + gas + límites: puede pasar de 320 px, así que
     * se reactiva el scroll vertical (ui_screen_base lo desactiva por defecto). */
    lv_obj_add_flag(ui_sensorEditScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(ui_sensorEditScreen, LV_DIR_VER);
    ui_nav_header(ui_sensorEditScreen, _t("Sensores"));

    /* ---- Unidades (presión + flujo, todas) ---- */
    lv_obj_t *u = ui_card(ui_sensorEditScreen);
    lv_obj_set_size(u, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(u, 7, 0);
    lv_obj_set_flex_flow(u, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(u, 6, 0);
    lv_obj_t *pr = unit_row(u, _t("Presión"));
    seg(pr, "psi",  !strcmp(pu, "psi"), "psi");
    seg(pr, "bar",  !strcmp(pu, "bar"), "bar");
    seg(pr, "kPa",  !strcmp(pu, "kpa"), "kpa");
    seg(pr, "MPa",  !strcmp(pu, "mpa"), "mpa");
    lv_obj_t *fr = unit_row(u, _t("Flujo"));
    seg(fr, "L/min", !strcmp(fu, "lpm"),  "lpm");
    seg(fr, "slm",   !strcmp(fu, "slpm"), "slpm");
    seg(fr, "m³/h",  !strcmp(fu, "m3h"),  "m3h");
    seg(fr, "SCCM",  !strcmp(fu, "sccm"), "sccm");

    /* ---- Decimales en el dashboard ---- */
    lv_obj_t *dcr = unit_row(u, _t("Decimales"));
    dec_seg(dcr, "0", ui_cfg_decimals() == 0, 0);
    dec_seg(dcr, "1", ui_cfg_decimals() == 1, 1);

    /* ---- Gas + norma de color ---- */
    lv_obj_t *g = ui_card(ui_sensorEditScreen);
    lv_obj_set_size(g, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(g, 7, 0);
    lv_obj_set_flex_flow(g, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(g, 8, 0);
    lv_obj_set_flex_align(g, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ui_label(g, _t("Gas"), UI_FONT_XS, UI_C_TEXT_2);

    lv_obj_t *dd = lv_dropdown_create(g);
    lv_obj_set_flex_grow(dd, 1);
    lv_obj_set_height(dd, 32);
    lv_obj_set_style_bg_color(dd, ui_col(UI_C_CARD_BG2), 0);
    lv_obj_set_style_border_color(dd, ui_col(UI_C_BORDER), 0);
    lv_obj_set_style_text_color(dd, ui_col(UI_C_TEXT), 0);
    char opts[256];
    opts[0] = '\0';
    for (int i = 0; i < ui_cfg_gas_count(); i++) {
        if (i) strncat(opts, "\n", sizeof(opts) - strlen(opts) - 1);
        strncat(opts, ui_cfg_gas_name(i), sizeof(opts) - strlen(opts) - 1);
    }
    lv_dropdown_set_options(dd, opts);
    lv_dropdown_set_selected(dd, (uint32_t)ui_cfg_gas_index());
    lv_obj_add_event_cb(dd, gas_cb, LV_EVENT_VALUE_CHANGED, NULL);

    std_seg(g, "NFPA", !iso, "nfpa");
    std_seg(g, "ISO",   iso, "iso");

    /* ---- Límites ---- */
    lv_obj_t *p = ui_box(ui_sensorEditScreen);
    lv_obj_set_size(p, LV_PCT(100), 82);
    lv_obj_set_flex_flow(p, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(p, 6, 0);
    pressure_card(p, _t("Presión baja"), false);
    pressure_card(p, _t("Presión alta"), true);

    lv_obj_t *f = ui_box(ui_sensorEditScreen);
    lv_obj_set_size(f, LV_PCT(100), 82);
    lv_obj_set_flex_flow(f, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(f, 6, 0);
    flow_card(f, _t("Cambio abrupto de flujo"), false);
    flow_card(f, _t("Flujo alto sostenido"), true);
}

void ui_sensorEditScreen_screen_destroy(void)
{
    if (ui_sensorEditScreen) { lv_obj_del(ui_sensorEditScreen); ui_sensorEditScreen = NULL; }
}
