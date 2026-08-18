# Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)

Documento para **retomar en una sesión nueva**. Estado real, qué está commiteado, el
bug abierto (watchdog LVGL) y qué sigue. Complementa a `SESION_HMI.md` (historial) y
`HMI_MIGRATION.md`.

---

## Actualización 2026-08-18 (sensores reales — se elimina la simulación)

Se reemplazó por completo la **tarea dummy** de `sensors_runtime` por adquisición real de hardware.

**Drivers nuevos (`components/drivers/`, patrón `i2c_master` como `rtc_rv3028`):**
- **`ms5803`** — presión/temperatura MS5803-14BA (I²C **0x76**, CSB alto; PROM + CRC4, D1/D2 OSR 4096, compensación 1er+2º orden). `pressure_kpa = P/100`. El init reintenta reset+PROM (la 1ª transacción NACKea aunque la dirección ACKee). Validado: **84.9 kPa = presión atmosférica de Medellín (~1495 m)**, T≈29.6 °C.
- **`ads1115`** — ADC 16-bit (I²C **0x48**, AIN0 single-ended, PGA ±4.096 V) → voltaje del sensor de flujo **FS7**.
- **`at24c256`** — EEPROM AT24C256C (I²C **0x50**, 32 KB, páginas 64 B). Guarda la **calibración de sensores** (bloque versionado `magic "SEN1"` + CRC16, auto-init en el primer arranque). Ámbito: solo parámetros de sensor (WiFi/BLE siguen en NVS).
- **`sfm3300`** — caudalímetro de referencia Sensirion **SFM3300-D** (I²C **0x40**) en un **2º bus I²C** (GPIO43=SDA / GPIO44=SCL). Para calibrar/observar el FS7. `flow[slm]=(raw-32768)/120`, CRC-8 poly 0x31. **HW: es de 5 V → requiere pull-ups a 3.3 V (o level-shifter con pull-ups) en el bus.**

**Modelo de flujo FS7** (ley de King, datasheet AFFS7): `U=Vain0/divisor; v=((U-U0)/k)^(1/n); flow_lpm=v·flow_scale+flow_offset`. Defaults `divisor=0.294, U0=3.6, k=0.91, n=0.51`. **`flow_scale` aún sin calibrar** (=1.0 → "flujo"≈velocidad); usar el SFM3300 como patrón para ajustarlo. El log imprime FS7 y `SFMref` lado a lado.

**Presión manométrica (gauge) — opción B (tara), EEPROM v4:** el MS5803 es absoluto; la línea médica necesita gauge = absoluta − atmosférica local. Un offset fijo se rompe entre altitudes (Medellín 84.9 vs Barranquilla ~101 kPa). `sensors_runtime_tare_pressure()` captura la atmosférica actual (línea venteada) como cero y persiste en EEPROM. Disparador **por BLE** en `fpm_ble_config_apply_json`: `{"pressure_tare":true}` / `{"pressure_mode":"gauge"|"absolute"}` (falta el botón en la app). Para producción se recomienda un **BMP581 venteado @0x46/0x47** (referencia barométrica continua, sin re-cero) que además cubre la temperatura de gabinete.

**Otros cambios:** touch FT5x06 ahora **no fatal** si no hay panel (evitaba boot-loop sin display). Escaneo I²C al arranque para diagnóstico. Temperatura del MS5803 añadida al `sensor_sample_t`, a `get_min_max` y al payload AWS (`transport_mqtt.c`). FS7 habilitado por defecto (`flow_enabled=1`).

**⚠️ Estado HW al cierre de sesión:** tras cablear el SFM3300, el scan I²C del bus 1 pasó de `0x48 0x50 0x52 0x60 0x76` a `0x38 0x52 0x60` → **desaparecieron los tres del módulo de sensor (MS5803/ADS/EEPROM) juntos**, mientras RTC (0x52) y touch nuevo (0x38) siguen. Diagnóstico: **conexión común del módulo de sensor perdida** (alim. 3.3 V / GND / rama I²C), no es software. Pendiente: reasentar el conector del módulo. El SFM (bus 2) es tema aparte y necesita pull-ups.

**Build:** el entorno del usuario (VS Code/IDF) compila bien; en Bash/PowerShell hay que fijar PATH/env a mano y Windows Defender pone en cuarentena archivos de `C:\Espressif` de forma intermitente (agregar exclusiones).

---

## Actualización 2026-08-09 (estabilización en HW + app)

Sesión larga de depuración en hardware. **Resueltos varios crashes recurrentes; causa raíz común: agotamiento de RAM interna.**

**Firmware (este repo):**
- **Fix de raíz de los cuelgues de `taskLVGL`:** la memoria de objetos de LVGL vivía en RAM interna (`CONFIG_LV_USE_CLIB_MALLOC` + `SPIRAM_USE_CAPS_ALLOC`); se agotaba tras el ciclo config/reconexión y `lv_malloc` fallaba → `LV_USE_ASSERT_MALLOC` en bucle → watchdog (al abrir config, "Audio de alarmas"/sensorDiag, etc.). **Solución:** `CONFIG_LV_USE_CUSTOM_MALLOC=y` + `main/lv_port_mem.c` enruta LVGL a **PSRAM** (con `-Wl,-u,lv_malloc_core` en `main/CMakeLists.txt` para forzar el enlace del objeto). Supersede el viejo intento "CLIB heap_caps".
- Crash al **guardar** config por BLE: stack de `nimble_host` 5120→**8192** + `text[]` estático (`transport_ble.c`).
- Pantalla **"Configurar por app"** (`ui_bleAppScreen.c`): QR real de emparejamiento (`medguard://pair?...`, `LV_USE_QRCODE`), indicador vivo Anunciando/Conectado (badge cyan arriba-dcha), cuenta atrás de auto-apagado (120 s), botón "Apagar Bluetooth", y **al desconectar la app vuelve al dashboard** (lo maneja el tick de la pantalla, NO un `lv_async_call` desde la tarea NimBLE). El statusbar del dashboard refleja BLE vivo.
- Pantalla **"Sensores"** (`ui_sensorEditScreen.c`): unidades completas (psi/bar/kPa/MPa · L/min/m³/h/SCCM) + **dropdown de gas** (catálogo corto en `ui_cfg`, gases con costo) + **norma NFPA/ISO** (`sensors.color_code`). El banner de gas del dashboard usa ese catálogo.

**App Flutter (`ClaudeHMI/mobile/medguard_config`):** detección Axira robusta (`device.name`/`model`/`bleName`/`enabledChannels==2`); el FPM muestra **2 sensores** (no 12 canales), **1 relé**, sin Modbus; editor de sensor sin "Tipo de sensor", sin "Calibración", sin "Escala 4-20 mA" (rango fijo → va en la EEPROM del módulo de sensado).

**Pendiente de validar en HW:** el último cambio (pantalla de Sensores) no se compiló en el entorno de edición; el usuario reflashea.

---

## 0. Contexto

- **Proyecto:** `ClaudeHMI-FW` = firmware del **medidor de flujo/presión** (FPM-200 / "Axira"),
  ESP32-S3 N16R2, ST7796 480×320 táctil (FT5x06), ESP-IDF 6.0.1, LVGL 9.
- **Equipo hermano:** `D:\SoftwareDevelop\Embedded\ClaudeHMI` = monitor de gases (MedGuard 12).
  Tiene la **app Flutter en `ClaudeHMI/mobile/medguard_config`** (Sensvax, multiproducto
  MedGuard+Axira) y firmware de referencia (`main/ble_service.c`, patrón BLE/red).
- **Simulador PC:** repo hermano `ClaudeHMI-Sim` (Visual Studio, LVGL Windows), compila las
  fuentes reales de `ClaudeHMI-FW/main/ui` por ruta relativa.

## 1. ⚠️ ESTADO — leer antes de nada

- **Rama de trabajo: `feature/ble-app-contract`.** Historial:
  - `66014fa` (en `main`) — trabajo previo YA commiteado (roles/usuarios, gating, datos reales,
    keypad, config_mode, dashboard, fix de animaciones, churn). El usuario lo commiteó.
  - `711b463` — **contrato BLE** con la app (servicio GATT `9f3c1000` + mapeo). §3.
  - `6e0b82c` — **fix del watchdog: LVGL usa heap_caps (CLIB)**. §2. ← HEAD
- **Todo está COMMITEADO** en la rama. Para llevarlo a `main`: `git checkout main && git merge
  feature/ble-app-contract`.
- **NADA de las últimas dos tandas (BLE + fix CLIB) se ha validado en HW.** El último flash del
  usuario fue `711b463` (SIN el fix CLIB) y **seguía crasheando** — falta rebuild+flash con `6e0b82c`.
- No hay ESP-IDF en el entorno donde se edita el código (solo edición). Compilación/flasheo los hace
  el usuario en su terminal IDF.

## 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR

**Síntoma:** el dashboard se congela **de forma determinista a ~43 s** del arranque (con datos
dummy que rampan la presión). `taskLVGL` queda al 100 % y el Task WDT dispara cada 5 s.

**Diagnóstico (definitivo, 3 logs de HW):** el PC del backtrace está **SIEMPRE exactamente en
`lv_realloc`** (`find_track_end` del layout flex, `lv_flex.c:281`). No es el layout el que gira: es
**el asignador de memoria de LVGL atascado en un bucle**. LVGL estaba configurado con su **TLSF
builtin + pool FIJO de 64 KB** (`CONFIG_LV_USE_BUILTIN_MALLOC`, `CONFIG_LV_MEM_SIZE_KILOBYTES=64`).
El `grow_dsc` del flex se realloca en CADA refresco (~5 Hz); ese pool chico se fragmenta de forma
determinista hasta que el TLSF entra en bucle. (Los fixes previos — quitar animaciones de
transición, quitar el churn de `appcfg_load` — eran reales pero solo movieron DÓNDE se golpea el
asignador roto; el crash migró de `lv_anim_start`/`lv_malloc` a `find_track_end`/`lv_realloc`.)

**FIX aplicado (commit `6e0b82c`, en `sdkconfig` + `sdkconfig.defaults`):**
`CONFIG_LV_USE_CLIB_MALLOC=y` → LVGL usa el **heap de ESP (`heap_caps`)**: robusto, no entra en
bucle, sin el techo de 64 KB.

**PARA VALIDAR (esto es lo primero en la sesión nueva):**
```bash
idf.py build            # (o fullclean && build); reconfigure si hubo cambios de archivos
idf.py -p COM3 flash monitor
```
- Confirmar en el boot que `App version` / `Compile time` **CAMBIARON** (si no, se flasheó el binario
  viejo — el error del usuario la última vez).
- Dejar el dashboard >1–2 min: NO debe dispararse el watchdog a los 43 s.
- **Si PERSISTE:** el asignador no era la causa única → reducir el churn de relayout del dashboard
  (`ui_mainScreen.c` `ui_main_update` re-escribe anchos `LV_PCT` de las barras y textos en CADA
  frame aunque no cambien; hacerlo solo cuando el valor cambia) y/o habilitar heap poisoning
  (`CONFIG_HEAP_POISONING_COMPREHENSIVE`) para localizar una corrupción real.

## 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW

**Estrategia (decisión del usuario):** la app maneja **los dos equipos**; detecta el modelo al
conectar (por `info.model` que contiene "axira") y comparte TODO el esquema de config **EXCEPTO
`channels`**. En el FPM (Axira) los canales son **2 sensores FIJOS** (Presión con gas+unidad,
Flujo con unidad), **SIN calibración** (fija/preprogramada).

- **`components/transport_ble/fpm_ble_config.{c,h}` (nuevo):** puente `AppConfig` del FPM ⇄ el
  **esquema JSON exacto** que parsea la app (`ClaudeHMI/mobile/.../models/device_config.dart`:
  `{ver, display, time, alarms, network, modbus, cloud, bluetooth, ota, users, admin, channels[]}`).
  Funciones: `fpm_ble_info_json`, `fpm_ble_config_read_json(redact)`, `fpm_ble_config_apply_json`,
  dedicadas `fpm_ble_wifi/cloud/channel/status_read/apply`, y `fpm_ble_user_op_json`
  (`user_upsert`/`user_remove`). Convierte unidades (kPa↔kpa, m³/h↔m3h) y gas↔color NFPA;
  `sensor_type` 0=Presión, 4=Flujo. **Secretos redactados** (PIN, pass WiFi, certs).
- **`transport_ble.c` reescrito:** servicio GATT `9f3c1000-6d4f-4d5b-8c9a-2f3e7a110001` + 7
  características `1001–1007` (Info/WiFi/AWS/Estado/Control/Canal/Config) en vez del pipe `0xF00D`.
  `chr_access` despacha lecturas (Config **troceado** por `mtu-3`, EOF=0 bytes) y ops de Control
  (`config_read`, `set_config`, `select_channel`, `finish`, `user_upsert/remove`). Advertising ahora
  incluye el UUID de servicio de 128 bits (la app filtra por servicio) + nombre en SCAN RESPONSE.
- **`main.c`:** `transport_ble_set_finish_cb(on_ble_finish)` → al "finish" de la app,
  `lv_async_call → ui_nav_show_root()` (dashboard) → al descargarse "Configurar por app" corre
  `config_mode_exit()` (restaura WiFi/AWS).

**Prueba objetivo:** abrir en la HMI Conectividad→Bluetooth→"Configurar por app" (entra en
config_mode: apaga WiFi/AWS, sube BLE adv). La app Sensvax debe: descubrir el equipo → leer Info
(detecta Axira, 2 variables) → `config_read` → editar WiFi/Cloud/canales/secciones → que se
**reflejen en la HMI**. Al "Finalizar" vuelve al dashboard y reconecta WiFi/AWS.

**FALTA del GATT** (no bloquea lo básico): `set_clock` (TODO, necesita hook al RTC/`time_mgr`);
**cifrado/passkey** (v1 usa características SIN cifrado / Just Works para que la app lea/escriba sin
PIN — decisión de seguridad: pasar a `*_ENC` + passkey mostrado en la HMI, como el hermano).
App-side: validar que el editor de canales soporte el caso Axira (2 fijos, sin exigir calibración).

## 4. Cómo compilar / gotchas de build

```bash
idf.py reconfigure   # OBLIGATORIO si hay .c nuevos (el GLOB no los detecta) — ya están commiteados
idf.py build         # errores raros de cache -> idf.py fullclean && idf.py build
idf.py -p COM3 flash monitor
```
- **`main/CMakeLists.txt` usa `GLOB_RECURSE`** y solo toma `main.c` + `ui/*.c`. Los `.c` nuevos en la
  RAÍZ de `main/` van **explícitos** en `SRCS` (ya está `config_mode.c`). Los de `ui/screens/` los
  toma el GLOB pero exigen `reconfigure`.
- **Simulador (ClaudeHMI-Sim):** sumar a mano al `.vcxproj`/`.filters` los `.c` nuevos de UI
  (`ui_usersScreen.c`, `ui_userEditScreen.c`). `config_mode.c` y `fpm_ble_config.c` NO van al sim
  (son firmware; lo que se llama desde pantallas va guardado con `#ifdef ESP_PLATFORM`).
- **NimBLE (transport_ble.c):** se siguió el patrón del hermano pero no se compiló aquí — revisar
  firmas al primer build (`ble_gap_adv_rsp_set_fields`, campo `uuids128` de `ble_hs_adv_fields`,
  `ble_att_mtu`).

## 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW)

- **Roles/usuarios/gating:** API `ui_auth_*` en `ui_cfg`; pantallas `ui_usersScreen` +
  `ui_userEditScreen` (acceso desde el chip de usuario de "Ajustes generales", ADMIN+); Nube/BLE
  requieren ADMIN (banner `ui_notice`).
- **Datos reales en Conectividad** (`ui_connectivityScreen.c`): RSSI/IP/enlace de los managers.
- **Keypad decimales** (bar/MPa/m³h); pulido de idioma/tema al volver (`ui_nav_swap`).
- **config_mode** (`main/config_mode.{c,h}`): apaga AWS (`transport_mqtt_stop`) + WiFi al entrar a
  "Configurar por app"; restaura al salir. Espejo de `ClaudeHMI/ble_service.c`.
- **Dashboard** alineado a mockups (ejes con rango seguro, subtítulo de consumo).
- **Fix de animaciones de navegación** (`ui_nav.c`/`ui.c`: `lv_screen_load` sin `_anim`) y **fix del
  churn** (`alarm_mgr.c` usa `appcfg_cache_get`; `appcfg_set/patch` recargan caché). ⚠️ Parciales:
  el crash de estabilidad real era el asignador (§2).

## 6. Pendiente (después de estabilizar §2 y validar §3)

1. **Completar GATT:** `set_clock`, cifrado/passkey.
2. **Login de FABRICANTE:** el pad del login es numérico; la passphrase del fabricante es
   alfanumérica → falta acceso oculto (candado) con teclado QWERTY que valide `general.factory.pin`.
   Hasta entonces las acciones FACTORY están en el código pero **inertes**.
3. **"Config mínima" (opcional):** con el BLE funcionando, la app configura TODO; en la HMI se puede
   dejar solo ergonomía local + red básica y **sensores como el mockup `5b`** (unidades+umbrales, sin
   cal). Recortar pantallas va DESPUÉS de que el BLE funcione (si no, la Nube queda inconfigurable).
4. **infoScreen:** quitar la fecha de calibración (este equipo no calibra).
5. **Consumo mensual** del dashboard (contador persistente NVS + rollover, estilo `metrics_store`).
6. **i18n** de los textos nuevos (solo ES); limpieza del DEBUG forense del cert en `transport_mqtt.c`.
7. **🔴 Caché de config SIN sincronización (riesgo de concurrencia ALTO — confirmado leyendo el
   código).** En `components/storage/storage.c`:
   - `appcfg_cache_peek()` (`storage.c:707`) **devuelve un puntero directo** al snapshot estático
     (`return &s_cfg_snapshot;` en `storage.c:714`; la estática es `static AppConfig s_cfg_snapshot;`
     en `storage.c:679`).
   - `appcfg_cache_get()` (`storage.c:693`) **copia** el snapshot (`memcpy(out, &s_cfg_snapshot, …)`
     en `storage.c:703`).
   - `appcfg_cache_reload()` (`storage.c:682`) reescribe la estática con `memcpy(&s_cfg_snapshot, …)`
     (`storage.c:688`). **No hay mutex, sección crítica ni acceso atómico** en ninguna de las tres.
   - **Riesgo ALTO:** un lector vía `peek` (tarea LVGL) puede observar el struct a medio sobrescribir
     (torn read) mientras otra tarea llama a `reload` (p. ej. `fpm_ble_config_apply_json` en la tarea
     host BLE). Incluso `get` puede copiar un estado parcialmente actualizado, porque los `memcpy` de
     lectura (L703) y escritura (L688) no están serializados.
   - **Antes de corregir:** localizar TODOS los consumidores de `appcfg_cache_peek()` y verificar si
     **guardan el puntero** (retención peligrosa) o **solo lo usan temporalmente** (uso puntual).
     Solo entonces decidir entre RW-lock en `storage`, devolver copia/snapshot inmutable, o forzar
     que todo consumo UI pase por la fachada `ui_cfg.c`.

## 7. Gotchas que no re-descubrir

- **Caché de config** = fuente de verdad en runtime (`appcfg_cache_peek/get`). El UI la muta en sitio
  (peek→modificar→`appcfg_save`); la config remota (HTTP/BLE) recarga la caché. **NO** poner
  `appcfg_load()` en bucles calientes (fragmenta/parsea; fue un problema).
- **Nombres que chocan con winsock** en archivos del sim (s_net/s_host/s_addr…). Ya pasó
  (`s_host`→`s_hostname` en `ui_netEthScreen.c`).
- **Sin animaciones de transición de pantalla** (a propósito). Si se reintroducen, validar en HW.
- **LVGL usa CLIB malloc ahora** (§2). No revertir a builtin sin subir mucho `LV_MEM_SIZE`.

---

*Actualizado 2026-08-01. Rama `feature/ble-app-contract` @ `6e0b82c`. Ver `SESION_HMI.md` §9 para el
historial detallado.*
