# Graph Report - FPM-200  (2026-09-24)

## Corpus Check
- 169 files · ~333,884 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1201 nodes · 3076 edges · 84 communities (80 shown, 4 thin omitted)
- Extraction: 68% EXTRACTED · 32% INFERRED · 0% AMBIGUOUS · INFERRED: 976 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `640b15c9`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_mainScreen.c
- esp_err_to_name
- transport_mqtt.c
- lv_port_mem.c
- alarm_mgr.c
- main.c
- esp_log.h
- app_main
- storage.c
- general_loaded_cb
- ft5x06.c
- ui.h
- ui_edit_begin
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- ui_label
- ui_nav_load
- ui.c
- ui_sensorEditScreen.c
- sensors_runtime.c
- http_api.c
- esp_timer_get_time
- ui_auth_can
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_wifi_main_icon.c
- ui_usersScreen.c
- eth_mgr.c
- flow_meter.c
- save_cb
- fpm_ble_config.c
- Migración HMI: SquareLine → Claude Design
- ui_generalSimpleScreen_screen_init
- net_core.c
- ui_statusbar_controller.c
- ui_col
- appcfg_cache_peek
- _t
- sensors_acq_task
- ui_confirmScreen_screen_init
- fresh_login
- ui_cfg.c
- FreeRTOS.h
- task_tracer.c
- touch_io_bounded.c
- README.md
- ui_loginScreen.c
- ui_userEditScreen.c
- ui_infoScreen_screen_init
- ui_pinScreen.c
- time_sync_worker
- gas_label_color
- save_and_refresh
- ui_style_button
- esp_timer.h
- ui_statusbar_request_refresh
- fresh_confirm
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- sig_acc_finish
- Consumo y endurecimiento 1.5.5-dev
- on_network_ready
- wifi_apply_cb
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
10. `screen_init_task()` - 26 edges

## Surprising Connections (you probably didn't know these)
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `app_main()` --calls--> `fpm_i2c_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/drivers/fpm_i2c_guard.c
- `app_main()` --calls--> `transport_mqtt_runtime_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/transport_mqtt.c
- `sensors_runtime_get_aws_telemetry()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/tests/host/test_alarm.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (84 total, 4 thin omitted)

### Community 0 - "ui_mainScreen.c"
Cohesion: 0.10
Nodes (40): card_state_t, sensors_runtime_get_debug(), ui_refresh_task(), alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb() (+32 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.25
Nodes (16): apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache(), wifi_mgr_init() (+8 more)

### Community 2 - "transport_mqtt.c"
Cohesion: 0.12
Nodes (25): conn_status_t, add_channel(), cJSON, sensor_signal_stats_t, cloud_mgr_connected(), mqtt_start_locked(), mqtt_stop_locked(), prepare_pem() (+17 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.10
Nodes (38): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+30 more)

### Community 5 - "main.c"
Cohesion: 0.06
Nodes (44): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+36 more)

### Community 6 - "esp_log.h"
Cohesion: 0.06
Nodes (46): ble_cmd_handler_t, append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event(), transport_ble_init() (+38 more)

### Community 7 - "app_main"
Cohesion: 0.20
Nodes (10): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), i2c_master_bus_handle_t (+2 more)

### Community 8 - "storage.c"
Cohesion: 0.14
Nodes (38): cfg_result_t, esp_event_base_t, mqtt_event(), appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+30 more)

### Community 9 - "general_loaded_cb"
Cohesion: 0.22
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_auth_logout(), ui_cfg_brightness(), ui_cfg_dim_minutes() (+3 more)

### Community 10 - "ft5x06.c"
Cohesion: 0.19
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 11 - "ui.h"
Cohesion: 0.13
Nodes (4): ui_confirmScreen_screen_destroy(), set_str(), lv_obj_t, status_chip()

### Community 12 - "ui_edit_begin"
Cohesion: 0.22
Nodes (13): lv_event_t, edit_cb(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes() (+5 more)

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
Cohesion: 0.17
Nodes (36): lv_event_cb_t, lv_obj_t, conn_card(), lv_obj_t, row(), ui_datetimeScreen_screen_init(), lv_obj_t, key() (+28 more)

### Community 17 - "ui_nav_load"
Cohesion: 0.23
Nodes (13): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color(), lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb() (+5 more)

### Community 18 - "ui.c"
Cohesion: 0.09
Nodes (39): ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_sensorDiagScreen_screen_destroy() (+31 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.22
Nodes (21): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+13 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.20
Nodes (16): ads1115_is_ready(), at24c256_is_ready(), ms5803_is_ready(), AppConfig, esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensor_cfg_set_defaults() (+8 more)

### Community 21 - "http_api.c"
Cohesion: 0.26
Nodes (19): add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+11 more)

### Community 22 - "esp_timer_get_time"
Cohesion: 0.26
Nodes (17): aws_telemetry_task(), flow_meter_get(), sensor_sample_t, lock(), sensors_runtime_get_ages(), sensors_runtime_get_faults(), sensors_runtime_get_last(), sensors_runtime_get_min_max() (+9 more)

### Community 23 - "ui_auth_can"
Cohesion: 0.35
Nodes (11): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+3 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.10
Nodes (20): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+12 more)

### Community 25 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 26 - "ui_usersScreen.c"
Cohesion: 0.20
Nodes (12): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+4 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.15
Nodes (22): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+14 more)

### Community 28 - "flow_meter.c"
Cohesion: 0.27
Nodes (7): esp_err_t, flow_meter_init(), flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t

### Community 29 - "save_cb"
Cohesion: 0.29
Nodes (6): lv_event_t, save_cb(), set_str(), ui_netEthScreen_screen_destroy(), fresh_net_eth(), ui_open_net_eth_cb()

### Community 30 - "fpm_ble_config.c"
Cohesion: 0.15
Nodes (38): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+30 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 33 - "ui_generalSimpleScreen_screen_init"
Cohesion: 0.17
Nodes (16): lv_event_cb_t, lv_obj_t, gen_tile(), ui_generalScreen_screen_destroy(), ui_generalScreen_screen_init(), base_card(), lv_event_cb_t, lv_event_t (+8 more)

### Community 34 - "net_core.c"
Cohesion: 0.33
Nodes (9): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), transport_mqtt_on_net_down() (+1 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.33
Nodes (10): wifi_mgr_get_netinfo(), transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible() (+2 more)

### Community 36 - "ui_col"
Cohesion: 0.19
Nodes (14): bleapp_set_connected(), ui_bleAppScreen_screen_init(), lv_obj_t, pkey(), ui_pinScreen_screen_init(), ui_splashScreen_screen_init(), lv_event_cb_t, ui_sd_error() (+6 more)

### Community 37 - "appcfg_cache_peek"
Cohesion: 0.14
Nodes (20): appcfg_cache_peek(), appcfg_save(), f_switch(), ui_auth_user_count(), ui_auth_user_remove(), ui_cfg_alarm_tone(), ui_cfg_check_pin(), ui_cfg_decimals() (+12 more)

### Community 38 - "_t"
Cohesion: 0.20
Nodes (17): ui_netBleScreen_screen_init(), ui_netCloudScreen_screen_init(), ui_netEthScreen_screen_init(), ui_netWifiScreen_screen_init(), ui_userEditScreen_screen_init(), lv_event_cb_t, lv_event_t, lv_obj_t (+9 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.24
Nodes (14): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+6 more)

### Community 40 - "ui_confirmScreen_screen_init"
Cohesion: 0.33
Nodes (8): ui_confirmScreen_screen_init(), allow_decimal(), key_cb(), refresh_value(), ui_keypadScreen_screen_init(), ui_edit_label(), ui_edit_new(), ui_edit_unit()

### Community 41 - "fresh_login"
Cohesion: 0.22
Nodes (10): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb() (+2 more)

### Community 42 - "ui_cfg.c"
Cohesion: 0.13
Nodes (25): apply_authenticated_cb(), lv_event_t, lv_event_t, next(), app_user_t, ui_auth_factory(), ui_auth_login(), ui_auth_user_at() (+17 more)

### Community 43 - "FreeRTOS.h"
Cohesion: 0.07
Nodes (60): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+52 more)

### Community 44 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 45 - "touch_io_bounded.c"
Cohesion: 0.29
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 47 - "ui_loginScreen.c"
Cohesion: 0.29
Nodes (9): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+1 more)

### Community 50 - "ui_userEditScreen.c"
Cohesion: 0.48
Nodes (6): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role()

### Community 51 - "ui_infoScreen_screen_init"
Cohesion: 0.32
Nodes (6): lv_event_t, lv_obj_t, gas_label(), kv_card(), reboot_cb(), ui_infoScreen_screen_init()

### Community 52 - "ui_pinScreen.c"
Cohesion: 0.27
Nodes (8): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 53 - "time_sync_worker"
Cohesion: 0.32
Nodes (6): app_metrics_t, time_sync_worker(), flow_meter_service(), appmetrics_load(), appmetrics_store(), esp_err_t

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "save_and_refresh"
Cohesion: 0.24
Nodes (12): is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit() (+4 more)

### Community 56 - "ui_style_button"
Cohesion: 0.46
Nodes (7): lv_obj_t, rebuild(), small_button(), stepper(), ui_sensorDiagScreen_refresh(), ui_sensorDiagScreen_screen_init(), ui_style_button()

### Community 57 - "esp_timer.h"
Cohesion: 0.29
Nodes (4): device_state_t, state_build_json(), state_get(), state_update_sensors()

### Community 58 - "ui_statusbar_request_refresh"
Cohesion: 0.14
Nodes (20): mem_diag_report(), esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, wifi_status_t (+12 more)

### Community 60 - "fresh_confirm"
Cohesion: 0.40
Nodes (5): accept_cb(), lv_event_t, ui_edit_set_new(), fresh_confirm(), ui_open_confirm_cb()

### Community 61 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 62 - "sig_acc_finish"
Cohesion: 0.50
Nodes (4): sensor_signal_stats_t, sig_acc_add(), sig_acc_finish(), sig_acc_t

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.12
Nodes (16): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, Audit and fixes: 1.5.18-dev (Claude, 2026-09-24), Consumo y endurecimiento 1.5.5-dev, CPU distribution and UI refresh: 1.5.8-dev, EEPROM diagnostic build 1.5.7-dev, EEPROM read pacing: 1.5.11-dev, Hardware follow-up: 1.5.6-dev, Interrupted v6 migration recovery: 1.5.15-dev (Claude) (+8 more)

### Community 66 - "on_network_ready"
Cohesion: 0.67
Nodes (3): esp_err_t, mdns_svc_start(), on_network_ready()

## Knowledge Gaps
- **80 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+75 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **4 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_mainScreen.c`, `transport_mqtt.c`, `alarm_mgr.c`, `main.c`, `esp_log.h`, `app_main`, `storage.c`, `general_loaded_cb`, `ui_edit_begin`, `cert_store.c`, `http_api.c`, `esp_timer_get_time`, `ui_auth_can`, `save_cb`, `fpm_ble_config.c`, `ui_col`, `_t`, `ui_cfg.c`, `ui_infoScreen_screen_init`, `gas_label_color`, `save_and_refresh`, `ui_statusbar_request_refresh`, `on_network_ready`?**
  _High betweenness centrality (0.159) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `esp_err_to_name`, `appcfg_cache_peek`, `sensors_acq_task`, `ft5x06.c`, `FreeRTOS.h`, `sensors_runtime.c`, `ui_wifi_main_icon.c`, `ui_statusbar_request_refresh`?**
  _High betweenness centrality (0.075) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `esp_err.h`, `on_network_ready`, `main.c`, `esp_log.h`, `sensors_acq_task`, `ft5x06.c`, `FreeRTOS.h`, `cert_store.c`, `sensors_runtime.c`, `time_sync_worker`, `http_api.c`, `ui_statusbar_request_refresh`, `eth_mgr.c`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 35 INFERRED edges - model-reasoned connections that need verification._