> **SINCRONIZACIÓN OBLIGATORIA — estado vivo 2026-09-25.** Antes de planificar o ejecutar trabajo de MedGuard/FPM, leer `D:\SoftwareDevelop\Embedded\MedGuard-12IoT\docs\entrega\LEEME_PRIMERO_FUENTE_DE_VERDAD.md`, después `ESTADO_ACTUAL.md` y, según haga falta, la hoja de ruta y el `STATUS` correspondiente. Decisiones vigentes: una persona y una sola actividad principal; MedGuard V1 = Producto A; Axira congelado; baseline MedGuard validada = 1.5.3 (QA formal cerrado 2026-09-15); AWS mínimo propio; G3 cerrada y G4 funcional+endurecido (Cognito, app, portal HTTPS/CloudFront, MFA TOTP opcional, grupos); Trends validado con DynamoDB (24 h) + S3/Athena (7/30 días); telemetría cloud objetivo 30–60 s y alarmas inmediatas. **FPM-200 EMPAREJADO con AWS (actividad (c)/DEC-021, 2026-09-15, validado en vivo):** comparte el stack `medguard-mvp` con identidad nativa `fpm-0001` (MedGuard=`medguard-0001`); remap retirado de los 4 Lambdas; firmware FPM v2 (`channels[]`+`stats`@30 s idéntico a MedGuard). **FPM firmware 1.5.24-dev (2026-09-25)** en HW: soak 18.8 h estable, FS7 recalibrado vs SFM3300 (EEPROM v7, volumen −0.0 %), alarmas de flujo con confirmación, telemetría con versión real; build estándar `idf.py -p COM3 build flash monitor` (detalle en `docs/HANDOFF.md` y `firmware/CONSUMPTION_FIXES.md`). Pendiente FPM: **completar QA formal** (`docs/QA_FPM_1.5.22.md`, 18 casos cerrados; faltan alarmas, HMI, app/portal) → primer baseline FPM; flujo < 12 L/min y patrón de presión sin equipo; provisioning propio (cert/thing/policy). Los informes anteriores son históricos.

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

When the user types `/graphify`, use the installed graphify skill or instructions before doing anything else.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- Dirty graphify-out/ files are expected after hooks or incremental updates; dirty graph files are not a reason to skip graphify. Only skip graphify if the task is about stale or incorrect graph output, or the user explicitly says not to use it.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
