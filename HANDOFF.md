# Handoff — ClaudeHMI-FW (FPM-200) · sesión 2026-07-31

Documento para **retomar el trabajo en otra máquina**. Resume todo lo hecho en esta
sesión, cómo continuar, y qué queda pendiente. Complementa a `SESION_HMI.md` (historial
detallado) y `HMI_MIGRATION.md`.

---

## 0. Contexto rápido

- **Proyecto:** `ClaudeHMI-FW` = firmware del **medidor de flujo/presión** (FPM-200), ESP32-S3
  N16R2, pantalla ST7796 480×320 táctil (FT5x06), ESP-IDF 6.0.1, LVGL 9.
- **Equipo hermano:** `D:\SoftwareDevelop\Embedded\ClaudeHMI` = monitor de gases medicinales.
  Tiene la **app Flutter en `ClaudeHMI/mobile`** y firmware de referencia (patrón BLE/red).
- **Base git:** rama `main`, último commit **`c03ba34`** ("Arregladas las pantallas... se
  adiciono la pantalla de autenticacion").
- **Simulador de PC:** repo hermano `ClaudeHMI-Sim` (Visual Studio, LVGL Windows). Compila
  las fuentes reales de `ClaudeHMI-FW/main/ui` por ruta relativa.

## 1. ⚠️ ESTADO CRÍTICO — leer antes de nada

- **TODO lo de esta sesión está SIN COMMITEAR y SIN COMPILAR/FLASHEAR.** No hay ESP-IDF en el
  entorno donde se editó (solo edición de código). El último binario en HW es `c03ba34` puro.
- **27 archivos modificados + 6 nuevos** (lista en §4). Un `git status` lo confirma.
- Antes de probar en HW hay que **compilar** (ver §2). El primer build necesita `reconfigure`
  porque hay **archivos nuevos** y `main/CMakeLists.txt` usa `GLOB_RECURSE`.

## 2. Cómo continuar en otra máquina

```bash
# 1. Traer el código (si no está commiteado, copiar el working tree o commitear+push antes)
cd D:\SoftwareDevelop\Embedded\ClaudeHMI-FW

# 2. Reconfigurar (OBLIGATORIO: hay .c nuevos que el GLOB no detecta sin esto)
idf.py reconfigure

# 3. Compilar
idf.py build          # si hay errores raros de cache: idf.py fullclean && idf.py build

# 4. Flashear y monitorear
idf.py -p COM3 flash monitor
```

- **Simulador (ClaudeHMI-Sim):** al abrirlo hay que **agregar a mano** los `.c` nuevos al
  `LvglWindowsSimulator.vcxproj` (+ `.filters`): `ui_usersScreen.c`, `ui_userEditScreen.c`.
  (`config_mode.c` NO va al sim: es solo firmware, va guardado con `#ifdef ESP_PLATFORM`.)
- **Managed components** no se versionan → tras clonar limpio: `idf.py reconfigure`.

## 3. El bug del watchdog (freeze al entrar a config) — DIAGNÓSTICO + FIX

**Síntoma:** tras autenticarse y entrar a Configuración, al tocar un tile (p. ej. "Pantalla")
la pantalla se congela.

**Causa (2 logs de HW, build `c03ba34`):** `taskLVGL` se cuelga **dentro de `lv_anim_start`
(lv_anim.c:129)**, llamado por `lv_screen_load_anim` — o sea la **animación de transición** de
pantalla, NO la construcción (que sí termina). Backtrace idéntico repetido cada 5 s = colgado.
No es concurrencia (`bsp_display_lock`→`lvgl_port_lock`, mismo mutex que la tarea LVGL). El
splash animó bien en el boot pero la navegación se colgó bajo carga (WiFi+ETH+AWS+HTTP activos,
RAM interna fragmentada ~59 %).

**FIX aplicado:** se quitaron TODAS las animaciones de transición
(`lv_screen_load_anim` → `lv_screen_load`) en `main/ui/ui_nav.c` (5 sitios) y en el splash
(`main/ui/ui.c`, con borrado manual del splash porque se pierde el `auto_del`). Cambio de
pantalla instantáneo = nunca se llama `lv_anim_start` = se elimina la ruta del cuelgue. Los
eventos `SCREEN_LOADED/UNLOADED` siguen funcionando.

**Caveat:** es una mitigación de alta confianza (elimina la ruta observada). Si tras flashear
se colgara por OTRA vía de animación (p. ej. transición de estado del tema al presionar), el
subsistema de animación estaría roto de raíz → siguiente paso sería desactivar transiciones del
tema globalmente. **Validar en HW.**

## 4. Cambios realizados en esta sesión (por tanda)

### A. Roles, usuarios y gating de config
- `ui_cfg.h/.c`: API de permisos y gestión de usuarios — `ui_auth_can(min)`,
  `ui_auth_can_manage/can_edit_user/max_assignable`, `ui_auth_user_add/update/remove`,
  `ui_auth_set_factory_pin`. Jerarquía TECH<ADMIN<FACTORY; ADMIN gestiona técnicos, FACTORY
  también administradores. Persisten en NVS.
- **Nuevas pantallas** `ui_usersScreen.{c,h}` (lista con badges; filas fuera de alcance con
  candado) y `ui_userEditScreen.{c,h}` (alta/edición: nombre, PIN, rol, eliminar). Acceso desde
  el **chip de usuario** del header de "Ajustes generales" (solo ADMIN+).
- Gating: **Nube/MQTT y Bluetooth = ADMIN** (`ui_netCloudScreen.c`, `ui_netBleScreen.c`): banner
  `ui_notice()` (nuevo en `ui_widgets`) + guardas en `save_cb` y "Borrar certificados".
  WiFi/Ethernet quedan en TÉCNICO.
- Cableado en `ui.c/.h` (`ui_open_users_cb`, `ui_open_useredit_cb`).

### B. Datos reales en Conectividad
- `ui_connectivityScreen.c` reescrito: SSID/IP/broker/nombre BLE desde AppConfig; estado en vivo
  (RSSI, IP, ETH up, BLE/nube conectados) desde los managers bajo `#ifdef ESP_PLATFORM`
  (`wifi_mgr_get_netinfo`, `eth_mgr_is_up`, `transport_ble_is_connected`, `cloud_mgr_connected`);
  en el sim cae a la config. Sin textos demo.

### C. Keypad decimales + pulido idioma/tema
- `ui_keypadScreen.c`: tecla `.` cuando la unidad admite decimales (bar/MPa/m³h).
- `ui_nav.c` (`ui_nav_swap` + back reaplica tema) y `ui_generalSimpleScreen.c` (recrea el
  `general` padre al cambiar idioma) para que "atrás" salga traducido/tematizado.

### D. RAM / estabilidad
- **Churn de config:** `alarm_mgr.c` hacía `appcfg_load()` (parseo NVS+JSON) en CADA muestra →
  ahora `appcfg_cache_get()` (memcpy). `storage_extras.c` (`appcfg_set/patch`) ahora llaman
  `appcfg_cache_reload()` para que dashboard/alarmas vean config remota.
- **Modo config BLE** (espejo de `ClaudeHMI/ble_service.c`): `main/config_mode.{c,h}` nuevos.
  `enter()`: `transport_mqtt_stop()` (nuevo en `transport_mqtt.c/.h`) + `wifi_mgr_stop()` +
  delay + BLE adv ON. `exit()`: adv OFF + `wifi_mgr_start()` + `transport_mqtt_on_time_ready()`.
  Cableado a `ui_bleAppScreen` (eventos `SCREEN_LOADED/UNLOADED`). `config_mode.c` está
  **explícito** en `main/CMakeLists.txt` (el GLOB no toma la raíz de `main/`).

### E. Dashboard alineado a mockups (`3a/3b/4b/4c`)
- `ui_mainScreen.c`: etiqueta central del eje = rango seguro de presión (`min-max seguro`) y
  umbral alto de flujo (`<80% escala> alto`); subtítulo de consumo `· desde 00:00`. Marca del
  header se dejó data-driven (mejor que el placeholder del mockup).

### F. El fix del watchdog (§3)
- `ui_nav.c` + `ui.c`: sin animaciones de transición.

### G. Coherencia con la app Sensvax — contrato BLE (EN CURSO)
Estrategia (decisión del usuario 2026-07-31): **la app maneja los dos equipos** (MedGuard 12 y
Axira/FPM), detecta el modelo al conectar (`info.model` contiene "axira") y **comparte TODO el
esquema de config EXCEPTO `channels`**. En el FPM (Axira) los canales son **2 sensores FIJOS**
(Presión con gas+unidad, Flujo con unidad), **sin calibración** (fija/preprogramada).
- **HECHO — módulo de mapeo** `components/transport_ble/fpm_ble_config.{c,h}` (nuevo, en el CMake
  del componente): traduce el `AppConfig` del FPM ⇄ el **esquema JSON exacto de la app**
  (`mobile/medguard_config/lib/src/models/device_config.dart`: `ver, display, time, alarms,
  network, modbus, cloud, bluetooth, ota, users, admin, channels[]`). Funciones:
  `fpm_ble_info_json()` (identidad, model="Axira…" para la detección), `fpm_ble_config_read_json(redact)`
  (config completa, secretos redactados, canales Axira 2 fijos), `fpm_ble_config_apply_json(json)`
  (merge parcial → AppConfig → NVS + reload). Incluye conversión de unidades y gas↔color.
- **HECHO — servicio GATT** (`components/transport_ble/transport_ble.c` reescrito): expone el servicio
  `9f3c1000-…` + 7 características `1001–1007` (Info/WiFi/AWS/Estado/Control/Canal/Config) en vez del pipe
  `0xF00D`. Dispatch `chr_access`: lecturas (Info/WiFi/Cloud/Status/Channel + Config **troceado** por
  `mtu-3`, EOF=0 bytes), escrituras (WiFi/Cloud/Channel dedicadas), y ops de Control: `config_read`
  (arma el snapshot con `fpm_ble_config_read_json(true)`), `set_config`→`fpm_ble_config_apply_json`,
  `select_channel {id}`, `finish` (hook `s_finish_cb` opcional + corta la conexión). **Advertising**
  actualizado: incluye el UUID de servicio de 128 bits (la app filtra por servicio) + nombre en
  SCAN RESPONSE. Se agregaron las 6 funciones de característica dedicada a `fpm_ble_config`
  (`fpm_ble_wifi/cloud/channel/status_read/apply`). Nuevo hook público
  `transport_ble_set_finish_cb()`.
- **HECHO — usuarios por BLE**: `fpm_ble_user_op_json()` (`user_upsert`/`user_remove` sobre
  `general.users[]`, alta con PIN obligatorio, edición preserva PIN si viene vacío, rechaza dejar el
  equipo sin admin). Cableado en el dispatch de Control.
- **HECHO — `finish` cableado**: `main.c` registra `transport_ble_set_finish_cb(on_ble_finish)`; el cb
  hace `lv_async_call(ble_finish_async)` → `ui_nav_show_root()` (dashboard) → al descargarse la pantalla
  "Configurar por app" se dispara `config_mode_exit()` (restaura Wi-Fi/AWS). Deferido a la tarea LVGL
  (el cb corre en la tarea host de NimBLE).
- **FALTA en el GATT** (no bloquea lo básico): (a) `set_clock` sigue como TODO (log "no implementada")
  — necesita un hook al RTC/`time_mgr`; (b) **cifrado/passkey**: v1 usa características SIN cifrado
  (Just Works) para que la app lea/escriba sin PIN — pasar a `*_ENC` + passkey mostrado en la HMI como
  el hermano (decisión de seguridad).

**Archivos nuevos:** `main/config_mode.{c,h}`, `main/ui/screens/ui_usersScreen.{c,h}`,
`main/ui/screens/ui_userEditScreen.{c,h}`, `components/transport_ble/fpm_ble_config.{c,h}`.

## 5. Lo que sigue / PENDIENTE

### Prioridad 1 — compilar y validar en HW el contrato BLE (nuevo, grande)
- **Compilar** `idf.py reconfigure && idf.py build` (nuevos `.c` en el componente `transport_ble`).
  Revisar la API NimBLE (firmas de `ble_gap_adv_rsp_set_fields`, campo `uuids128` de
  `ble_hs_adv_fields`, `ble_att_mtu`) — se siguió el patrón del hermano pero no se compiló aquí.
- **Probar con la app Sensvax**: abrir en la HMI "Configurar por app" (entra en config_mode → BLE adv
  con el servicio `9f3c1000`), la app debe **descubrir** el equipo (filtra por servicio), leer Info
  (`model:"Axira…"` → detecta 2 variables), hacer `config_read`, editar WiFi/Cloud/canales/secciones y
  que se **reflejen en la HMI** (la caché se recarga). Verificar la sección `channels` (2 fijos, sin cal).
- **Completar el GATT** (ver §4.G "FALTA"): `set_clock`, `user_upsert/remove`, cifrado+passkey, y
  cablear `transport_ble_set_finish_cb()` en `main.c` a `config_mode_exit()`+salir de la pantalla.
- **App-side (repo hermano):** validar que el editor de canales soporte Axira (2 fijos, sin exigir cal).

### Prioridad 2 — validar en HW el resto (ya implementado)
- Confirmar que **el freeze desapareció** al navegar a las pantallas de config (fix de animaciones).
- Modo config BLE: al abrir "Configurar por app" caen WiFi/AWS (logs `config_mode: heap interno
  [antes/después]`) y al salir reconecta WiFi+MQTT.
- Revisar warnings de compilación (`-Werror` si aplica).

### Nota — "Config mínima" (decisión previa, ahora enmarcada en el contrato compartido)
Con el contrato BLE compartido, la app configura TODO (incl. Nube/AWS/avanzado). En la HMI se puede
mantener solo **ergonomía local + red básica** (brillo/tema, idioma, fecha/hora, audio, info, WiFi/Eth)
y **sensores como el mockup** `5b` (unidades + umbrales, sin calibración). El recorte de pantallas de la
HMI es OPCIONAL y va DESPUÉS de que el BLE funcione (si no, la Nube quedaría inconfigurable). No urge.

### Prioridad 3 — deuda/pulido
- **Login de FABRICANTE:** el pad del login es numérico; la passphrase del fabricante es
  alfanumérica → falta un acceso oculto (candado) con teclado QWERTY que valide contra
  `general.factory.pin`. Hasta entonces las acciones FACTORY están en el código pero **inertes**.
- **Consumo mensual** del dashboard (`· acum. mes N m³`): falta contador mensual persistente
  (NVS + rollover, estilo `metrics_store`). El diario ya funciona.
- **infoScreen:** quitar la fecha de calibración (no se calibra en este equipo).
- **Coherencia dashboard:** el "acum. mes" y detalles finos de posición de ejes.
- **i18n:** textos nuevos solo en ES; el catálogo `_t` no trae EN de esta tanda.
- **Limpieza de logs:** el DEBUG forense del cert CA en `transport_mqtt.c` es ruido (E-level).

## 6. Gotchas importantes (no re-descubrir)

- **`main/CMakeLists.txt` GLOB** solo toma `main.c` + `ui/*.c`. Cualquier `.c` nuevo en la raíz
  de `main/` (como `config_mode.c`) va **explícito** en `SRCS`. Los de `ui/screens/` los toma el
  GLOB pero exigen `idf.py reconfigure`.
- **Simulador (`#ifdef ESP_PLATFORM`):** las pantallas compilan en el sim, que NO tiene los
  managers de red ni `config_mode`. Todo lo que llame a `wifi_mgr/transport_*/config_mode` desde
  una pantalla va guardado con `#ifdef ESP_PLATFORM`.
- **Nombres que chocan con winsock** en archivos del sim (s_net, s_host, s_addr...). Ya pasó con
  `s_host` → `s_hostname` en `ui_netEthScreen.c`.
- **Caché de config:** la fuente de verdad en runtime es el snapshot `appcfg_cache_peek/get`. El
  UI lo muta en sitio (peek → modificar → `appcfg_save`). La config remota (HTTP/BLE) ahora
  recarga la caché. NO volver a poner `appcfg_load()` en bucles calientes (fragmenta el heap).
- **Sin animaciones de transición** (a propósito, por el watchdog). Si se reintroducen, validar
  en HW bajo carga.

---

*Generado el 2026-07-31. Ver `SESION_HMI.md` §9 para el historial completo con detalles.*
