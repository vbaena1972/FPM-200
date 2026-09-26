# QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)

**Fecha de inicio:** 2026-09-25 · **Objetivo:** batería de casos con evidencia para **establecer el
primer baseline validado del FPM-200** (hoy no tiene ninguno). · **Equipo:** `fpm-0001` (identidad
nativa, stack AWS `medguard-mvp`). · **Firmware:** 1.5.22-dev (commit `2d2dd33` + docs), ESP-IDF 6.0.2,
`esp32s3`, flash 16 MB, PSRAM 2 MB @80 MHz. · **Referencia de formato:** QA MedGuard 1.5.3
(`MedGuard-12IoT/docs/entrega/historial/QA_1.5.3_2026-09-15.md`).

Estados: **PASS** · **FAIL** · **ACEPT** (limitación conocida/aceptada) · **PEND** · **N/A**.
Verificación: visual (HMI) + log de monitor (`idf.py -p COM3 monitor`) + telemetría
(MQTT/DynamoDB/Athena) + app (LAN/BLE).

**Precondición:** equipo **encendido ≥ 20 h** antes de la sesión (temperatura estable; la prueba
de estabilidad K1 cuenta desde el último arranque). No reiniciar hasta capturar el log de K1.

---

## Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo)
- **EEPROM**: transacciones byte a byte paceadas (10 ms), carga de calibración antes del touch con 4
  lecturas idénticas, reparación de la migración v5→v6 interrumpida, cero de presión v6 (+0.415 kPa).
- **Consumo**: integración trapezoidal con timestamps reales, checkpoint NVS cada 10 min, marca
  PARCIAL, campos `consumption_*` en LAN/AWS.
- **WiFi**: fin del bootloop de calibración PHY (sensores fuera del core 0), backoff de reconexión
  1→30 s, `WIFI_STORAGE_RAM`, fix SSID de 32 / clave de 64 caracteres.
- **Rendimiento**: 240 MHz, FreeRTOS 1000 Hz, `-O2`, PSRAM 80 MHz, UI sin redibujados (LVGL 42→4 %).
- **Seguridad de alarmas**: datos inválidos ya no se evalúan como 0 kPa (evitaba ALERT falso).
- **Arranque**: pantalla a 1.5 s, splash animado 3.2 s.
- **Particiones**: NVS 84 KB + coredump 128 KB (requirió erase-flash + importación por microSD).

---

## Parte 0 — Pendientes previos (se ejecutan primero)

| ID | Caso | Procedimiento | Esperado | Resultado | Evidencia |
|----|------|---------------|----------|-----------|-----------|
| P1 | Cero de presión en caliente | Tras ≥ 20 h, entrada abierta a atmósfera, 10 min de log `P=… T=…` | Registrar P0(T). Decidir: recalibrar offset en caliente y/o zona muerta de display alrededor de 0 | **PASS** | `longFPM.log`: desde la hora 4 (T≈47 °C) cero = 0.00 ± 0.035 kPa hasta la hora 18. Sin recalibración; el −0.45 era arranque en frío (37 °C) |
| P2 | Consumo contra patrón | SFM3300 conectado en serie; caudal fijo (p. ej. 10 L/min) durante un tiempo medido (≥ 10 min) | `consumption_m3` ≈ caudal × tiempo (anotar error %); `consumption_partial=false` | **PASS (integrador + modelo) / PEND (< 12 slm)** | Integrador en HW: 915.5 L vs 914.8 L integrados del log con la curva vieja (0.1 %). Curva v7: volumen -0.0 % vs SFM3300 (800 L). Falta sesion 2-12 slm (ventilador no llega; usar estrangulamiento, bomba de acuario o aire/O2 con regulador) |
| P3 | Estabilidad 20 h (soak) | Equipo encendido sin reinicios desde la noche anterior | Sin `rst:` ni `task_wdt`; heap interno `largest` estable (≥ 15 KB); `StkHW` de tareas sin caer; WiFi/AWS conectados; touch y buzzer responden | **PASS** | `longFPM.log`: 18.8 h sin reinicios ni errores; heap interno 42.4 KB / largest 19.4 KB planos; taskLVGL 5 %; 2261 telemetrías; 0 desconexiones WiFi |
| P4 | Core dump funcional | Provocar un crash controlado (build de prueba o comando), reiniciar | Log al arrancar: `Core dump from previous crash … reason: …`; `idf.py coredump-info` muestra backtrace | **PASS (captura)** / ABIERTO (causa) | Volcado real 30 112 B: `InterruptWDTTimoutCPU0`, contexto de interrupcion, handle en la zona de la tarea del driver WiFi (core 0). Motivo original perdido: el IWDT salto de nuevo al escribir el volcado (PC en `esp_core_dump_do_write_elf_pass`). Volcado ya borrado. Una sola vez, antes del soak (hipotesis: calibracion PHY completa tras erase-flash). Vigilar: si se repite, guardar con `esptool read-flash 0x620000 0x20000` y `coredump-info --gdb-timeout-sec 20` |
| P5 | Pruebas de host | `./tests/host/run.ps1` (MSVC) | test_alarm, test_eeprom, test_flow_integrator: todos PASS | **PASS** | `tests/host/run.ps1` (MSVC, 2026-09-25): test_eeprom, test_alarm (+ caso nuevo de confirmación de flujo), test_flow_integrator |
| P6 | SFM3300 (referencia, bus 2) | Conectar con 5 V y pull-ups correctas | Log `SFMref=… slm` en vez de `SFM=noInit`; lecturas estables | **PASS** | SFM3300 leyendo en ambos logs. Fix 1.5.23: `00 00 00` pasaba el CRC y decodificaba −273 slm (62 veces) → rechazado fuera de ±250 slm |
| P7 | Mantenimiento menor | (a) migrar `esp_lcd_touch_get_coordinates` → `esp_lcd_touch_get_data`; (b) aviso CMake `ui_bind`↔`main`; (c) quitar `read_slot_json` sin uso; (d) compilar Simulador VS con la UI actual | Compila sin esos avisos; simulador abre y navega | **PASS** (a,c) / ACEPT (b) / PEND (d) | (a) touch migrado a `esp_lcd_touch_get_data`; (b) aviso CMake = dependencia circular ui_bind↔network_core, refactor futuro (igual que MedGuard); (c) `read_slot_json` eliminado; (d) simulador con LVGL 9.5 y APIs verificadas, falta compilar en VS |

---

## Plan para completar (2026-09-26) — firmware 1.5.24-dev flasheado (build estándar)

**Hoy, con flujo > 30 L/min y ajuste de límites (sin fuente de presión):**
- F4: encender el ventilador de golpe → `flow_warn`; activar `flow_high` con límite ~20 L/min.
- F1: mínimo de presión 5 kPa → WARNING con línea abierta; 20 kPa → ALERT (0 < 20−15).
- F2/F3: silenciar la WARNING y escalar a ALERT (debe romper el silencio).
- F5: la transición llega a email/push. F6: cambiar un límite en HMI y verlo en la telemetría.
- C4/D3/D4/E4: desconectar el módulo de sensores unos segundos → alarma técnica amarilla
  (nunca ALERT), consumo PARCIAL sin sumar; reconectar → `Sensors recovered`.
- Sin flujo: confirmar en el log que no hay alarmas falsas de fuga (mediana 5 + confirmación).
- B3, C5, I1, I2, I4, I5, J3: recorrido de HMI. G2: apagar el router 1 min. E3: pasar medianoche
  con el log capturando. A3: ver `firmware:"1.5.24-dev"` en AWS.

**Con la app/portal:** H1–H3, E5, G5, G6.
**Sin equipo (propuesto ACEPT con pendiente):** C2 (patrón de presión), C3/P2 < 12 L/min.

---

## Matriz de casos

### A. Arranque / versión / estabilidad
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| A1 | Banner de versión `App version: 1.5.22-dev` | **PASS** | log 1.5.23: `App version: 1.5.23-dev` |
| A2 | Pantalla encendida ≤ 2 s; splash con barra animada → dashboard | **PASS** | backlight a 1.32 s, UI antes de la verificación EEPROM; splash animado (1.5.20, confirmado por el usuario) |
| A3 | Versión en telemetría (`firmware`) | **FAIL → corregido 1.5.24** | AWS publicaba `firmware:"1.0.0"` (campo de config viejo). Ahora usa la versión del build como LAN/BLE/mDNS. Verificar en AWS |
| A4 | Estabilidad ≥ 20 h sin reinicio (= P3) | **ACEPT (18.8 h)** | `longFPM.log`: 18.8 h continuas sin reinicio (criterio 20 h; la corrida actual puede completarlo) |
| A5 | Tabla de particiones nueva (nvs 0x15000, coredump @0x620000) | **PASS** | boot: nvs 0x9000/0x15000, coredump 0x620000/0x20000 |

### B. Calibración EEPROM
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| B1 | 4 lecturas `stored=computed=7AF0`, `calibration loaded` antes del touch | **PASS** | 1.5.23: 4 lecturas stored=computed=E9E8 (= registro v7 con curva nueva), calibration loaded, EEPROM=VERIFIED |
| B2 | `EEPROM=VERIFIED`, sin alarma de calibración (faults sin 0x10) | **PASS** | `EEPROM=VERIFIED`; alarmas del soak con `faults=0x0` |
| B3 | Guardar calibración desde HMI → reinicio → CRC nuevo estable | PEND | |

### C. Medición presión / flujo
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| C1 | Presión gauge en atmósfera (en caliente) dentro de ±0.2 kPa tras P1 | **PASS** | horas 4–18 del soak: 0.00 ± 0.035 kPa (T≈47 °C) |
| C2 | Presión con línea presurizada vs manómetro de referencia | **ACEPT (sin patrón)** | No hay manómetro/fuente de presión de referencia. MS5803-14BA calibrado de fábrica (PROM). Pendiente para cuando haya patrón |
| C3 | Flujo FS7 vs SFM3300 en 3 puntos (p. ej. 5 / 15 / 30 L/min) | **PASS 15–40 / PEND < 12** | curva v7 ±2 % en 15–40 slm vs SFM3300; bajo 12 slm sin datos (ver P2) |
| C4 | Flujo cero estable (Ucta ≤ u0 → 0 L/min), sin "GLITCH" repetidos | PEND | |
| C5 | Unidades (psi / kPa / bar) y decimales correctos en dashboard | PEND | |

### D. Adquisición / temporización
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| D1 | `Acquisition work` ≈ 38 ms, `overruns` 0–1 por 30 s | **PASS** | work 38–44 ms; 3 overruns en 18.8 h |
| D2 | Sin `Acquisition stall`/`gap` ni `i2c_guard: slow` durante conexión AWS | **ACEPT** | 1 bloqueo del bus I2C ~0.7 s por conexion AWS (handshake TLS: `xfer=372 ms` err OK); < 1 s, sin alarma. Causa no identificada (MPI por interrupcion ya activo) |
| D3 | Pérdida controlada del sensor (desconectar módulo) → alarma técnica amarilla silenciable, **nunca** ALERT | PEND | |
| D4 | Reconexión del sensor → `Sensors recovered`, alarma se limpia | PEND | |

### E. Consumo
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| E1 | Exactitud contra patrón (= P2) | **PASS (= P2)** | volumen −0.0 % vs SFM3300 (800 L) con curva v7; integrador verificado en HW (0.1 %) |
| E2 | Reinicio: el total del día se conserva y queda PARCIAL | **PASS** | tras reinicio: `Consumo: 0.9155 m3 ... partial=1 dated=1` (total del día conservado y marcado PARCIAL) |
| E3 | Cambio de día (00:00): total nuevo, intervalo cruzado marcado perdido | PEND | |
| E4 | Hueco > 1 s (sensor desconectado) no suma y marca PARCIAL | PEND | |
| E5 | `consumption_m3`, `consumption_missing_ms`, `consumption_partial` en LAN y AWS | PEND | |

### F. Alarmas
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| F1 | Presión baja/alta (WARNING) y fuera de ±15 kPa (ALERT) | PEND | |
| F2 | Silenciar: WARNING respeta `reannounce_minutes`, ALERT `max_silence_minutes` | PEND | |
| F3 | Escalada WARNING→ALERT rompe el silencio | PEND | |
| F4 | Delta de flujo (fuga) y flujo alto | PEND | |
| F5 | Transición publicada a `.../fpm-0001/alarms` → alarm-router (email/push) | PEND | |
| F6 | Cambiar límites en HMI → telemetría AWS usa los límites nuevos (sin reiniciar) | PEND | |

### G. Red / AWS
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| G1 | WiFi conecta (radio start < 1 s, sin `task_wdt`) | **PASS** | radio start 47–111 ms en todos los arranques; sin task_wdt |
| G2 | Router apagado → reintentos 1→2→4…30 s; al volver, reconecta | PEND | |
| G3 | AWS IoT MQTTS conecta con `client_id=fpm-0001`, shadow delta suscrito | **PASS** | `Conectado exitosamente a AWS IoT Core`, shadow delta suscrito (fpm-0001) |
| G4 | Telemetría v2 cada 30 s con `channels[]` + `stats` | **PASS** | 2261 telemetrías en 18.8 h (= 1 cada 30 s) |
| G5 | Ingesta DynamoDB `DEVICE#fpm-0001` y Athena (stats) | PEND | |
| G6 | App/portal muestran el FPM por AWS | PEND | |

### H. LAN / BLE / microSD
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| H1 | mDNS `fpm-0001.local`; app conecta por LAN (Bearer token) | PEND | |
| H2 | `/api/v1/state` con 2 canales; `/config` GET/PUT | PEND | |
| H3 | BLE: emparejar app, obtener token LAN, `set_config`, `set_clock` | PEND | |
| H4 | microSD: importar AppConfig + certificados, "Aplicar y reiniciar" / "Cerrar" | **PASS** | 2026-09-24: tras erase-flash se importaron AppConfig y certificados desde la SD; WiFi y AWS conectaron |

### I. HMI / roles / persistencia
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| I1 | Todas las pantallas navegables; touch responde | PEND | |
| I2 | Login con PIN, roles, cambio de PIN obligatorio | PEND | |
| I3 | Persistencia NVS tras reinicio (config, usuarios, consumo) | **PASS (parcial)** | config (WiFi/nube) y consumo persisten tras reinicio; usuarios/PIN pendiente de verificar |
| I4 | Brillo / atenuación por inactividad; no atenúa con alarma | PEND | |
| I5 | Indicadores: pulso DATOS, nube con flecha al publicar | PEND | |

### J. Recursos
| ID | Caso | Resultado | Evidencia |
|----|------|-----------|-----------|
| J1 | taskLVGL ≤ 10 % CPU en dashboard | **PASS** | taskLVGL 3–5 % (soak 18.8 h) |
| J2 | Heap interno: `largest` ≥ 15 KB estable; PSRAM sin fuga | **PASS** | heap interno largest 19.4 KB plano 18.8 h; PSRAM estable |
| J3 | `StkHW` taskLVGL ≥ 2 KB recorriendo todas las pantallas | PEND | |

---

## Hallazgos del soak (2026-09-25) → 1.5.23

### OBS-4 — Alarmas de fuga falsas por ruido del TX WiFi (CORREGIDO en 1.5.23)
- 5 alarmas `flow_warn` en 18 h (y 8 en la sesión de calibración) sin flujo real (SFM3300 = 0).
  Cada una ~50 ms después de un publish MQTT: `v_ain0` salta 1.018 → 1.04–1.12 V durante **dos**
  muestras seguidas, que la mediana de 3 dejaba pasar. Origen: acople del TX WiFi en la entrada
  analógica del FS7/ADS1115 (revisar desacople/tierra en la próxima revisión de HW).
- Firmware: mediana de 5 en el flujo + confirmación de 500 ms en `flow_delta` y `flow_high`
  (re-evaluada contra la línea base del momento de detección). Test de host agregado.

### OBS-5 — Curva del FS7 marcaba +10…25 % (CORREGIDO en 1.5.23, calibración v7)
- Ver P2. Migración EEPROM v6→v7 solo si los parámetros del FS7 siguen de fábrica.

## Hallazgos conocidos (a confirmar / aceptar)

### OBS-1 — Cero de presión depende de la temperatura
- ~0.06 kPa/°C: −0.45 kPa a 37.5 °C, ~+0.1 kPa a 46 °C (logs 2026-09-24). Dentro de la exactitud del
  MS5803-14BA (~±2 kPa); en línea de O₂ (~345 kPa) ≈ 0.13 %. Decidir en P1: recalibrar en caliente,
  zona muerta de display, o compensación por temperatura (requiere más puntos).

### OBS-2 — SFM3300 no responde (bus 2)
- `SFM3300 no responde en 0x40`. Hardware (5 V / pull-ups). Solo afecta la calibración de referencia.

### OBS-3 — Arranque: 2.3 s de verificación EEPROM
- Ya no se ve (queda detrás del splash). Aceptable.

---

## Cierre
- **Verificados (PASS):** —
- **Aceptados:** —
- **Abiertos:** —
- **Parte 0 (2026-09-25):** P1, P3, P5, P6 PASS; P2 modelo PASS (falta verificar en HW con 1.5.23); P4 captura PASS (falta leer el volcado); P7 parcial.
- **Estado:** EN CURSO. Al cerrar con todos los casos PASS/ACEPT, promover **FPM 1.5.22** como baseline
  validado en `MedGuard-12IoT/docs/entrega/ESTADO_ACTUAL.md` y en `CLAUDE.md`.
