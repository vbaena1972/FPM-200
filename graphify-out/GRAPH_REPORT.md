# Graph Report - FPM-200  (2026-09-24)

## Corpus Check
- 170 files · ~337,668 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1237 nodes · 3147 edges · 86 communities (81 shown, 5 thin omitted)
- Extraction: 69% EXTRACTED · 31% INFERRED · 0% AMBIGUOUS · INFERRED: 977 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `2d2dd33f`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_statusbar_request_refresh
- wifi_mgr.c
- save_cb
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
- Matriz de casos
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
- esp_timer.h
- ui_usersScreen.c
- general_loaded_cb
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- ui_bleAppScreen.c
- net_core.c
- ui_statusbar_controller.c
- appcfg_cache_peek
- ui_cfg.c
- ui_wifi_main_icon.c
- sensors_acq_task
- ui_nav_load
- main.c
- app_main
- fpm_i2c_guard.c
- ui_keypadScreen.c
- test_alarm.c
- README.md
- transport_mqtt.c
- ui_pinScreen.c
- bsp_touch_new
- mem_diag.c
- save_and_refresh
- gas_label_color
- task_tracer.c
- esp_err_to_name
- screen_init_task
- esp_log.h
- ui_auth_can
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- fresh_login
- Consumo y endurecimiento 1.5.5-dev
- ui_form.c
- at24c256.c
- rtc_rv3028.c
- ui_userEditScreen.c
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

## Communities (86 total, 5 thin omitted)

### Community 0 - "ui_statusbar_request_refresh"
Cohesion: 0.14
Nodes (18): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, wifi_status_t, log_dns_info() (+10 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.16
Nodes (19): apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache() (+11 more)

### Community 2 - "save_cb"
Cohesion: 0.16
Nodes (15): conn_status_t, eth_mgr_is_up(), cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick() (+7 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.18
Nodes (25): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute() (+17 more)

### Community 5 - "ui_mainScreen.c"
Cohesion: 0.11
Nodes (42): card_state_t, alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual(), banner_open_cb() (+34 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.14
Nodes (20): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+12 more)

### Community 7 - "driver_delay_ms"
Cohesion: 0.42
Nodes (11): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 8 - "storage.c"
Cohesion: 0.13
Nodes (40): cfg_result_t, esp_event_base_t, mqtt_event(), mqtt_start_locked(), prepare_pem(), appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+32 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.18
Nodes (16): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_refresh(), volume_cb(), limit_edit() (+8 more)

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
Cohesion: 0.08
Nodes (93): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+85 more)

### Community 17 - "Matriz de casos"
Cohesion: 0.10
Nodes (19): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+11 more)

### Community 18 - "ui.c"
Cohesion: 0.08
Nodes (45): ui_bleAppScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_netWifiScreen_screen_destroy() (+37 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.21
Nodes (22): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), is_press_unit() (+14 more)

### Community 20 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 21 - "http_api.c"
Cohesion: 0.18
Nodes (22): esp_err_t, mdns_svc_start(), add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t (+14 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.19
Nodes (23): aws_telemetry_task(), flow_meter_get(), sensor_sample_t, sensor_signal_stats_t, lock(), sensors_runtime_get_ages(), sensors_runtime_get_aws_telemetry(), sensors_runtime_get_debug() (+15 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.10
Nodes (20): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+12 more)

### Community 25 - "ui_loginScreen.c"
Cohesion: 0.33
Nodes (9): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+1 more)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.12
Nodes (25): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+17 more)

### Community 28 - "esp_timer.h"
Cohesion: 0.16
Nodes (9): device_state_t, flow_meter_record(), flow_integrator_push(), state_build_json(), state_get(), state_update_sensors(), close_to(), main() (+1 more)

### Community 29 - "ui_usersScreen.c"
Cohesion: 0.27
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 30 - "general_loaded_cb"
Cohesion: 0.22
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_auth_logout(), ui_cfg_brightness(), ui_cfg_dim_minutes() (+3 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "ui_bleAppScreen.c"
Cohesion: 0.14
Nodes (14): esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_goroot_async(), bleapp_leave_to_dashboard(), bleapp_loaded_cb(), bleapp_pair_pin() (+6 more)

### Community 34 - "net_core.c"
Cohesion: 0.29
Nodes (10): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), time_mgr_start_sntp() (+2 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.38
Nodes (9): transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb() (+1 more)

### Community 36 - "appcfg_cache_peek"
Cohesion: 0.12
Nodes (23): appcfg_cache_peek(), appcfg_save(), transport_ble_stop_adv(), bleapp_off_cb(), lv_event_t, save_cb(), f_switch(), ui_auth_user_count() (+15 more)

### Community 37 - "ui_cfg.c"
Cohesion: 0.13
Nodes (25): apply_authenticated_cb(), lv_event_t, lv_event_t, next(), app_user_t, ui_auth_factory(), ui_auth_login(), ui_auth_user_at() (+17 more)

### Community 38 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_nav_load"
Cohesion: 0.23
Nodes (13): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color(), lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb() (+5 more)

### Community 41 - "main.c"
Cohesion: 0.09
Nodes (24): esp_trace_open_params_t, transport_mqtt_publish_now(), ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), bsp_i2c_init(), alarm_clinical_state_t, esp_err_t, lv_event_t (+16 more)

### Community 42 - "app_main"
Cohesion: 0.18
Nodes (11): app_metrics_t, fpm_i2c_init(), transport_mqtt_runtime_init(), esp_err_t, flow_meter_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), appmetrics_load() (+3 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.24
Nodes (8): ui_confirmScreen_screen_destroy(), accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_edit_set_new(), fresh_confirm(), ui_open_confirm_cb()

### Community 45 - "test_alarm.c"
Cohesion: 0.16
Nodes (13): alarm_mgr_init(), esp_err_t, appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config() (+5 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.24
Nodes (9): time_mgr_set_datetime(), add_channel(), cJSON, sensor_signal_stats_t, mqtt_stop_locked(), publish_alarm_transition(), transport_mqtt_on_time_ready(), transport_mqtt_stop() (+1 more)

### Community 50 - "ui_pinScreen.c"
Cohesion: 0.27
Nodes (8): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 51 - "bsp_touch_new"
Cohesion: 0.18
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 52 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str()

### Community 53 - "save_and_refresh"
Cohesion: 0.25
Nodes (11): gas_cb(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas() (+3 more)

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 56 - "esp_err_to_name"
Cohesion: 0.18
Nodes (19): ads1115_is_ready(), at24c256_is_ready(), ms5803_is_ready(), time_sync_worker(), flow_meter_service(), AppConfig, esp_err_t, crc16_ccitt() (+11 more)

### Community 57 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 60 - "ui_auth_can"
Cohesion: 0.35
Nodes (11): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+3 more)

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 62 - "fresh_login"
Cohesion: 0.22
Nodes (10): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb() (+2 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.08
Nodes (25): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, Audit and fixes: 1.5.18-dev (Claude, 2026-09-24), Batch 2: 1.5.20-dev (Claude) (+17 more)

### Community 66 - "ui_form.c"
Cohesion: 0.24
Nodes (4): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 67 - "at24c256.c"
Cohesion: 0.53
Nodes (8): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), main()

### Community 69 - "rtc_rv3028.c"
Cohesion: 0.33
Nodes (8): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init()

### Community 70 - "ui_userEditScreen.c"
Cohesion: 0.48
Nodes (6): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role()

## Knowledge Gaps
- **100 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+95 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **5 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_statusbar_request_refresh`, `save_cb`, `alarm_mgr.c`, `ui_mainScreen.c`, `storage.c`, `ui_sensorDiagScreen.c`, `cert_store.c`, `ui_label`, `ui_sensorEditScreen.c`, `fpm_ble_config.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `general_loaded_cb`, `ui_cfg.c`, `app_main`, `save_and_refresh`, `gas_label_color`, `screen_init_task`, `ui_auth_can`?**
  _High betweenness centrality (0.146) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `ui_statusbar_request_refresh`, `wifi_mgr.c`, `esp_err.h`, `at24c256.c`, `rtc_rv3028.c`, `transport_ble.c`, `driver_delay_ms`, `sensors_acq_task`, `ads1115.c`, `bmp280.c`, `fpm_i2c_guard.c`, `main.c`, `cert_store.c`, `bsp_touch_new`, `fpm_ble_config.c`, `http_api.c`, `screen_init_task`, `eth_mgr.c`?**
  _High betweenness centrality (0.052) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_statusbar_request_refresh`, `appcfg_cache_peek`, `rtc_rv3028.c`, `ui_wifi_main_icon.c`, `sensors_acq_task`, `main.c`, `bsp_touch_new`, `esp_err_to_name`?**
  _High betweenness centrality (0.049) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 41 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 41 INFERRED edges - model-reasoned connections that need verification._