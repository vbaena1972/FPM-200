#include "ui_infoScreen.h"
#include "ui_i18n.h"
#include "ui_widgets.h"
#include "ui_theme.h"
#include "ui_nav.h"
#include "storage.h"
#include "cert_store.h"
#include "ui_cfg.h"
#include "nvs.h"
#include "metrics_store.h"
#include "esp_app_desc.h" // esp_app_get_description() -> version real (PROJECT_VER)
#include "esp_system.h"   // esp_restart (botón de reinicio)
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Información del equipo (mockup 4f). */

lv_obj_t *ui_infoScreen = NULL;

static int32_t s_kv_col[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
static int32_t s_kv_row[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
static int32_t s_bot_col[] = { LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
static int32_t s_bot_row[] = { LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST };

/* tarjeta clave-valor */
static lv_obj_t *kv_card(lv_obj_t *parent, const char *label, const char *value, uint32_t vcol)
{
    lv_obj_t *c = ui_card(parent);
    lv_obj_set_style_radius(c, 10, 0);
    lv_obj_set_style_pad_hor(c, 11, 0);
    lv_obj_set_style_pad_ver(c, 7, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(c, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_row(c, 2, 0);
    lv_obj_t *l = ui_label(c, label, UI_FONT_XS, UI_C_TEXT_MUTED);
    lv_obj_set_style_text_letter_space(l, 1, 0);
    ui_label(c, value, UI_FONT_MD, vcol);
    return c;
}

/* Botón de reinicio con confirmación de DOBLE TOQUE (sin timer que limpiar): el
 * 1er toque "arma" y cambia la etiqueta; un 2º toque dentro de ~4 s reinicia.
 * Evita reinicios accidentales en un equipo médico sin diálogos modales. */
static uint32_t s_reboot_arm_tick = 0;
static void reboot_cb(lv_event_t *e)
{
    lv_obj_t *lbl = (lv_obj_t *)lv_event_get_user_data(e);
    if (s_reboot_arm_tick != 0 && lv_tick_elaps(s_reboot_arm_tick) < 4000)
    {
        ESP_LOGW("info", "Reinicio del equipo solicitado por el usuario");
        if (lbl) lv_label_set_text(lbl, _t("Reiniciando…"));
        vTaskDelay(pdMS_TO_TICKS(150));
        esp_restart();
        return;
    }
    s_reboot_arm_tick = lv_tick_get();
    if (lbl) lv_label_set_text(lbl, _t("Toca otra vez para reiniciar"));
}

/* Restablecer de fabrica en dos niveles (1.5.32, paridad con MedGuard 1.7.13).
 * Ambos conservan la identidad de hardware (modelo/serie/version HW) y la
 * calibracion de sensores: en el FPM toda la calibracion es de FABRICA (ajuste
 * del FS7, cero de presion/flujo: EEPROM, se hace en laboratorio) y ningun
 * nivel la toca; sensors.cal del AppConfig solo guarda fechas de servicio.
 * 1.5.33 (decision 2026-10-11): nivel 1 = Administrador o Fabricante; nivel 2
 * = solo Fabricante.
 * Nivel 1 "Restablecer config.": ademas conserva la nube (broker/topico) y la
 * cuenta Fabricante; certificados, token LAN y vinculos BLE intactos.
 * Nivel 2 "Borrado total": borra ademas certificados AWS, token LAN y vinculos
 * BLE (hay que reaprovisionar los certificados por microSD). */
static void erase_nvs_namespace(const char *ns)
{
    nvs_handle_t h;
    esp_err_t err = nvs_open(ns, NVS_READWRITE, &h);
    if (err == ESP_ERR_NVS_NOT_FOUND) return;
    if (err == ESP_OK) {
        err = nvs_erase_all(h);
        if (err == ESP_OK) err = nvs_commit(h);
        nvs_close(h);
    }
    if (err == ESP_OK) ESP_LOGW("info", "Borrado total: NVS '%s' borrado", ns);
    else ESP_LOGE("info", "Borrado total: NVS '%s': %s", ns, esp_err_to_name(err));
}

static void factory_reset_apply(bool wipe)
{
    AppConfig *cur = malloc(sizeof *cur);
    AppConfig *def = malloc(sizeof *def);
    if (!cur || !def) {
        ESP_LOGE("info", "Restablecer: sin memoria");
        free(cur); free(def);
        return;
    }
    if (appcfg_cache_get(cur) != ESP_OK) appcfg_load(cur);
    appcfg_defaults(def);
    memcpy(def->general.model, cur->general.model, sizeof def->general.model);
    memcpy(def->general.serial, cur->general.serial, sizeof def->general.serial);
    memcpy(def->general.hw_version, cur->general.hw_version, sizeof def->general.hw_version);
    def->sensors.cal = cur->sensors.cal;
    if (!wipe) {
        def->general.factory = cur->general.factory;
        def->cloud = cur->cloud;
    }
    esp_err_t err = appcfg_save(def);
    free(cur); free(def);
    if (err != ESP_OK) {
        ESP_LOGE("info", "Restablecer: no se pudo guardar la config: %s", esp_err_to_name(err));
        return;
    }
    if (wipe) {
        esp_err_t c = cert_store_erase_all();
        if (c != ESP_OK) ESP_LOGE("info", "Borrado total: certificados: %s", esp_err_to_name(c));
        erase_nvs_namespace("httpmon");
        erase_nvs_namespace("nimble_bond");
    }
    ESP_LOGW("info", "%s hecho; reiniciando", wipe ? "Borrado total" : "Restablecer configuracion");
    vTaskDelay(pdMS_TO_TICKS(150));
    esp_restart();
}

/* Mismo doble toque que el reinicio (~4 s); cada boton arma su propio contador. */
static uint32_t s_reset_arm_tick = 0, s_wipe_arm_tick = 0;
static void factory_btn_cb(lv_event_t *e)
{
    lv_obj_t *lbl = (lv_obj_t *)lv_event_get_user_data(e);
    bool wipe = lv_obj_get_user_data(lv_event_get_current_target(e)) != NULL;
    uint32_t *arm = wipe ? &s_wipe_arm_tick : &s_reset_arm_tick;
    if (!ui_auth_can(wipe ? APP_ROLE_FACTORY : APP_ROLE_ADMIN)) return;
    if (*arm != 0 && lv_tick_elaps(*arm) < 4000) {
        if (lbl) lv_label_set_text(lbl, _t("Aplicando…"));
        factory_reset_apply(wipe);
        return;
    }
    *arm = lv_tick_get();
    if (lbl) lv_label_set_text(lbl, _t("Toca otra vez para confirmar"));
}

static void factory_btn(lv_obj_t *parent, const char *text, bool wipe)
{
    lv_obj_t *b = ui_card(parent);
    lv_obj_set_height(b, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_style_radius(b, 10, 0);
    lv_obj_set_style_pad_ver(b, 11, 0);
    lv_obj_set_flex_flow(b, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(b, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(b, 8, 0);
    lv_obj_add_flag(b, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(b, wipe ? (void *)1 : NULL);
    ui_icon(b, UI_SYM_ALERT_TRIANGLE, UI_ICON_SM, UI_C_ALARM);
    lv_obj_t *l = ui_label(b, text, UI_FONT_SM, UI_C_ALARM);
    lv_obj_add_event_cb(b, factory_btn_cb, LV_EVENT_CLICKED, l);
}

static const char *gas_label(const char *g)
{
    if (g && strcmp(g, "air_med") == 0) return _t("Aire med. (AIR)");
    if (g && strcmp(g, "n2o") == 0)     return _t("Óxido nitroso (N₂O)");
    if (g && strcmp(g, "vac") == 0)     return _t("Vacío (VAC)");
    return _t("Oxígeno (O₂)");
}

void ui_infoScreen_screen_init(void)
{
    const AppConfig *cfg = appcfg_cache_peek();
    ui_infoScreen = ui_screen_base();
    lv_obj_set_flex_flow(ui_infoScreen, LV_FLEX_FLOW_COLUMN);
    /* 1.5.33: con sesion de Administrador/Fabricante se agregan los botones de
     * restablecer y el contenido pasa los 320 px: scroll vertical para llegar. */
    lv_obj_add_flag(ui_infoScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(ui_infoScreen, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_infoScreen, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_pad_all(ui_infoScreen, 8, 0);
    lv_obj_set_style_pad_row(ui_infoScreen, 8, 0);

    /* header: back + título + badge normativa */
    lv_obj_t *hdr = ui_box(ui_infoScreen);
    lv_obj_set_size(hdr, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(hdr, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(hdr, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *hb = ui_box(hdr);
    lv_obj_set_size(hb, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(hb, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(hb, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(hb, 9, 0);
    lv_obj_t *back = ui_icon_badge(hb, UI_SYM_ARROW_LEFT, UI_ICON_SM, UI_C_TEXT, UI_C_CARD_BG, 28);
    lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(back, ui_nav_back_event_cb, LV_EVENT_CLICKED, NULL);
    ui_label(hb, _t("Información del equipo"), UI_FONT_TITLE, UI_C_TEXT);

    lv_obj_t *badge = ui_pill(hdr, "", UI_FONT_XS, UI_C_OK_SOFT, UI_C_OK_BG, UI_C_OK_BORDER);
    lv_obj_set_flex_flow(badge, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(badge, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(badge, 5, 0);
    lv_obj_t *bl = lv_obj_get_child(badge, 0);
    lv_label_set_text(bl, "NFPA 99 · ISO 7396-1");
    lv_obj_t *bi = ui_icon(badge, UI_SYM_SHIELD_CHECK, UI_ICON_SM, UI_C_OK_SOFT);
    lv_obj_move_to_index(bi, 0);

    /* fila: tarjeta de modelo + grid 2x2 */
    lv_obj_t *toprow = ui_box(ui_infoScreen);
    lv_obj_set_size(toprow, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(toprow, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(toprow, 10, 0);

    lv_obj_t *dev = ui_card(toprow);
    lv_obj_set_style_pad_hor(dev, 18, 0);
    lv_obj_set_style_pad_ver(dev, 12, 0);
    lv_obj_set_flex_flow(dev, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(dev, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *db = ui_icon_badge(dev, UI_SYM_ACTIVITY, UI_ICON_LG, 0x9fe1cb, UI_C_HEADER_ICONBG, 44);
    lv_obj_set_style_radius(db, 12, 0);
    lv_obj_t *dm = ui_label(dev, cfg && cfg->general.model[0] ? cfg->general.model : "MPF-4200",
                            UI_FONT_MD, UI_C_TEXT);
    lv_obj_set_style_margin_top(dm, 8, 0);
    ui_label(dev, cfg && cfg->general.info_text[0] ? cfg->general.info_text : "Axira Equipos",
             UI_FONT_XS, UI_C_TEXT_MUTED);

    lv_obj_t *kvg = ui_box(toprow);
    lv_obj_set_flex_grow(kvg, 1);
    lv_obj_set_height(kvg, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_column(kvg, 7, 0);
    lv_obj_set_style_pad_row(kvg, 7, 0);
    lv_obj_set_grid_dsc_array(kvg, s_kv_col, s_kv_row);
    lv_obj_set_layout(kvg, LV_LAYOUT_GRID);
    lv_obj_t *k;
    k = kv_card(kvg, _t("N.º DE SERIE"), cfg && cfg->general.serial[0] ? cfg->general.serial : "AX-4200-01847", UI_C_TEXT);
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    k = kv_card(kvg, _t("GAS CONFIGURADO"), gas_label(cfg ? cfg->sensors.gas_type : "o2"), UI_C_OK);
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    char verbuf[40]; /* esp_app_desc version[] llega a 32 chars + "v" + NUL */
    snprintf(verbuf, sizeof(verbuf), "v%s",
             cfg && cfg->general.hw_version[0] ? cfg->general.hw_version : "-");
    k = kv_card(kvg, _t("VERSIÓN HW"), verbuf, UI_C_TEXT);
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    /* Versión REAL del firmware flasheado (PROJECT_VER via esp_app_desc),
       no el campo de config general.fw_version (que podía quedar desfasado). */
    const esp_app_desc_t *appdesc = esp_app_get_description();
    snprintf(verbuf, sizeof(verbuf), "v%s", appdesc ? appdesc->version : "-");
    k = kv_card(kvg, _t("FW APLICACIÓN"), verbuf, UI_C_TEXT);
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);

    /* grid inferior 2x1 (compacto, alto de contenido): calibración + mantenim.
     * (Se quitó "Tiempo en servicio": no aportaba valor operativo.) */
    lv_obj_t *botg = ui_box(ui_infoScreen);
    lv_obj_set_width(botg, LV_PCT(100));
    lv_obj_set_height(botg, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_column(botg, 7, 0);
    lv_obj_set_grid_dsc_array(botg, s_bot_col, s_bot_row);
    lv_obj_set_layout(botg, LV_LAYOUT_GRID);
    k = kv_card(botg, _t("ÚLTIMA CALIBRACIÓN"),
                cfg && cfg->sensors.cal.last_cal_date[0] ? cfg->sensors.cal.last_cal_date : "\xE2\x80\x94",
                UI_C_TEXT);
    lv_obj_set_height(k, LV_SIZE_CONTENT);   /* antes alto por defecto de lv_obj (muy alta) */
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    k = kv_card(botg, _t("PRÓX. MANTENIM."),
                cfg && cfg->sensors.cal.next_service_date[0] ? cfg->sensors.cal.next_service_date : "\xE2\x80\x94",
                UI_C_WARN_SOFT);
    lv_obj_set_height(k, LV_SIZE_CONTENT);
    lv_obj_set_grid_cell(k, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    /* espaciador: absorbe el alto sobrante y empuja botón+pie abajo */
    lv_obj_t *spacer = ui_box(ui_infoScreen);
    lv_obj_set_width(spacer, LV_PCT(100));
    lv_obj_set_flex_grow(spacer, 1);

    /* botón de REINICIO del equipo (doble toque para confirmar) */
    lv_obj_t *rb = ui_card(ui_infoScreen);
    lv_obj_set_width(rb, LV_PCT(100));
    /* 1.5.34: alto de contenido; con el scroll de esta pantalla el alto por
     * defecto de lv_obj lo dejaba enorme. */
    lv_obj_set_height(rb, LV_SIZE_CONTENT);
    lv_obj_set_style_radius(rb, 10, 0);
    lv_obj_set_style_pad_ver(rb, 11, 0);
    lv_obj_set_flex_flow(rb, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(rb, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(rb, 8, 0);
    lv_obj_add_flag(rb, LV_OBJ_FLAG_CLICKABLE);
    ui_icon(rb, UI_SYM_REFRESH, UI_ICON_SM, UI_C_WARN_SOFT);
    lv_obj_t *rl = ui_label(rb, _t("Reiniciar equipo"), UI_FONT_SM, UI_C_WARN_SOFT);
    lv_obj_add_event_cb(rb, reboot_cb, LV_EVENT_CLICKED, rl);

    /* Restablecer de fabrica: dos niveles, doble toque. "Restablecer config."
     * desde Administrador; "Borrado total" solo Fabricante. */
    if (ui_auth_can(APP_ROLE_ADMIN)) {
        s_reset_arm_tick = s_wipe_arm_tick = 0;
        lv_obj_t *fr = ui_box(ui_infoScreen);
        lv_obj_set_size(fr, LV_PCT(100), LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(fr, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_pad_column(fr, 8, 0);
        factory_btn(fr, _t("Restablecer config."), false);
        if (ui_auth_can(APP_ROLE_FACTORY))
            factory_btn(fr, _t("Borrado total"), true);
    }

    /* pie con QR */
    lv_obj_t *foot = ui_box(ui_infoScreen);
    lv_obj_set_size(foot, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(foot, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(foot, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(foot, 8, 0);
    ui_icon(foot, UI_SYM_QRCODE, UI_ICON_SM, 0x4a5058);
    ui_label(foot, _t("Escanea para historial y manual · soporte@axira.com"), UI_FONT_XS, 0x4a5058);
}

void ui_infoScreen_screen_destroy(void)
{
    if (ui_infoScreen) { lv_obj_del(ui_infoScreen); ui_infoScreen = NULL; }
}
