> **SINCRONIZACIÓN OBLIGATORIA — estado vivo 2026-09-15.** Antes de planificar o ejecutar trabajo de MedGuard/FPM, leer `D:\SoftwareDevelop\Embedded\MedGuard-12IoT\docs\entrega\LEEME_PRIMERO_FUENTE_DE_VERDAD.md`, después `ESTADO_ACTUAL.md` y, según haga falta, la hoja de ruta y el `STATUS` correspondiente. Decisiones vigentes: una persona y una sola actividad principal; MedGuard V1 = Producto A; Axira congelado; baseline MedGuard validada = 1.4.1 (`03705f8` sobre `6801bd2`), aunque la telemetría reporta 1.4.2 pendiente de validación formal; AWS mínimo propio; G3 cerrada y G4 funcional+endurecido (Cognito, app, portal HTTPS/CloudFront, MFA TOTP opcional, grupos); Trends validado con DynamoDB (24 h) + S3/Athena (7/30 días); telemetría cloud objetivo 30–60 s y alarmas inmediatas. **FPM-200 EMPAREJADO con AWS (actividad (c)/DEC-021, 2026-09-15, validado en vivo):** comparte el stack `medguard-mvp` con identidad nativa `fpm-0001` (MedGuard=`medguard-0001`); remap retirado de los 4 Lambdas; firmware FPM v2 (`channels[]`+`stats`@30 s idéntico a MedGuard). Pendiente FPM: reflash del ajuste de casing, provisioning propio (cert/thing/policy) y QA. Los informes anteriores son históricos.

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
