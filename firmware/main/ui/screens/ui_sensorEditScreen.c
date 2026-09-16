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

static void rebuild(void)
{
    lv_obj_t *o = ui_sensorEditScreen;
    ui_sensorEditScreen = NULL;
    ui_sensorEditScreen_screen_init();
    ui_nav_replace(ui_sensorEditScreen);
    lv_obj_delete_delayed(o, 250);
}

/* Reconstruye la pantalla en su lugar dentro de la pila SIN cargarla (se usa
 * cuando la pantalla activa es otra, p.ej. el dialogo de confirmacion del
 * teclado). Al hacer pop_to despues, se mostrara la version nueva. */
void ui_sensorEditScreen_refresh(void)
{
    if (!ui_sensorEditScreen) return;
    lv_obj_t *old = ui_sensorEditScreen;
    ui_sensorEditScreen = NULL;
    ui_sensorEditScreen_screen_init();
    ui_nav_swap(old, ui_sensorEditScreen);
    lv_obj_del(old);
}

/* ---- unidades ---- */
static bool is_press_unit(const char *k)
{
    return !strcmp(k, "psi") || !strcmp(k, "bar") || !strcmp(k, "kpa") || !strcmp(k, "mpa");
}
static void unit_cb(lv_event_t *e)
{
    const char *k = lv_event_get_user_data(e);
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
    ui_cfg_set_gas(ui_cfg_gas_key((int)lv_dropdown_get_selected(dd)));
}
static void std_cb(lv_event_t *e)
{
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
    ui_cfg_set_pressure_limit_enabled(hi, lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED));
}
static void limit_edit(lv_event_t *e)
{
    ui_edit_begin((ui_edit_target_t)(intptr_t)lv_event_get_user_data(e));
    ui_open_keypad_cb(e);
}
static void f_switch(lv_event_t *e)
{
    bool hi = (bool)(intptr_t)lv_event_get_user_data(e);
    ui_cfg_set_flow_alarm(hi, lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED), ui_cfg_flow_limit(hi));
}
static void limit_step(lv_event_t *e)
{
    int c = (int)(intptr_t)lv_event_get_user_data(e);
    ui_edit_target_t t = (ui_edit_target_t)(c < 0 ? -c : c);
    ui_edit_begin(t);
    float step = (t == UI_EDIT_FLOW_HIGH ? 5.f : 1.f);
    ui_edit_set_new(ui_edit_new() + (c > 0 ? step : -step));
    ui_edit_apply();
    rebuild();
}
static lv_obj_t *card(lv_obj_t *p, const char *t)
{
    lv_obj_t *c = ui_card(p);
    lv_obj_set_flex_grow(c, 1);
    lv_obj_set_height(c, 78);
    lv_obj_set_style_pad_all(c, 8, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(c, 4, 0);
    ui_label(c, t, UI_FONT_XS, UI_C_TEXT_2);
    return c;
}
static void mini(lv_obj_t *p, const char *t, int code)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_size(b, 30, 30);
    ui_style_button(b, UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, t, UI_FONT_MD, UI_C_TEXT));
    lv_obj_add_event_cb(b, limit_step, LV_EVENT_CLICKED, (void *)(intptr_t)code);
}
static void value_button(lv_obj_t *p, const char *txt, ui_edit_target_t target, uint32_t color)
{
    lv_obj_t *b = lv_button_create(p);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_height(b, 30);
    ui_style_button(b, UI_C_CARD_BG2);
    lv_obj_center(ui_label(b, txt, UI_FONT_MD, color));
    lv_obj_add_event_cb(b, limit_edit, LV_EVENT_CLICKED, (void *)(intptr_t)target);
}
static void pressure_card(lv_obj_t *p, const char *t, bool hi)
{
    ui_edit_target_t target = hi ? UI_EDIT_PRES_MAX : UI_EDIT_PRES_MIN;
    lv_obj_t *c = card(p, t);
    lv_obj_t *r = ui_box(c);
    lv_obj_set_size(r, LV_PCT(100), 45);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(r, 5, 0);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *s = lv_switch_create(r);
    lv_obj_set_size(s, 38, 20);
    if (ui_cfg_pressure_limit_enabled(hi)) lv_obj_add_state(s, LV_STATE_CHECKED);
    lv_obj_add_event_cb(s, p_switch, LV_EVENT_VALUE_CHANGED, (void *)(intptr_t)hi);
    mini(r, "-", -target);
    char b[24], n[16];
    ui_cfg_press_fmt(n, sizeof(n), ui_cfg_press_to_disp(hi ? ui_cfg_pressure_max() : ui_cfg_pressure_min()));
    snprintf(b, sizeof(b), "%s %s", n, ui_cfg_pressure_unit());
    value_button(r, b, target, UI_C_ALARM);
    mini(r, "+", target);
}
static void flow_card(lv_obj_t *p, const char *t, bool hi)
{
    ui_edit_target_t target = hi ? UI_EDIT_FLOW_HIGH : UI_EDIT_FLOW_DELTA;
    lv_obj_t *c = card(p, t);
    lv_obj_t *r = ui_box(c);
    lv_obj_set_size(r, LV_PCT(100), 45);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(r, 5, 0);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *s = lv_switch_create(r);
    lv_obj_set_size(s, 38, 20);
    if (ui_cfg_flow_limit_enabled(hi)) lv_obj_add_state(s, LV_STATE_CHECKED);
    lv_obj_add_event_cb(s, f_switch, LV_EVENT_VALUE_CHANGED, (void *)(intptr_t)hi);
    mini(r, "-", -target);
    char b[28], n[16];
    ui_cfg_flow_fmt(n, sizeof(n), ui_cfg_flow_to_disp(ui_cfg_flow_limit(hi)));
    snprintf(b, sizeof(b), "%s %s", n, ui_cfg_flow_unit());
    value_button(r, b, target, UI_C_WARN);
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
    lv_obj_set_size(p, LV_PCT(100), 78);
    lv_obj_set_flex_flow(p, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(p, 6, 0);
    pressure_card(p, _t("Presión baja"), false);
    pressure_card(p, _t("Presión alta"), true);

    lv_obj_t *f = ui_box(ui_sensorEditScreen);
    lv_obj_set_size(f, LV_PCT(100), 78);
    lv_obj_set_flex_flow(f, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(f, 6, 0);
    flow_card(f, _t("Cambio abrupto de flujo"), false);
    flow_card(f, _t("Flujo alto sostenido"), true);
}

void ui_sensorEditScreen_screen_destroy(void)
{
    if (ui_sensorEditScreen) { lv_obj_del(ui_sensorEditScreen); ui_sensorEditScreen = NULL; }
}
