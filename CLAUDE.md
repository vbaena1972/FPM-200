> **SINCRONIZACIÓN OBLIGATORIA — estado vivo 2026-10-11.** Antes de planificar o ejecutar trabajo de MedGuard/FPM, leer `D:\SoftwareDevelop\Embedded\MedGuard-12IoT\docs\entrega\LEEME_PRIMERO_FUENTE_DE_VERDAD.md`, después `ESTADO_ACTUAL.md` y, según haga falta, la hoja de ruta y el `STATUS` correspondiente. Decisiones vigentes: una persona y una sola actividad principal; MedGuard V1 = Producto A; Axira congelado; baseline MedGuard validada = 1.5.3 (línea 1.7.x en QA); AWS mínimo propio; telemetría cloud objetivo 30–60 s y alarmas inmediatas. **FPM-200 EMPAREJADO con AWS** (identidad `fpm-0001`, stack `medguard-mvp`, telemetría v2 idéntica a MedGuard). **FPM firmware 1.5.31-dev (2026-10-08, rama única `main`)**: 1.5.27 aprobó soak de 21 h (0 reinicios, RAM estable, alarmas de flujo correctas); paridad de seguridad con MedGuard SIN eFuses (OTA firmada por microSD validada en HW, vuelta atrás de OTA, PIN con hash PBKDF2; clave en `firmware/keys/`, fuera de git; ver `docs/SEGURIDAD_PRODUCCION.md`); firmware para microSD = `build/fpm200_<version>.bin`; 1.5.28 corrige el diagnóstico (nombres de tarea copiados en la foto). Entorno: PowerShell 7 (`pwsh`) + `. C:\Espressif\tools\Microsoft.v6.1.PowerShell_profile.ps1` (ESP-IDF 6.1 desde 1.5.31; antes 6.0.2). **1.5.30**: `SPIRAM_MALLOC_ALWAYSINTERNAL` 8192→0 (malloc prefiere PSRAM, como MedGuard; en el soak de 1.5.29 la RAM interna llegó a 23 B al cambiar SSID desde la HMI tras ~4 h sin router y el driver Wi-Fi abortó) y driver KSZ8851SNL vendorizado y parcheado en `firmware/components/espressif__ksz8851snl` (desborde del buffer RX hallado en MedGuard; NO usar el del registry). **1.5.30 validado** (soak ~80 h sin reinicios + cambio de SSID con RAM interna ~78 KB). **1.5.31**: el totalizador ya no integra flujo crudo mientras la mediana de 5 se llena al arrancar (+0,9 L fantasma). SFM3300 desconectado: pruebas de calibración suspendidas. **1.5.31 VALIDADO (soak 42 h, 2026-10-08/10: 0 reinicios, RAM interna plana ~80 KB, una sola sesión AWS). CIERRE de pruebas en HW V1 (DEC-026, 2026-10-10): referencia V1 = 1.5.31-dev; siguiente fase V3 (doble MCU), financiero y app/portal AWS por empresas.** **1.5.32-dev** (compilado, pendiente de HW): calentamiento del FS7 tras corte de energía (el totalizador sumaba ~1 L), total con decimal, reloj `--:--` sin hora válida y restablecer de fábrica en dos niveles; por DEC-027 OTA HTTPS, buzzer IEC 60601-1-8 y ensayo de Secure Boot (en DevKit) se hacen ya en V1. 1.5.32 probado en HW el 2026-10-11 (falló el fin del calentamiento del FS7 tras un corte real: sumó ~1,1 L). **1.5.33/1.5.34-dev** (probados 2026-10-11; 1.5.35-dev sin compilar: íconos de barra en vivo, touch sin dobles clics, login sin usuario, ojo en PIN/passphrase): calentamiento del FS7 por medias de 1 s (flujo ~0 o deriva baja en 20 s, tope 180 s), tarjeta de flujo sin superposición, acceso de Fabricante en el login (candado + passphrase QWERTY) y roles del restablecer (DEC-028: nivel 1 = Administrador o Fabricante; borrado total = solo Fabricante; la calibración del FPM es toda de fábrica, en la EEPROM, y no se toca). Pruebas en HW: `MedGuard-12IoT/docs/entrega/PRUEBAS_HW_1.7.14_1.5.33.md`; prompt de la próxima sesión: `MedGuard-12IoT/docs/entrega/PROMPT_PROXIMA_SESION.md`. El usuario compila (no correr `idf.py build`). Build: `idf.py fullclean` tras cambios de sdkconfig, luego `idf.py -p COM3 build flash monitor`; capturar logs con `| Out-File -Encoding utf8`. Pendiente FPM (trasladado por DEC-026): QA formal `docs/QA_FPM_1.5.22.md` de sensores/HMI → V3; casos de nube/app (G5–G7 shadow `config` + policy IoT de fpm-0001, F5 push, H1–H3 LAN/BLE) durante el trabajo de app/portal; flujo < 12 L/min y SFM3300 → V3; provisioning propio (cert/thing/policy) con el portal por empresas. Detalle en `docs/HANDOFF.md`. Los informes anteriores son históricos.

## Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)
- **`firmware/`** — TODO el proyecto ESP-IDF: `CMakeLists.txt`, `sdkconfig*`,
  `partitions.csv`, `main/` (con `main/ui/` + `main/ui/screens/`) y `components/`.
  `idf.py` se corre **desde `firmware/`** (`cd firmware && idf.py build`).
- **`Simulator/`** — simulador de PC de la HMI (Windows), **parte de este repo** desde 2026-10-14
  (antes repo propio `FPM-200-Sim`, respaldado en `../_respaldo_simuladores_2026-10-14/`). Proyecto
  CMake: compila por glob `firmware/main/ui/**.c` con LVGL 9.5.0 (FetchContent) usando los encabezados
  REALES de `firmware/components/*/include`; solo ESP-IDF está simulado (`Simulator/stubs/`) y las
  funciones del firmware que llama la HMI, en `Simulator/sim_backend.c` (si al enlazar falta un símbolo
  nuevo, agregarlo ahí). `cmake -S Simulator -B Simulator/build && cmake --build Simulator/build
  --config Debug` → `Simulator\build\Debug\fpm200_sim.exe`. Ver `Simulator/README.md`.
- **`docs/`** — `HANDOFF.md` (estado vivo), `historial/` (HMI_MIGRATION, SESION_HMI, CONSUMPTION_FIXES, sesiones viejas del HANDOFF),
  `CONTRATO_BLE_FPM.md`, `financiero/`, `mockups/`.
- **`Hardware/`** — `Datasheets/`, `Squematic.png`, datasheet del MS5803.
  **`scripts/`** — utilidades (`sw_probe*.vbs`). Meta en raíz: `CLAUDE.md`, `AGENTS.md`.
- Los docs históricos (`docs/HANDOFF.md` etc.) referencian rutas viejas tipo
  `main/ui/...`; hoy son `firmware/main/ui/...` (breadcrumbs, no build-crítico).

## Alarmas: silencio (1.6.7)
- `alarm_mgr.c`: WARNING = presión fuera de límite, flujo (delta/alto) o falla de sensor (solo falla = técnica,
  patrón BAJA); ALERT = presión > 15 kPa más allá del límite. Silenciar dura `reannounce_minutes` (WARNING) o
  `max_silence_minutes` (ALERT) y vale SOLO para las condiciones presentes (`s_muted_conds`): una nueva, una que
  vuelve tras `COND_REARM_US` (10 s) ausente, o WARNING→ALERT rompen el silencio. Prueba: `tests/host/test_alarm.c`.
- **Logs solo en ASCII o UTF-8 correcto**: un literal con mojibake (UTF-8 releído como cp1252) puede llevar
  U+0081/U+008D y tumba `idf.py monitor` en Windows (`UnicodeEncodeError cp1252`). Para capturas: `$env:PYTHONUTF8=1`.
- **Varias alarmas (1.6.8)**: la UI no recalcula causas; lee `alarm_mgr_get_active_conditions(&crit)` (bits
  `ALARM_COND_*`). Banner "N ADVERTENCIAS/ALARMAS · presión + flujo", detalle = lista con valores en vivo
  (`alarm_overlay_refresh`, rearmado por firma de condiciones). Sim: `fpm200_sim.exe esc N [det]`.

## Consumo, totalizador y cortes de energía (1.6.8)
- `flow_meter.c`: HOY (se reinicia a las 00:00 locales) + TOTAL de vida; checkpoint en NVS `metrics` cada 2 min
  y al cambiar de día (`metrics_store.h`, blob v3; los blobs viejos cargan). `metrics` no lo borra ningún
  "Restablecer" (no tocarlo en `factory_reset_apply`). Tras un corte: HOY se restaura si es el mismo día y el
  tiempo apagado va a "sin medir". `flow_meter_boot_checkpoint()` = valores al corte → aviso MQTT
  "power_restored" (canal 0 del topic `/alarms`); el corte en sí lo anuncia el Last Will. Pruebas:
  `tests/host/test_flow_meter.c`, `test_minmax24.c` (min/max 24 h de las tarjetas).
- "24H" (1.6.9): cubetas por hora de reloj, guardadas en NVS `metrics`/`mm24` cada 10 min y fusionadas al
  arrancar. Fila de consumo: TOTAL primero (grande), HOY después. TOTAL nunca < HOY.
- **Verificar compilando a objeto**, no solo `-fsyntax-only`: `-Werror=format-truncation` aparece con -O2.

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
