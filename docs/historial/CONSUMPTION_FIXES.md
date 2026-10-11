# Consumo y endurecimiento 1.5.5-dev

La adquisición integra el flujo filtrado del ADS1115 mediante trapecios y timestamps
monotónicos (microsegundos al terminar la lectura). Objetivo: ciclo de 100 ms a
100 Hz de tick RTOS; no es una garantía de plazo frente a fallos I2C. Los retrasos
se registran. No se suman muestras inválidas, intervalos con un extremo inválido,
ni huecos de más de 1 s. No se extrapola consumo perdido.

La pantalla solo consulta el acumulador. LAN/AWS incluyen consumption_m3,
consumption_missing_ms y consumption_partial. PARCIAL indica cobertura incompleta,
reinicio o asignación de fecha incierta. La ausencia del SFM3300 es esperada.

El worker time_sync guarda en NVS cada 10 minutos fuera de adquisición, alarmas y
LVGL. Se conserva el formato anterior al leer checkpoints viejos. Un apagado puede
perder lo acumulado desde el último checkpoint: al restaurar se marca PARCIAL.
Los cambios hacia atrás del reloj no borran el total. El intervalo que cruza un
cambio de fecha se excluye y marca como perdido (no se asigna al día equivocado).
No se declara exactitud metrológica sin calibración y prueba contra patrón.

## Validación pendiente del usuario

No se compiló el firmware tras completar esta integración, por instrucción del
usuario. Ejecutar idf.py -B build-fixes build y pruebas de host con
./tests/host/run.ps1 (MSVC). Las pruebas nuevas cubren 10 L/min durante 6 s = 1 L,
rampa con tiempos irregulares, timestamps duplicados/regresivos y huecos/NaN.
EEPROM y alarmas ya pasaron las pruebas de host en la etapa anterior.

En hardware: caudal patrón durante un tiempo medido; navegar/cambiar pantalla;
provocar pérdida controlada del sensor y comprobar PARCIAL sin sumar valores
retenidos; recuperar conexión; verificar cambio de día, reinicio y persistencia;
prueba prolongada de pilas/heap, WiFi/MQTT, tacto y buzzer. Usar el ELF de
build-fixes correspondiente al binario flasheado.


## Hardware follow-up: 1.5.6-dev
- EEPROM: bounded 16-byte writes, readback after every attempt (including NACK),
  fallback to verified single-byte writes; bounded read retries. Logs separate
  transmit errors, read errors and mismatch. No destructive diagnostic writes.
- Missing EEPROM now remains calibration-invalid instead of accepting RAM defaults.
- Acquisition pinned to core 0 at priority 6, above the graphics task (core 1,
  priority 5). Target remains 100 ms and RTOS remains 100 Hz; hardware timeouts
  can still miss the deadline and must remain visible in overrun diagnostics.
- Timer service stack 3072 -> 4096 bytes. General malloc internal preference
  threshold 8192 -> 1024 bytes, allowing larger ordinary allocations to prefer
  PSRAM; explicit internal/DMA allocations retain their capability requirements.
- Host EEPROM regression cases updated for byte fallback and write protection.
  No compilation or test execution performed, per user instruction.
- Pending: build/flash; verify EEPROM survives reboot with CRC, alarm clears only
  after verified persistence, measure overruns under graphics + AWS and minimum
  internal heap/timer stack. Software cannot prove electrical bus integrity.


## EEPROM diagnostic build 1.5.7-dev
- Boot-only scratch test uses 0x0130..0x016f, outside calibration. First backs up
  the 64 bytes and confirms them with byte reads; unstable backup aborts all writes.
- Writes 00, FF, AA/55 and address-dependent patterns across a page boundary,
  verifies three delayed full-block reads per pattern, then attempts restoration
  even on failure. Restoration is checked by individual byte reads.
- EEPROM_TEST logs distinguish pattern failures from restoration failures.
  This diagnostic writes each boot; remove the boot call after diagnosis. Power
  interruption can leave the scratch area altered; calibration is not used as scratch.
- EEPROM_CAL logs three full reads and one byte-wise read of calibration, including
  raw bytes, stored/computed CRC and layout fields. Uses offsetof for CRC boundary.
  Invalid calibration is preserved, never automatically replaced with defaults.
- PASS verifies this boot's I/O, not retention through a power cycle. Restart and
  compare EEPROM_CAL dumps/CRCs to validate retention. Calibration alarm remains
  active if no valid record was read.
- Host regression cases added for scratch restoration and write protection.
  No builds or tests executed (user compiles). Static diff check only.


## CPU distribution and UI refresh: 1.5.8-dev
- aws_pub pinned to CPU0 at priority 5, below sensors_acq (6). HTTP and SD retain ANY.
- Removed unconditional whole-screen invalidation every 200 ms. Widget setters
  still invalidate changed regions. Main-screen text setters skip identical text.
- LVGL9 port already delays at least one tick each loop. Its display flush path
  can busy-wait for SPI completion; synchronization has not been altered here.
- Pending hardware comparison: LVGL CPU, IDLE1, sensor overruns, AWS delivery,
  screen transitions/alarms/graphs and EEPROM diagnostics from 1.5.7-dev.
- No compilation or test execution; static diff check only.


## Validated byte protocol integration: 1.5.9-dev
- User's standalone ESP-IDF test passed five patterns and 15 reads at 100 kHz
  with single-byte writes and STOP-separated address/read transactions.
- Production driver now uses that sequence for probe, all reads, and all writes.
  The bus guard covers address+STOP+read as one operation, preventing interleaving
  with resets or other clients. Each transaction retains a finite timeout.
- Every byte write waits for completion and verifies; the entire range is read
  back twice after the last write. No block-write fallback or repeated START.
- Removed automatic scratch writes on boot. Calibration diagnostics remain;
  invalid calibration is preserved and still raises the calibration alarm.
  Fixing transport cannot reconstruct bytes overwritten by earlier tests at 0x10.
- Updated host regression expectations for byte transactions, speed and retries.
  Not compiled or executed here; static diff checks only. User must validate
  calibration load/save and reboot retention under the full application workload.


## Pre-touch calibration validation: 1.5.10-dev
- Load EEPROM immediately after its driver is initialized, before other sensor
  drivers and before display/touch start. Runtime initialization reuses the result.
- Require four identical successful full-record reads; any mismatch preserves
  the record and leaves calibration invalid. Touch behavior itself is unchanged.
- Narrow repair for the documented Arduino write: byte 0x10 must equal 42;
  replacing only that byte with its default must reproduce the ENTIRE known
  defaults record including the ORIGINAL stored CRC. Otherwise no write occurs.
- After repairing that one byte, require three full-record comparisons before
  accepting calibration. No arbitrary CRC regeneration or custom calibration reset.
- Offline check on captured log bytes: CRC E8D2 becomes stored CRC 2987 when
  byte 0x10 changes from 2A to 00. This confirms the specific repair predicate;
  firmware/hardware tests and reboot retention remain pending. No compilation.


## EEPROM read pacing: 1.5.11-dev
- Standalone hardware result: 10 consecutive sweeps failed at 0x000C;
  all 10 sweeps paced with vTaskDelay(1) passed with stored/computed CRC 2987.
- Added the same one-tick delay after every successful EEPROM byte read,
  including the last byte so consecutive API calls also yield. The shared bus
  guard is released before sleeping. This is observed mitigation, not a proven
  diagnosis of the underlying controller/electrical cause.
- Calibration is loaded once at startup with four matching reads before touch;
  approximately 2 seconds total at 100 Hz. Acquisition never reads EEPROM per
  sample. Later explicit calibration saves also perform paced verification.
- No compile/test execution; static diff check only. Validate in full firmware
  and after reboot before declaring the hardware issue resolved.


## Wi-Fi startup sequencing: 1.5.12-dev
- Confirmed startup defect: cached configuration was scheduled for +2 s while
  esp_wifi_start was already performing PHY initialization. Apply it synchronously
  before starting the radio; propagate configuration/IP errors rather than marking
  failed configuration as applied.
- Later deferred configuration runs in a worker, not the FreeRTOS timer callback.
  Intentional disconnect suppresses reconnect while replacing configuration.
- Logs report radio-start entry, return status and elapsed time. Watchdog remains
  enabled at 5 s and PHY/NVS contents remain unchanged.
- The captured watchdog is inside RF calibration. Removing overlapping config
  operations is necessary but does not prove the RF-calibration stall is fixed.
  Repeat boot test; if the same PHY stack stalls before radio-start returns, further
  PHY investigation is required. No compilation or hardware testing performed.


## RF watchdog isolation: 1.5.13-dev
- 1.5.12 hardware log still stalls in pwdet_tone_start/rfcal_txiq before
  esp_wifi_start returns, with no overlapping deferred config. That sequencing
  fix alone did not resolve the watchdog.
- Revert our generic allocation threshold change: SPIRAM_MALLOC_ALWAYSINTERNAL
  1024 -> original 8192 in sdkconfig and defaults. Local IDF phy_init.c allocates
  esp_phy_calibration_data_t with calloc; that structure is 1904 bytes, so the
  1 KB threshold changes its preferred memory location. This is a controlled
  regression isolation, not evidence that the PHY library requires internal RAM.
- Larger ordinary allocations may again consume more internal memory. Hardware
  boot and post-AWS heap measurements required; no watchdog weakening or NVS erase.
- No compilation performed. If the identical RF stall persists, this memory
  hypothesis is insufficient and further PHY diagnosis is required.


## Unit-specific pressure zero: 1.5.14-dev
- User confirmed inlet at atmosphere for -0.36..-0.47 kPa readings. Add +0.415 kPa
  to the existing pressure offset once when migrating a stable CRC-valid v5
  calibration to v6. Existing flow parameters and pressure scale are preserved.
- Driver verifies the complete written record twice. v6 skips migration on reboot.
  Failed persistence leaves calibration invalid. Defaults now include this unit's
  correction; this is not a universal calibration for other units.
- Applies in pressure computation before gauge subtraction, including live BMP280
  reference. Does not clamp noise to zero; expected observed range ~+/-0.055 kPa.
- No compilation performed. Validate zero after warm-up and after reboot.
- Intermittent Wi-Fi PHY watchdog remains unresolved; no further radio changes.


## Interrupted v6 migration recovery: 1.5.15-dev (Claude)
- 1.5.14 hardware log: four identical reads, CRC stored=2987 computed=7AF0,
  "stable but invalid". Offline check: 2987 = CRC of v5 defaults (offset 0.0);
  7AF0 = CRC of the v6 migrated record (offset +0.415). The data bytes are
  already v6 but bytes 0x30..0x31 (CRC, written last) kept the v5 CRC: the
  migration write was interrupted (reset/power loss) in an earlier boot.
- Transport is fine (reads stable). New narrow recovery: only when magic/v6/size
  match, undoing the migration (version=5, offset-0.415 with bit-exact
  round-trip) reproduces a record whose CRC equals the stored CRC. Then only the
  2 CRC bytes are written, followed by three full-record verifications.
- Expected on next boot: "EEPROM_CAL completing interrupted v6 migration: CRC
  2987 -> 7AF0", then "calibration loaded" and EEPROM=VERIFIED. Following boots
  must show stored=computed=7AF0 with no writes.
- Same log, unrelated: boot 1 hit the intermittent Wi-Fi PHY watchdog (IDLE0,
  5 s) inside esp_wifi_start; boot 2 radio start took 2213 ms (sensors_acq
  overrun 2282 ms). Still open. Monitor warned ELF/binary checksum mismatch, so
  that backtrace symbolization is unreliable.


## Wi-Fi PHY watchdog bootloop: 1.5.16-dev (Claude)
- 1.5.15 log1: EEPROM fixed (stored=computed=7AF0). But every boot panics:
  IDLE0 TWDT inside rfcal_txiq/pwdet_tone_start (esp_wifi_start, PHY cal).
- Cause of the loop: CONFIG_ESP_TASK_WDT_PANIC=y was enabled in this cycle (the
  committed baseline only warned). Slow PHY calibration (>5 s) now panics before
  calibration data reaches NVS, so the next boot repeats full calibration.
- Decision (user): keep TWDT panic enabled. wifi_mgr_start() widens the TWDT
  timeout to 20 s (panic still on, same idle cores) only around esp_wifi_start()
  and restores CONFIG_ESP_TASK_WDT_TIMEOUT_S afterwards. A real PHY hang >20 s
  still resets the device.
- Still open: root cause of 2-5+ s PHY calibration (suspect 3.3 V droop during
  RF TX tone, or temperature ~45 C). Check "Radio start returned ... after N ms".


## PHY hang is real, not slow: 1.5.17-dev (Claude)
- 1.5.16 log2: with a 20 s TWDT window the device still hangs 20 s in
  pwdet_tone_start (rfcal_txiq) and resets. So it is a hang, not slow calibration.
  The 20 s window is kept (harmless, still panics).
- No PHY/sdkconfig change in this cycle besides TWDT panic. Codex did pin
  sensors_acq (NO_AFFINITY -> CPU0, prio 6) and aws_pub (CPU1 -> CPU0), i.e.
  onto the Wi-Fi/PHY core. Reverted both to the committed HEAD layout. This is a
  hypothesis, not a proven cause.
- If 1.5.17 still hangs: build/flash HEAD 640b15c unchanged on the same board
  (A/B). Hang on HEAD too = hardware/power/NVS PHY data; otherwise bisect diff.

### 1.5.17 hardware result (log3, 2026-09-24): VALIDATED
- Radio start ESP_OK after 163 ms (was hang >20 s). Splash no longer freezes.
  EEPROM stored=computed=7AF0, calibration loaded. AWS connected, telemetry v2 OK.
- CPU after AWS: taskLVGL 53%, IDLE1 35% (core 1 no longer saturated),
  IDLE0 94%. sensors_acq on core 1 (NO_AFFINITY), 3% CPU.
- Open: one ~2 s acquisition gap (11.19 s -> 13.20 s) during the AWS TLS connect;
  raised SENSOR_FAULT_STALE alarm (faults=0x2) and overruns=1. Cause not yet
  identified. Internal heap after AWS: free 43 KB, min_ever 28 KB, largest 15 KB.


## Audit and fixes: 1.5.18-dev (Claude, 2026-09-24)
Fixed:
- ALARM (safety): main loop passed 0 kPa / 0 L/min to alarm_mgr when a sample was
  missing or invalid; with pressure_min enabled that is a false clinical ALERT for
  a sensor fault. Now invalid/stale channels pass NAN -> only the technical
  (silenceable) WARNING via faults. Flow-delta baseline restarts on invalid flow.
- Stack: alarm_mgr copied the whole AppConfig (~2 KB) onto the stack every 50 ms
  and on the LVGL stack (1.4 KB free) when pressing mute. Now reads the snapshot
  via appcfg_cache_peek(). Host test stub updated.
- appcfg_cache_reload() kept an AppConfig on the caller's stack (httpd 6 KB, BLE,
  MQTT, LVGL callers); now heap. Snapshot get/reload guarded by a mutex (no torn
  copies while BLE/LAN/AWS reload the config).
- AWS telemetry used alarm limits captured once at task start; now read each cycle.
- cJSON allocations go to PSRAM (cJSON_InitHooks) to stop internal-heap
  fragmentation (largest internal block fell 31 KB -> 11 KB).
- CONFIG_HEAP_TRACING off and unused 400-record trace buffer removed
  (~16 KB internal DRAM + per-malloc overhead).
- CPU 160 -> 240 MHz (LVGL was 53% of core 1).
- HTTP PUT body read: bounded retries on socket timeout (was infinite loop).
- SD AppConfig.json: ftell() failure/oversize guarded (max 64 KB).
- Removed a 1 s boot delay after lv_init().
- Acquisition diagnostics: "Acquisition stall" (per-phase ms when a cycle > 500
  ms) and "Acquisition gap" (> 1 s between cycles) to find the ~2 s gap at AWS
  connect.
Recommended, not applied (need a planned test):
- CONFIG_COMPILER_OPTIMIZATION_DEBUG (-Og) -> PERF (-O2): large LVGL CPU gain.
- CONFIG_FREERTOS_HZ 100 -> 1000: driver_delay_ms() rounds 10 ms to 20 ms, so a
  cycle spends ~62 ms of 100 ms waiting; pdMS_TO_TICKS(<10) is 0 at 100 Hz.
- NVS partition is 24 KB (certs + config + PHY cal + checkpoints). A full NVS
  makes main.c erase it (loses Wi-Fi, certs, consumption). The 60 KB gap
  0x11000-0x20000 could grow it, but moving otadata needs a planned reflash.
- Core dump to flash (currently none) for field crash analysis.
- Wi-Fi reconnect has no backoff (immediate esp_wifi_connect on disconnect).
- BLE on_ble_json_shim queues into s_ble_q but ble_json_worker is never started:
  up to 4 buffers (<=1 KB) are never freed and those commands are ignored.
- PSRAM at 40 MHz could run at 80 MHz (module permitting).

### 1.5.18 hardware result (log4): VALIDATED, committed a25efbf
- 240 MHz, radio 138 ms, AWS OK, splash done at 4.0 s (was 5.2 s).
- Internal heap after AWS: largest 20 KB stable (was 11 KB), frag 56%, min 32 KB.
- No STALE alarm and no stall/gap warnings at AWS connect.
- taskLVGL still ~53% at 240 MHz: LVGL cost is not CPU-bound (SPI 40 MHz,
  single 40-line buffer, flush wait / continuous redraw). -O2 will help little;
  double buffering needs +25.6 KB internal DMA RAM. Left for a dedicated session.

## Recommendations batch 1: 1.5.19-dev (Claude)
- FreeRTOS tick 100 -> 1000 Hz. driver_delay_ms(10) now ~11 ms instead of 20 ms,
  so an acquisition cycle should drop from ~62 ms to ~35 ms of work.
  EEPROM byte pacing was vTaskDelay(1) (validated ~10 ms at 100 Hz); now
  vTaskDelay(pdMS_TO_TICKS(10)) to keep the same tested pacing. Host stubs stay
  at 100 Hz (pdMS_TO_TICKS(10) = 1 tick there, same as before).
- Wi-Fi reconnect: exponential backoff 1 s -> 30 s via esp_timer, reset on GOT_IP
  (was immediate esp_wifi_connect on every disconnect).
- BLE: removed dead JSON pipe (on_ble_json_shim/s_ble_q/ble_json_worker). The
  transport_ble cmd handler is never invoked, so this was dead code, not a leak.
- Pending next batches: -O2, PSRAM 80 MHz, NVS/partitions + core dump.

### 1.5.19 hardware result (log5)
- EEPROM stored=computed=7AF0 with 10 ms pacing at 1000 Hz. Acquisition work
  62 -> 38 ms. Radio 207 ms, AWS OK. Heap largest 20 KB stable.
- One "Acquisition stall 701 ms: ms5803=686" at 9.8 s (during AWS connect):
  the 2 s gap is inside the MS5803 read (I2C bus 1), not scheduling. Next step:
  log ms5803 per-transaction timing / who holds the bus (touch, RTC) then.
- User report: screen stayed dark ~4 s and the splash was barely visible.
  Cause (since 1.5.10): 4 x EEPROM calibration reads (~2.3 s) ran BEFORE the
  display was started; the 2.2 s splash then ended almost immediately.

## Batch 2: 1.5.20-dev (Claude)
- screen_init order: display + UI + splash first (SPI only, no I2C), then I2C
  scan + EEPROM calibration + sensors (init_bus1_sensors), then touch LAST.
  The 1.5.10 rule (calibration read before touch traffic on bus 1) is kept.
  If the display fails, sensors are still initialized.
- Splash: progress bar animates 0 -> 100 % over UI_SPLASH_MS = 3200 ms (was a
  static 72 % for 2.2 s), covering calibration + sensor start.
- Compiler -Og -> -O2 (CONFIG_COMPILER_OPTIMIZATION_PERF). Checked: no busy-wait
  loops on non-volatile shared flags. App bin ~2.0 MB of 3 MB partition.
- Next: PSRAM 80 MHz, then partitions/NVS + core dump (separate session).

### 1.5.20 hardware result (log6): OK
- Screen lights at 1.5 s (was 4.1 s); splash covers the EEPROM check; touch
  starts after calibration (4.3 s). EEPROM 7AF0 OK. Radio 102 ms, AWS OK.
- -O2: taskLVGL 49% -> 42%; internal heap free 49.8 KB, largest 27.6 KB,
  frag 45% (was 20 KB / 56%). BUT taskLVGL stack HWM 1524 -> 784 B (inlining).
- -O2 build needed strncpy -> snprintf everywhere (-Werror=stringop-truncation).
  Real bug found: wifi_config ssid/password copied with sizeof-1, so a 32-char
  SSID or 64-char hex PSK was truncated; now memcpy of strnlen(src, field).
- Stall at AWS connect moved to the ADS read ("ads=664 ms") vs MS5803 before:
  it is the whole I2C bus 1 stalling ~0.7 s during the TLS handshake, not one
  sensor. Below the 1 s STALE threshold (no alarm). Next: time the bus guard /
  I2C driver wait and check whether the touch read (LVGL) holds the bus.
- Pressure at atmosphere read -0.30..-0.34 kPa at 37.7 C (board cold) vs ~0.1
  at 46 C: MS5803 zero drifts with temperature; the +0.415 kPa v6 correction
  was taken warm. Re-check zero after warm-up before any further correction.

## Batch 3: 1.5.21-dev (Claude)
- LVGL task stack 7168 -> 9216 B (internal RAM +2 KB; heap has margin).
- PSRAM 40 -> 80 MHz (quad, flash already 80 MHz). If random crashes/garbled
  display appear, revert CONFIG_SPIRAM_SPEED_80M first.

### 1.5.21 hardware result (log7): VALIDATED
- PSRAM 80 MHz: memory test OK, no resets in ~95 s run. LVGL stack HWM 2832 B.
- Internal heap stable: free 47.7 KB, largest 27.6 KB, frag 43%.
- taskLVGL still ~42%: not PSRAM-bound (flush/redraw cost; see 1.5.18 note).
- I2C bus-1 stall at AWS TLS handshake unchanged (ads=668 ms), below alarm.

## Batch 4: 1.5.22-dev (Claude)
- Partitions: NVS 0x6000 -> 0x15000 (84 KB) using the unused gap before app0;
  otadata moves 0xF000 -> 0x1E000; new 128 KB coredump at 0x620000; storage
  (FAT, not mounted by firmware) moves to 0x640000. app0/app1 unchanged.
  Migration WITHOUT losing NVS: erase only the new NVS tail and the coredump
  area before flashing (docs/HANDOFF.md). Old NVS pages 0x9000-0xEFFF are kept.
- Core dump to flash (ELF, CRC32). Boot logs "Core dump from previous crash" with
  the panic reason; read it with `idf.py -B build-fixes coredump-info`.
- Wi-Fi: esp_wifi_set_storage(WIFI_STORAGE_RAM). Credentials come from AppConfig
  every boot; the driver no longer writes NVS on each connect (flash writes
  stall both cores: prime suspect for the ~0.7 s bus-1 stall at AWS connect).
- I2C guard diagnostics: "i2c_guard: slow <op>: wait=.. xfer=.. err=.. task=.."
  when a guarded operation takes > 80 ms, and "gate timeout" with the task name.
- UI redraw: set_bg/set_border/set_txt_color/bg_opa/border_opa/hidden only write
  when the value changes (LVGL 9 invalidates on every style set, even equal),
  position_overlay no longer rewrites align; status bar repaints only on real
  change (RSSI compared as bars). Expected: taskLVGL well below 42 %.
- Pressure zero vs temperature: no code change; needs a warm measurement.

### 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED
- New partition table active (nvs 0x15000, coredump @0x620000 found). Config and
  AWS certs re-imported from microSD; Wi-Fi + AWS OK (connected at 9.7 s).
- taskLVGL 42% -> 3-5% CPU (IDLE1 89-92%): redundant style writes were the cost.
- No "Acquisition stall", no "i2c_guard: slow", overruns=0 in 130 s including
  the AWS TLS connect: the ~0.7 s bus stall is gone (Wi-Fi NVS writes removed).
- Heap internal: free 43.5 KB, largest 20 KB, frag 53% (stable).
- Pressure at atmosphere -0.40..-0.50 kPa at 37.5 C (was ~+0.1 at 46 C): zero
  follows temperature (~0.06 kPa/C). Within MS5803-14BA accuracy; no code change.


## Soak 18.8 h + calibration session -> 1.5.23-dev (Claude, 2026-09-25)
Evidence: `firmware/longFPM.log` (18.8 h, 36 MB) and `firmware/lonCal.log` (40 min with
flow through FS7 + SFM3300 in series). Analysed with scripts (not committed; logs are
git-ignored).
- Stability: 0 resets, 0 errors, heap internal free 42.4 KB / largest 19.4 KB flat for
  18 h, taskLVGL 5 %, 2261 telemetry messages, 0 Wi-Fi disconnects.
- Pressure zero: 0.00 +/- 0.035 kPa from hour 4 on (T ~47 C). The -0.45 kPa seen before
  was a cold start (37 C). No recalibration needed.
- Core dump: a real 30 112 B dump with valid checksum exists (crash between the 1.5.22
  validation and the soak). Panic reason not extracted; read with coredump-info.
- FS7 vs SFM3300 (800 L): old fit +10..25 % in 12-30 slm, +14.3 % in volume. New fit
  with u0 pinned above the real zero (3.462-3.471 V): u0 3.472, n 0.75, k 1,
  scale 253.4 -> volume -0.0 %, +/-2 % in 15-40 slm, RMS 1.0 slm. Few points < 12 slm.
  Applied as EEPROM calibration v7 (one-time v6->v7 migration only when the FS7 fit is
  still the factory one; interrupted-migration recovery like v6).
- False leak alarms: 5 in 18 h + 8 in the calibration session, all ~50 ms after an MQTT
  publish; Vain0 jumps for TWO consecutive samples (Wi-Fi TX coupling into the FS7/ADS
  analog input). Median-3 let them through. Now median-5 on flow + 500 ms confirmation
  for flow_delta / flow_high (re-checked against the baseline captured at detection,
  lifetime ~one window). New host test case. Hardware follow-up: FS7/ADS decoupling.
- SFM3300 `00 00 00` passes CRC-8 and decodes to -273 slm (62 times): now rejected
  outside +/-250 slm.
- New log line every 30 s: `Consumo: <m3> (<L>) missing=.. partial=.. dated=..`.
- Touch: esp_lcd_touch_get_coordinates (deprecated) -> esp_lcd_touch_get_data.
  Removed unused read_slot_json. CMake ui_bind<->main warning kept (circular dep,
  refactor later, same as MedGuard).
- Host tests (MSVC): all PASS including the new flow-confirmation case.

### 1.5.23 boot check + 1.5.24-dev (2026-09-25)
- 1.5.23 on HW: EEPROM v7 loaded (CRC E9E8 = new FS7 fit), `Consumo: 0.9155 m3` after reboot
  = 914.8 L integrated offline from lonCal.log with the old fit (integrator correct, 0.1 %).
- I2C bus-1 stall (~0.7 s) still appears once per AWS TLS handshake (`xfer=372 ms`, err OK);
  accepted (below STALE threshold). CONFIG_MBEDTLS_MPI_USE_INTERRUPT already enabled.
- 1.5.24: AWS telemetry `firmware` used general.fw_version (stale "1.0.0"); now
  esp_app_get_description()->version. Buffer 24 -> 32 B (-Wformat-truncation at -O2).
- Build switched to the standard `build/` directory (build-fixes removed).
