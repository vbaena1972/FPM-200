> **SINCRONIZACIÓN OBLIGATORIA — estado vivo 2026-09-14.** Antes de planificar o ejecutar trabajo de MedGuard/Axira, leer `D:\SoftwareDevelop\Embedded\ClaudeHMI\Entrega_Cierre_2026-08-23\LEEME_PRIMERO_FUENTE_DE_VERDAD.md`, después `ESTADO_ACTUAL.md` y, según haga falta, la hoja de ruta y el `STATUS` correspondiente. Decisiones vigentes: una persona y una sola actividad principal; MedGuard V1 = Producto A; Axira congelado; baseline MedGuard validada = 1.4.1 (`03705f8` sobre `6801bd2`), aunque la telemetría reporta 1.4.2 pendiente de validación formal; AWS mínimo propio; G3 cerrada y G4 funcional con Cognito, app y portal; Trends validado con DynamoDB (24 h) + S3/Athena (7/30 días); telemetría cloud objetivo 30–60 s y alarmas inmediatas. Próxima actividad: CLD-02 (identidad y cadencia, requiere hardware) o endurecimiento G4, una a la vez. Los informes anteriores son históricos.

## graphify

This project has a knowledge graph at graphify-out/ with god nodes, community structure, and cross-file relationships.

Rules:
- For codebase questions, first run `graphify query "<question>"` when graphify-out/graph.json exists. Use `graphify path "<A>" "<B>"` for relationships and `graphify explain "<concept>"` for focused concepts. These return a scoped subgraph, usually much smaller than GRAPH_REPORT.md or raw grep output.
- If graphify-out/wiki/index.md exists, use it for broad navigation instead of raw source browsing.
- Read graphify-out/GRAPH_REPORT.md only for broad architecture review or when query/path/explain do not surface enough context.
- After modifying code, run `graphify update .` to keep the graph current (AST-only, no API cost).
