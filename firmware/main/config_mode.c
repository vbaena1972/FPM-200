#include "config_mode.h"
#include "wifi_mgr.h"
#include "transport_mqtt.h"
#include "transport_ble.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "config_mode";
static bool s_active = false;

bool config_mode_active(void) { return s_active; }

static void log_heap(const char *when)
{
    ESP_LOGW(TAG, "heap interno [%s]: free=%u  bloque_mayor=%u", when,
             (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
             (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
}

esp_err_t config_mode_enter(void)
{
    if (s_active) return ESP_OK;
    ESP_LOGW(TAG, "Entrando en modo configuración: apagando AWS y Wi-Fi para liberar RAM");
    log_heap("antes");

    transport_mqtt_stop();                 /* AWS/MQTT/TLS -> libera RAM */
    (void)wifi_mgr_stop();                 /* Wi-Fi -> libera RAM (Ethernet sigue) */
    vTaskDelay(pdMS_TO_TICKS(150));        /* deja asentar el heap antes de usar BLE */

    (void)transport_ble_set_enabled(true); /* BLE ya está init desde el boot */
    (void)transport_ble_start_adv();       /* visible para emparejar con la app */

    s_active = true;
    log_heap("después");
    return ESP_OK;
}

esp_err_t config_mode_exit(void)
{
    if (!s_active) return ESP_OK;
    s_active = false;
    ESP_LOGW(TAG, "Saliendo de modo configuración: restaurando Wi-Fi y AWS");

    (void)transport_ble_stop_adv();        /* deja de anunciar (BLE queda residente) */
    (void)wifi_mgr_start();                /* reconecta Wi-Fi; net_core hará SNTP al obtener IP */
    transport_mqtt_on_time_ready();        /* re-arma MQTT (idempotente; conecta al haber IP) */
    return ESP_OK;
}
