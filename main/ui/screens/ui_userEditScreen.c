#include "ui_userEditScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_form.h"
#include "ui_theme.h"
#include "ui.h"
#include "ui_nav.h"
#include "ui_cfg.h"
#include <string.h>
#include <stdio.h>

/* Formulario de alta/edición de cuenta. Reusa la infraestructura de ui_form
 * (textarea + teclado en pantalla). El índice objetivo se fija con
 * ui_userEditScreen_set_index() antes de abrir. */

lv_obj_t *ui_userEditScreen = NULL;

static int s_idx = UI_USER_EDIT_NEW;
static lv_obj_t *s_name, *s_pin, *s_role, *s_error;

void ui_userEditScreen_set_index(int index) { s_idx = index; }

static void flash_error(const char *msg)
{
    if (!s_error) return;
    lv_label_set_text(s_error, msg);
    lv_obj_clear_flag(s_error, LV_OBJ_FLAG_HIDDEN);
}

/* Rol elegido en el dropdown -> valor del enum (TECH es la 1ª opción). */
static app_user_role_t selected_role(void)
{
    if (!s_role) return APP_ROLE_TECH;
    return (app_user_role_t)(APP_ROLE_TECH + lv_dropdown_get_selected(s_role));
}

static void save_cb(lv_event_t *e)
{
    (void)e;
    const char *pin = s_pin ? lv_textarea_get_text(s_pin) : "";

    if (s_idx == UI_USER_EDIT_FACTORY) {
        if (ui_auth_set_factory_pin(pin)) ui_nav_back();
        else flash_error(_t("Passphrase inválida (mínimo 8 caracteres)"));
        return;
    }

    const char *name = s_name ? lv_textarea_get_text(s_name) : "";
    app_user_role_t role = selected_role();
    bool ok = (s_idx == UI_USER_EDIT_NEW)
                  ? ui_auth_user_add(name, pin, role)
                  : ui_auth_user_update(s_idx, name, pin, role);
    if (ok) ui_nav_back();
    else flash_error(_t("Revise nombre y PIN (4-8 dígitos)"));
}

static void delete_cb(lv_event_t *e)
{
    (void)e;
    if (ui_auth_user_remove(s_idx)) ui_nav_back();
    else flash_error(_t("No se pudo eliminar la cuenta"));
}

void ui_userEditScreen_screen_init(void)
{
    s_name = s_pin = s_role = s_error = NULL;
    const app_user_t *u = (s_idx >= 0) ? ui_auth_user_at(s_idx) : NULL;

    const char *title = (s_idx == UI_USER_EDIT_FACTORY) ? _t("PIN de fabricante")
                        : (s_idx == UI_USER_EDIT_NEW)    ? _t("Nueva cuenta")
                                                         : _t("Editar cuenta");
    lv_obj_t *content;
    ui_userEditScreen = ui_form_begin(title, &content, save_cb);

    /* Mensaje de error (oculto hasta que falle una validación). */
    s_error = ui_label(content, "", UI_FONT_XS, UI_C_ALARM_SOFT);
    lv_obj_add_flag(s_error, LV_OBJ_FLAG_HIDDEN);

    if (s_idx == UI_USER_EDIT_FACTORY) {
        s_pin = ui_form_textarea(content, _t("Passphrase (mín. 8 caracteres)"), "", true);
        return;
    }

    s_name = ui_form_textarea(content, _t("Nombre"), u ? u->name : "", false);

    const char *pin_cap = (s_idx == UI_USER_EDIT_NEW)
                              ? _t("PIN (4-8 dígitos)")
                              : _t("PIN (vacío = no cambiar)");
    s_pin = ui_form_textarea(content, pin_cap, "", true);

    /* Roles asignables: desde TÉCNICO hasta el máximo que la sesión puede otorgar. */
    char opts[48] = "";
    static const char tecnico[] = { 'T', (char)0xC3, (char)0xA9, 'c', 'n', 'i', 'c', 'o', 0 };
    strncat(opts, tecnico, sizeof(opts) - 1);
    if (ui_auth_max_assignable() >= APP_ROLE_ADMIN)
        strncat(opts, "\nAdministrador", sizeof(opts) - strlen(opts) - 1);
    int sel = (u && u->role >= APP_ROLE_TECH) ? (int)(u->role - APP_ROLE_TECH) : 0;
    s_role = ui_form_dropdown(content, _t("Rol"), opts, sel);

    /* Botón eliminar (solo al editar una cuenta existente). */
    if (s_idx >= 0) {
        lv_obj_t *del = ui_card(content);
        lv_obj_set_width(del, LV_PCT(100));
        ui_style_button(del, UI_C_ALARM_BG);
        lv_obj_set_style_border_width(del, 1, 0);
        lv_obj_set_style_border_color(del, ui_col(UI_C_ALARM_BORDER), 0);
        lv_obj_set_style_pad_ver(del, 9, 0);
        lv_obj_add_flag(del, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(del, delete_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_center(ui_label(del, _t("Eliminar cuenta"), UI_FONT_SM, UI_C_ALARM_SOFT));
    }
}

void ui_userEditScreen_screen_destroy(void)
{
    if (ui_userEditScreen) { lv_obj_del(ui_userEditScreen); ui_userEditScreen = NULL;
                             s_name = s_pin = s_role = s_error = NULL; }
}
