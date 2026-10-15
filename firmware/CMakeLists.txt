cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# 1.6.8-dev (2026-10-15): cierre de diseno V1 (lista del usuario tras el soak 1.6.7 de 11 h).
#   (1) ADVERTENCIA muestra "buzzer activo" como ALARMA. (2) VARIAS ALARMAS: alarm_mgr expone las
#   condiciones activas (alarm_mgr_get_active_conditions); el banner dice "2 ADVERTENCIAS / 3 ALARMAS
#   · presion + flujo" y el detalle (tocar el banner) lista cada una con su valor EN VIVO, como
#   "Alarmas activas" de MedGuard; se rearma si aparece otra. Tarjetas: ambar = advertencia, rojo =
#   critica; la de flujo sigue a flujo alto / variacion. (3) "24H" de las tarjetas: era el ULTIMO
#   MINUTO (get_min_max(60000)) y descartaba muestras con cualquier bit invalido; ahora 24 cubetas
#   horarias (minmax24.h) con presion y flujo independientes. (4) TOTALIZADOR de vida en la fila de
#   consumo ("HOY | TOTAL"), en NVS "metrics" (ningun restablecer lo borra). (5) HOY se guarda cada
#   2 min (antes 10) y al cambiar de dia; "desde HH:MM" real; la pildora PARCIAL pasa a "N min sin
#   medir" (incluye el tiempo apagado). Corte de energia -> AWS: Last Will MQTT (canal 0, "fault"
#   ~1,5 x keepalive tras perder al equipo; keepalive de la config, 60 s) y al volver un aviso
#   "Energia restablecida" con la hora aprox. del corte y el consumo del dia/total a esa hora.
#   Telemetria/LAN: + consumption_total_m3. Pruebas host: test_minmax24, test_flow_meter, test_alarm.
# 1.6.7-dev (2026-10-14): (1) el silencio tapaba CUALQUIER alarma nueva hasta vencer (15 min en
#   ADVERTENCIA): ahora vale solo para las condiciones presentes al silenciar; una condicion nueva
#   (flujo, falla de sensor, el otro limite de presion) o una que vuelve tras 10 s ausente rompe el
#   silencio (IEC 60601-1-8). (2) Textos con UTF-8 doble/triple (mojibake) en logs y comentarios de
#   alarm_mgr/main/sensors_runtime/storage: el de "escalo a CRITICA" llevaba U+008D y TUMBABA
#   idf.py monitor en Windows (cp1252) justo al escalar. (3) tests/host/test_alarm.c reescrita
#   (la anterior no compilaba desde alarm_tone, 1.6.0). QA formal V1, P2.
# 1.6.6-dev (2026-10-14): una ADVERTENCIA no se podia silenciar desde la pantalla (el boton solo
#   existia en ALARMA, aunque alarm_mgr si lo permite: silencio por reannounce_minutes que se rompe si
#   escala a ALARMA). Ahora "Silenciar advertencia" y el banner muestra "silenciada". QA formal V1, P2.
# 1.6.5-dev (2026-10-14): el banner superior decia siempre "ADVERTENCIA · vigilar" / "ALARMA ·
#   revisar linea"; ahora dice la causa (presion baja/alta, flujo anormal, falla de sensores), con
#   la misma regla que alarm_mgr (solo limites habilitados). QA formal V1, P2.
# 1.6.4-dev (2026-10-14): login: la tarjeta de cada usuario cortaba el rol bajo el nombre
#   (alto fijo 44/32 px); ahora crece con el contenido (QA formal V1, P3).
# 1.6.3-dev (2026-10-12): el core dump de una caida se dejaba en la flash y se
#   reportaba en TODOS los arranques (sin saber de cuando era). Ahora se deja en
#   el log el resumen (tarea, PC, backtrace, SHA del ELF) y se borra.
# 1.6.2-dev (2026-10-12): alarm_tone: el buzzer sonaba CONTINUO en 1.6.0/1.6.1
#   (B6 en HW): esp_timer_is_active() en ESP-IDF 6.1 da true dentro del propio
#   callback; ahora se compara con la hora de vencimiento (igual que MedGuard 1.8.2).
#   (1.6.1-dev fue el mismo 1.6.0-dev, solo para la prueba O9 de OTA.)
# 1.6.0-dev (2026-10-12), salto de version menor (OTA por red + alarmas IEC):
#   - Buzzer con patrones IEC 60601-1-8 (components/drivers/alarm_tone.c, el mismo
#     modulo que MedGuard 1.8.0): ALERT = ALTA (10 pulsos cada 3 s), WARNING de
#     presion/flujo = MEDIA (3 cada 5 s), solo fallo de sensor = BAJA (2 cada
#     16 s); antes 150/150 ms y 500/500 ms. Prueba del buzzer: 2 s -> 3 s.
#   - Reiniciar / Restablecer no actuan durante una OTA por red ("Espere:
#     actualizacion en curso").
#   (1.5.40-dev fue la imagen de prueba sin confirmar de O6: no reutilizar.)
# 1.5.39-dev (2026-10-12): el temporizador de inactividad no saca de Informacion
#   durante una OTA por red; buffer HTTP de 8 KB (como MedGuard 1.7.20).
#   (1.5.38-dev fue el mismo 1.5.37-dev, solo para probar la OTA.)
# 1.5.37-dev (2026-10-12), fase 2 (DEC-027): OTA por RED (antes solo microSD).
#   Informacion (Administrador+): Estable/Beta, Buscar, Instalar con progreso. Lee
#   <cloud.ota_url>/fpm200/<canal>/manifest.json (ota_manifest.c, identico al de
#   MedGuard), solo instala una version MAS NUEVA, valida proyecto + FIRMA, borra
#   la cola de la particion (firma vieja) y reinicia. MQTT se pausa durante la OTA.
# 1.5.36-dev (2026-10-11), tras probar 1.5.35 en HW: tras 2 min sin tocar la
#   pantalla, desde cualquier pantalla se vuelve al dashboard y se cierra la sesion
#   (igual que MedGuard).
# 1.5.35-dev (2026-10-11), tras probar 1.5.34 en HW: (1) iconos de Wi-Fi/Ethernet/
#   BT/nube del dashboard siguen la config en vivo (solo se leian al arrancar: tras
#   un restablecer + habilitar Wi-Fi/nube no aparecian). (2) Touch: un glitch I2C
#   con el dedo apoyado ya no se reporta como "soltado" (doble clic: "1" -> "11");
#   el pad de PIN ademas ignora la misma tecla repetida en < 120 ms. (3) Login: sin
#   usuario elegido no se acepta el PIN ni cuenta como intento; ojo mostrar/ocultar
#   en el PIN y en la passphrase de Fabricante. (4) Boton "Reiniciar equipo" con
#   alto de contenido.
# 1.5.34-dev (2026-10-11), tras probar 1.5.33 en HW: tarjetas (ui_card) y pildoras
#   NO clickeables por defecto (las informativas reaccionaban al toque); Informacion
#   con scroll vertical (los botones de restablecer quedaban fuera de los 320 px) y
#   tarjetas de calibracion/mantenimiento ajustadas al contenido.
# 1.5.33-dev (2026-10-11), tras probar 1.5.32 en HW: (1) Calentamiento del FS7: el
#   criterio de 3 s daba por estable una deriva lenta (corte real: 2.41 L/min falsos a
#   los 16.8 s que sumaron ~1.1 L). Ahora medias de 1 s y termina con flujo ~0 (3 s
#   < 0.05 L/min) o deriva baja en 20 s (<= max(0.1 L/min; 5 %)), max 180 s; el log
#   de 1 Hz muestra f= (media) y d20= (deriva). (2) Tarjeta de flujo: si el estado
#   (ESTABILIZANDO) y el 24H no caben, se oculta el 24H (antes se superponian).
#   (3) Acceso de Fabricante en el login: candado arriba a la derecha + passphrase
#   con teclado QWERTY (antes no habia forma de entrar). (4) Restablecer config. =
#   Administrador o Fabricante; Borrado total = solo Fabricante.
#   (5) Progreso del calentamiento en la tarjeta: valor "--" (no 0.00) y "calentando
#   N s" en la etiqueta central; la presion y sus alarmas siguen en vivo.
# 1.5.32-dev (2026-10-10): (1) Calentamiento del FS7: tras un corte de energia el
#   sensor termico arranca frio y el flujo "converge" a 0 desde valores altos (sumaba
#   ~1 L al totalizador). El flujo queda invalido (no suma, sin alarma clinica, sin
#   STALE) hasta que el filtrado se estabiliza 3 s (max 60 s); la tarjeta dice
#   ESTABILIZANDO. (2) Total del dia con un decimal ("0.4 L hoy") y fuente 28.
#   (3) Sin hora valida el reloj muestra "--:--"/"Sin fecha" (antes 1970).
#   (4) Restablecer de fabrica en dos niveles en Informacion (solo Fabricante,
#   doble toque): config (conserva identidad, nube y certificados) o borrado total
#   (ademas certificados AWS, token LAN y vinculos BLE).
# 1.5.31-dev (2026-10-08): el totalizador de consumo integraba flujo CRUDO mientras
#   la mediana de 5 se llenaba (4 muestras al arrancar): saltos del ADS1115 en el
#   arranque sumaban ~0.9 L sin flujo. Ahora esas muestras son flujo invalido.
# 1.5.30-dev (2026-10-01): malloc() prefiere PSRAM (SPIRAM_MALLOC_ALWAYSINTERNAL
#   8192 -> 0, igual que MedGuard): en el soak de 1.5.29 la RAM interna llego a
#   23 B al usar la HMI con Wi-Fi caida y el driver Wi-Fi aborto (ESP_ERR_NO_MEM,
#   43.6 h). Driver KSZ8851SNL vendorizado con el fix de desborde RX de MedGuard.
# 1.5.28-dev (2026-09-29): diagnostico (mem_diag/task_tracer) copia los nombres
#   de tarea en la foto: leer pcTaskName de una tarea borrada mientras se imprime
#   provoco panic en MedGuard (soak 12 h). Mismo riesgo latente aqui.
# 1.5.27-dev (2026-09-28): OTA por microSD con nombre versionado
#   fpm200_<version>.bin (el build lo genera; firmware.bin sigue valiendo; si hay
#   varios gana la version mas alta del descriptor). FIX de seguridad: se borra la
#   zona de firma vieja de la particion antes de verificar (un .bin SIN firmar
#   identico al firmado que ya ocupaba la particion heredaba su firma; HW 09-27).
# 1.5.26-dev (2026-09-26): OTA por microSD (components/ota_update, portado de
#   MedGuard): /sdcard/firmware.bin firmado -> valida proyecto + firma RSA,
#   instala en la otra particion (escritura secuencial) y pide reiniciar dentro
#   del overlay existente de microSD. Rollback automatico si no arranca. Sin HW.
# 1.5.25-dev (2026-09-26, paridad de seguridad con MedGuard 1.7.0-dev, SIN eFuses):
#   vuelta atras de OTA (BOOTLOADER_APP_ROLLBACK + confirmacion en main.c; grabar
#   bootloader por USB una vez), OTA firmada sin Secure Boot (clave en
#   firmware/keys/, tools/gen_ota_signing_key.sh), PIN/passphrase como hash
#   PBKDF2-SHA256 con migracion automatica, y el PIN ya no se borra cuando la app
#   reenvia la config redactada. Ver docs/SEGURIDAD_PRODUCCION.md. Sin HW.
# 1.5.5..1.5.15-dev (Codex + Claude, 2026-09-24): EEPROM byte-a-byte paceada,
#   consumo integrado, cero de presion v6 (+0.415 kPa), secuencia WiFi, y
#   1.5.15: completa migracion v5->v6 interrumpida (datos v6, CRC viejo v5).
#   1.5.16: TWDT con panic se MANTIENE; timeout 20 s solo durante esp_wifi_start
#   (calibracion PHY lenta causaba bootloop: panic antes de guardar cal en NVS).
#   1.5.17: PHY sigue colgada con 20 s (hang real, no lentitud). Se revierte el
#   pin a CPU0 de sensors_acq (NO_AFFINITY) y aws_pub (core 1) como en HEAD.
#   1.5.18: auditoria: alarma no evalua datos invalidos como 0 kPa, cJSON en PSRAM,
#   cache AppConfig con mutex y sin copias de 2 KB en stack, limites AWS vivos,
#   heap tracing OFF (~16 KB RAM interna), CPU 240 MHz, diagnostico de stalls.
#   1.5.19: FreeRTOS 1000 Hz (pacing EEPROM fijo en 10 ms), backoff de reconexion
#   WiFi 1->30 s, eliminado codigo muerto del pipe JSON BLE.
#   1.5.20: pantalla+splash ANTES de la verificacion EEPROM (touch sigue al final),
#   barra de splash animada (3.2 s), compilacion -O2 (PERF).
#   1.5.21: stack LVGL 7168->9216 (-O2 dejo 784 B libres), PSRAM 40->80 MHz,
#   strncpy->snprintf (errores -O2) y fix SSID 32 / PSK 64 recortados en wifi_config.
#   1.5.22: NVS 24->84 KB + particion coredump (requiere erase-region, ver HANDOFF),
#   WiFi storage RAM (sin escrituras NVS al conectar), UI solo redibuja cambios,
#   diagnostico de operaciones I2C lentas (i2c_guard).
#   1.5.23 (soak 18.8 h + calibracion vs SFM3300): curva FS7 v7 (EEPROM v6->v7),
#   mediana de 5 + confirmacion 500 ms en alarmas de flujo (ruido del TX WiFi),
#   SFM3300 rechaza 00 00 00, linea "Consumo:" cada 30 s, touch get_data.
#   1.5.24: telemetria AWS reporta la version real (antes "1.0.0" fijo).
#   Detalle en docs/historial/CONSUMPTION_FIXES.md.
# 1.5.4-dev: microSD (ui_sd + sd_monitor) + BT-off REAL en "Configurar por app".
#   microSD (main.c/ui_sd): (1) el overlay ahora tiene botón "Cerrar" SIEMPRE
#   visible (antes solo aparecía "Aplicar y reiniciar" cuando había cambios y la
#   ventana quedaba atrapada); (2) el FALLO de montaje ahora se MUESTRA en pantalla
#   (ui_sd_error, insignia/texto rojo) — antes hacía `continue` sin UI y el usuario
#   no sabía qué pasaba; (3) FIX del storm "0x105 no available sd host controller":
#   se DESMONTA la tarjeta apenas termina la lectura (AppConfig/certs ya están en
#   NVS) para liberar el host SDMMC, y una bandera s_sd_flow_active + drenado del
#   semáforo bloquean los rebotes de la interrupción CD (que disparaban re-montajes
#   mientras el host seguía ocupado). El botón "Cerrar" desmonta + limpia el flujo.
#   BT: el botón grande "Apagar Bluetooth" de la pantalla "Configurar por app"
#   (ui_bleAppScreen) ahora PERSISTE bt.enabled=false y REINICIA (igual que
#   ui_netBleScreen y MedGuard). Antes solo volvía al dashboard: el BT seguía
#   "activo" y AWS no reconectaba hasta reiniciar a mano. (Este es el botón que el
#   usuario pulsaba; el reinicio de ui_netBleScreen ya estaba, era otra pantalla.)
# 1.5.3-dev: mismo glitch de I2C ahora en PRESIÓN (MS5803): un sample devolvió
#   ESP_OK con dato CORRUPTO (-858 kPa, real ~-1) -> SENSOR_FAULT_INVALID -> alarma.
#   Dos sensores del bus 1 glicheando (ADS flujo + MS5803 presión, y SFM clavado)
#   confirma inestabilidad del BUS I2C 1. FIX: mediana de 3 también en presión
#   (mismo patrón que flujo) + log "GLITCH presion descartado". Software = red de
#   seguridad; causa raíz de bus pendiente (HW: pull-ups, bajar clock I2C, cableado).
#   + BT: al APAGAR el Bluetooth (ui_netBleScreen) el equipo REINICIA — set_enabled(false)
#   no libera NimBLE ni restaura red/AWS (quedaba "activo" y sin AWS hasta reiniciar);
#   bt.enabled=false ya está en NVS, así arranca limpio (igual que MedGuard).
#   + Pantalla Información: se quitó la tarjeta "Tiempo en servicio"; calibración/
#   mantenimiento ahora en 2 tarjetas compactas (alto de contenido, sin estirar);
#   nuevo botón "Reiniciar equipo" (confirmación por doble toque).
# 1.5.2-dev: FIX "alarma fantasma de flujo" = glitch de UN sample del ADS1115
#   (con flujo real ~22 L/min salía una muestra a 0.00 -> disparaba flow_delta).
#   Confirmado con el log alarm_mgr (F=0.00, flow_warn=1). FIX: filtro de MEDIANA
#   DE 3 sobre el flujo en sensors_runtime -> el outlier aislado se descarta antes
#   del display Y de la máquina de alarmas (1 muestra de lag; un cambio real
#   sostenido sí pasa). Log WARN "GLITCH flujo descartado" cuando el filtro actúa.
#   ADEMÁS diagnóstico de CAUSA RAÍZ (bus vs ADC) en ads1115_read_raw: loguea cada
#   intento I2C fallido (aunque el reintento lo recupere) y un "SALTO ADC" con el
#   código crudo/delta/reintentos -> distingue conversión mala del ADC (I2C OK) de
#   inestabilidad del bus (NACK/timeout). El filtro es la red de seguridad; los
#   logs dicen si hay que arreglar el bus (pull-ups/cableado del bus 1 compartido).
# 1.5.1-dev: LAN /state lee lecturas VIVAS (sensors_runtime_get_last; g_state nunca
#   se actualizaba -> 0s en la app) + decimales: pantalla FPM flujo=2 dec y presión
#   por unidad (psi/bar=2, kPa=1; evita "-0 psi"/"1 lpm"); /state envía decimals
#   flujo=2/presión=1. Diagnóstico phantom: alarm_mgr loguea el valor y la condición
#   exacta al entrar en alarma (P/F + p<min/p>max/flow_high/flow_warn/faults).
#   PRESENTACIÓN: EMA (alpha 0.25) sobre el número mostrado de presión/flujo en el
#   dashboard (refresca a 5 Hz sobre lecturas ruidosas -> con 2 dec saltaba feo). El
#   suavizado es SOLO visual; la máquina de alarmas usa el valor crudo.
# 1.5.0-dev: PARIDAD DE CONECTIVIDAD CON MEDGUARD + fixes de HW (este ciclo).
#   - mDNS: publica <serial>.local + _fpm._tcp (network_core/mdns_svc).
#   - LAN config REAL: http_api cfg_get/cfg_put reusan fpm_ble_config_read/apply_json
#     (mismo contrato set_config que BLE); +POST; token LAN (Bearer) en /config.
#   - LAN monitoreo: nuevas rutas /api/v1/alarms y /logs (evita 404 que tumbaba la
#     conexion de la app); /api/v1/state ahora emite channels[] (Presion/Flujo) en
#     el esquema de la app (antes {flow,pressure} plano -> dashboard vacio).
#   - BLE: bloque `lan` (ip/puerto/token/host=<serial>.local/id) en info -> la app
#     marca hasLan y descubre por nombre.
#   - AWS: FIX recuperacion tras BLE/config — config_mode_exit ya NO arranca MQTT
#     antes de tener IP; net_core relanza SNTP al (re)obtener IP y el callback de
#     sync re-arma MQTT (en contexto seguro; la llamada directa desde el event loop
#     reventaba la pila -> crash 'Unhandled debug exception', revertida).
#   - TOUCH: FT5x06 forzado a modo INT POLLING (G_MODE 0xA4=0). El default
#     trigger/pulso hacia que el INT-gating por nivel perdiera toques (touch torpe).
#   - Diagnostico "alarma fantasma de flujo": ADS1115 descarta conversion no lista
#     (OS!=1) y loguea; sensors_runtime avisa flujo alto sospechoso con v_ain0.
# 1.4.0: sensores reales estables (SFM3300 leyendo, touch tolerante a I2C,
# recuperacion de bus, 1er paso calibracion FS7, decimales de dashboard).
set(PROJECT_VER "1.6.8-dev")
project(esp32s3_hmi_skeleton)

# Copia del binario FIRMADO con nombre versionado para la microSD:
# build/fpm200_<version>.bin (p.ej. fpm200_1.5.27.bin). El equipo valida
# proyecto, version y firma que van DENTRO de la imagen; el nombre es para las
# personas. No se genera desde el -unsigned.
add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin"
            "${CMAKE_BINARY_DIR}/fpm200_${PROJECT_VER}.bin"
    COMMENT "Firmware para microSD: build/fpm200_${PROJECT_VER}.bin"
    VERBATIM)
