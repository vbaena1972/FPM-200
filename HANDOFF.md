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

**Archivos nuevos:** `main/config_mode.{c,h}`, `main/ui/screens/ui_usersScreen.{c,h}`,
`main/ui/screens/ui_userEditScreen.{c,h}`.

## 5. Lo que sigue / PENDIENTE

### Prioridad 1 — validar en HW (tras compilar+flashear)
- Confirmar que **el freeze desapareció** al navegar a las pantallas de config.
- Probar el **modo config BLE**: al abrir "Configurar por app" deben caer WiFi/AWS (ver logs
  `config_mode: heap interno [antes/después]`), la app Flutter empareja por BLE, y al salir se
  reconecta WiFi+MQTT.
- Revisar warnings de compilación (`-Werror` si aplica).

### Prioridad 2 — "Config mínima" (DECISIÓN DEL USUARIO 2026-07-31)
En la HMI queda **ergonomía local + red básica**; el resto por la app Flutter:
- **Se queda en HMI:** brillo/tema, idioma, fecha/hora, audio de alarmas, info/estado, y
  **WiFi/Ethernet básico**.
- **Se mueve a la app:** Nube/AWS (endpoint, certs, MQTT), IoT, gestión avanzada.
- **Sensores: dejar como el mockup** `5b sensorScreen-Editable` (ver/editar unidades y umbrales;
  **sin calibración** — los sensores traen su memoria de cal).
- *Acción:* recortar/ocultar de la HMI las pantallas/campos que pasan a la app (empezar por
  `ui_netCloudScreen` avanzado y lo que sobre); mantener la coherencia con lo que la app
  (`ClaudeHMI/mobile`) espera escribir vía BLE (contrato = `AppConfig.json`).

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
