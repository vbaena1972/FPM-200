cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
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
#   Detalle en firmware/CONSUMPTION_FIXES.md y docs/HANDOFF.md.
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
set(PROJECT_VER "1.5.23-dev")
project(esp32s3_hmi_skeleton)
