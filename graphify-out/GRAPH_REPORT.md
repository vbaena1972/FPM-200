# Graph Report - FPM-200  (2026-09-25)

## Corpus Check
- 170 files · ~340,135 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1245 nodes · 3155 edges · 91 communities (84 shown, 7 thin omitted)
- Extraction: 69% EXTRACTED · 31% INFERRED · 0% AMBIGUOUS · INFERRED: 978 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `5bb4856e`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- wifi_mgr.c
- esp_err_to_name
- ui_connectivityScreen_screen_init
- lv_port_mem.c
- alarm_mgr.c
- ui_mainScreen_screen_destroy
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
- lv_obj_t
- ui_sensorEditScreen.c
- fpm_ble_config.c
- http_api.c
- esp_timer_get_time
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_loginScreen.c
- ws_stream.c
- eth_mgr.c
- esp_timer.h
- ui_usersScreen.c
- example_eth_init
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- ui_bleAppScreen.c
- net_core.c
- ui_statusbar_controller.c
- appcfg_cache_peek
- ui_cfg.c
- ui_generalSimpleScreen.c
- vTaskDelay
- ui_nav.c
- main.c
- appmetrics_load
- fpm_i2c_guard.c
- ui_keypadScreen.c
- test_alarm.c
- README.md
- transport_mqtt.c
- ui_pinScreen.c
- bsp_touch_new
- next
- save_and_refresh
- gas_label_color
- task_tracer.c
- sensors_runtime.c
- ui_cfg_apply_visual_mode
- esp_log.h
- ui_auth_can
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- ui.c
- Consumo y endurecimiento 1.5.5-dev
- ui_theme.h
- at24c256.c
- on_network_ready
- ui_nav_back
- AGENTS.md
- CLAUDE.md
- app_main
- save_cb
- sig_acc_finish
- lv_timer_t

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
10. `sensors_acq_task()` - 24 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `app_main()` --calls--> `fpm_i2c_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/drivers/fpm_i2c_guard.c
- `app_main()` --calls--> `transport_mqtt_runtime_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/transport_mqtt.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c

## Import Cycles
- None detected.

## Communities (91 total, 7 thin omitted)

### Community 0 - "wifi_mgr.c"
Cohesion: 0.13
Nodes (20): eth_mgr_is_up(), on_eth_event(), esp_event_base_t, esp_netif_t, wifi_status_t, log_dns_info(), notify(), wifi_apply_cb() (+12 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.23
Nodes (16): time_sync_worker(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache() (+8 more)

### Community 2 - "ui_connectivityScreen_screen_init"
Cohesion: 0.26
Nodes (13): conn_status_t, lv_event_cb_t, lv_obj_t, lv_timer_t, conn_card(), conn_signature(), cpy(), gather() (+5 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.17
Nodes (27): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute() (+19 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.11
Nodes (25): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+17 more)

### Community 7 - "driver_delay_ms"
Cohesion: 0.42
Nodes (11): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 8 - "storage.c"
Cohesion: 0.14
Nodes (37): cfg_result_t, esp_event_base_t, mqtt_event(), appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+29 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.33
Nodes (10): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+2 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.21
Nodes (13): apply_authenticated_cb(), lv_event_t, lv_event_t, rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_refresh(), volume_cb() (+5 more)

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
Cohesion: 0.06
Nodes (128): card_state_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_obj_t, row(), ui_datetimeScreen_screen_init(), lv_event_cb_t (+120 more)

### Community 17 - "Matriz de casos"
Cohesion: 0.08
Nodes (23): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+15 more)

### Community 18 - "lv_obj_t"
Cohesion: 0.09
Nodes (22): ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_infoScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_netWifiScreen_screen_destroy() (+14 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.22
Nodes (21): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+13 more)

### Community 20 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 21 - "http_api.c"
Cohesion: 0.13
Nodes (28): eTaskState, diag_worker(), mem_diag_report(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+20 more)

### Community 22 - "esp_timer_get_time"
Cohesion: 0.26
Nodes (17): aws_telemetry_task(), flow_meter_get(), sensor_sample_t, lock(), sensors_runtime_get_ages(), sensors_runtime_get_faults(), sensors_runtime_get_last(), sensors_runtime_get_min_max() (+9 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.09
Nodes (22): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+14 more)

### Community 25 - "ui_loginScreen.c"
Cohesion: 0.33
Nodes (9): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+1 more)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.25
Nodes (12): eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage() (+4 more)

### Community 28 - "esp_timer.h"
Cohesion: 0.16
Nodes (9): device_state_t, flow_meter_record(), flow_integrator_push(), state_build_json(), state_get(), state_update_sensors(), close_to(), main() (+1 more)

### Community 29 - "ui_usersScreen.c"
Cohesion: 0.22
Nodes (11): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+3 more)

### Community 30 - "example_eth_init"
Cohesion: 0.36
Nodes (10): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, esp_err_t, eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init() (+2 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "ui_bleAppScreen.c"
Cohesion: 0.19
Nodes (12): transport_ble_is_connected(), bleapp_goroot_async(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_loaded_cb(), bleapp_pair_pin(), bleapp_tick_cb(), bleapp_unloaded_cb() (+4 more)

### Community 34 - "net_core.c"
Cohesion: 0.23
Nodes (12): esp_event_base_t, on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event() (+4 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.33
Nodes (10): wifi_mgr_get_netinfo(), transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible() (+2 more)

### Community 36 - "appcfg_cache_peek"
Cohesion: 0.11
Nodes (26): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), bright_cb(), f_switch(), clamp_brightness(), ui_auth_user_count(), ui_auth_user_remove() (+18 more)

### Community 37 - "ui_cfg.c"
Cohesion: 0.14
Nodes (23): lv_event_t, general_loaded_cb(), protected_back_cb(), app_user_t, ui_edit_target_t, ui_auth_factory(), ui_auth_login(), ui_auth_logout() (+15 more)

### Community 38 - "ui_generalSimpleScreen.c"
Cohesion: 0.36
Nodes (7): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 39 - "vTaskDelay"
Cohesion: 0.27
Nodes (11): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_init(), sfm3300_read(), sfm3300_send_cmd(), sfm3300_start_measurement(), lv_event_t (+3 more)

### Community 40 - "ui_nav.c"
Cohesion: 0.21
Nodes (9): ui_bleAppScreen_screen_destroy(), get_bleapp(), lv_obj_t, ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace(), ui_nav_swap() (+1 more)

### Community 41 - "main.c"
Cohesion: 0.05
Nodes (53): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+45 more)

### Community 42 - "appmetrics_load"
Cohesion: 0.47
Nodes (4): app_metrics_t, appmetrics_load(), appmetrics_store(), esp_err_t

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.35
Nodes (16): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_init() (+8 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.28
Nodes (7): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new(), fresh_keypad()

### Community 45 - "test_alarm.c"
Cohesion: 0.18
Nodes (12): alarm_mgr_init(), esp_err_t, appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config() (+4 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.19
Nodes (13): time_mgr_set_datetime(), add_channel(), cJSON, sensor_signal_stats_t, cloud_mgr_connected(), mqtt_start_locked(), mqtt_stop_locked(), prepare_pem() (+5 more)

### Community 50 - "ui_pinScreen.c"
Cohesion: 0.29
Nodes (6): lv_event_t, pin_key_cb(), pin_wrong_flash(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin()

### Community 51 - "bsp_touch_new"
Cohesion: 0.18
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 52 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 53 - "save_and_refresh"
Cohesion: 0.24
Nodes (12): is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit() (+4 more)

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 56 - "sensors_runtime.c"
Cohesion: 0.15
Nodes (23): ads1115_is_ready(), at24c256_is_ready(), ms5803_is_ready(), sfm3300_get_last_rx(), sfm3300_is_ready(), AppConfig, esp_err_t, crc16_ccitt() (+15 more)

### Community 57 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 60 - "ui_auth_can"
Cohesion: 0.35
Nodes (11): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+3 more)

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 62 - "ui.c"
Cohesion: 0.18
Nodes (28): ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), edit_cb(), lv_event_t, fresh_login(), get_general(), ui_nav_load() (+20 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 66 - "ui_theme.h"
Cohesion: 0.19
Nodes (5): ui_datetimeScreen_screen_destroy(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 67 - "at24c256.c"
Cohesion: 0.53
Nodes (8): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), main()

### Community 69 - "on_network_ready"
Cohesion: 0.40
Nodes (3): esp_err_t, mdns_svc_start(), on_network_ready()

### Community 70 - "ui_nav_back"
Cohesion: 0.24
Nodes (11): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), fresh_useredit() (+3 more)

### Community 75 - "app_main"
Cohesion: 0.40
Nodes (5): esp_err_t, flow_meter_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), app_main()

### Community 76 - "save_cb"
Cohesion: 0.50
Nodes (3): lv_event_t, save_cb(), set_str()

### Community 77 - "sig_acc_finish"
Cohesion: 0.50
Nodes (4): sensor_signal_stats_t, sig_acc_add(), sig_acc_finish(), sig_acc_t

## Knowledge Gaps
- **106 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+101 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `wifi_mgr.c`, `ui_connectivityScreen_screen_init`, `alarm_mgr.c`, `transport_ble.c`, `storage.c`, `ui_sensorDiagScreen.c`, `cert_store.c`, `ui_label`, `fpm_ble_config.c`, `http_api.c`, `esp_timer_get_time`, `ui_cfg.c`, `main.c`, `next`, `save_and_refresh`, `gas_label_color`, `ui_cfg_apply_visual_mode`, `ui_auth_can`, `on_network_ready`, `app_main`, `save_cb`?**
  _High betweenness centrality (0.145) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `wifi_mgr.c`, `esp_err.h`, `at24c256.c`, `on_network_ready`, `transport_ble.c`, `driver_delay_ms`, `vTaskDelay`, `main.c`, `ads1115.c`, `bmp280.c`, `fpm_i2c_guard.c`, `cert_store.c`, `bsp_touch_new`, `fpm_ble_config.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`?**
  _High betweenness centrality (0.052) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `wifi_mgr.c`, `appcfg_cache_peek`, `vTaskDelay`, `bmp280.c`, `ui_label`, `bsp_touch_new`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 41 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 41 INFERRED edges - model-reasoned connections that need verification._