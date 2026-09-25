# Graph Report - FPM-200  (2026-09-24)

## Corpus Check
- 169 files · ~335,773 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1216 nodes · 3127 edges · 80 communities (77 shown, 3 thin omitted)
- Extraction: 69% EXTRACTED · 31% INFERRED · 0% AMBIGUOUS · INFERRED: 977 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `3f536c21`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_statusbar_request_refresh
- wifi_mgr.c
- ui_connectivityScreen.c
- lv_port_mem.c
- alarm_mgr.c
- ui_mainScreen.c
- transport_ble.c
- driver_delay_ms
- storage.c
- ads1115.c
- bmp280.c
- ui.h
- ui_sensorDiagScreen.c
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- ui_label
- ui_cfg_apply_visual_mode
- ui.c
- ui_sensorEditScreen.c
- fpm_ble_config.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_loginScreen.c
- ws_stream.c
- eth_mgr.c
- flow_meter.c
- ui_usersScreen.c
- apply_authenticated_cb
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- ui_generalSimpleScreen.c
- net_core.c
- ui_statusbar_controller.c
- appcfg_cache_peek
- ui_cfg.c
- ui_wifi_main_icon.c
- sensors_acq_task
- ui_nav.c
- main.c
- app_main
- fpm_i2c_guard.c
- ui_keypadScreen.c
- semphr.h
- README.md
- transport_mqtt.c
- ui_pinScreen.c
- esp_log.h
- mem_diag.c
- save_and_refresh
- gas_label_color
- task_tracer.c
- esp_err_to_name
- state_pub.c
- save_cb
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- Consumo y endurecimiento 1.5.5-dev
- AGENTS.md
- CLAUDE.md

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 56 edges
3. `ui_col()` - 44 edges
4. `esp_err_to_name()` - 42 edges
5. `ui_box()` - 41 edges
6. `_t()` - 35 edges
7. `ui_card()` - 33 edges
8. `appcfg_save()` - 32 edges
9. `ui_icon()` - 30 edges
10. `ui_sensorEditScreen_screen_init()` - 24 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c
- `state_build_json()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (80 total, 3 thin omitted)

### Community 0 - "ui_statusbar_request_refresh"
Cohesion: 0.17
Nodes (16): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, wifi_status_t, log_dns_info() (+8 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.18
Nodes (17): apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache() (+9 more)

### Community 2 - "ui_connectivityScreen.c"
Cohesion: 0.24
Nodes (11): conn_status_t, cloud_mgr_connected(), wifi_mgr_get_netinfo(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick() (+3 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.20
Nodes (23): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute() (+15 more)

### Community 5 - "ui_mainScreen.c"
Cohesion: 0.11
Nodes (43): card_state_t, ui_refresh_task(), alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual() (+35 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (41): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+33 more)

### Community 7 - "driver_delay_ms"
Cohesion: 0.38
Nodes (11): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 8 - "storage.c"
Cohesion: 0.12
Nodes (41): cfg_result_t, aws_telemetry_task(), esp_event_base_t, mqtt_event(), mqtt_start_locked(), prepare_pem(), publish_alarm_transition(), appcfg_cache_get() (+33 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "ui.h"
Cohesion: 0.09
Nodes (6): ui_datetimeScreen_screen_destroy(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), get_datetime()

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.20
Nodes (15): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_refresh(), volume_cb(), ui_edit_target_t (+7 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.21
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 16 - "ui_label"
Cohesion: 0.09
Nodes (86): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+78 more)

### Community 17 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 18 - "ui.c"
Cohesion: 0.09
Nodes (49): ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_generalScreen_screen_destroy(), ui_infoScreen_screen_destroy(), menu_item_cb(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy() (+41 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.17
Nodes (27): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+19 more)

### Community 20 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (37): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+29 more)

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): flow_meter_get(), add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.20
Nodes (22): AppConfig, sensor_sample_t, sensor_signal_stats_t, lock(), sensors_runtime_get_ages(), sensors_runtime_get_aws_telemetry(), sensors_runtime_get_debug(), sensors_runtime_get_faults() (+14 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.10
Nodes (20): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+12 more)

### Community 25 - "ui_loginScreen.c"
Cohesion: 0.23
Nodes (13): add_user(), app_user_role_t, app_user_t, lv_event_t, lv_obj_t, key_cb(), refresh(), role_color() (+5 more)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.12
Nodes (25): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+17 more)

### Community 28 - "flow_meter.c"
Cohesion: 0.12
Nodes (16): app_metrics_t, time_mgr_set_datetime(), time_sync_worker(), transport_mqtt_on_time_ready(), esp_err_t, flow_meter_init(), flow_meter_record(), flow_meter_service() (+8 more)

### Community 29 - "ui_usersScreen.c"
Cohesion: 0.22
Nodes (13): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name() (+5 more)

### Community 30 - "apply_authenticated_cb"
Cohesion: 0.15
Nodes (13): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt(), ui_cfg_flow_to_disp() (+5 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "ui_generalSimpleScreen.c"
Cohesion: 0.33
Nodes (8): lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy(), get_general_simple()

### Community 34 - "net_core.c"
Cohesion: 0.29
Nodes (10): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), time_mgr_start_sntp() (+2 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.38
Nodes (9): activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_set_enabled() (+1 more)

### Community 36 - "appcfg_cache_peek"
Cohesion: 0.12
Nodes (25): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), bright_cb(), f_switch(), clamp_brightness(), ui_auth_user_count(), ui_cfg_alarm_tone() (+17 more)

### Community 37 - "ui_cfg.c"
Cohesion: 0.15
Nodes (26): lv_event_t, next(), app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+18 more)

### Community 38 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): ads1115_is_ready(), ms5803_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready() (+8 more)

### Community 40 - "ui_nav.c"
Cohesion: 0.16
Nodes (15): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), lv_event_t, lv_obj_t (+7 more)

### Community 41 - "main.c"
Cohesion: 0.07
Nodes (43): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+35 more)

### Community 42 - "app_main"
Cohesion: 0.20
Nodes (8): fpm_i2c_init(), esp_err_t, mdns_svc_start(), transport_mqtt_runtime_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), app_main(), on_network_ready()

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.24
Nodes (8): accept_cb(), allow_decimal(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new(), fresh_keypad()

### Community 45 - "semphr.h"
Cohesion: 0.14
Nodes (15): alarm_mgr_init(), esp_err_t, set_buzzer_acoustic(), xSemaphoreCreateRecursiveMutex(), appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig (+7 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.38
Nodes (5): add_channel(), cJSON, sensor_signal_stats_t, mqtt_stop_locked(), transport_mqtt_stop()

### Community 50 - "ui_pinScreen.c"
Cohesion: 0.28
Nodes (7): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin()

### Community 51 - "esp_log.h"
Cohesion: 0.13
Nodes (14): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+6 more)

### Community 52 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str()

### Community 53 - "save_and_refresh"
Cohesion: 0.33
Nodes (9): AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 56 - "esp_err_to_name"
Cohesion: 0.22
Nodes (21): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+13 more)

### Community 57 - "state_pub.c"
Cohesion: 0.33
Nodes (4): device_state_t, state_build_json(), state_get(), state_update_sensors()

### Community 58 - "save_cb"
Cohesion: 0.29
Nodes (5): eth_mgr_is_up(), ui_connectivityScreen_refresh(), lv_event_t, save_cb(), set_str()

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.08
Nodes (24): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, Audit and fixes: 1.5.18-dev (Claude, 2026-09-24), Batch 2: 1.5.20-dev (Claude), Batch 3: 1.5.21-dev (Claude) (+16 more)

## Knowledge Gaps
- **84 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+79 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_connectivityScreen.c`, `alarm_mgr.c`, `ui_mainScreen.c`, `transport_ble.c`, `storage.c`, `ui_sensorDiagScreen.c`, `cert_store.c`, `ui_label`, `ui_cfg_apply_visual_mode`, `ui_sensorEditScreen.c`, `fpm_ble_config.c`, `http_api.c`, `eth_mgr.c`, `apply_authenticated_cb`, `ui_statusbar_controller.c`, `ui_cfg.c`, `main.c`, `app_main`, `save_and_refresh`, `gas_label_color`, `save_cb`?**
  _High betweenness centrality (0.154) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `ui_statusbar_request_refresh`, `wifi_mgr.c`, `esp_err.h`, `transport_ble.c`, `driver_delay_ms`, `sensors_acq_task`, `main.c`, `bmp280.c`, `ads1115.c`, `fpm_i2c_guard.c`, `app_main`, `cert_store.c`, `esp_log.h`, `fpm_ble_config.c`, `http_api.c`, `eth_mgr.c`, `flow_meter.c`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `ui_statusbar_request_refresh`, `ui_statusbar_controller.c`, `appcfg_cache_peek`, `ui_wifi_main_icon.c`, `sensors_acq_task`, `esp_log.h`, `esp_err_to_name`?**
  _High betweenness centrality (0.051) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 41 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 41 INFERRED edges - model-reasoned connections that need verification._