# Graph Report - FPM-200  (2026-09-29)

## Corpus Check
- 177 files · ~346,276 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1293 nodes · 3250 edges · 88 communities (83 shown, 5 thin omitted)
- Extraction: 69% EXTRACTED · 31% INFERRED · 0% AMBIGUOUS · INFERRED: 995 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `10b627ae`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- save_cb
- esp_err_to_name
- ui_pin_hash.c
- lv_port_mem.c
- alarm_mgr.c
- ui_wifi_main_icon.c
- transport_ble.c
- ms5803.c
- storage.c
- ads1115.c
- bmp280.c
- ui.h
- ui_sensorDiagScreen_screen_init
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- ui_label
- Matriz de casos
- mem_diag.c
- ui_sensorEditScreen.c
- fpm_ble_config.c
- http_api.c
- esp_timer_get_time
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- ws_stream.c
- eth_mgr.c
- esp_timer.h
- ui_usersScreen.c
- example_eth_init
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- on_eth_event
- net_core.c
- ui_statusbar_controller.c
- ui_cfg.c
- apply_authenticated_cb
- ui_generalSimpleScreen.c
- sensors_acq_task
- state_pub.c
- main.c
- appmetrics_load
- fpm_i2c_guard.c
- ui_keypadScreen.c
- test_alarm.c
- README.md
- transport_mqtt.c
- gen_ota_signing_key.sh script
- bsp_touch_new
- ui_datetimeScreen.c
- gas_label_color
- esp_log.h
- sensors_runtime.c
- ui_nav_back
- time_mgr.c
- ui_auth_can
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- ui.c
- Consumo y endurecimiento 1.5.5-dev
- ui_theme.h
- driver_delay_ms
- on_network_ready
- ui_userEditScreen.c
- AGENTS.md
- CLAUDE.md
- ui_netEthScreen.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 56 edges
3. `esp_err_to_name()` - 45 edges
4. `ui_col()` - 44 edges
5. `ui_box()` - 41 edges
6. `_t()` - 35 edges
7. `appcfg_save()` - 33 edges
8. `ui_card()` - 33 edges
9. `ui_icon()` - 30 edges
10. `sensors_acq_task()` - 24 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `fpm_i2c_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/drivers/fpm_i2c_guard.c
- `app_main()` --calls--> `transport_mqtt_runtime_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/transport_mqtt.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c
- `sensors_runtime_get_aws_telemetry()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/tests/host/test_alarm.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (88 total, 5 thin omitted)

### Community 0 - "save_cb"
Cohesion: 0.22
Nodes (10): eth_mgr_is_up(), esp_event_base_t, wifi_status_t, log_dns_info(), notify(), wifi_event_handler(), wifi_mgr_stop(), wifi_schedule_retry() (+2 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.17
Nodes (20): apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache() (+12 more)

### Community 2 - "ui_pin_hash.c"
Cohesion: 0.25
Nodes (17): derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final(), sha256_init() (+9 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.17
Nodes (27): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+19 more)

### Community 5 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (52): ble_cmd_handler_t, conn_status_t, fpm_ble_config_set_clock_cb(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+44 more)

### Community 7 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 8 - "storage.c"
Cohesion: 0.14
Nodes (38): cfg_result_t, esp_event_base_t, mqtt_event(), appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+30 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.44
Nodes (8): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t, i2c_master_bus_handle_t

### Community 11 - "ui.h"
Cohesion: 0.14
Nodes (3): ui_confirmScreen_screen_destroy(), set_str(), ui_netWifiScreen_screen_destroy()

### Community 12 - "ui_sensorDiagScreen_screen_init"
Cohesion: 0.21
Nodes (16): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), ui_sensorDiagScreen_screen_init(), volume_cb() (+8 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.20
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 16 - "ui_label"
Cohesion: 0.06
Nodes (125): card_state_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), lv_obj_t (+117 more)

### Community 17 - "Matriz de casos"
Cohesion: 0.08
Nodes (24): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+16 more)

### Community 18 - "mem_diag.c"
Cohesion: 0.29
Nodes (6): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), task_state_str()

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.14
Nodes (30): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+22 more)

### Community 20 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (37): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+29 more)

### Community 21 - "http_api.c"
Cohesion: 0.26
Nodes (19): add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+11 more)

### Community 22 - "esp_timer_get_time"
Cohesion: 0.15
Nodes (26): mem_diag_start_periodic(), ads1115_is_ready(), ms5803_is_ready(), aws_telemetry_task(), esp_err_t, flow_meter_get(), flow_meter_init(), AppConfig (+18 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.08
Nodes (23): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+15 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.25
Nodes (12): eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage() (+4 more)

### Community 28 - "esp_timer.h"
Cohesion: 0.31
Nodes (5): flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t

### Community 29 - "ui_usersScreen.c"
Cohesion: 0.24
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 30 - "example_eth_init"
Cohesion: 0.36
Nodes (10): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, esp_err_t, eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init() (+2 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "on_eth_event"
Cohesion: 0.33
Nodes (7): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop()

### Community 34 - "net_core.c"
Cohesion: 0.29
Nodes (10): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), time_mgr_start_sntp() (+2 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.36
Nodes (10): activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_controller_init() (+2 more)

### Community 36 - "ui_cfg.c"
Cohesion: 0.12
Nodes (41): appcfg_cache_peek(), appcfg_save(), bright_cb(), f_switch(), app_user_t, AppConfig, clamp_brightness(), save_and_refresh() (+33 more)

### Community 37 - "apply_authenticated_cb"
Cohesion: 0.17
Nodes (13): apply_authenticated_cb(), lv_event_t, allow_decimal(), ui_sensorDiagScreen_refresh(), ui_sensorEditScreen_refresh(), ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_from_disp() (+5 more)

### Community 38 - "ui_generalSimpleScreen.c"
Cohesion: 0.29
Nodes (8): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy()

### Community 39 - "sensors_acq_task"
Cohesion: 0.17
Nodes (18): bmp280_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read() (+10 more)

### Community 40 - "state_pub.c"
Cohesion: 0.33
Nodes (4): device_state_t, state_build_json(), state_get(), state_update_sensors()

### Community 41 - "main.c"
Cohesion: 0.06
Nodes (51): esp_app_desc_t, esp_partition_t, esp_trace_open_params_t, FILE, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set() (+43 more)

### Community 42 - "appmetrics_load"
Cohesion: 0.47
Nodes (4): app_metrics_t, appmetrics_load(), appmetrics_store(), esp_err_t

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.35
Nodes (16): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_init() (+8 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 45 - "test_alarm.c"
Cohesion: 0.18
Nodes (11): appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config(), ledc_set_duty(), ledc_timer_config() (+3 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.24
Nodes (10): add_channel(), cJSON, sensor_signal_stats_t, cloud_mgr_connected(), mqtt_start_locked(), mqtt_stop_locked(), prepare_pem(), publish_alarm_transition() (+2 more)

### Community 51 - "bsp_touch_new"
Cohesion: 0.18
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 52 - "ui_datetimeScreen.c"
Cohesion: 0.29
Nodes (5): lv_event_t, next(), ui_datetimeScreen_screen_destroy(), ui_cfg_tz_count(), ui_cfg_tz_label()

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "esp_log.h"
Cohesion: 0.19
Nodes (8): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), ui_infoScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), TaskHandle_t

### Community 56 - "sensors_runtime.c"
Cohesion: 0.16
Nodes (17): esp_err_t, i2c_master_bus_handle_t, sensor_signal_stats_t, crc16_ccitt(), sensor_cfg_persist(), sensor_cfg_set_defaults(), sensors_runtime_cal_flow_point(), sensors_runtime_get_aws_telemetry() (+9 more)

### Community 57 - "ui_nav_back"
Cohesion: 0.17
Nodes (12): lv_event_t, general_loaded_cb(), protected_back_cb(), save_cb(), lv_event_t, save_cb(), lv_color_t, lv_obj_t (+4 more)

### Community 58 - "time_mgr.c"
Cohesion: 0.17
Nodes (13): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+5 more)

### Community 60 - "ui_auth_can"
Cohesion: 0.30
Nodes (12): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_logout(), ui_auth_max_assignable() (+4 more)

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 62 - "ui.c"
Cohesion: 0.06
Nodes (73): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+65 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 66 - "ui_theme.h"
Cohesion: 0.18
Nodes (4): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 67 - "driver_delay_ms"
Cohesion: 0.34
Nodes (12): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+4 more)

### Community 69 - "on_network_ready"
Cohesion: 0.40
Nodes (3): esp_err_t, mdns_svc_start(), on_network_ready()

### Community 70 - "ui_userEditScreen.c"
Cohesion: 0.39
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

## Knowledge Gaps
- **114 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+109 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **5 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `ui_cfg.c` to `save_cb`, `alarm_mgr.c`, `on_network_ready`, `transport_ble.c`, `apply_authenticated_cb`, `storage.c`, `main.c`, `ui_sensorDiagScreen_screen_init`, `ui_label`, `ui_sensorEditScreen.c`, `fpm_ble_config.c`, `http_api.c`, `esp_timer_get_time`, `gas_label_color`, `ui_nav_back`, `ui_auth_can`?**
  _High betweenness centrality (0.142) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `ui_statusbar_controller.c`, `ui_cfg.c`, `ui_wifi_main_icon.c`, `driver_delay_ms`, `sensors_acq_task`, `ui_label`, `bsp_touch_new`, `time_mgr.c`?**
  _High betweenness centrality (0.049) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `save_cb`, `transport_ble.c`, `ms5803.c`, `storage.c`, `ads1115.c`, `bmp280.c`, `cert_store.c`, `fpm_ble_config.c`, `http_api.c`, `esp_timer_get_time`, `eth_mgr.c`, `esp_err.h`, `sensors_acq_task`, `main.c`, `fpm_i2c_guard.c`, `bsp_touch_new`, `sensors_runtime.c`, `time_mgr.c`, `driver_delay_ms`, `on_network_ready`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 44 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._