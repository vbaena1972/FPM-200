#include "ui_netBleScreen.h"
#include "ui_i18n.h"
#include "ui_form.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_nav.h"
#include "ui.h"
#include "ui_cfg.h"
#include "storage.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "transport_ble.h"
#include "esp_system.h"   // esp_restart
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

/* Configuración Bluetooth LE (formulario) + acceso a "Configurar por app" (QR). */

lv_obj_t *ui_netBleScreen = NULL;
static lv_obj_t *s_sw, *s_adv, *s_name, *s_pin, *s_sec, *s_txp;

/* Nota: BLE mesh fue RETIRADO de la UI por decisión de producto (equipo
 * hospitalario: la topología gateway/nodo rotativo no se consideró segura).
 * Los campos bt.mesh.* siguen en AppConfig por compatibilidad de schema. */

static void set_str(char *dst, size_t cap, const char *src)
{ snprintf(dst, cap, "%s", src ? src : ""); }

static void save_cb(lv_event_t *e)
{
    (void)e;
#ifdef ESP_PLATFORM
    ESP_LOGW("netble", "Guardar BLE (click en boton Guardar)");
#endif
    if (!ui_auth_can(APP_ROLE_ADMIN)) {
#ifdef ESP_PLATFORM
        ESP_LOGW("netble", "Sin rol ADMIN: no se guarda ni reinicia (inicia sesion como Administrador)");
#endif
        ui_nav_back(); return;
    }  /* BLE operativo: ADMIN */
    AppConfig *cfg = appcfg_cache_peek();
    if (!cfg) return;

    bool was_enabled = cfg->bt.enabled;   /* estado previo, para detectar apagado */
    cfg->bt.enabled   = lv_obj_has_state(s_sw, LV_STATE_CHECKED);
    cfg->bt.advertise = lv_obj_has_state(s_adv, LV_STATE_CHECKED);
    set_str(cfg->bt.legacy.name, sizeof(cfg->bt.legacy.name), lv_textarea_get_text(s_name));
    set_str(cfg->bt.legacy.pin,  sizeof(cfg->bt.legacy.pin),  lv_textarea_get_text(s_pin));
    cfg->bt.legacy.sec_mode = (app_bt_sec_mode_t)lv_dropdown_get_selected(s_sec);
    cfg->bt.tx_power        = (app_bt_tx_power_t)lv_dropdown_get_selected(s_txp);

    (void)appcfg_save(cfg);

#ifdef ESP_PLATFORM
    transport_ble_set_enabled(cfg->bt.enabled);
    if (cfg->bt.legacy.name[0]) transport_ble_set_name(cfg->bt.legacy.name);
    bool use_pk = (cfg->bt.legacy.sec_mode == APP_BT_SEC_PASSKEY) && cfg->bt.legacy.pin[0];
    transport_ble_set_passkey_mode(use_pk, use_pk ? (uint32_t)strtoul(cfg->bt.legacy.pin, NULL, 10) : 0);
    transport_ble_set_tx_power((uint8_t)cfg->bt.tx_power);
    if (cfg->bt.enabled && cfg->bt.advertise) transport_ble_start_adv();
    else                                      transport_ble_stop_adv();

    /* Al APAGAR el Bluetooth: REINICIAR el equipo. transport_ble_set_enabled(false)
     * solo detiene el advertising; el controlador NimBLE NO libera su RAM interna y
     * la red/AWS no se restauran limpio (el BT queda "activo" y AWS no reconecta
     * hasta reiniciar). Como bt.enabled=false ya quedó en NVS (appcfg_save arriba),
     * al arrancar el BT no sube y la red/AWS quedan limpias. Mismo criterio que
     * MedGuard. Solo en la transición on->off (no en cada guardado). */
    ESP_LOGW("netble", "BT guardado: antes=%d ahora=%d (reinicia si antes=1 y ahora=0)",
             (int)was_enabled, (int)cfg->bt.enabled);
    if (was_enabled && !cfg->bt.enabled) {
        ESP_LOGW("netble", "Bluetooth apagado -> reiniciando para liberar RAM y "
                           "restaurar red/AWS");
        vTaskDelay(pdMS_TO_TICKS(150));   /* deja asentar la escritura NVS */
        esp_restart();
    }
#endif
    ui_nav_back();
}

void ui_netBleScreen_screen_init(void)
{
    const AppConfig *cfg = appcfg_cache_peek();
    lv_obj_t *content;
    ui_netBleScreen = ui_form_begin("Bluetooth LE", &content, save_cb);

    if (!ui_auth_can(APP_ROLE_ADMIN))
        ui_notice(content, _t("Requiere administrador — cambios deshabilitados"));

    s_sw  = ui_form_switch(content, _t("Bluetooth habilitado"), _t("app de servicio por BLE"),
                           cfg && cfg->bt.enabled);
    s_adv = ui_form_switch(content, _t("Anunciando (visible)"), _t("detectable para emparejar"),
                           cfg && cfg->bt.advertise);
    s_name = ui_form_textarea(content, _t("NOMBRE DEL EQUIPO"), cfg ? cfg->bt.legacy.name : "", false);
    s_pin  = ui_form_textarea(content, _t("PIN (6 dígitos)"), cfg ? cfg->bt.legacy.pin : "", false);
    s_sec  = ui_form_dropdown(content, _t("SEGURIDAD"), "Just Works\nPasskey (PIN)",
                              cfg ? (int)cfg->bt.legacy.sec_mode : 0);
    s_txp  = ui_form_dropdown(content, _t("POTENCIA TX"),
                              _t("Muy bajo\nBajo\nBajo-medio\nMedio\nMedio-alto\nAlto\nMuy alto\nMáximo"),
                              cfg ? (int)cfg->bt.tx_power : 3);

    /* acceso a la pantalla de emparejamiento por QR */
    lv_obj_t *btn = ui_card(content);
    lv_obj_set_width(btn, LV_PCT(100));
    lv_obj_set_style_radius(btn, 10, 0);
    lv_obj_set_style_pad_ver(btn, 10, 0);
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(btn, 8, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(btn, ui_open_bleapp_cb, LV_EVENT_CLICKED, NULL);
    ui_icon(btn, UI_SYM_RADAR_2, UI_ICON_SM, UI_C_BLUE);
    ui_label(btn, _t("Configurar por app (QR de emparejamiento)"), UI_FONT_SM, UI_C_BLUE);
}

void ui_netBleScreen_screen_destroy(void)
{
    if (ui_netBleScreen) { lv_obj_del(ui_netBleScreen); ui_netBleScreen = NULL; }
}
