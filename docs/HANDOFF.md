# Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)

Documento para **retomar en una sesión nueva**. Estado real, qué está commiteado, el
bug abierto (watchdog LVGL) y qué sigue. Complementa a `SESION_HMI.md` (historial) y
`HMI_MIGRATION.md`.

---

## Actualización 2026-09-24 — EEPROM, consumo, cero de presión (FW 1.5.5 → 1.5.22-dev)

Trabajo hecho mayormente con Codex; detalle versión a versión en
`firmware/CONSUMPTION_FIXES.md`. Nada de esto está commiteado aún.

- **EEPROM AT24C256**: driver pasó a transacciones byte a byte (dirección+STOP+lectura,
  100 kHz) con `vTaskDelay(1)` tras cada byte leído (sin pacing fallaba en 0x000C) y
  escritura verificada. Calibración se carga ANTES del touch con 4 lecturas idénticas.
  Nunca regenera CRC arbitrariamente: solo reparaciones estrechas documentadas.
- **Cero de presión v6**: migración única v5→v6 suma +0.415 kPa (unidad fpm-0001).
- **1.5.15 (Claude)**: el log mostró CRC guardado 2987 (v5) vs calculado 7AF0 (v6) →
  migración v6 interrumpida antes de escribir el CRC. Nueva recuperación: solo si
  deshacer la migración reproduce el CRC guardado, reescribe los 2 bytes de CRC.
- **Consumo**: integración trapezoidal del flujo, checkpoint NVS cada 10 min, marca
  PARCIAL; `consumption_*` en LAN/AWS. Tests de host en `firmware/tests/host/`.
- **1.5.16 WDT/WiFi**: Codex activó `CONFIG_ESP_TASK_WDT_PANIC=y`; la calibración PHY
  lenta (>5 s) causaba bootloop. Se MANTIENE el panic; solo durante `esp_wifi_start()`
  el timeout sube a 20 s y luego vuelve a 5 s (`wifi_mgr.c`).
- **1.5.17**: con 20 s igual se cuelga en `pwdet_tone_start` → cuelgue real. Se revirtió
  el pin a CPU0 de `sensors_acq`/`aws_pub` hecho por Codex (hipótesis). Si persiste:
  A/B flasheando HEAD `640b15c` en la misma placa.
- **1.5.17 VALIDADO en HW (log3)**: radio arranca en 163 ms, splash fluido, EEPROM OK,
  AWS OK. CPU1 ya no saturado (LVGL 53 %, IDLE1 35 %).
- **1.5.18 auditoría** (detalle en `firmware/CONSUMPTION_FIXES.md`): alarma ya no evalúa
  datos inválidos como 0 kPa (evita ALERT falso por fallo de sensor); cJSON en PSRAM;
  caché AppConfig con mutex y sin copias de 2 KB en stack; límites AWS vivos; heap
  tracing OFF; CPU 240 MHz; diagnóstico `Acquisition stall/gap`. Pendiente probar en HW.
- **1.5.18 validado (log4) y commiteado `a25efbf`.**
- **1.5.19**: FreeRTOS 1000 Hz (pacing EEPROM fijado en 10 ms), backoff WiFi 1→30 s,
  limpieza de código muerto BLE. Pendiente probar en HW. Siguen: -O2, PSRAM 80 MHz,
  particiones/NVS + core dump.
- **1.5.19 validado (log5)**: EEPROM OK a 1000 Hz, ciclo de adquisición 62→38 ms.
- **1.5.20**: pantalla+splash antes de la verificación EEPROM (touch al final),
  barra animada 3.2 s, compilación -O2. Pendiente probar. Luego PSRAM 80 MHz.
- **1.5.20 validado (log6)**: pantalla a 1.5 s, heap interno largest 27.6 KB, LVGL 42 %.
  Fix real WiFi: SSID 32 / clave 64 caracteres se recortaban.
- **1.5.21 validado (log7)**: stack LVGL 9216 (2832 B libres), PSRAM 80 MHz OK.
- **1.5.22 validado (log8, erase-flash + SD)**: LVGL 42→4 %, sin stall I2C, NVS 24→84 KB + partición coredump, WiFi sin
  escrituras NVS, UI redibuja solo cambios, diagnóstico `i2c_guard: slow`.
  **Flasheo especial UNA vez** (cambia la tabla de particiones, conserva la NVS):
  ```
  idf.py -B build-fixes build
  python -m esptool --chip esp32s3 -p COM3 erase-region 0xF000 0xF000
  python -m esptool --chip esp32s3 -p COM3 erase-region 0x620000 0x20000
  idf.py -B build-fixes -p COM3 flash monitor
  ```
  (0xF000-0x1DFFF = nueva cola de la NVS, antes otadata+hueco; 0x620000 = coredump.)
  Volver a una versión ≤1.5.21 requiere el partitions.csv viejo.
- **Abierto**: bus I2C 1 se detiene ~0.7 s durante el handshake TLS de AWS (no llega a
  alarma). Cero de presión deriva con temperatura (-0.3 kPa en frío).
- **Abierto**: hueco de ~2 s en adquisición durante el handshake TLS con AWS → alarma
  STALE (faults=0x2) al arrancar. Heap interno min 28 KB tras AWS (vigilar).
- **Histórico** (resuelto con 1.5.17): causa raíz de la calibración PHY de 2–5+ s (sospecha: caída de 3.3 V
  en TX de calibración o temperatura ~45 °C). Mirar `Radio start returned ... after N ms`.
- **Validar en HW**: reboot muestra `stored=computed=7AF0`, EEPROM=VERIFIED, sin alarma
  de calibración; cero de presión ±0.055 kPa en atmósfera; pruebas de consumo.

## Actualización 2026-09-21 — microSD, AWS por SD, BT-off y splash (FW 1.5.4-dev)

- **microSD (overlay `ui_sd` + `sd_monitor_task` en `main.c`)**: (1) botón **"Cerrar"**
  SIEMPRE visible (antes solo salía "Aplicar y reiniciar" con cambios y la ventana
  quedaba atrapada); (2) el **fallo de montaje** ahora se MUESTRA (`ui_sd_error`, rojo)
  en vez de un `continue` mudo; (3) FIX del **storm `0x105 no available sd host
  controller`**: se DESMONTA apenas termina la lectura (todo ya está en NVS) para
  liberar el host SDMMC + bandera `s_sd_flow_active` y drenado del semáforo bloquean
  los rebotes de la interrupción CD. (4) FIX de **layout**: la tarjeta usaba la altura
  default (~100 px) de `lv_obj_create` y cortaba estado/botones → `LV_SIZE_CONTENT` +
  botones en una fila con altura de contenido.
- **AWS por microSD (el bug real)**: `CONFIG_FATFS_LFN_NONE=y` (nombres largos OFF) →
  `AmazonRootCA1.pem`/`certificate.pem.crt`/`private.pem.key` no se podían abrir (solo
  8.3), y `cert_store_import_from_sd` **devolvía ESP_OK aunque no leyera nada** →
  "Certificados procesados" falso y AWS sin certs. FIX: **FATFS LFN activado**
  (`CONFIG_FATFS_LFN_HEAP`, `MAX_LFN=255`) + el importador ahora **exige los 3** y falla
  honesto con log de cuál faltó. **Verificado en HW: AWS IoT conecta** (certs 1187/1220/
  1675 B escritos, "Conectado exitosamente a AWS IoT Core").
- **BT-off**: el botón **"Apagar Bluetooth"** que el usuario pulsa está en
  `ui_bleAppScreen.c` (pantalla "Configurar por app"/QR), NO en `ui_netBleScreen.c`.
  `bleapp_off_cb` ahora **persiste `bt.enabled=false` y reinicia** (igual que MedGuard);
  antes solo volvía al dashboard.
- **Splash (`ui_splashScreen.c`)**: la línea de versión se **sobreponía** con los chips
  (usaba `IGNORE_LAYOUT`+align al fondo) → al flujo normal. Datos **reales**: quitado el
  falso `FW app v1.8.0`; ahora `Modelo: <general.model> · S/N: <general.serial> · FW:
  v<esp_app_desc>`. NOTA: los 3 chips (MCU/Bus/Sensores) siguen **cosméticos** (no es un
  autotest real) — pendiente si se quiere cablear.

---

## Actualización 2026-08-21 — EEPROM persiste + recalibración FS7 estable

- **🎉 La EEPROM AT24C256 YA ESCRIBE** (`Calibración por defecto escrita en EEPROM`, sin el
  `INVALID_RESPONSE`). Lo resolvió **consolidar las pull-ups del bus 1** (los dev-boards del ADS y
  la AT24C256 traían sus propias 10k en paralelo con las 4.7k de control → efectivo impredecible;
  se dejaron en un solo punto) **+** el endurecimiento del driver (50 kHz, chunks 32 B, reintento
  4×). También el touch inicializó OK y el scan da los 7 dispositivos. **La calibración ya persiste.**
- **Recalibración FS7 (datos ESTABLES, ~180 muestras 8–37 slm):** cero reconstruido **Ucta=3.466 V**
  (muy estable, n=21). Ajuste ley de potencia `flujo[slm]=209·(Ucta−3.466)^1.17` → **`u0=3.466,
  k=1, n=0.857, flow_scale=209`**, **RMS ~2.6 slm**. Falta caracterizar >37 slm.
- **Chequeo de versión en la carga de EEPROM:** `sensor_cfg_load_or_init` ahora valida
  `tmp.version == SENS_CFG_VERSION` (antes solo magic+size+CRC). Se subió **`SENS_CFG_VERSION` 4→5**
  para que la cal vieja guardada en EEPROM se rechace y se reescriban los nuevos defaults. **Regla:
  subir SENS_CFG_VERSION cada vez que cambien los defaults de calibración.**

---

## Actualización 2026-08-20 — BMP280 (ref. atmosférica), cero FS7, debug en dashboard

- **Cero del FS7 a 3.60 V:** medido el analog output del FS7 a flujo cero en **3.595–3.605 V**
  (más alto de lo programado). `fs7_u0` = **3.60** en `sensor_cfg_set_defaults()`. (El auto-cero
  `flow_tare` sigue siendo la vía robusta ante deriva; captura el U reconstruido real.)
- **Barómetro BMP280/BME280 @0x77 (nuevo driver `components/drivers/bmp280.c`):** referencia de
  presión atmosférica para el **cero de la presión de línea**. Chip ID 0x58/0x60, calib de fábrica,
  normal mode osr_p×16 + IIR×16, compensación Bosch (double). Init en `main.c` (bus 1, 0x77). En
  `sensors_acq_task` se lee cada ciclo; **el modo gauge ahora resta la atmosférica EN VIVO del
  BMP280** (`p_abs - atm_baro`) y cae a la tara (opción B) si no hay BMP. Supersede la recomendación
  del BMP581 del handoff previo.
- **Diagnóstico TEMPORAL en el dashboard** (se quita en producción): línea pequeña en cada tarjeta
  vía `sensors_runtime_get_debug(atm, sfm, fs7)`. Presión → `atm XX.XX <u> (BMP)`. Flujo →
  `SFM XX.X  dif ±X.X <u>` (referencia SFM3300 y su diferencia con el FS7). **Ambos respetan la
  unidad configurada** (presión/flujo) vía `pressure_to_disp`/`flow_to_disp`. Campo `dbg` en
  `metric_card_t` (`ui_mainScreen.c`). **Para producción: borrar el `dbg` y `get_debug`.**
- **Presión gauge por defecto:** `pressure_mode` default = **GAUGE** (antes ABS). Con BMP presente
  la presión de línea resta la atmosférica en vivo (mostraba la absoluta ~12 psi). Puede quedar un
  pequeño offset (~-0.2 kPa) por diferencia MS5803↔BMP; despreciable.
- **FS7 calibrado (2026-08-20) vs SFM3300 (0–50 slm):** el cero RECONSTRUIDO (Ucta) es **3.49 V**
  (no los 3.60 del multímetro; desajuste del divisor → se calibra en el dominio reconstruido). La
  respuesta es CONVEXA (el CTA satura): `flujo[slm] ≈ 271·(Ucta−3.49)^1.29`. En el modelo:
  **`u0=3.49, k=1, n=0.773, flow_scale=271`**. Datos ruidosos (flujo rampando) → refinar con
  caudales ESTABLES; la auto-tara de flujo corrige la deriva del cero.
- **Touch: reintento en el init** (`ft5x06.c`): un glitch del bus 1 al arranque dejaba el equipo
  SIN touch toda la sesión (visto en log: `FT5x06 init failed 0x108`). Ahora reintenta 3×.
- **Gauge validado:** con el BME280 la presión de línea lee ~0 kPa en reposo (resta atmosférica OK).

---

## Actualización 2026-08-18 (tarde) — SFM leyendo, robustez I²C, calibración FS7 · **v1.4.0**

Sesión de estabilización tras cablear el SFM3300 y poner los pull-ups. **Estado: firmware
estable** — SFM leyendo, sin crashes, sin cuelgues de bus. Se sube `PROJECT_VER` a **1.4.0**
(`CMakeLists.txt`, no existía antes: la versión salía del git hash).

**Versión en pantalla/app:** la HMI (Info → "FW APLICACIÓN") y el JSON de info BLE mostraban
`cfg->general.fw_version` (config, default "1.0.0"), no la del build. Ahora ambos leen
`esp_app_get_description()->version` (= `PROJECT_VER`), fuente única. Se agregó `app_update` a
`REQUIRES` de `main` y `transport_ble` para resolver `esp_app_desc.h`.

**Resuelto:**
- **SFM3300 no leía → FIX CRC.** El SFM3300-D usa el chip Sensirion **SF05**, cuyo CRC-8
  arranca en **init 0x00** (no 0xFF como SHT3x/SFM3019). Se copió la convención equivocada →
  cada lectura fallaba el CRC. Confirmado con el código de referencia `sf05.c`. Ahora lee 0–71
  slm limpio. Además el init reintenta el START 5× (NACKeaba el 1º tras power-on) y hay
  auto-recuperación de medición continua si falla sostenido (`sfm3300_start_measurement()`).
- **Presión "a 0" + falsa alarma.** `alarm_mgr` disparaba ALERT ante **cualquier** fault, y un
  glitch del ADS (bus compartido) prendía `SENSOR_FAULT_MODULE_I2C`. Fix en `sensors_acq_task`:
  reintento + **antirrebóte** (`SENSORS_FAULT_DEBOUNCE=3`, ~300 ms) + **retención del último
  valor** (no publica 0 ante un miss transitorio).
- **Reboot por el touch (crash).** `esp_lvgl_port` hacía `ESP_ERROR_CHECK` sobre la lectura I²C
  del FT5x06 → `abort()`/reboot ante un solo glitch del bus 1. Se reemplazó `lvgl_port_add_touch`
  por un `lv_indev` propio en `ft5x06.c` con read callback **tolerante** (error I²C = "sin toque").
- **Recuperación de bus I²C 1.** Si MS5803+ADS fallan juntos de forma sostenida (bus colgado),
  `sensors_acq_task` llama `i2c_master_bus_reset()` cada ~3 s. `main.c` pasa el handle vía
  `sensors_runtime_set_bus()`. Timeouts de lectura MS5803/ADS bajados 100→50 ms.

**Calibración FS7 (1er paso, empírica vs patrón SFM3300):** el default `fs7_u0=3.6` estaba por
encima del voltaje CTA a flujo cero (medido ~3.46–3.52 V) → `diff<0` → flujo siempre 0. Nuevo
modelo en `sensor_cfg_set_defaults()`: **`u0=3.522`, `n=1.0`, `k=1.0`, `flow_scale=222`**
(≈lineal en 0–60 slm; satura >60).

**Auto-cero (tara) + calibración de flujo en vivo (BLE):** el cero del FS7 **deriva** (3.46→3.52
en warm-up), así que un `u0` fijo es frágil. Nuevas funciones en `sensors_runtime`:
- `sensors_runtime_tare_flow()` — con la línea SIN flujo, fija el Ucta actual como `u0` (auto-cero).
  Captura `s_last_ucta` en `sensor_flow_from_voltage`. Disparo BLE: **`{"flow_tare": true}`**.
- `sensors_runtime_cal_flow_point(ref_slm)` — con un caudal CONOCIDO estable (del SFM3300) ajusta
  `flow_scale` para coincidir. Disparo BLE: **`{"flow_cal_slm": <caudal>}`**. Flujo de campo:
  tare a cero → cal_slm a un caudal estable (calibración de 2 puntos sin reflashear).

⚠️ **La EEPROM AT24C256 no persiste — NO es el WP** (el WP está a GND = escritura habilitada, ver
datasheet). Es **integridad de señal del bus 1**: las lecturas (cortas) siempre van, pero el write
del bloque de cal (~54 B, la transacción más larga) NACKea en el bus marginal (6 dispositivos, 4.7k,
ruido de 5 V). Firmware endurecido en `at24c256.c` (**2026-08-20**): EEPROM a **50 kHz**, write en
chunks de **32 B**, **reintento 4×**. Si persiste → HW: subir pull-ups del bus 1 a **2.2–3.3k**.
Mientras no persista, tara/cal viven en RAM y la cal base corre desde los defaults del código.

**UI:** dashboard con **decimales configurables** (0 o 1) en Config→Sensores; nuevo campo
`sensors.decimals` (storage + JSON + `ui_cfg_decimals/set_decimals`). Añadida unidad **slm**
(=slpm, numéricamente = L/min) al selector de flujo.

**Pendientes:** (1) EEPROM AT24C256 (pin WP del módulo) para persistir calibración en campo;
(2) calibración fina definitiva con banco de caudales estables; (3) exponer `decimals`/`slm` y los
comandos `flow_tare`/`flow_cal_slm` en la app Flutter.

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

## 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41`

Reportados 4 problemas en HW. Estado:

- **#1 Buzzer atascado / deja de registrar — RESUELTO y validado.**
  Causa raíz: el touch FT5x06 comparte el **bus I2C 1** con los sensores y su lectura
  (`esp_lcd_panel_io_i2c`) usa **timeout -1 (infinito)**. Al colgarse el bus, esa lectura bloquea la
  tarea LVGL con el **mutex de display tomado para siempre** → UI congelada + `alarm_mgr` en ALERT
  permanente (buzzer). Fix en `ft5x06.c`: **INT-gating (GPIO39)** → solo se lee el touch con dedo
  presente; en reposo no se toca el bus (elimina el sondeo ~30 Hz que colgaba el bus). Además se
  aparta si `sensors_runtime_bus1_hung()`. En `sensors_runtime.c`: recuperación del bus cada ~500 ms
  (antes ~3 s) + flag `bus1_hung`.
  **Gotcha:** se probó `i2c_master_probe` como guarda con timeout finito → **NO usar**: concurrente
  con las transacciones de la tarea de sensores corrompe el driver → **panic StoreProhibited en el
  ISR de recepción** (`i2c_master.c:767`). Se descartó.
- **#2 WiFi no aplica hasta reiniciar — RESUELTO y validado.**
  La UI usaba `wifi_mgr_update_credentials` (config STA incompleta, sin actualizar `s_cfg_cur`). Ahora
  `ui_netWifiScreen.c` usa la **misma ruta diferida del arranque** (`wifi_mgr_apply_from_cache` +
  `wifi_mgr_start`). Validado: cambio de red en caliente sin reiniciar.
- **#2 Alarma no se silencia al desactivar límites — DIAGNOSTICADO, pendiente decidir fix.**
  NO es la alarma de presión (el gateo de `pressure_min/max_enabled` en `alarm_mgr.c` es correcto y en
  caché vivo). Es el **path de fallo de sensor**: `ADS1115` (flujo) con `rdErr` sostenido →
  `SENSOR_FAULT_MODULE_I2C` → `alarm_mgr.c:112` fuerza **ALERT irrevocable** ignorando los flags.
  Decisión pendiente: tratar el fallo-de-comunicaciones como alarma **silenciable/distinta**.
- **ADS1115 (flujo) `rdErr` sostenido — pendiente.** Intermitente (recupera a veces), MS5803 del mismo
  bus OK → no es bus/pull-ups. Sospecha: **timing de lectura** (se lee antes de conversión lista /
  timeout corto) en `ads1115_read`, no el driver base. Opción provisional: deshabilitar canal de flujo.
- **#3 (ojito de contraseña WiFi) y #4 (publish inmediato de alarma a AWS) — NO empezados.**

---

*Actualizado 2026-09-16. Rama `main` @ `a353c41`. Sesión previa: 2026-08-01 (`feature/ble-app-contract`
@ `6e0b82c`). Ver `SESION_HMI.md` §9 para el historial detallado.*

## Sesion 2026-09-16 (#3) — Codex/Claude
- #1 ADS1115: retry+backoff, timeout real, ads1115_recover() (re-add device) disparado en sensors_runtime cuando el ADS cae solo con bus sano.
- #2 alarma: SENSOR_FAULT -> WARNING silenciable (no ALERT permanente).
- #4 buzzer: alarm_mgr_set_audio_inhibit_noncritical() activo en login, limpiado en dashboard (ALERT critico sigue sonando).
- #5 AWS: callback de cambio de estado -> transport_mqtt_publish_now() (ulTaskNotifyTake) publica al instante.
- Pendiente: #3/#3.1/#3.2 tarjeta WiFi (UI LVGL). Validado compilado+flasheado en HW.

## Sesion 2026-09-16 (tarde) — UI/memoria/notificaciones
- Memoria: SPIRAM_USE_MALLOC (RAM interna liberada; fin de crash mbedtls/wifi/WDT). LVGL refr 33->20ms.
- UI: refresco vivo tarjeta Conectividad; alturas de cards de formulario; alarma dashboard respeta flags min/max/flujo; decimales por unidad (bar/MPa/m3h); heartbeat por adquisicion; icono nube = fa-cloud-upload-alt (F382, como MedGuard); iconos red a MD (proporcion).
- Feedback de presion (push+resalte) en botones/tiles/menu/badges.
- Fix: tarjeta Sensores se refresca tras editar limite por teclado (ui_sensorEditScreen_refresh).
- Notificaciones: firmware publica transiciones de alarma por canal a <base>/.../alarms (medguard.alarm.v1) -> alarm-router (email+push).
- Pendiente: validacion en HW de estos ultimos cambios.

## Cierre sesion 2026-09-16 (tarde)
- HMI: refresco de tarjetas tras editar por teclado en Sensores (limites) Y Audio de alarmas (reanunciar/silencio) via ui_*_refresh() + pop_to.
- Icono nube = fa-cloud-upload-alt (F382, mg_font_18 regenerado con el glifo) al subir datos, como MedGuard.
- Notificaciones: firmware publica transiciones por canal a topic /alarms (medguard.alarm.v1).
- AWS (repo MedGuard): alarm-router limpia tokens FCM muertos; app registra token multi-equipo; multi-tenant core (DEC-022, aislamiento por org + binding admin) desplegado.
- Commits FPM: 3ba6ad1 (UI/mem/notif), b4d2a32 (fix audio). PENDIENTE: validacion en HW de estos ultimos cambios.
- Proximos pasos (plan 17f83c2): terminar HMI FPM, UX Flutter contra la API ya aislada, web multi-tenant, luego G5 piloto. Provisioning+facturacion al final.
