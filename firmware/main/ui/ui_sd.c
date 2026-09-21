#include "ui_sd.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"

/* Overlay modal de mantenimiento por microSD. Diseñado para caber en 480x320:
 * tarjeta COMPACTA, botones "Aplicar y reiniciar" + "Cerrar" en UNA fila abajo
 * (antes la tarjeta crecía más de 320 px y los botones quedaban FUERA de la
 * pantalla -> parecía que no había forma de cerrar ni reiniciar). */

static lv_obj_t *s_overlay = NULL;
static lv_obj_t *s_badge   = NULL;
static lv_obj_t *s_bar     = NULL;
static lv_obj_t *s_status  = NULL;
static lv_obj_t *s_btn     = NULL;   /* "Aplicar y reiniciar" (oculto hasta terminar) */
static lv_obj_t *s_close   = NULL;   /* "Cerrar" (siempre visible) */

static lv_event_cb_t s_close_cb = NULL;
void ui_sd_set_close_cb(lv_event_cb_t cb) { s_close_cb = cb; }

static void close_click_cb(lv_event_t *e)
{
    if (s_close_cb) s_close_cb(e);
    else            ui_sd_close();
}

void ui_sd_show(void)
{
    if (s_overlay) return;

    s_overlay = lv_obj_create(lv_layer_top());
    ui_kill_scroll(s_overlay);
    lv_obj_set_size(s_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(s_overlay, ui_col(0x000000), 0);
    lv_obj_set_style_bg_opa(s_overlay, LV_OPA_60, 0);
    lv_obj_set_style_border_width(s_overlay, 0, 0);
    lv_obj_set_style_radius(s_overlay, 0, 0);
    lv_obj_add_flag(s_overlay, LV_OBJ_FLAG_CLICKABLE);

    /* tarjeta central compacta */
    lv_obj_t *card = ui_card(s_overlay);
    lv_obj_set_width(card, 440);
    lv_obj_set_height(card, LV_SIZE_CONTENT);   /* crece para mostrar TODO (antes
                                                   la altura default ~100px cortaba
                                                   estado y botones) */
    lv_obj_center(card);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(card, 14, 0);
    lv_obj_set_style_pad_row(card, 7, 0);

    s_badge = ui_icon_badge(card, UI_SYM_FILE_EXPORT, UI_ICON_MD, UI_C_TEAL, UI_C_CARD_BG2, 36);
    lv_obj_set_style_radius(s_badge, 10, 0);

    ui_label(card, _t("Mantenimiento · microSD"), UI_FONT_MD, UI_C_TEXT);

    s_bar = lv_bar_create(card);
    lv_obj_set_size(s_bar, LV_PCT(100), 8);
    lv_bar_set_range(s_bar, 0, 100);
    lv_bar_set_value(s_bar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(s_bar, ui_col(0x22262d), LV_PART_MAIN);
    lv_obj_set_style_bg_color(s_bar, ui_col(UI_C_TEAL), LV_PART_INDICATOR);
    lv_obj_set_style_radius(s_bar, 4, LV_PART_MAIN);
    lv_obj_set_style_radius(s_bar, 4, LV_PART_INDICATOR);

    s_status = ui_label(card, _t("Tarjeta detectada…"), UI_FONT_SM, UI_C_TEXT_2);
    lv_obj_set_width(s_status, LV_PCT(100));
    lv_label_set_long_mode(s_status, LV_LABEL_LONG_WRAP);

    /* fila de botones al fondo (una sola línea) */
    lv_obj_t *row = ui_box(card);
    lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(row, 8, 0);

    /* "Aplicar y reiniciar" (oculto hasta que haya cambios) */
    s_btn = ui_box(row);
    lv_obj_set_height(s_btn, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(s_btn, 1);
    lv_obj_set_style_bg_color(s_btn, ui_col(UI_C_TEAL), 0);
    lv_obj_set_style_bg_opa(s_btn, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(s_btn, UI_RADIUS_SM, 0);
    lv_obj_set_style_pad_ver(s_btn, 11, 0);
    lv_obj_set_flex_flow(s_btn, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(s_btn, 6, 0);
    lv_obj_add_flag(s_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(s_btn, LV_OBJ_FLAG_HIDDEN);
    ui_icon(s_btn, UI_SYM_REFRESH, UI_ICON_SM, 0x06251f);
    ui_label(s_btn, _t("Aplicar y reiniciar"), UI_FONT_MD, 0x06251f);

    /* "Cerrar" — SIEMPRE visible */
    s_close = ui_box(row);
    lv_obj_set_height(s_close, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(s_close, 1);
    lv_obj_set_style_bg_opa(s_close, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(s_close, 1, 0);
    lv_obj_set_style_border_color(s_close, ui_col(0x3a3f47), 0);
    lv_obj_set_style_radius(s_close, UI_RADIUS_SM, 0);
    lv_obj_set_style_pad_ver(s_close, 10, 0);
    lv_obj_set_flex_flow(s_close, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(s_close, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(s_close, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(s_close, close_click_cb, LV_EVENT_CLICKED, NULL);
    ui_label(s_close, _t("Cerrar"), UI_FONT_MD, UI_C_TEXT);
}

void ui_sd_progress(int pct, const char *status)
{
    if (!s_overlay) return;
    if (s_bar) {
        lv_obj_remove_flag(s_bar, LV_OBJ_FLAG_HIDDEN);
        lv_bar_set_value(s_bar, pct, LV_ANIM_ON);
    }
    if (s_status && status) {
        lv_obj_set_style_text_color(s_status, ui_col(UI_C_TEXT_2), 0);
        lv_label_set_text(s_status, _t(status));
    }
}

void ui_sd_error(const char *msg)
{
    if (!s_overlay) ui_sd_show();
    if (!s_overlay) return;
    if (s_bar) lv_obj_add_flag(s_bar, LV_OBJ_FLAG_HIDDEN);
    if (s_btn) lv_obj_add_flag(s_btn, LV_OBJ_FLAG_HIDDEN);
    if (s_badge) lv_obj_set_style_bg_color(s_badge, ui_col(UI_C_ALARM_BG), 0);
    if (s_status) {
        lv_obj_set_style_text_color(s_status, ui_col(UI_C_ALARM_SOFT), 0);
        lv_label_set_text(s_status, msg ? _t(msg) : _t("No se pudo montar la microSD"));
    }
}

void ui_sd_finish_restart(lv_event_cb_t restart_cb)
{
    if (!s_overlay || !s_btn) return;
    if (s_bar) lv_bar_set_value(s_bar, 100, LV_ANIM_OFF);
    if (s_status) {
        lv_obj_set_style_text_color(s_status, ui_col(UI_C_OK_SOFT), 0);
        lv_label_set_text(s_status, _t("Certificados y config aplicados. Reinicia para conectar a AWS."));
    }
    lv_obj_remove_flag(s_btn, LV_OBJ_FLAG_HIDDEN);
    if (restart_cb) lv_obj_add_event_cb(s_btn, restart_cb, LV_EVENT_CLICKED, NULL);
}

void ui_sd_close(void)
{
    if (!s_overlay) return;
    lv_obj_del(s_overlay);
    s_overlay = NULL; s_badge = NULL; s_bar = NULL;
    s_status = NULL; s_btn = NULL; s_close = NULL;
}
