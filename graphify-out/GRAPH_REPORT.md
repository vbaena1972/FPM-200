# Graph Report - FPM-200  (2026-09-24)

## Corpus Check
- 169 files · ~334,924 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1208 nodes · 3083 edges · 79 communities (75 shown, 4 thin omitted)
- Extraction: 68% EXTRACTED · 32% INFERRED · 0% AMBIGUOUS · INFERRED: 974 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `a25efbf6`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- save_cb
- esp_err_to_name
- ui_connectivityScreen_screen_init
- lv_port_mem.c
- alarm_mgr.c
- rtc_rv3028.c
- transport_ble.c
- ms5803.c
- fpm_ble_config.c
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
- vTaskDelay
- http_api.c
- app_main
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- example_eth_init
- ws_stream.c
- eth_mgr.c
- flow_meter.c
- ui_usersScreen.c
- ui_userEditScreen.c
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- ui_generalSimpleScreen.c
- on_eth_event
- ui_statusbar_controller.c
- appcfg_cache_peek
- ui_auth_can
- ui_form.c
- sensors_acq_task
- ui_nav.c
- main.c
- next
- fpm_i2c_guard.c
- save_cb
- test_alarm.c
- README.md
- transport_mqtt.c
- esp_log.h
- mem_diag.c
- appmetrics_load
- ui_cfg.c
- task_tracer.c
- sensors_runtime.c
- state_pub.c
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- ui_cfg_preview_brightness
- Consumo y endurecimiento 1.5.5-dev
- AGENTS.md
- CLAUDE.md

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 56 edges
3. `ui_col()` - 44 edges
4. `ui_box()` - 41 edges
5. `esp_err_to_name()` - 41 edges
6. `_t()` - 35 edges
7. `ui_card()` - 33 edges
8. `appcfg_save()` - 32 edges
9. `ui_icon()` - 30 edges
10. `ui_sensorEditScreen_screen_init()` - 24 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c
- `state_build_json()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c
- `statusbar_timer_cb()` --calls--> `transport_ble_is_advertising()`  [INFERRED]
  firmware/components/ui_bind/ui_statusbar_controller.c → firmware/components/transport_ble/transport_ble.c
- `get_bleapp()` --calls--> `ui_bleAppScreen_screen_destroy()`  [INFERRED]
  firmware/main/ui/ui.c → firmware/main/ui/screens/ui_bleAppScreen.c

## Import Cycles
- None detected.

## Communities (79 total, 4 thin omitted)

### Community 0 - "save_cb"
Cohesion: 0.15
Nodes (12): eth_mgr_is_up(), esp_event_base_t, wifi_status_t, log_dns_info(), notify(), wifi_event_handler(), wifi_mgr_stop(), wifi_schedule_retry() (+4 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (23): esp_err_t, mdns_svc_start(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending() (+15 more)

### Community 2 - "ui_connectivityScreen_screen_init"
Cohesion: 0.29
Nodes (12): conn_status_t, lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct(), str_hash() (+4 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.17
Nodes (26): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+18 more)

### Community 5 - "rtc_rv3028.c"
Cohesion: 0.33
Nodes (8): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init()

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (43): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+35 more)

### Community 7 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 8 - "fpm_ble_config.c"
Cohesion: 0.08
Nodes (75): cfg_result_t, esp_event_base_t, mqtt_event(), appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+67 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.44
Nodes (8): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t, i2c_master_bus_handle_t

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.20
Nodes (15): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb(), ui_edit_target_t (+7 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "ui_label"
Cohesion: 0.06
Nodes (129): card_state_t, sensors_runtime_get_debug(), ui_refresh_task(), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t (+121 more)

### Community 17 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 18 - "ui.c"
Cohesion: 0.06
Nodes (70): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), app_user_role_t, app_user_t, lv_event_t (+62 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.18
Nodes (25): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), f_switch(), flow_card() (+17 more)

### Community 20 - "vTaskDelay"
Cohesion: 0.28
Nodes (13): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), driver_delay_ms() (+5 more)

### Community 21 - "http_api.c"
Cohesion: 0.26
Nodes (19): add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+11 more)

### Community 22 - "app_main"
Cohesion: 0.17
Nodes (23): fpm_i2c_init(), aws_telemetry_task(), transport_mqtt_runtime_init(), esp_err_t, flow_meter_get(), flow_meter_init(), sensor_sample_t, lock() (+15 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.10
Nodes (20): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+12 more)

### Community 25 - "example_eth_init"
Cohesion: 0.36
Nodes (10): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, esp_err_t, eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init() (+2 more)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.25
Nodes (12): eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage() (+4 more)

### Community 28 - "flow_meter.c"
Cohesion: 0.27
Nodes (7): time_sync_worker(), flow_meter_record(), flow_meter_service(), flow_integrator_push(), close_to(), main(), flow_integrator_t

### Community 29 - "ui_usersScreen.c"
Cohesion: 0.27
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 30 - "ui_userEditScreen.c"
Cohesion: 0.33
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "ui_generalSimpleScreen.c"
Cohesion: 0.18
Nodes (12): lv_event_t, protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general() (+4 more)

### Community 34 - "on_eth_event"
Cohesion: 0.18
Nodes (16): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event() (+8 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.36
Nodes (10): activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_controller_init() (+2 more)

### Community 36 - "appcfg_cache_peek"
Cohesion: 0.15
Nodes (21): appcfg_cache_peek(), is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_cfg_alarm_tone() (+13 more)

### Community 37 - "ui_auth_can"
Cohesion: 0.18
Nodes (17): apply_authenticated_cb(), lv_event_t, ui_sensorDiagScreen_refresh(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+9 more)

### Community 38 - "ui_form.c"
Cohesion: 0.24
Nodes (4): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 39 - "sensors_acq_task"
Cohesion: 0.18
Nodes (16): ads1115_is_ready(), bmp280_is_ready(), ms5803_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init() (+8 more)

### Community 40 - "ui_nav.c"
Cohesion: 0.39
Nodes (7): lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to()

### Community 41 - "main.c"
Cohesion: 0.06
Nodes (48): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+40 more)

### Community 42 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.30
Nodes (13): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+5 more)

### Community 44 - "save_cb"
Cohesion: 0.40
Nodes (4): lv_event_t, save_cb(), set_str(), ui_netEthScreen_screen_destroy()

### Community 45 - "test_alarm.c"
Cohesion: 0.18
Nodes (12): set_buzzer_acoustic(), appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config(), ledc_set_duty() (+4 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.21
Nodes (12): time_mgr_set_datetime(), add_channel(), cJSON, sensor_signal_stats_t, cloud_mgr_connected(), mqtt_start_locked(), mqtt_stop_locked(), prepare_pem() (+4 more)

### Community 51 - "esp_log.h"
Cohesion: 0.13
Nodes (14): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+6 more)

### Community 52 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str()

### Community 53 - "appmetrics_load"
Cohesion: 0.47
Nodes (4): app_metrics_t, appmetrics_load(), appmetrics_store(), esp_err_t

### Community 54 - "ui_cfg.c"
Cohesion: 0.15
Nodes (24): gas_label_color(), app_user_t, ui_auth_factory(), ui_auth_login(), ui_auth_user_at(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_flow_decimals() (+16 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.27
Nodes (6): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 56 - "sensors_runtime.c"
Cohesion: 0.16
Nodes (20): at24c256_is_ready(), AppConfig, esp_err_t, i2c_master_bus_handle_t, sensor_signal_stats_t, crc16_ccitt(), sensor_cfg_persist(), sensor_cfg_set_defaults() (+12 more)

### Community 57 - "state_pub.c"
Cohesion: 0.33
Nodes (4): device_state_t, state_build_json(), state_get(), state_update_sensors()

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 62 - "ui_cfg_preview_brightness"
Cohesion: 0.32
Nodes (8): bright_cb(), lv_timer_t, clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb(), splash_done_cb()

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.09
Nodes (22): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, Audit and fixes: 1.5.18-dev (Claude, 2026-09-24), Batch 2: 1.5.20-dev (Claude), Batch 3: 1.5.21-dev (Claude), Consumo y endurecimiento 1.5.5-dev (+14 more)

## Knowledge Gaps
- **83 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+78 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **4 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `save_cb`, `esp_err_to_name`, `ui_connectivityScreen_screen_init`, `alarm_mgr.c`, `transport_ble.c`, `fpm_ble_config.c`, `ui_sensorDiagScreen.c`, `cert_store.c`, `ui_label`, `ui_cfg_apply_visual_mode`, `ui.c`, `ui_sensorEditScreen.c`, `http_api.c`, `app_main`, `ui_generalSimpleScreen.c`, `ui_auth_can`, `main.c`, `next`, `save_cb`, `ui_cfg.c`, `ui_cfg_preview_brightness`?**
  _High betweenness centrality (0.152) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `ui_statusbar_controller.c`, `appcfg_cache_peek`, `rtc_rv3028.c`, `sensors_acq_task`, `ui_label`, `esp_log.h`, `vTaskDelay`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `save_cb`, `esp_err.h`, `rtc_rv3028.c`, `transport_ble.c`, `ms5803.c`, `sensors_acq_task`, `main.c`, `ads1115.c`, `bmp280.c`, `fpm_ble_config.c`, `cert_store.c`, `esp_log.h`, `vTaskDelay`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `flow_meter.c`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 35 INFERRED edges - model-reasoned connections that need verification._