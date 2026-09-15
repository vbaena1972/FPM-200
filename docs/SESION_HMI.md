# Sesión — Migración HMI Axira (SquareLine → Claude Design)

Resumen de handoff para continuar en otra sesión. Producto: medidor de presión/flujo de gases medicinales,
ESP32-S3 **N16R2**, pantalla ST7796 480×320 táctil (FT5x06), ESP-IDF 6.0.1, LVGL 9.4.

---

## 1. Objetivo y estado

Reemplazar la HMI generada por **SquareLine Studio** por el diseño nuevo (16 mockups en `mockups/`),
borrando todo rastro anterior e integrándola con el firmware existente.

**Estado: las pantallas están codificadas, navegables y con lógica de config real (16 del diseño original +
login por rol, fecha/hora dedicada y gestión de usuarios agregadas después). Falta pulido y algunos ítems de
backend (abajo).** Validado visualmente en un simulador de PC.

> **Actualización 2026-08-09:** sesión de estabilización en HW. Causa raíz de los cuelgues de
> `taskLVGL` = memoria de LVGL en RAM interna (se agotaba); movida a **PSRAM**
> (`CONFIG_LV_USE_CUSTOM_MALLOC` + `main/lv_port_mem.c`). Además: fix del guardado BLE (stack
> `nimble_host` 8192), pantalla "Configurar por app" con QR real/timer/indicador/apagado, y
> pantalla "Sensores" con unidades completas + dropdown de gas + norma NFPA/ISO. App: FPM
> diferenciado (2 sensores, 1 relé, sin Modbus, editor recortado). Ver `HANDOFF.md`.

---

## 2. Repositorios (GitHub, cuenta vbaena1972)

| Repo | Contenido | URL |
|---|---|---|
| **ClaudeHMI-FW** | Firmware ESP-IDF con la HMI nueva | https://github.com/vbaena1972/ClaudeHMI-FW |
| **ClaudeHMI-Sim** | Simulador PC (LVGL Windows/VS) | https://github.com/vbaena1972/ClaudeHMI-Sim |

> ⚠️ **Acoplados**: el sim compila las fuentes reales de `..\..\ClaudeHMI-FW\main\ui` por ruta relativa.
> Clonar **ambos como carpetas hermanas** en el mismo directorio padre.
> El firmware, tras clonar, necesita `idf.py reconfigure` (managed_components no se versiona).

Ambos ya están pusheados (rama `main`). Este archivo y cambios recientes de la última fase **aún no
commiteados** — hacer `git add . && git commit && git push` en cada repo cuando corresponda.

---

## 3. Arquitectura de la nueva HMI (`main/ui/`)

- **`ui_theme.h`** — tokens de diseño (paleta, radios, espaciados, fuentes). Alineado 1:1 al design system
  "MedGuard" del proyecto hermano `D:\SoftwareDevelop\Embedded\ClaudeHMI` (dark, Nest híbrido).
- **`ui_icons.h`** + `fonts/ui_font_tabler{16,20,28}.c` — subconjunto de iconos **Tabler** (generado con
  `lv_font_conv`, macros `UI_SYM_*`).
- **Fuentes de texto**: **Inter Medium** (`fonts/mg_font_{12,14,18,22,36}.c` + `mg_num_48.c`), copiadas de
  ClaudeHMI para paridad de diseño. (Se eliminaron las Montserrat de SquareLine.)
- **`ui_widgets.{c,h}`** — fábricas: `ui_card`, `ui_box`, `ui_label`, `ui_icon`, `ui_pill`, `ui_icon_badge`,
  `ui_nav_header`, `ui_menu_row`. (Ojo: `ui_box/ui_label/ui_icon_badge` limpian `LV_OBJ_FLAG_CLICKABLE`
  porque en LVGL9 todo `lv_obj_create` nace clickeable y robaría el toque de las tarjetas.)
- **`ui_nav.{c,h}`** — navegación con pila (`ui_nav_load/back/pop_to/show_root`).
- **`ui.{c,h}`** — orquestador: `ui_init()` crea main, muestra splash ~2.2 s → main. Dispatch de todos los
  `ui_open_*_cb` (getters perezosos + `fresh_*` para diálogos/formularios).
- **`ui_cfg.{c,h}`** — puente config: `peek → modificar → appcfg_save → refrescar main`. Setters de unidad/
  gas, validación de PIN (`general.admin.pass`), y estado de "edición de umbral" (keypad→confirm→PIN→apply).
- **`ui_form.{c,h}`** — formularios: textarea + **teclado en pantalla**, switch, dropdown, botón Guardar.
- **`ui_events.{c,h}`** — vacío (placeholder; la lógica real vive ahora en ui_cfg/pantallas).
- **`screens/`** — 16 pantallas (ver §5).

### Contrato de integración con el firmware (preservado)
- `main/main.c` `ui_refresh_task` llama a **`ui_main_update(&last,…, NULL, state, muted)`** (NULL = usa el
  **caché vivo** de AppConfig → los cambios de config se reflejan sin reiniciar).
- `components/ui_bind/` reescrito para **glifos** (no imágenes): `ui_statusbar_controller.c` (BT/WiFi/ETH/
  Nube por color) y `ui_wifi_main_icon.c` (nivel por color). API pública intacta. REQUIRES ampliado
  (storage, sensors_runtime, drivers).

---

## 4. Design system (alineado a ClaudeHMI/MedGuard, dark)

Paleta (0xRRGGBB): bg `15181e`, surface `191c22`, surface2 `12151a`, hairline `2a2f37`, text `f1f3f5`,
dim `9aa1ab`, mute `6f767f`, ok `34d399`, warn `fbbf24`, **alarm `f87171`**, info `38bdf8`, accent/teal
`2dd4bf`. Gas: O2 `1d9e75`, aire `e0a800`, N2O `2b6cb0`, vacío `dfe3e8`. Radios: card 14 / tile 11 / pill 6.
Pad 12 / gap 8 (en pantallas de 480×320 se aprieta a 8/4-5).

---

## 5. Pantallas (16) y navegación

| Pantalla | Archivo | Origen |
|---|---|---|
| Splash | `ui_splashScreen` | boot → main |
| Principal (4 estados) | `ui_mainScreen` | raíz; `ui_main_update()` desde main.c |
| Ajustes generales (menú) | `ui_generalScreen` | ⚙ de main |
| Ajustes simple | `ui_generalSimpleScreen` | general → Pantalla / Fecha |
| Información | `ui_infoScreen` | general → Información |
| Conectividad (estado) | `ui_connectivityScreen` | general → Red y nube |
| Sensores (edición) | `ui_sensorEditScreen` | general → Unidades |
| Sensores (diagnóstico) | `ui_sensorDiagScreen` | general → Alarmas |
| Keypad umbral | `ui_keypadScreen` | sensorEdit → tocar umbral |
| Confirmar cambio | `ui_confirmScreen` | keypad → Aceptar |
| PIN | `ui_pinScreen` | confirm → Confirmar con PIN |
| Config por app (QR) | `ui_bleAppScreen` | ble form → botón |
| Form WiFi/Eth/Nube/BLE | `ui_net{Wifi,Eth,Cloud,Ble}Screen` | Conectividad → tocar tarjeta |
| Login por usuario + rol | `ui_loginScreen` | Configuración → acceso (agregada 07-30) |
| Fecha y hora | `ui_datetimeScreen` | general → Fecha y hora (agregada 07-30) |
| Usuarios (lista) | `ui_usersScreen` | chip de usuario del header de general (ADMIN+, 07-31) |
| Usuario (alta/edición) | `ui_userEditScreen` | usersScreen → fila / Agregar (07-31) |

Gating por rol (07-31): Nube/MQTT y Bluetooth exigen **ADMIN** (banner + guardas en save/borrar certs);
WiFi/Ethernet en **TÉCNICO**. Gestión de usuarios: ADMIN toca técnicos, FABRICANTE también administradores.

**Flujo de umbral (completo, con persistencia):** sensorEdit → keypad (precargado) → confirm (antes→después
reales) → PIN (valida `admin.pass`, default `1234`) → `ui_edit_apply()` guarda en NVS → vuelve a sensorEdit.

---

## 6. Lógica / persistencia (hecho)

- **Sensores**: unidades P/F guardan al tocar el segmento; gas cicla y guarda; umbrales por el flujo de PIN.
- **Red**: formularios wifi/eth/cloud/ble cargan de AppConfig, guardan a NVS y **aplican a los managers**
  (`wifi_mgr`, `eth_mgr`, `transport_ble`) bajo `#ifdef ESP_PLATFORM` (el sim omite los managers).
- **JSON**: `wifi/eth/bt/cloud` ya tenían todos los campos → **no se tocó**. `AppConfig.json` intacto.

---

## 7. Simulador de PC (`ClaudeHMI-Sim`)

- Copia del sim LVGL Windows/VS del usuario, cableada a las fuentes reales del firmware.
- Stubs de dependencias ESP en `sim_app/` (`storage.h` con AppConfig reducido, `sensors_runtime.h`,
  `alarm_mgr.h`, `ui_statusbar_controller.h`, `sim_backend.c` con datos de ejemplo + `appcfg_save` no-op).
- `LvglWindowsSimulator.cpp`: ventana 480×320, `ui_init()`, cicla los 4 estados cada 2.5 s, **reanuda el
  refr timer** del driver Windows (si no, la UI dinámica se congela).
- Abrir `LVGL.slnx`, proyecto de inicio **LvglWindowsSimulator**, **Debug | x64**, F5.
- **Al agregar/quitar archivos de UI hay que actualizar `LvglWindowsSimulator.vcxproj`** (lista `ClCompile`).

---

## 8. Cómo compilar

**Firmware** (terminal ESP-IDF):
```
idf.py fullclean   # primer build tras muchos cambios de archivos
idf.py build
idf.py -p COMx flash monitor
```
**Sim**: Visual Studio → `ClaudeHMI-Sim\LVGL.slnx` → LvglWindowsSimulator → Debug|x64 → F5.

### Arreglos preexistentes de build (IDF 6.0.1, no-HMI) ya aplicados
- `components/drivers/CMakeLists.txt`: quitado `ms5837.c` (no existía).
- `CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID=y` (para `TaskStatus_t.xCoreID`).
- Buffers `core_str/core_s` a `[12]` en `diag/mem_diag.c` y `task_tracer/task_tracer.c` (format-truncation).

---

## 9. Estado de pendientes (actualizado 2026-07-31)

### Hecho en la sesión 2026-07-22
1. ✅ **`brightness`**: campo por todo el pipeline (struct, defaults, json↔cfg, `AppConfig.json`, stub sim,
   saneo 10–100 en `appcfg_migrate` para NVS viejos). `ui_cfg_preview_brightness()` (HW sin persistir) y
   `ui_cfg_set_brightness()` (NVS+HW). Slider **funcional** en generalSimple (preview al arrastrar, guarda
   al soltar). `main.c` aplica el brillo configurado al arrancar (antes 100% fijo).
2. ✅ **Consumo (m³)** en main: integral de flujo (`∫ L/min·dt → m³`) en `ui_mainScreen.c` usando `ts_ms`
   de la muestra (dt=0 si se repite muestra → sin doble conteo; huecos >5 min se descartan). Reset a
   medianoche por rollover del reloj (`ui_main_set_clock`). Muestra "N L hoy" bajo 0.1 m³. **Vive en RAM**
   (se pierde al reiniciar). El sim ahora pasa `lv_tick_get()` como ts.
3. ✅ (parcial) **infoScreen**: VERSIÓN HW / FW APLICACIÓN reales (`general.hw_version/fw_version`);
   fechas desde los campos nuevos `sensors.cal.last_cal_date` / `next_service_date` (pipeline completo,
   los fija SD/app; "—" si vacíos). "Tiempo en servicio" quedó "—" (sin backend aún).
7. ✅ **Pulido umbral**: sensorEdit refresca valores/unidades en `LV_EVENT_SCREEN_LOADED` (patrón también
   aplicado en generalScreen: tiles Pantalla y Unidades muestran valores reales).
8. ✅ **Verificación firmware**: `idf.py fullclean + build` OK (2026-07-22, IDF 6.0.1). *Nota: el build dir
   viejo estaba configurado con otro venv de Python → exigió fullclean.* Falta **flashear en hardware**.
- ✅ **Fix sim**: `s_host` en `ui_netEthScreen.c` colisionaba con la macro winsock de `inaddr.h`
  (`#define s_host S_un.S_un_b.s_b2`) y rompía el build del sim → renombrado `s_hostname`.

4. ✅ **Certs AWS + BLE mesh + UI de SD** (portado del `ui_events.c` viejo):
   - **`ui_sd.{c,h}` (nuevo)**: overlay modal de mantenimiento por microSD (velo + tarjeta + barra +
     estado por etapa + botón "Aplicar y reiniciar"). `main.c` sd_monitor_task lo usa en vez de los
     widgets NULL de SquareLine (`sd_progress_bar`/`sd_restart_btn`, **crasheaban al insertar SD**).
     Todas las funciones null-safe. ⚠️ Archivo nuevo → exigió `idf.py reconfigure` (GLOB de CMake) y
     alta en el `.vcxproj` del sim.
   - **netBleScreen**: sección Mesh completa (switch, TTL 1-7, relay, net/app key, identidad UUID/unicast
     RO, botón provisioning). El provisioning sigue siendo el **stub del viejo** (marca `provisioned=true`;
     flujo BLE real TODO). Save aplica `transport_ble_set_mesh_enabled()`.
   - **netCloudScreen**: pills de presencia de certs (CA/cert/key) vía `cert_store_info()` (demo en sim),
     nota de la ruta `/sdcard/aws`, y botón "Borrar certificados" (`cert_store_erase_all()`).
   - El import en sí ya era automático en `sd_monitor_task` (AppConfig.json + `/sdcard/aws`); lo que
     faltaba era la UI.

5. ✅ **i18n es/en + zona horaria** (funcionales):
   - **`ui_i18n.{c,h}` (nuevo)**: `_t("literal es")` + tabla es→en (~140 entradas). Sin traducción →
     muestra español. ~154 literales envueltos en 17 archivos (script mecánico).
   - Getters de `ui.c` ahora **fresh** (destruir+recrear al abrir): el idioma aplica al navegar y los
     datos siempre están frescos. Seguro: al abrir X hacia adelante, X nunca está en la pila.
   - Fila **Idioma** (generalSimple): alterna es⇄en, guarda, reconstruye la pantalla en su lugar
     (`ui_nav_replace` + `lv_obj_delete_delayed`) y retraduce el main (títulos vía apply_config).
     *Limitación menor: la pantalla `general` (padre en la pila) queda en el idioma viejo hasta reabrirla.*
   - Fila **Zona horaria**: cicla 5 zonas (tabla IANA→POSIX en ui_cfg), guarda y aplica
     `setenv("TZ")+tzset()`; `main.c` también la aplica al boot (reloj del header usa localtime).
   - ⚠️ `_t()` NO sirve en inicializadores `static` (no es constante) → traducir en el punto de uso.
6. ✅ **Escalas/conversiones**:
   - **bar corregido**: era `kPa/10` (bug del FW original) → `/100`. Agregadas MPa (`/1000`) y m³/h (`×0.06`).
   - Conversiones centralizadas en `ui_cfg` (`press_to_disp/from_disp/flow_to_disp/press_fmt`).
   - **Fix**: sensorEdit/keypad/confirm mostraban umbrales crudos en kPa etiquetados con la unidad elegida;
     ahora el flujo de edición opera en unidad de display y convierte a kPa solo al persistir.
   - `FLOW_AXIS_FULLSCALE_LPM` (100 L/min) sigue siendo el fondo de escala a calibrar con el sensor real.

### Cierre final (misma sesión 2026-07-22)
- ✅ **BLE mesh RETIRADO de la UI** por decisión de producto (equipo hospitalario: la topología
  gateway/nodo rotativo no se consideró segura). Los campos `bt.mesh.*` siguen en el schema de
  AppConfig por compatibilidad; la app los ignora. `transport_ble_set_mesh_enabled` ya no se llama.
- ✅ **`sensors.flow_fullscale_lpm`** (default 100.0) por todo el pipeline + saneo en migrate:
  el eje de flujo del main es **calibrable sin recompilar** (JSON por SD o app BLE).
- ✅ **`metrics_store.{c,h}`** (nuevo, en components/storage, NVS namespace "metrics" separado de
  AppConfig): consumo del día (+fecha) y minutos de servicio. `ui_refresh_task` guarda cada 10 min
  y siembra el consumo al arrancar si la fecha coincide (sobrevive reinicios). infoScreen muestra
  "N días" reales (`appmetrics_service_min()`; stub del sim devuelve 184 días).
- ✅ **AWS verificado**: el stack MQTT del proyecto original está intacto y cableado
  (`net_core_init()` en main.c → transport_mqtt carga certs de cert_store, usa `cloud.broker_url`
  y publica en `<topic_base>/telemetry`). Requisitos: certs importados por SD, `cloud.enabled`,
  red activa. **Contratos para la app Flutter**: AppConfig.json vía BLE (config) + MQTT (telemetría).

### Sesión 2026-07-30 (previa, ya commiteada en `c03ba34`)
- ✅ **Autenticación por USUARIO + PIN con roles** (`ui_loginScreen.c` nuevo): se elige la cuenta y luego
  se valida el PIN de ESA cuenta. Modelo de usuarios en `storage.h` (`app_user_t`, jerarquía
  `APP_ROLE_NONE<TECH<ADMIN<FACTORY`, cuenta `factory` oculta) + API `ui_auth_*` en `ui_cfg.c`
  (login/logout/sesión con timeout 5 min/rol actual/usuario actual). Bloqueo tras 5 fallos (30 s).
  Seed por defecto: 1 admin "Administrador" PIN 1234 + fabricante "Fabricante" passphrase `axira-2026`.
- ✅ Arreglos de dashboard/menú/alarma + `ui_datetimeScreen.c` (fecha/hora dedicada).

### Sesión 2026-07-31 (ESTA sesión — NO commiteada aún; sin compilar en este entorno, sin HW)
1. ✅ **Gating por rol** (los roles ahora RESTRINGEN): helper `ui_auth_can(min)` + gating en las pantallas
   de red críticas — **Nube/MQTT y Bluetooth = ADMIN** (banner `ui_notice()` de "requiere administrador" +
   guardas en `save_cb` y en "Borrar certificados"). WiFi/Ethernet quedan en TÉCNICO (ya lo exige el login).
2. ✅ **Gestión de usuarios en el equipo** (NO está en los mockups; complementa el login): dos pantallas
   nuevas `ui_usersScreen.{c,h}` (lista con badges de rol; filas fuera de tu alcance salen con candado) y
   `ui_userEditScreen.{c,h}` (alta/edición: nombre, PIN, rol; botón Eliminar). API en `ui_cfg`:
   `ui_auth_user_add/update/remove`, `ui_auth_can_manage/can_edit_user/max_assignable`,
   `ui_auth_set_factory_pin`. **Jerarquía**: ADMIN solo toca TÉCNICOS; asignar/editar ADMIN = FABRICANTE.
   Persisten en NVS vía `appcfg_save`. **Acceso**: chip de usuario del header de "Ajustes generales"
   (solo visible para ADMIN+). ⚠️ *El "Cambiar PIN de fabricante" y todo lo FACTORY están gateados pero
   INERTES*: el login numérico no puede teclear la passphrase alfanumérica → falta un login de fabricante.
3. ✅ **Datos reales en Conectividad** (`ui_connectivityScreen.c` reescrito): SSID/IP/modo/broker/nombre BLE
   desde AppConfig; enlace en vivo (RSSI, IP obtenida, ETH up, BLE conectado, nube conectada) desde los
   managers bajo `#ifdef ESP_PLATFORM` (`wifi_mgr_get_netinfo`, `eth_mgr_is_up`, `transport_ble_is_connected`,
   `cloud_mgr_connected`); en el sim cae a la config. Se eliminaron los textos demo (Hospital-BIOMED, IPs,
   MAC, broker.axira). *Nota: "diag" resultó ser la pantalla de Audio de alarmas, que YA estaba con datos
   reales — la nota vieja estaba obsoleta.*
4. ✅ **Keypad con decimales** (`ui_keypadScreen.c`): la tecla inferior izquierda pasa a `.` cuando la unidad
   lo admite (bar/MPa/m³h; el borrado lo cubre el backspace) y el valor inicial usa 1 decimal. Un solo punto,
   arranca en "0." si está vacío.
5. ✅ **Pulido de idioma/tema al volver**: `ui_nav_back()` reaplica el modo visual (arregla el tema stale del
   padre); nuevo `ui_nav_swap()` + `lang_cb` recrea la pantalla `general` apilada al cambiar idioma, así
   "atrás" ya la muestra traducida.

⚠️ **Al abrir el proyecto**: `main/CMakeLists.txt` usa `GLOB_RECURSE` → los 2 archivos nuevos
(`ui_usersScreen.c`, `ui_userEditScreen.c`) exigen `idf.py reconfigure` (o fullclean) para detectarse.
En el **sim** (repo hermano ClaudeHMI-Sim) hay que sumarlos a mano al `.vcxproj`/`.filters`.

### Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW

Diagnóstico de un log de HW (`new 6.txt`): **watchdog de tarea sobre `taskLVGL`** al abrir la pantalla
"Pantalla" (`ui_open_general_simple_cb → ui_nav_load → lv_screen_load_anim`), backtrace idéntico a 5 s
(colgado, no lento). Contexto: RAM interna asfixiada (`free≈93 KB, mayor 43 KB, frag 55 %`) con
WiFi+ETH+AWS+HTTP activos, y **`AppConfig.json` re-parseándose ~2 Hz**. AWS conectó OK (el "off-by-one"
del cert CA es solo debug).

**CAUSA DIRECTA (2º log `Log.txt`, mismo build):** el hang es **dentro de `lv_anim_start` (lv_anim.c:129)**
llamado por `lv_screen_load_anim` — es decir, la **animación de transición** de pantalla, NO la construcción
(que sí terminaba: la pantalla se creaba y sus decenas de allocs pasaban). El splash animó bien en el boot
pero la navegación se colgó a los 84 s → problema en el subsistema de animación bajo carga/estado acumulado
(no es concurrencia: `bsp_display_lock`→`lvgl_port_lock`, el mismo mutex que la tarea LVGL). **FIX:** se
quitaron TODAS las animaciones de transición (`lv_screen_load_anim`→`lv_screen_load`) en `ui_nav.c` (5 sitios)
y en el splash (`ui.c`, con borrado manual del splash ya que se pierde el `auto_del`). Cambio instantáneo de
pantalla = sin `lv_anim_start` = sin la ruta del cuelgue. Los eventos `SCREEN_LOADED/UNLOADED` siguen
disparándose (el modo config BLE sigue enganchado). Es una **mitigación de alta confianza** (elimina la ruta
observada); si tras esto algún OTRO uso de animación (p.ej. transición de estado del tema al presionar) se
colgara, el subsistema de animación estaría roto de raíz y habría que desactivar transiciones globalmente.
Cambios adicionales:

1. ✅ **Quitado el churn de config (probable causa de la fragmentación):** `alarm_mgr_process()` hacía
   `appcfg_load()` (parseo profundo NVS+JSON, con malloc/free de buffers) **en cada muestra** → ahora usa
   `appcfg_cache_get()` (memcpy del snapshot en RAM). Igual en `alarm_mgr_press_mute()`. Para que la caché
   no quede vieja con config remota, `appcfg_set()/appcfg_patch()` (storage_extras) ahora llaman
   `appcfg_cache_reload()` al persistir (también corrige que el dashboard no reflejaba cambios remotos).
2. ✅ **Modo configuración por BLE (espejo del hermano `ClaudeHMI/ble_service.c`):** nuevo módulo
   `main/config_mode.{c,h}` con `config_mode_enter()/exit()`. Enter: `transport_mqtt_stop()` (nuevo, en
   `transport_mqtt.c/.h`: detiene+destruye el cliente MQTT y baja `s_is_connected`; la tarea `aws_pub` queda
   viva pero inactiva, con handle `s_pub_task` para no duplicarla) + `wifi_mgr_stop()` + delay 150 ms +
   `transport_ble_set_enabled(true)`+`start_adv()`. Exit: `stop_adv` + `wifi_mgr_start()` +
   `transport_mqtt_on_time_ready()` (re-arma MQTT, idempotente por el guard `if(s_client)`).
   **Cableado a `ui_bleAppScreen`** (Conectividad→Bluetooth→"Configurar por app"): entra en
   `LV_EVENT_SCREEN_LOADED`, sale en `LV_EVENT_SCREEN_UNLOADED` (guardado con `#ifdef ESP_PLATFORM` para el
   sim). Así, al ir a configurar por la app Flutter, se apagan AWS+WiFi y sube BLE — aunque BT estuviera
   deshabilitado en config. Ethernet se deja como está (el hermano tampoco lo toca).
   ⚠️ `config_mode.c` va **explícito** en `main/CMakeLists.txt` (el GLOB solo toma `main.c`+`ui/*.c`).
   ⚠️ Enter/exit corren en el hilo LVGL y bloquean ~200-500 ms (freeze breve de UI durante la transición).

3. ✅ **Dashboard alineado a los mockups `3a/3b/4b/4c`** (`ui_mainScreen.c`): la etiqueta central del eje
   ahora muestra el **rango seguro** de presión (`min-max seguro`, p.ej. `500-2000 seguro`) y el **umbral
   alto** de flujo (`<80% escala> alto`, p.ej. `1200 alto`) en vez de solo "seguro"/"alto"; se agregó el
   **subtítulo del consumo** (`· desde 00:00`) como en el mockup. *La marca del header se dejó como está: el
   código ya es data-driven (client/model desde AppConfig), mejor que el placeholder del mockup.*
   ⚠️ **Acum. mes** (`· acum. mes 284 m³`) NO se agregó: necesita un contador mensual persistente (estilo
   `metrics_store` del hermano); queda pendiente para no mostrar un dato inventado.

### Sesión 2026-08-01 — contrato BLE con la app + saga del watchdog (rama `feature/ble-app-contract`)

Lo previo (2026-07-31) fue commiteado por el usuario en `66014fa`. Sobre eso, en la rama
`feature/ble-app-contract`:

1. ✅ **Contrato BLE FPM ↔ app Sensvax** (commit `711b463`). Decisión del usuario: la app maneja los DOS
   equipos (MedGuard 12 y Axira/FPM), detecta el modelo al conectar y comparte TODO el esquema de config
   EXCEPTO `channels` (Axira = 2 sensores fijos: Presión con gas+unidad, Flujo con unidad; SIN calibración).
   - **`components/transport_ble/fpm_ble_config.{c,h}` (nuevo):** mapeo `AppConfig` ⇄ esquema JSON exacto
     de la app (`device_config.dart`). info/config_read/set_config + dedicadas wifi/cloud/channel/status +
     user_upsert/remove. Unidades y gas↔color; secretos redactados.
   - **`transport_ble.c` reescrito:** servicio GATT `9f3c1000` + 7 características `1001–1007` (config
     troceado por `mtu-3`), reemplaza el pipe `0xF00D`. Advertising con UUID de servicio + nombre en scan
     response. Hook `transport_ble_set_finish_cb`.
   - **`main.c`:** cablea "finish" → `ui_nav_show_root` → `config_mode_exit`.
   - **FALTA:** `set_clock`, cifrado/passkey (v1 Just Works sin cifrado). Sin compilar/probar en HW.

2. 🔴 **Watchdog en `taskLVGL` — CAUSA RAÍZ ENCONTRADA + FIX (commit `6e0b82c`).** Tras flashear, el HW
   seguía colgándose (ahora en el dashboard a ~43 s, no al navegar). Diagnóstico definitivo (3 logs): el PC
   está SIEMPRE exactamente en **`lv_realloc`** (`find_track_end`/flex) → el **asignador de LVGL atascado**,
   no el layout. LVGL usaba su **TLSF builtin con pool FIJO de 64 KB**; el `grow_dsc` del flex se realloca
   en cada refresco y ese pool chico se fragmenta de forma determinista hasta entrar en bucle. Los fixes
   previos (quitar animaciones, quitar el churn de `appcfg_load`) solo movieron dónde se golpeaba el
   asignador. **Fix:** `CONFIG_LV_USE_CLIB_MALLOC=y` (heap_caps de ESP, robusto, sin techo de 64 KB) en
   `sdkconfig` + `sdkconfig.defaults`. **REQUIERE REBUILD** — el usuario había flasheado el binario viejo.
   Pendiente validar en HW; si persiste, reducir el churn de relayout del dashboard.

Ver `HANDOFF.md` (reescrito 2026-08-01) para el estado de arranque, comandos y prioridades.

### Aún pendiente
- **Validar en HW el fix del watchdog** (rebuild con `6e0b82c` = CLIB malloc) — es lo #1 (ver
  `HANDOFF.md` §2). Ya corrió en HW pero seguía colgándose con el binario previo.
- **Login de FABRICANTE**: el pad del login es numérico; la passphrase del fabricante es alfanumérica.
  Falta un acceso oculto (icono candado) que abra un teclado QWERTY y valide contra `general.factory.pin`
  (fijar rol/usuario con `ui_auth_*`). Hasta entonces, las acciones FACTORY (cambiar PIN de fabricante,
  gestionar administradores) están en el código pero no se pueden alcanzar.
- **Compilar** (`idf.py reconfigure && idf.py build`) los cambios de esta sesión: no hay ESP-IDF en el
  entorno donde se editó. Revisar warnings/`-Werror` si aplica.
- **Validar en HW el fix del watchdog**: confirmar que al navegar a las pantallas de config (Pantalla, etc.)
  YA NO se cuelga `taskLVGL` (ahora sin animación de transición). Si aún se colgara en `lv_anim_start` por
  otra vía (transición de estado del tema), desactivar transiciones del tema.
- **Validar en HW el modo config BLE**: confirmar que al entrar a "Configurar por app" caen WiFi/AWS, sube
  el heap interno (ver logs `config_mode: heap interno [antes/después]`), la app Flutter empareja por BLE, y
  al salir se reconecta WiFi+MQTT.
- **Coherencia HMI ↔ app Flutter** (`ClaudeHMI/mobile`) y **"config mínima"** — DECISIÓN DEL USUARIO
  (2026-07-31): en la HMI queda **ergonomía local + red básica** (brillo/tema, idioma, fecha/hora, audio de
  alarmas, info/estado, y WiFi/Ethernet básico); el resto (nube/AWS, IoT, gestión avanzada) por la app. En
  **sensores, dejar la HMI como el mockup** (`5b sensorScreen-Editable`: ver/editar unidades y umbrales; sin
  calibración — los sensores traen su memoria de cal). Falta aplicar el recorte a las pantallas.
- **Consumo mensual del dashboard** (`· acum. mes N m³`): falta un contador mensual persistente (NVS, con
  rollover de mes) — estilo `metrics_store` del hermano. El diario ya funciona (RAM, reset a medianoche).
- **Calibración**: en FW no hay pantalla de calibración (bien, los sensores traen su memoria de cal). Solo
  queda una fecha en `ui_infoScreen.c` — quitar/renombrar si se confirma.
- Pulido menor restante: login de FABRICANTE (arriba); i18n de textos nuevos (solo ES); "Tiempo en servicio"
  del infoScreen; MAC BLE real en Conectividad.

---

## 10. Backup

Respaldo del HMI SquareLine anterior: `hmi_squareline_backup.tar.gz` (raíz de `ClaudeHMI-FW`, gitignored).
Contiene el `ui_events.c` (68KB) con la lógica original de wifi/eth/cloud/ble/certs para portar.
