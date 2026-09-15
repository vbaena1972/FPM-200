#pragma once
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Modo configuración por BLE / app Flutter.
 *
 * Espeja el patrón del equipo hermano (ClaudeHMI/ble_service.c): al configurar
 * por la app, se APAGAN AWS (MQTT/TLS) y Wi-Fi para liberar RAM interna y darle
 * espacio al stack BLE + al procesamiento de la app. Ethernet se deja como está.
 *
 * enter(): detiene MQTT y Wi-Fi, deja asentar el heap y levanta el advertising BLE.
 * exit():  detiene el advertising, reanuda Wi-Fi y re-arma MQTT (reconecta al
 *          recuperar IP). Idempotentes. */
esp_err_t config_mode_enter(void);
esp_err_t config_mode_exit(void);
bool      config_mode_active(void);

#ifdef __cplusplus
}
#endif
