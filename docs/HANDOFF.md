# Handoff — FPM-200 (firmware + HMI)

Documento para **retomar en una sesión nueva**: estado real por sesión (lo más nuevo arriba), pendientes y
gotchas. Estado global de ambos equipos: `MedGuard-12IoT/docs/entrega/ESTADO_ACTUAL.md`. Historia de la
migración de HMI y sesiones viejas: `docs/historial/`.

---

> Sesiones anteriores al 2026-10-10 y el estado de agosto/septiembre (§0–§5, §8): `docs/historial/HANDOFF_sesiones_2026-08_a_2026-10-08.md`. Aquí quedan las sesiones vigentes, el pendiente (§6) y los gotchas (§7).

## 2026-10-12 noche — FW 1.6.3-dev en HW, soak de ~12 h en curso

- Buzzer IEC aprobado en HW (B6 ALTA, B7 MEDIA, B9 silenciar/prueba). Soak O8 con MedGuard 1.8.3 desde ~21:06.
- **Core dump:** al arrancar 1.6.2 apareció `abort() was called at PC 0x403868b3` (en `esp_rom_vprintf`), de origen
  desconocido: el dump nunca se borraba y se repetía en cada arranque. 1.6.3 lo resume en el log (tarea, PC, backtrace,
  SHA del ELF) y lo borra (`main.c`). El resumen del dump viejo no quedó grabado. En el soak, buscar `Core dump:`.
- Gotcha: `idf.py coredump-info > archivo` en PowerShell solo guarda la primera línea; usar `*>`.

## 2026-10-12 — FW 1.6.2-dev: corrección del buzzer continuo (sin compilar)

- En HW 1.6.0/1.6.1 el buzzer sonaba continuo: en ESP-IDF 6.1 `esp_timer_is_active()` es true dentro del propio
  callback y `alarm_tone` no pasaba del 1er pulso. Corregido (vencimiento por hora), mismo archivo que MedGuard 1.8.2;
  prueba de host en `MedGuard-12IoT/test/alarm_tone/host`. O9 (OTA con ALERT sonando) aprobada en 1.6.0 → 1.6.1.

## 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar)

- OTA por red **aprobada en HW** (O1–O7, 1.5.37 → 1.5.39). Salto de versión menor: 1.6.0-dev (DEC-030).
- Buzzer: `components/drivers/alarm_tone.c` (IDÉNTICO al de MedGuard `firmware/system/alarm_tone.c`; copiar si se
  cambia) temporiza los patrones por `esp_timer`. `alarm_mgr.c` solo elige la prioridad: ALERT = ALTA (10 pulsos cada
  3 s), WARNING de presión/flujo = MEDIA (3 cada 5 s), solo fallo de sensor = BAJA (2 cada 16 s). Prueba del buzzer 3 s.
- Reiniciar / Restablecer no actúan durante una OTA por red. **1.5.40-dev no se reutiliza** (imagen de prueba de O6).
- Pruebas: `MedGuard-12IoT/docs/entrega/PRUEBAS_HW_1.8.0_1.6.0.md` (B6–B9, O9, O8).

## 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar)

- Información (Administrador+): Estable/Beta, Buscar, Instalar con barra; la tarea `ota_net` (stack interno 8 KB)
  pausa MQTT (`transport_mqtt_stop`), lee `<cloud.ota_url>/fpm200/<canal>/manifest.json` (`ota_manifest.c`, idéntico
  al de MedGuard), solo instala una versión más nueva (`ota_update_from_url`: proyecto + firma + borrado de la cola de
  la partición) y reinicia; si falla, reanuda MQTT. La pantalla solo lee el estado con un lv_timer.
- Prueba de vuelta atrás: `idf.py -DFPM_OTA_TEST_NO_CONFIRM=1 build` (imagen que no se confirma y reinicia a los 30 s).
- Servidor y publicación: DEC-029 y `MedGuard-12IoT/scripts/publish_firmware.sh`. Pruebas:
  `MedGuard-12IoT/docs/entrega/PRUEBAS_HW_OTA_1.7.18_1.5.37.md`.

## 2026-10-12 — Soak final de 1.5.36-dev APROBADO (~28 h): baseline de V1

- Log `firmware/logMedFPM_1.5.36_20261011_1301.txt`: 1 arranque, 0 panics; RAM interna plana 80.203 B (h1→h28, min_ever
  46.876); 3.359 telemetrías (1/30 s); 2 cortes de AWS (22,97 h y 23,91 h; el segundo a la vez que MedGuard → red) con
  reconexión inmediata; FS7 estable a 3,4 s; 1.557 glitches de flujo descartados (~55/h) y 13 `SALTO ADC` (conocidos).
- **Fase 1 cerrada.** Siguiente: fase 2 (OTA HTTPS, buzzer IEC 60601-1-8, Secure Boot en DevKit) según
  `MedGuard-12IoT/docs/entrega/PROMPT_PROXIMA_SESION.md`.

## 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar)

- 1.5.34 OK en HW: tarjetas no clicables, Información con scroll, restablecer en dos niveles + reimportación de
  certificados (log `logMedFPM_1.5.34_20261011_1147.txt`).
- **1.5.35:** (1) `ui_statusbar_controller.c` relee `wifi/eth/bt/cloud.enabled` de la config en cada vuelta (antes
  solo en `ui_init`: tras un restablecer + habilitar Wi-Fi/nube no aparecían los íconos). (2) `ft5x06.c`: un glitch
  de I2C con el INT asertado repite el último estado hasta 3 lecturas (antes devolvía "soltado" → doble clic, "1"
  salía "11"); el pad de PIN además ignora la misma tecla en < 120 ms. (3) Login: sin usuario elegido no se acepta
  el PIN ni cuenta intento; ojo mostrar/ocultar en el PIN y en la passphrase de Fabricante. (4) "Reiniciar equipo"
  con alto de contenido.
- Pruebas: "Ronda mínima" (M4–M9) de `MedGuard-12IoT/docs/entrega/historial/PRUEBAS_HW_1.7.14_1.5.33.md`.

## 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar)

- 1.5.33: arranques en caliente OK (`FS7 estabilizado tras 8762/3399 ms (flujo cero…)`), acceso de Fabricante
  OK, RAM interna 80.235 B. Pruebas F bloqueadas: con sesión, Información pasaba de 320 px y no tenía scroll
  (botones fuera de vista). Ojo: desde el menú ⋮ del dashboard Información se abre SIN sesión (sin botones);
  con sesión es Configuración → Ajustes generales → Información.
- **1.5.34:** `ui_card()` y `ui_pill()` ya NO son clickeables por defecto (las informativas reaccionaban al
  toque; las que son botones ya agregaban `LV_OBJ_FLAG_CLICKABLE`); Información con scroll vertical y tarjetas de
  calibración/mantenimiento con alto de contenido (antes el alto por defecto de lv_obj). La tecla "ocultar
  teclado" de la hoja de passphrase cierra la hoja (= cancelar, como en los formularios): se deja así.
- Pruebas: sección "Ronda corta" de `MedGuard-12IoT/docs/entrega/historial/PRUEBAS_HW_1.7.14_1.5.33.md` (R5–R8, F1–F3).

## 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW)

- **Pruebas de 1.5.32** (log `firmware/logMedFPM_1.5.32_20261011_0713.txt`; detalle en
  `MedGuard-12IoT/docs/entrega/historial/PRUEBAS_HW_1.7.13_1.5.32.md`, sección Resultados): OK arranque/AWS/telemetría,
  total con decimal, hora inválida + SNTP, reinicio en caliente (FS7 estable en 3400 ms). **Falla tras un corte
  real:** `FS7 estabilizado tras 16799 ms (flujo 2.41 L/min…)` sin flujo; el valor decayó solo a 0 en ~115 s y
  sumó ≈ 1,1 L (la banda de ±0,3 L/min en 3 s no ve una deriva lenta de ~0,09 L/min por s). ESTABILIZANDO se
  superponía con "24H". **Pruebas F imposibles:** no había forma de entrar como Fabricante (§6.2).
- **1.5.33:** (1) calentamiento por medias de 1 s: termina con flujo ~0 (3 medias < 0,05 L/min, tras ≥ 3 s) o
  con deriva baja (media de 3 s vs. la de hace 20 s ≤ max(0,1 L/min; 5 %)); tope 180 s (`FS7_WARM_*`);
  simulado con el log: ~112 s y ~0,006 L residuales. El log de 1 Hz muestra `[FS7 calentando f=… d20=…]`.
  (2) Tarjeta: si estado + "24H" no caben, se oculta el 24H (`ui_mainScreen.c`, `lv_text_get_size`).
  (3) **Acceso de Fabricante** en el login: candado arriba a la derecha → hoja con passphrase y teclado QWERTY
  (`ui_auth_login(ui_auth_factory(), …)`, mismo bloqueo de 5 intentos/30 s) — cierra §6.2. (4) DEC-028:
  "Restablecer config." = Administrador o Fabricante; "Borrado total" = solo Fabricante. Toda la calibración
  del FPM (ajuste del FS7, ceros; EEPROM) es de fábrica: ningún nivel la toca. (5) Progreso en la tarjeta de
  flujo: valor "--" y "calentando N s" (`sensors_runtime_flow_warming_elapsed_ms`); no se alarga el splash a
  propósito: la presión y sus alarmas deben volver en segundos tras un corte (decisión 2026-10-11).
- **1.6.4-dev (2026-10-14):** login: la tarjeta de cada usuario cortaba el rol bajo el nombre → crece con
  el contenido (QA formal V1, P3). Lista para grabar (sin cambio de tabla de particiones: sirve OTA). QA formal
  V1: `MedGuard-12IoT/docs/entrega/QA_FORMAL_V1_2026-10-14.md` (P1 → V3; P2 pendiente con límites de presión).
- **Simulador rehecho (2026-10-14):** `Simulator/` es ahora parte de este repo (proyecto CMake con los
  encabezados reales de los componentes; ESP-IDF simulado en `Simulator/stubs/`, backend en
  `Simulator/sim_backend.c`). Compila y corre; el repo `FPM-200-Sim` queda obsoleto.
- Sintaxis verificada con la toolchain del proyecto (`-fsyntax-only` con `build/compile_commands.json`).
  **Probar:** `MedGuard-12IoT/docs/entrega/historial/PRUEBAS_HW_1.7.14_1.5.33.md` (B, F, G). Pendiente menor:
  `general.factory.must_change_pin` se guarda pero ninguna pantalla obliga a cambiar la passphrase demo.

## 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027

- **Prueba de corte de energía (usuario):** arranca solo, conserva la config y reconecta a AWS; la hora
  se pierde si el corte supera ~5–10 s en V1. **El totalizador seguía sumando ~1 L tras el corte.**
- **Causa:** calentamiento del FS7 (anemómetro térmico frío tras un corte; con reset por USB sigue
  caliente, por eso no salía en los soaks). No es el búfer del filtro (arranca en ceros y 1.5.31 ya
  descarta las 4 primeras muestras).
- **1.5.32:** (1) flujo inválido mientras el FS7 se estabiliza (filtrado estable 3 s dentro de ±max(0,3
  L/min; 5 %), máx. 60 s; reinicia al habilitar el flujo): no suma, no evalúa alarmas clínicas y NO marca
  STALE; la tarjeta dice ESTABILIZANDO; logs `FS7: esperando estabilizacion…` / `FS7 estabilizado tras N
  ms`; el log de 1 Hz marca `[FS7 calentando]`. (2) Total del día con un decimal y fuente 28 (`mg_font_28`
  copiada de MedGuard, añadida al `.vcxproj` del simulador). (3) Sin hora válida (`time_mgr_is_time_valid`)
  el reloj muestra `--:--`/"Sin fecha" (antes 01/01/1970). (4) **Restablecer de fábrica en dos niveles**
  en Información (solo Fabricante, doble toque): "Restablecer config." conserva identidad HW, calibración,
  nube y cuenta Fabricante; "Borrado total" además borra certificados (`cert_store_erase_all`), token LAN
  (`httpmon`) y vínculos BLE (`nimble_bond`).
- **DEC-027:** OTA HTTPS, patrones IEC 60601-1-8 del buzzer y el ensayo de Secure Boot (en DevKit) se
  hacen ya en V1. **Probar en HW:** corte de energía (≥ 1 min) → log `FS7 estabilizado tras …` y el total
  no debe subir; los dos niveles de restablecer.
- **Ojo:** la config de fábrica deja Wi-Fi, Ethernet y BT deshabilitados: tras "Restablecer config." hay que
  configurar la Wi-Fi (AWS conecta sin reaprovisionar); tras "Borrado total", además certificados por
  microSD (`/sdcard/aws`) y la passphrase de Fabricante vuelve a la de demostración (pide cambiarla).
- **Procedimiento completo de pruebas:** `MedGuard-12IoT/docs/entrega/historial/PRUEBAS_HW_1.7.13_1.5.32.md` (secciones
  A, B, C, F, G). Prompt de la próxima sesión: `MedGuard-12IoT/docs/entrega/PROMPT_PROXIMA_SESION.md`.
- **El usuario compila y flashea** (Claude no corre `idf.py build`). 1.5.32 compiló sin warnings (31 % libre).

## Cierre 2026-10-10 — 1.5.31 VALIDADO; FIN de las pruebas en Hardware V1 (DEC-026)

- **Soak 1.5.31-dev (ESP-IDF 6.1), 42 h** (8-oct 20:17 → 10-oct ~14:20), log
  `firmware/logMedFPM_1.5.31_20261008_2017.txt`: **0 reinicios**; RAM interna **plana 80.295 B** (h6→h42,
  min_ever 46.968); PSRAM ~1,93 MB estable; **una sola sesión AWS sin cortes** en 42 h; 5.045 telemetrías
  (1/30 s); `MQTT ya estaba inicializado` cada hora = resincronización del RTC (inofensivo).
- 1 pausa de adquisición de 787 ms solo al arrancar (MS5803 724 ms + timeout del mutex de pantalla); el
  filtro descartó 1.783 glitches de flujo (~40/h, igual que soaks previos); 6 `SALTO ADC`.
- Observación aceptada: 12 lecturas aisladas de flujo (0,02–4,56 L/min, 1 s, sin cambio de presión)
  pasaron el filtro en 42 h (≤ 0,02 L c/u al totalizador). El arreglo del totalizador al arrancar (1.5.31)
  no se ve en este log (no hay línea que lo muestre).
- **Decisión del usuario (DEC-026 en `MedGuard-12IoT/docs/entrega/REGISTRO_DECISIONES.md`):** se cierra la
  campaña de estabilidad en HW V1; **versión de referencia de V1 = 1.5.31-dev**. Siguiente fase: V3 (doble
  MCU), financiero y app/portal AWS por empresas. Los casos PEND del QA formal que dependen de sensores/HW
  V1 (presión F1–F3, fuga, consumo E3–E5, C4/C5, D3/D4, HMI I1–I5, J3) se trasladan a V3; los de nube/app
  (G5–G7 shadow `config` + policy IoT, F5 push, H1–H3 LAN/BLE) se cubren durante el trabajo de app/portal.

## 6. Pendiente (después de estabilizar §2 y validar §3)

1. **Completar GATT:** `set_clock`, cifrado/passkey.
2. ~~**Login de FABRICANTE**~~ HECHO en 1.5.33 (candado + passphrase QWERTY en `ui_loginScreen.c`). Texto original: el pad del login es numérico; la passphrase del fabricante es
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

