#include "ui_sensorDiagScreen.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_cfg.h"
#include "ui_i18n.h"
#include "ui_nav.h"
#include "ui.h"
#include "alarm_mgr.h"

lv_obj_t *ui_sensorDiagScreen = NULL;
static lv_obj_t *s_vol;

static void volume_cb(lv_event_t *e)
{
    int v = lv_slider_get_value(lv_event_get_target(e));
    lv_label_set_text_fmt(s_vol, "%d%%", v);
    if (lv_event_get_code(e) == LV_EVENT_RELEASED)
        ui_cfg_set_alarm_audio(v, ui_cfg_reannounce_minutes(), ui_cfg_max_silence_minutes());
}
static void test_cb(lv_event_t *e) { (void)e; alarm_mgr_test_buzzer(); }

/* 1.6.13: como en Sensores, los +/- ya no reconstruyen la pantalla ni guardan en
 * NVS en cada toque (el touch se sentia "duro"): actualizan el numero y el cache, y
 * la NVS se escribe una vez, 1,5 s despues del ultimo toque o al salir. */
static lv_obj_t *s_min_lbl[3];          /* [1] reanunciar, [2] silencio maximo */
static lv_timer_t *s_save_timer;
static void save_timer_cb(lv_timer_t *t) { (void)t; s_save_timer = NULL; ui_cfg_save_now(); }
static void save_flush(void)
{
    if (!s_save_timer) return;
    lv_timer_delete(s_save_timer);
    s_save_timer = NULL;
    ui_cfg_save_now();
}
static void refresh_values(void)
{
    if (s_min_lbl[1]) lv_label_set_text_fmt(s_min_lbl[1], "%d min", ui_cfg_reannounce_minutes());
    if (s_min_lbl[2]) lv_label_set_text_fmt(s_min_lbl[2], "%d min", ui_cfg_max_silence_minutes());
}
static void screen_del_cb(lv_event_t *e)
{
    save_flush();
    if (lv_event_get_target(e) == ui_sensorDiagScreen || !ui_sensorDiagScreen)
        s_min_lbl[1] = s_min_lbl[2] = NULL;
}

/* Tras editar con el teclado (confirm): la pantalla sigue en la pila; basta con
 * actualizar los numeros en su lugar. */
void ui_sensorDiagScreen_refresh(void)
{
    if (!ui_sensorDiagScreen) return;
    refresh_values();
}
static void step_cb(lv_event_t *e)
{
    int code = (int)(intptr_t)lv_event_get_user_data(e);
    int r = ui_cfg_reannounce_minutes(), s = ui_cfg_max_silence_minutes();
    if (code == 1 && r < 60) r++;
    if (code == -1 && r > 1) r--;
    if (code == 2 && s < 15) s++;
    if (code == -2 && s > 1) s--;
    const bool re = code == 1 || code == -1;
    ui_edit_begin(re ? UI_EDIT_AUDIO_REANNOUNCE : UI_EDIT_AUDIO_SILENCE);
    ui_edit_set_new((float)(re ? r : s));
    ui_edit_apply_unsaved();
    refresh_values();
    if (s_save_timer) lv_timer_reset(s_save_timer);
    else {
        s_save_timer = lv_timer_create(save_timer_cb, 1500, NULL);
        lv_timer_set_repeat_count(s_save_timer, 1);
    }
}
static void edit_cb(lv_event_t *e)
{
    int kind = (int)(intptr_t)lv_event_get_user_data(e);
    ui_edit_begin(kind == 1 ? UI_EDIT_AUDIO_REANNOUNCE : UI_EDIT_AUDIO_SILENCE);
    ui_open_keypad_cb(e);
}
static lv_obj_t *small_button(lv_obj_t *parent, const char *text, int code)
{
    lv_obj_t *b = lv_button_create(parent);
    ui_style_button(b, UI_C_CARD_BG2);
    lv_obj_set_size(b, 38, 40);
    lv_obj_t *l = ui_label(b, text, UI_FONT_LG, UI_C_TEXT);
    lv_obj_center(l);
    /* toque = un paso; mantener = repetir */
    lv_obj_add_event_cb(b, step_cb, LV_EVENT_SHORT_CLICKED, (void *)(intptr_t)code);
    lv_obj_add_event_cb(b, step_cb, LV_EVENT_LONG_PRESSED, (void *)(intptr_t)code);
    lv_obj_add_event_cb(b, step_cb, LV_EVENT_LONG_PRESSED_REPEAT, (void *)(intptr_t)code);
    return b;
}
static void stepper(lv_obj_t *parent, const char *title, int value, int kind)
{
    lv_obj_t *c = ui_card(parent);
    lv_obj_set_flex_grow(c, 1);
    lv_obj_set_height(c, 88);
    lv_obj_set_style_pad_row(c, 8, 0);
    lv_obj_set_style_pad_all(c, 8, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    ui_label(c, title, UI_FONT_XS, UI_C_TEXT_2);

    lv_obj_t *row = ui_box(c);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, 40);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(row, 6, 0);
    small_button(row, "-", -kind);

    lv_obj_t *value_btn = lv_button_create(row);
    ui_style_button(value_btn, UI_C_CARD_BG2);
    lv_obj_set_flex_grow(value_btn, 1);
    lv_obj_set_height(value_btn, 40);
    lv_obj_t *value_lbl = ui_label(value_btn, "", UI_FONT_MD, UI_C_TEXT);
    lv_label_set_text_fmt(value_lbl, "%d min", value);
    lv_obj_center(value_lbl);
    if (kind == 1 || kind == 2) s_min_lbl[kind] = value_lbl;
    lv_obj_add_event_cb(value_btn, edit_cb, LV_EVENT_CLICKED, (void *)(intptr_t)kind);
    small_button(row, "+", kind);
}
void ui_sensorDiagScreen_screen_init(void)
{
    ui_sensorDiagScreen = ui_screen_base();
    s_min_lbl[1] = s_min_lbl[2] = NULL;
    lv_obj_add_event_cb(ui_sensorDiagScreen, screen_del_cb, LV_EVENT_DELETE, NULL);
    lv_obj_set_flex_flow(ui_sensorDiagScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ui_sensorDiagScreen, 8, 0);
    lv_obj_set_style_pad_row(ui_sensorDiagScreen, 7, 0);

    lv_obj_t *h = ui_nav_header(ui_sensorDiagScreen, _t("Audio de alarmas"));
    lv_obj_t *tb = lv_button_create(h);
    lv_obj_set_size(tb, 118, 30);
    lv_obj_set_style_bg_opa(tb, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(tb, 1, 0);
    lv_obj_set_style_border_color(tb, ui_col(UI_C_BORDER), 0);
    lv_obj_set_style_radius(tb, UI_RADIUS_SM, 0);
    lv_obj_set_style_shadow_width(tb, 0, 0);
    lv_obj_set_flex_flow(tb, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tb, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(tb, 5, 0);
    lv_obj_t *ti = lv_label_create(tb);
    lv_label_set_text(ti, LV_SYMBOL_VOLUME_MAX);
    lv_obj_set_style_text_color(ti, ui_col(UI_C_TEAL), 0);
    ui_label(tb, _t("Probar buzzer"), UI_FONT_XS, UI_C_TEXT);
    ui_press_feedback(tb);              /* 1.6.13 */
    lv_obj_set_ext_click_area(tb, 5);
    lv_obj_add_event_cb(tb, test_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *v = ui_card(ui_sensorDiagScreen);
    lv_obj_set_size(v, LV_PCT(100), 76);
    lv_obj_set_style_pad_all(v, 10, 0);
    lv_obj_set_flex_flow(v, LV_FLEX_FLOW_COLUMN);
    lv_obj_t *vh = ui_box(v);
    lv_obj_set_size(vh, LV_PCT(100), 20);
    lv_obj_set_flex_flow(vh, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(vh, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    ui_label(vh, _t("Volumen del audible"), UI_FONT_MD, UI_C_TEXT);
    s_vol = ui_label(vh, "", UI_FONT_MD, UI_C_OK);
    lv_label_set_text_fmt(s_vol, "%d%%", ui_cfg_alarm_volume());
    lv_obj_t *sl = lv_slider_create(v);
    lv_obj_set_size(sl, LV_PCT(100), 7);
    lv_slider_set_range(sl, 30, 100);
    lv_slider_set_value(sl, ui_cfg_alarm_volume(), LV_ANIM_OFF);
    lv_obj_set_ext_click_area(sl, 10);
    lv_obj_add_event_cb(sl, volume_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(sl, volume_cb, LV_EVENT_RELEASED, NULL);

    lv_obj_t *pat = ui_card(ui_sensorDiagScreen);
    lv_obj_set_size(pat, LV_PCT(100), 60);
    lv_obj_set_style_pad_all(pat, 9, 0);
    lv_obj_set_flex_flow(pat, LV_FLEX_FLOW_COLUMN);
    ui_label(pat, _t("Patrón por prioridad"), UI_FONT_XS, UI_C_TEXT_2);
    ui_label(pat, _t("Alta: ráfaga rápida   -   Advertencia: pulsos espaciados"), UI_FONT_XS, UI_C_TEXT);

    lv_obj_t *r = ui_box(ui_sensorDiagScreen);
    lv_obj_set_size(r, LV_PCT(100), 88);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(r, 7, 0);
    stepper(r, _t("Reanunciar tras silencio"), ui_cfg_reannounce_minutes(), 1);
    stepper(r, _t("Silencio máximo"), ui_cfg_max_silence_minutes(), 2);
}
void ui_sensorDiagScreen_screen_destroy(void)
{
    if (ui_sensorDiagScreen) { lv_obj_del(ui_sensorDiagScreen); ui_sensorDiagScreen = NULL; s_vol = NULL; }
}