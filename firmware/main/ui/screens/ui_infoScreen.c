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
#include "ota_update.h"     // OTA por red (1.5.37)
#include "esp_ota_ops.h"    // ESP_ERR_OTA_VALIDATE_FAILED
#include "transport_mqtt.h" // pausar/reanudar AWS durante la OTA
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
    /* 1.6.0: no reiniciar a mitad de una OTA (no rompe nada, pero se pierde la
     * descarga). MedGuard ya lo impide: una accion de Sistema a la vez. */
    if (ui_infoScreen_ota_busy()) {
        s_reboot_arm_tick = 0;
        if (lbl) lv_label_set_text(lbl, _t("Espere: actualizacion en curso"));
        return;
    }
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
    if (ui_infoScreen_ota_busy()) {
        *arm = 0;
        if (lbl) lv_label_set_text(lbl, _t("Espere: actualizacion en curso"));
        return;
    }
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
    ui_press_feedback(b);
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

/* ---- OTA por red (1.5.37) ----
 * La descarga corre en una tarea propia (stack INTERNO: hace escrituras de
 * flash) y solo escribe el estado en estas variables; la pantalla lo lee con un
 * lv_timer que muere con ella (sin punteros a widgets desde otra tarea). MQTT se
 * pausa mientras hay TLS de OTA (RAM interna) y se reanuda si no se reinicia. */
typedef enum { OTA_IDLE, OTA_BUSY, OTA_AVAILABLE, OTA_DONE, OTA_FAIL } ota_ui_state_t;
static volatile ota_ui_state_t s_ota_state = OTA_IDLE;
static volatile int s_ota_pct;
static char s_ota_msg[112] = "";
static int s_ota_channel;            /* 0 = estable, 1 = beta (no se guarda) */
static bool s_ota_install;           /* accion de la tarea: buscar o instalar */
static lv_obj_t *s_ota_lbl, *s_ota_bar, *s_ota_install_btn, *s_ota_ch_btn[2];
static lv_timer_t *s_ota_timer;

bool ui_infoScreen_ota_busy(void)
{
    return s_ota_state == OTA_BUSY || s_ota_state == OTA_DONE;
}

static void ota_progress_cb(int pct, const char *stage, void *user)
{
    (void)user;
    s_ota_pct = pct;
    snprintf(s_ota_msg, sizeof s_ota_msg, "%s... %d %%", stage ? stage : "", pct);
}

static void ota_task(void *arg)
{
    (void)arg;
    const AppConfig *cfg = appcfg_cache_peek();
    char base[sizeof cfg->cloud.ota_url];
    snprintf(base, sizeof base, "%s", cfg ? cfg->cloud.ota_url : "");
    char version[32] = "";
    esp_err_t err;
    transport_mqtt_stop();
    if (!s_ota_install) {
        bool newer = false;
        err = ota_update_check_url(base, s_ota_channel, version, sizeof version, &newer);
        if (err == ESP_OK && newer) {
            snprintf(s_ota_msg, sizeof s_ota_msg, "Disponible v%s (instalada v%s)", version,
                     esp_app_get_description()->version);
            s_ota_state = OTA_AVAILABLE;
        } else if (err == ESP_OK) {
            snprintf(s_ota_msg, sizeof s_ota_msg, "Sin actualizacion: servidor v%s, ya instalada",
                     version);
            s_ota_state = OTA_IDLE;
        }
    } else {
        err = ota_update_from_url(base, s_ota_channel, ota_progress_cb, NULL, version, sizeof version);
        if (err == ESP_OK) {
            snprintf(s_ota_msg, sizeof s_ota_msg, "v%s instalada. Reiniciando...", version);
            s_ota_state = OTA_DONE;
            vTaskDelay(pdMS_TO_TICKS(2500));
            esp_restart();
        }
    }
    if (err != ESP_OK) {
        const char *why = err == ESP_ERR_INVALID_ARG ? "configure la URL OTA (https://) en Nube"
                        : err == ESP_ERR_NOT_FOUND ? "no hay firmware publicado en ese canal"
                        : err == ESP_ERR_NOT_SUPPORTED ? "no hay una version mas nueva"
                        : err == ESP_ERR_INVALID_VERSION ? "el firmware no es de GasLineKeeper"
                        : err == ESP_ERR_OTA_VALIDATE_FAILED ? "firma invalida o archivo danado"
                        : esp_err_to_name(err);
        snprintf(s_ota_msg, sizeof s_ota_msg, "Error: %s. Sigue la version actual.", why);
        s_ota_state = OTA_FAIL;
    }
    transport_mqtt_on_time_ready();   /* reanuda AWS (si no se reinicio) */
    vTaskDelete(NULL);
}

static void ota_start(bool install)
{
    if (s_ota_state == OTA_BUSY || s_ota_state == OTA_DONE) return;
    s_ota_install = install;
    s_ota_pct = 0;
    snprintf(s_ota_msg, sizeof s_ota_msg, "%s", install ? "Preparando descarga..." : "Consultando servidor...");
    s_ota_state = OTA_BUSY;
    if (xTaskCreatePinnedToCore(ota_task, "ota_net", 8192, NULL, 4, NULL, 0) != pdPASS) {
        snprintf(s_ota_msg, sizeof s_ota_msg, "Error: sin memoria para la tarea OTA");
        s_ota_state = OTA_FAIL;
    }
}

/* 1.6.10: antes un toque sin sesion de Administrador (o "Instalar" sin haber buscado)
 * se ignoraba EN SILENCIO y parecia que el touch no respondia. Ahora se explica. */
static bool ota_allowed(void)
{
    if (ui_auth_can(APP_ROLE_ADMIN)) return true;
    if (s_ota_state != OTA_BUSY && s_ota_state != OTA_DONE)
        snprintf(s_ota_msg, sizeof s_ota_msg, "%s",
                 _t("Requiere sesion de Administrador (entrar por Configuracion)"));
    return false;
}
static void ota_check_cb(lv_event_t *e) { (void)e; if (ota_allowed()) ota_start(false); }
static void ota_install_cb(lv_event_t *e)
{
    (void)e;
    if (!ota_allowed()) return;
    if (s_ota_state == OTA_AVAILABLE) ota_start(true);
    else if (s_ota_state != OTA_BUSY && s_ota_state != OTA_DONE)
        snprintf(s_ota_msg, sizeof s_ota_msg, "%s", _t("Primero toque Buscar"));
}
static void ota_channel_cb(lv_event_t *e)
{
    if (s_ota_state == OTA_BUSY || s_ota_state == OTA_DONE) return;
    s_ota_channel = (int)(intptr_t)lv_event_get_user_data(e);
    s_ota_state = OTA_IDLE;
    s_ota_msg[0] = '\0';
}

static void ota_timer_cb(lv_timer_t *t)
{
    (void)t;
    if (!s_ota_lbl) return;
    const char *txt = s_ota_msg[0] ? s_ota_msg : _t("Buscar una version nueva en el servidor");
    if (strcmp(lv_label_get_text(s_ota_lbl), txt) != 0) lv_label_set_text(s_ota_lbl, txt);
    lv_obj_set_style_text_color(s_ota_lbl, ui_col(s_ota_state == OTA_FAIL ? UI_C_ALARM_SOFT :
                                s_ota_state == OTA_AVAILABLE || s_ota_state == OTA_DONE ? UI_C_OK : UI_C_TEXT_2), 0);
    lv_bar_set_value(s_ota_bar, s_ota_pct, LV_ANIM_OFF);
    if (s_ota_state == OTA_BUSY || s_ota_state == OTA_DONE) lv_obj_clear_flag(s_ota_bar, LV_OBJ_FLAG_HIDDEN);
    else lv_obj_add_flag(s_ota_bar, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_opa(s_ota_install_btn, s_ota_state == OTA_AVAILABLE ? LV_OPA_COVER : LV_OPA_40, 0);
    for (int i = 0; i < 2; i++)
        lv_obj_set_style_border_color(s_ota_ch_btn[i], ui_col(i == s_ota_channel ? UI_C_TEAL : UI_C_BORDER), 0);
}

static void ota_screen_del_cb(lv_event_t *e)
{
    (void)e;
    if (s_ota_timer) { lv_timer_delete(s_ota_timer); s_ota_timer = NULL; }
    s_ota_lbl = s_ota_bar = s_ota_install_btn = s_ota_ch_btn[0] = s_ota_ch_btn[1] = NULL;
}

static lv_obj_t *ota_button(lv_obj_t *parent, const char *text, lv_event_cb_t cb, void *user)
{
    lv_obj_t *b = ui_card(parent);
    lv_obj_set_height(b, LV_SIZE_CONTENT);
    lv_obj_set_flex_grow(b, 1);
    lv_obj_set_style_radius(b, 10, 0);
    lv_obj_set_style_pad_ver(b, 9, 0);
    lv_obj_add_flag(b, LV_OBJ_FLAG_CLICKABLE);
    ui_press_feedback(b);   /* 1.6.10: se ve que el toque se registro */
    lv_obj_add_event_cb(b, cb, LV_EVENT_CLICKED, user);
    lv_obj_center(ui_label(b, text, UI_FONT_SM, UI_C_TEXT));
    return b;
}

static void ota_card(lv_obj_t *parent)
{
    lv_obj_t *c = ui_card(parent);
    lv_obj_set_size(c, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_radius(c, 10, 0);
    lv_obj_set_flex_flow(c, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(c, 6, 0);
    ui_label(c, _t("ACTUALIZAR FIRMWARE (RED)"), UI_FONT_XS, UI_C_TEXT_MUTED);
    lv_obj_t *row = ui_box(c);
    lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_column(row, 6, 0);
    s_ota_ch_btn[0] = ota_button(row, _t("Estable"), ota_channel_cb, (void *)0);
    s_ota_ch_btn[1] = ota_button(row, _t("Beta"), ota_channel_cb, (void *)1);
    ota_button(row, _t("Buscar"), ota_check_cb, NULL);
    s_ota_install_btn = ota_button(row, _t("Instalar"), ota_install_cb, NULL);
    s_ota_lbl = ui_label(c, "", UI_FONT_XS, UI_C_TEXT_2);
    lv_label_set_long_mode(s_ota_lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(s_ota_lbl, LV_PCT(100));
    s_ota_bar = lv_bar_create(c);
    lv_obj_set_size(s_ota_bar, LV_PCT(100), 8);
    lv_bar_set_range(s_ota_bar, 0, 100);
    s_ota_timer = lv_timer_create(ota_timer_cb, 300, NULL);
    lv_obj_add_event_cb(ui_infoScreen, ota_screen_del_cb, LV_EVENT_DELETE, NULL);
    ota_timer_cb(NULL);
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
    ui_press_feedback(back);
    lv_obj_set_ext_click_area(back, 8);   /* 28 px es chico para el dedo */
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
    lv_obj_t *dm = ui_label(dev, FPM_PRODUCT_NAME,   /* nombre comercial (general.model es tecnico) */
                            UI_FONT_MD, UI_C_TEXT);
    lv_obj_set_style_margin_top(dm, 8, 0);
    ui_label(dev, cfg && cfg->general.info_text[0] ? cfg->general.info_text : "Axira",
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
    ui_press_feedback(rb);
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
        ota_card(ui_infoScreen);
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
