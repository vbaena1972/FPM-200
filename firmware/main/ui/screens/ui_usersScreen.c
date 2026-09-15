#include "ui_usersScreen.h"
#include "ui_userEditScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui.h"
#include "ui_nav.h"
#include "ui_cfg.h"
#include <stdint.h>

/* Gestión de usuarios: lista + botón agregar + (fabricante) cambiar passphrase.
 * Cada fila abre el formulario de edición solo si la sesión puede tocar ESA cuenta. */

lv_obj_t *ui_usersScreen = NULL;

static const char s_tecnico[] = { 'T', (char)0xC3, (char)0xA9, 'c', 'n', 'i', 'c', 'o', 0 };
static const char *role_name(app_user_role_t r)
{
    return r == APP_ROLE_FACTORY ? "Fabricante" : r == APP_ROLE_ADMIN ? "Administrador" : s_tecnico;
}
static uint32_t role_color(app_user_role_t r)
{
    return r == APP_ROLE_FACTORY ? 0x8b5cf6 : r == APP_ROLE_ADMIN ? UI_C_BLUE : UI_C_OK;
}

static void row_cb(lv_event_t *e)
{
    ui_userEditScreen_set_index((int)(intptr_t)lv_event_get_user_data(e));
    ui_open_useredit_cb(NULL);
}
static void add_cb(lv_event_t *e)
{
    (void)e;
    ui_userEditScreen_set_index(UI_USER_EDIT_NEW);
    ui_open_useredit_cb(NULL);
}
static void factory_pin_cb(lv_event_t *e)
{
    (void)e;
    ui_userEditScreen_set_index(UI_USER_EDIT_FACTORY);
    ui_open_useredit_cb(NULL);
}

static void add_user_row(lv_obj_t *p, int i)
{
    const app_user_t *u = ui_auth_user_at(i);
    if (!u) return;
    bool editable = ui_auth_can_edit_user(i);

    lv_obj_t *r = ui_card(p);
    lv_obj_set_size(r, LV_PCT(100), 46);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(r, 9, 0);
    lv_obj_set_style_pad_column(r, 9, 0);
    if (editable) {
        lv_obj_add_flag(r, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(r, row_cb, LV_EVENT_CLICKED, (void *)(intptr_t)i);
    }

    ui_icon_badge(r, u->role == APP_ROLE_ADMIN ? UI_SYM_SHIELD_CHECK : UI_SYM_SETTINGS,
                  UI_ICON_SM, role_color(u->role), UI_C_CARD_BG2, 30);

    lv_obj_t *col = ui_box(r);
    lv_obj_set_flex_grow(col, 1);
    lv_obj_set_width(col, 1);
    lv_obj_set_height(col, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
    ui_label(col, u->name, UI_FONT_MD, UI_C_TEXT);
    ui_label(col, role_name(u->role), UI_FONT_XS, UI_C_TEXT_MUTED);

    if (editable)
        ui_icon(r, UI_SYM_CHEVRON_RIGHT, UI_ICON_SM, UI_C_TEXT_MUTED);
    else
        ui_icon(r, UI_SYM_LOCK, UI_ICON_SM, UI_C_TEXT_MUTED); /* fuera de tu alcance */
}

void ui_usersScreen_screen_init(void)
{
    ui_usersScreen = ui_screen_base();
    lv_obj_set_flex_flow(ui_usersScreen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(ui_usersScreen, 8, 0);
    lv_obj_set_style_pad_row(ui_usersScreen, 8, 0);

    lv_obj_t *hdr = ui_nav_header(ui_usersScreen, _t("Usuarios"));

    /* Botón "+ Agregar" (si queda cupo y la sesión puede asignar algún rol). */
    if (ui_auth_can_manage() && ui_auth_user_count() < APP_MAX_USERS &&
        ui_auth_max_assignable() >= APP_ROLE_TECH) {
        lv_obj_t *add = ui_box(hdr);
        lv_obj_set_size(add, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        ui_style_button(add, UI_C_OK);
        lv_obj_set_style_pad_hor(add, 12, 0);
        lv_obj_set_style_pad_ver(add, 6, 0);
        lv_obj_add_flag(add, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(add, add_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_center(ui_label(add, _t("Agregar"), UI_FONT_SM, 0x06251f));
    }

    lv_obj_t *content = ui_box(ui_usersScreen);
    lv_obj_set_width(content, LV_PCT(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(content, 6, 0);
    lv_obj_set_scroll_dir(content, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_add_flag(content, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < ui_auth_user_count(); i++)
        add_user_row(content, i);

    /* Cambiar la passphrase del fabricante: solo con sesión de fabricante. */
    if (ui_auth_can(APP_ROLE_FACTORY)) {
        lv_obj_t *r = ui_card(content);
        lv_obj_set_size(r, LV_PCT(100), 46);
        lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(r, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_hor(r, 9, 0);
        lv_obj_set_style_pad_column(r, 9, 0);
        lv_obj_add_flag(r, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(r, factory_pin_cb, LV_EVENT_CLICKED, NULL);
        ui_icon_badge(r, UI_SYM_SHIELD_LOCK, UI_ICON_SM, 0x8b5cf6, UI_C_CARD_BG2, 30);
        lv_obj_t *col = ui_box(r);
        lv_obj_set_flex_grow(col, 1);
        lv_obj_set_height(col, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
        ui_label(col, _t("Cambiar PIN de fabricante"), UI_FONT_MD, UI_C_TEXT);
        ui_label(col, _t("passphrase alfanumérica"), UI_FONT_XS, UI_C_TEXT_MUTED);
        ui_icon(r, UI_SYM_CHEVRON_RIGHT, UI_ICON_SM, UI_C_TEXT_MUTED);
    }
}

void ui_usersScreen_screen_destroy(void)
{
    if (ui_usersScreen) { lv_obj_del(ui_usersScreen); ui_usersScreen = NULL; }
}
