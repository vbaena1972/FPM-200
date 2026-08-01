#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Handler que procesa comandos BLE recibidos en JSON, etc */
typedef void (*ble_cmd_handler_t)(const char *json, int len);

/* Inicialización base del stack BLE */
esp_err_t transport_ble_init(bool enable);
/* Registrar handler de comandos entrantes (OBSOLETO: el pipe RX/TX se reemplazó
 * por el servicio GATT compatible con la app Sensvax). */
void transport_ble_set_cmd_handler(ble_cmd_handler_t h);
/* Hook opcional: se llama cuando la app envía la op de control "finish". main lo
 * liga a config_mode_exit() + salir de la pantalla de configuración por app. */
void transport_ble_set_finish_cb(void (*cb)(void));
/* Encender / apagar completamente BLE */
esp_err_t transport_ble_set_enabled(bool on);
/* Nombre GAP / public name */
esp_err_t transport_ble_set_name(const char *name);
/* Modo passkey clásico */
esp_err_t transport_ble_set_passkey_mode(bool enable, uint32_t passkey);
/* Potencia TX */
esp_err_t transport_ble_set_tx_power(uint8_t level);
/* Advertising on/off */
esp_err_t transport_ble_start_adv(void);
esp_err_t transport_ble_stop_adv(void);
/* ¿Actualmente BLE inicializado/habilitado? */
bool transport_ble_is_enabled(void);
/* ¿Está haciendo advertising ahora mismo? */
bool transport_ble_is_advertising(void);

/* Mesh control (nuevo) */
esp_err_t transport_ble_set_mesh_enabled(bool on);
bool transport_ble_is_mesh_enabled(void);
/* ¿Hay alguna conexión BLE activa? */
bool transport_ble_is_connected(void);
#ifdef __cplusplus
}
#endif
