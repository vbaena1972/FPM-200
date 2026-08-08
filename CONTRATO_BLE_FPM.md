# Contrato BLE FPM (Axira) ↔ app Sensvax

Fuente de verdad del **puente de configuración por BLE** entre el firmware FPM-200/Axira
(este repo) y la app Flutter Sensvax (`ClaudeHMI/mobile/medguard_config`).

- **Firmware (puente):** `components/transport_ble/fpm_ble_config.c` + `transport_ble.c`.
- **Estructura de config firmware:** `AppConfig` en `components/storage/include/storage.h`.
- **App (contrato Dart):** `lib/src/models/device_config.dart`, `lib/src/contract/config_contract.dart`,
  `lib/medguard_ble_client.dart`.

> La app es multiproducto (MedGuard 12-IoT / Axira). Comparte TODO el esquema EXCEPTO
> la sección de canales: el FPM expone **2 sensores fijos** (Presión id1, Flujo id2),
> sin calibración de campo. La app detecta el modelo por el nombre BLE ("axira").

---

## 1. GATT

- **Servicio:** `9f3c1000-6d4f-4d5b-8c9a-2f3e7a110001`
- **Características** (mismas en firmware y app):

| UUID (sufijo) | Nombre   | Flags        | Payload |
|---|---|---|---|
| `1001` | Info     | READ         | Identidad para detectar modelo |
| `1002` | WiFi     | READ/WRITE   | Red WiFi + eth flags (plano) |
| `1003` | Cloud    | READ/WRITE   | AWS IoT (plano, parcial) |
| `1004` | Estado   | READ         | Lectura viva del canal (Axira: mínima) |
| `1005` | Control  | WRITE        | Ops: `config_read`, `set_config`, `select_channel`, `finish`, `user_upsert`, `user_remove`, `set_clock` |
| `1006` | Canal    | READ/WRITE   | Un canal en esquema **PLANO** |
| `1007` | Config   | READ         | Documento completo, troceado por `MTU-3`, EOF=0 bytes |

- **Escritura:** JSON UTF-8, < 512 bytes (app) / < 640 bytes (firmware `BLE_JSON_MAX`).
- **Lectura de config completa:** `{"op":"config_read"}` en Control → luego lecturas repetidas
  de `1007` hasta 0 bytes.
- **Seguridad v1:** características SIN cifrado (Just Works). Pendiente: `*_ENC` + passkey.

---

## 2. Dos esquemas de canal (importante)

| Camino | Característica | Esquema | Firmware | App |
|---|---|---|---|---|
| Documento completo | `1007` / `set_config` | **ANIDADO** (`scale{}`, `alarm{}`, `cal{}`) | `build_channel_obj` / `apply_channel_obj` | `ChannelConfig` |
| Canal individual | `1006` | **PLANO** (`range_min`, `lo_limit`… en raíz; solo `cal` anidado) | `build_channel_flat` / `apply_channel_obj` | `BleChannelConfig` |

`apply_channel_obj` acepta **ambos**: toma los campos de alarma del objeto `alarm` si existe,
o del nivel raíz si no. Así el mismo aplicador sirve para el documento completo y para la
característica de canal dedicada.

---

## 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)

### display ↔ `general` / `DisplayConfig`
| JSON | AppConfig | Flutter | Notas |
|---|---|---|---|
| `brightness` | `general.brightness` | `brightness` | 10..100 |
| `dim_minutes` | `general.dim_minutes` | `dimMinutes` | 0,1,5,10 |
| `theme` | `general.theme` (`"dark"/"light"`) | `theme` (0/1) | 0=dark, 1=light |
| `language` | `general.lang` (`"es"/"en"`) | `language` | |
| `session_timeout_minutes` | — | `sessionTimeoutMinutes` | firmware fija 5 |

### time ↔ `TimeConfig`
`timezone` ↔ `general.timezone`. El resto (`manual/dst/hour_format/date_format/ntp_server`)
la app lo maneja; el firmware solo persiste `timezone`. La hora se ajusta con la op `set_clock`.

### alarms ↔ `general.alarm` / `AlarmsConfig`
`volume`, `reannounce_minutes`, `max_silence_minutes` ↔ `general.alarm.*`.
Relés (`relay*`) los ignora el FPM (no tiene).

### network ↔ `wifi`/`eth` / `NetworkConfig`
| JSON | AppConfig |
|---|---|
| `network.wifi.{enabled,dhcp,ip,mask,gateway,dns}` | `wifi.{enabled,ip_mode,ip,mask,gw,dns1}` |
| `network.ethernet.{...}` | `eth.{...}` |
| `wifi_ssid` | `wifi.ssid` |
| `wifi_password` (secreto) | `wifi.password` |
| `hostname` | `eth.hostname` |

`dhcp:bool` ↔ `ip_mode:"dhcp"/"static"`.

### cloud ↔ `cloud` + `general.serial` / `CloudConfig`
`enabled`, `endpoint`↔`broker_url`, `topic_base`, `qos`, `keepalive_s`↔`keepalive`.
`thing_name`/`client_id` se derivan de `general.serial` (el FPM ignora los que mande la app).
`port` fijo 8883. Certs (`root_ca`/`device_cert`/`private_key`) nunca viajan por BLE.

### bluetooth ↔ `bt` / `BluetoothConfig`
`enabled`↔`bt.enabled`, `name`↔`bt.legacy.name`, `tx_power`↔`bt.tx_power` (enum 0..7).

### ota ↔ `cloud.ota_url` / `OtaConfig`
`url`↔`cloud.ota_url`. `channel`/`source` no aplican en FPM.

### users ↔ `general.users[]` + `general.factory` / `UserConfig`
`{name,pin,role,locked,last}`. `pin` redactado en lectura. Roles: 0=NONE,1=TECH,2=ADMIN,3=FACTORY.
`admin` en el JSON = `general.factory` (no editable por `user_upsert`, que solo toca `users[]`).

### channels ↔ `sensors` / `ChannelConfig`/`BleChannelConfig`
| JSON canal | AppConfig | Notas |
|---|---|---|
| `id` | — | 1=Presión, 2=Flujo |
| `unit` | `sensors.pressure_unit` / `flow_unit` | etiqueta app ↔ interna (kPa↔kpa, m³/h↔m3h) |
| `gas_color` | `sensors.gas_type` | color NFPA/ISO ↔ clave gas |
| `range_min`/`range_max` | 0 / `sensors.pressure_fullscale_kpa`·`flow_fullscale_lpm` | escala (min<max garantizado) |
| `lo_enabled`/`lo_limit` | `alarm_limits.pressure_min_*` | Presión |
| `hi_enabled`/`hi_limit` | `alarm_limits.pressure_max_*` / `flow_high_*` | |
| `cal` | — | Axira: fija (offset 0, gain 1) |
| `sensor_type` | fijo | 0=Presión, 4=Flujo |

**Unidades internas:** el FPM guarda presión en **kPa** y flujo en **L/min**; convierte a/desde
la unidad de la app en `fpm_ble_config.c` (`press_to_disp`/`press_from_disp`, `flow_*`).

---

## 4. Límites de longitud (deben coincidir)

`config_contract.dart :: ContractLimits` es la fuente de verdad; los buffers de `AppConfig`
se dimensionaron para cumplirlos:

| Campo | ContractLimits (chars) | Buffer AppConfig |
|---|---|---|
| `wifi_password` | 64 | `wifi.password[65]` |
| `hostname` | 39 | `eth.hostname[40]` |
| `timezone` | 47 | `general.timezone[48]` |
| `bluetooth.name` | 31 | `bt.legacy.name[32]` |
| `wifi_ssid` | 32 | `wifi.ssid[33]` |
| `user.name/pin/last` | 23 | `app_user_t.*[24]` |

Buffers internos alineados: `wifi_mgr.c::wifi_view_t` (ssid[33], password[65]),
`eth_mgr.c::eth_view_t` (hostname[40]), `ui_connectivityScreen.c` (ble_name[32]).

---

## 5. Secretos y versión

- **Secretos** (`wifi_password`, `root_ca`, `device_cert`, `private_key`, `pin`): el firmware
  los **redacta** en lectura; la app los elimina si están vacíos antes de enviar
  (`ContractSecrets.stripEmpty`). Regla: **vacío/omitido = conservar el actual**.
- **Versión:** documento `ver = 3` (`ContractVersion.current`).

---

## 6. Cambios aplicados en el firmware (esta tanda)

1. **C1** — Característica de canal (`1006`) ahora usa esquema **PLANO** (`build_channel_flat`)
   y `apply_channel_obj` acepta plano + anidado. Antes emitía/esperaba anidado → los umbrales
   de alarma se perdían en ambos sentidos.
2. **C2** — Op `set_clock` implementada: `fpm_ble_set_clock_json` → callback → `time_mgr_set_datetime`
   (hora del sistema + RTC RV-3028). Registrado en `main.c`.
3. **C3** — Buffers agrandados a los límites del contrato (WiFi pass, hostname, timezone, BT name)
   en `AppConfig` y en los consumidores de red.
4. **C4** — `info` ahora envía `enabled_channel_count` (además de `channels`).
5. **R1** — Los aplicadores BLE trabajan sobre una **copia** en heap (`cfg_dup`) y recargan el
   snapshot de una vez, reduciendo torn-reads con la tarea LVGL.
6. **R2** — Nuevo `sensors.pressure_fullscale_kpa` (default 1000) para emitir una escala de
   presión válida (min<max) que pase la validación de la app.

**No requiere cambios en la app** (el firmware se alineó al contrato existente).

## 7. Pendiente

- Seguridad: pasar características a `*_ENC` + passkey mostrado en la HMI.
- Serialización completa de la caché de config (mutex/RW-lock) — R1 solo reduce la ventana.
- Validar en hardware: rebuild + flash (`idf.py reconfigure && build && flash`), luego probar el
  editor de canales (leer→editar umbral→guardar→releer) y `set_clock` desde la app.
