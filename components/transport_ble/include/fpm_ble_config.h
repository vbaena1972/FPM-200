#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Puente entre el AppConfig del FPM y el ESQUEMA JSON de la app Sensvax
 * (`mobile/medguard_config/lib/src/models/device_config.dart`).
 *
 * La app es multiproducto (MedGuard 12-IoT / Axira). Ambos comparten TODO el
 * esquema de config EXCEPTO la sección `channels`:
 *   - MedGuard: 12 canales universales (con calibración).
 *   - Axira (este equipo, FPM): 2 sensores FIJOS — Presión (con gas + unidad) y
 *     Flujo (unidad). SIN calibración (fija/preprogramada en el sensor).
 * La app detecta el modelo por `info.model` (contiene "axira") y adapta la UI.
 *
 * Estas funciones producen/consumen exactamente las claves del esquema de la app,
 * para que su `DeviceConfig.fromJson/toJson` funcione sin traducción. */

/* Característica Info (1001): identidad para detección de modelo. malloc → free(). */
char *fpm_ble_info_json(void);

/* Config completa en el esquema de la app (op `config_read`). malloc → free().
 * redact=true oculta secretos (PIN, password Wi-Fi, certificados/claves). */
char *fpm_ble_config_read_json(bool redact);

/* Aplica (merge parcial) un JSON del esquema de la app (op `set_config`):
 * solo las secciones presentes; mapea a AppConfig, persiste y refresca la caché.
 * Devuelve false si el JSON no parsea. */
bool fpm_ble_config_apply_json(const char *json);

/* --- Características dedicadas (payloads planos que usa la app) --- */
/* WiFi (1002): {enabled, ethernet_enabled, dhcp, ssid, hostname, ip, mask, gateway, dns}
 * (la password nunca se devuelve; solo se escribe si viene NO vacía). */
char *fpm_ble_wifi_read_json(void);
bool  fpm_ble_wifi_apply_json(const char *json);
/* Cloud (1003): {enabled, endpoint, port, thing_name, client_id, topic_base, qos, keepalive_s}
 * (certs/clave nunca viajan). La escritura es parcial (merge de las claves presentes). */
char *fpm_ble_cloud_read_json(void);
bool  fpm_ble_cloud_apply_json(const char *json);
/* Canal (1006): un ChannelConfig del canal seleccionado (sel 0=Presión, 1=Flujo). */
char *fpm_ble_channel_read_json(int sel);
bool  fpm_ble_channel_apply_json(const char *json);
/* Estado (1004): lectura "viva" del canal seleccionado (Axira no calibra; mínimo). */
char *fpm_ble_status_read_json(int sel);

/* Gestión de usuarios por BLE (ops de control `user_upsert`/`user_remove`).
 * upsert: {op,index,name,role,locked,pin?} — alta si index==count (PIN obligatorio),
 * si no edita (preserva el PIN si viene vacío). remove: {op,index} — rechaza dejar
 * el equipo sin ningún administrador. El JSON incluye la clave "op". */
bool fpm_ble_user_op_json(const char *json);

#ifdef __cplusplus
}
#endif
