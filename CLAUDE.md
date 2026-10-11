> **SINCRONIZACIÓN OBLIGATORIA — estado vivo 2026-10-10.** Antes de planificar o ejecutar trabajo de MedGuard/FPM, leer `D:\SoftwareDevelop\Embedded\MedGuard-12IoT\docs\entrega\LEEME_PRIMERO_FUENTE_DE_VERDAD.md`, después `ESTADO_ACTUAL.md` y, según haga falta, la hoja de ruta y el `STATUS` correspondiente. Decisiones vigentes: una persona y una sola actividad principal; MedGuard V1 = Producto A; Axira congelado; baseline MedGuard validada = 1.5.3 (línea 1.7.x en QA); AWS mínimo propio; telemetría cloud objetivo 30–60 s y alarmas inmediatas. **FPM-200 EMPAREJADO con AWS** (identidad `fpm-0001`, stack `medguard-mvp`, telemetría v2 idéntica a MedGuard). **FPM firmware 1.5.31-dev (2026-10-08, rama única `main`)**: 1.5.27 aprobó soak de 21 h (0 reinicios, RAM estable, alarmas de flujo correctas); paridad de seguridad con MedGuard SIN eFuses (OTA firmada por microSD validada en HW, vuelta atrás de OTA, PIN con hash PBKDF2; clave en `firmware/keys/`, fuera de git; ver `docs/SEGURIDAD_PRODUCCION.md`); firmware para microSD = `build/fpm200_<version>.bin`; 1.5.28 corrige el diagnóstico (nombres de tarea copiados en la foto). Entorno: PowerShell 7 (`pwsh`) + `. C:\Espressif\tools\Microsoft.v6.1.PowerShell_profile.ps1` (ESP-IDF 6.1 desde 1.5.31; antes 6.0.2). **1.5.30**: `SPIRAM_MALLOC_ALWAYSINTERNAL` 8192→0 (malloc prefiere PSRAM, como MedGuard; en el soak de 1.5.29 la RAM interna llegó a 23 B al cambiar SSID desde la HMI tras ~4 h sin router y el driver Wi-Fi abortó) y driver KSZ8851SNL vendorizado y parcheado en `firmware/components/espressif__ksz8851snl` (desborde del buffer RX hallado en MedGuard; NO usar el del registry). **1.5.30 validado** (soak ~80 h sin reinicios + cambio de SSID con RAM interna ~78 KB). **1.5.31**: el totalizador ya no integra flujo crudo mientras la mediana de 5 se llena al arrancar (+0,9 L fantasma). SFM3300 desconectado: pruebas de calibración suspendidas. **1.5.31 VALIDADO (soak 42 h, 2026-10-08/10: 0 reinicios, RAM interna plana ~80 KB, una sola sesión AWS). CIERRE de pruebas en HW V1 (DEC-026, 2026-10-10): referencia V1 = 1.5.31-dev; siguiente fase V3 (doble MCU), financiero y app/portal AWS por empresas.** Build: `idf.py fullclean` tras cambios de sdkconfig, luego `idf.py -p COM3 build flash monitor`; capturar logs con `| Out-File -Encoding utf8`. Pendiente FPM (trasladado por DEC-026): QA formal `docs/QA_FPM_1.5.22.md` de sensores/HMI → V3; casos de nube/app (G5–G7 shadow `config` + policy IoT de fpm-0001, F5 push, H1–H3 LAN/BLE) durante el trabajo de app/portal; flujo < 12 L/min y SFM3300 → V3; provisioning propio (cert/thing/policy) con el portal por empresas. Detalle en `docs/HANDOFF.md`. Los informes anteriores son históricos.

## Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)
- **`firmware/`** — TODO el proyecto ESP-IDF: `CMakeLists.txt`, `sdkconfig*`,
  `partitions.csv`, `main/` (con `main/ui/` + `main/ui/screens/`) y `components/`.
  `idf.py` se corre **desde `firmware/`** (`cd firmware && idf.py build`).
- **`Simulator/`** — simulador de Visual Studio (repo git PROPIO `FPM-200-Sim`,
  anidado e ignorado por este repo; se pushea entrando a `Simulator/`). Su `.vcxproj`
  referencia `..\..\firmware\main\ui\...` (rutas relativas).
- **`docs/`** — `HANDOFF.md`, `HMI_MIGRATION.md`, `SESION_HMI.md`,
  `CONTRATO_BLE_FPM.md`, `financiero/`, `mockups/`.
- **`Hardware/`** — `Datasheets/`, `Squematic.png`, datasheet del MS5803.
  **`scripts/`** — utilidades (`sw_probe*.vbs`). Meta en raíz: `CLAUDE.md`, `AGENTS.md`.
- Los docs históricos (`docs/HANDOFF.md` etc.) referencian rutas viejas tipo
  `main/ui/...`; hoy son `firmware/main/ui/...` (breadcrumbs, no build-crítico).

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
