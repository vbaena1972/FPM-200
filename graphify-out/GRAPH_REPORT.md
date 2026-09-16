# Graph Report - FPM-200  (2026-09-16)

## Corpus Check
- 145 files · ~316,574 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1011 nodes · 2449 edges · 93 communities (90 shown, 3 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 798 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `a353c415`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- main.c
- lv_port_mem.c
- ui_mainScreen.c
- ui.h
- transport_ble.c
- appcfg_patch
- fpm_ble_config.c
- ui_cfg.c
- http_api.c
- example_eth_init
- ui_cfg_preview_brightness
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- task_tracer.c
- ui_sensorEditScreen.c
- ui.c
- bmp280.c
- sensors_runtime.c
- app_main
- ui_edit_begin
- save_cb
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- statusbar_timer_cb
- gas_label_color
- eth_mgr.c
- rtc_rv3028.c
- ui_nav.c
- ui_pinScreen.c
- Migración HMI: SquareLine → Claude Design
- save_and_refresh
- sd_monitor_task
- ui_loginScreen.c
- state_on_net_available
- ws_stream.c
- ui_statusbar_request_refresh
- ui_cfg_pressure_unit
- on_eth_event
- next
- ui_nav_load
- lv_event_t
- save_cb
- get_connectivity
- get_general_simple
- README.md
- screen_init_task
- bsp_display_indev_init
- ui_wifi_main_icon.c
- AGENTS.md
- CLAUDE.md
- fresh_net_wifi
- get_sensor_diag
- get_sensor_edit
- get_users

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 69 edges
2. `ui_label()` - 56 edges
3. `ui_box()` - 40 edges
4. `ui_col()` - 39 edges
5. `_t()` - 33 edges
6. `ui_card()` - 32 edges
7. `appcfg_save()` - 31 edges
8. `ui_icon()` - 30 edges
9. `screen_init_task()` - 23 edges
10. `ui_sensorEditScreen_screen_init()` - 23 edges

## Surprising Connections (you probably didn't know these)
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  firmware/main/ui/screens/ui_infoScreen.c → firmware/components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `get_bleapp()` --calls--> `ui_bleAppScreen_screen_destroy()`  [INFERRED]
  firmware/main/ui/ui.c → firmware/main/ui/screens/ui_bleAppScreen.c
- `ui_confirmScreen_screen_init()` --calls--> `ui_edit_old()`  [INFERRED]
  firmware/main/ui/screens/ui_confirmScreen.c → firmware/main/ui/ui_cfg.c

## Import Cycles
- None detected.

## Communities (93 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.08
Nodes (91): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+83 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.19
Nodes (17): apply_ip_mode(), AppConfig, esp_err_t, wifi_status_t, net_cfg_equals(), notify(), wifi_apply_cb(), wifi_mgr_apply_from_cache() (+9 more)

### Community 2 - "main.c"
Cohesion: 0.13
Nodes (11): esp_trace_open_params_t, ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), ble_json_worker(), bsp_i2c_init(), esp_err_t, lv_event_t, esp_trace_get_user_params() (+3 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.08
Nodes (42): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_clinical_state_t (+34 more)

### Community 5 - "ui.h"
Cohesion: 0.05
Nodes (34): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), lv_event_t, protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb() (+26 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.07
Nodes (40): ble_cmd_handler_t, transport_mqtt_stop(), fpm_ble_config_set_clock_cb(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+32 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.12
Nodes (40): cfg_result_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), prepare_pem() (+32 more)

### Community 8 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (37): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+29 more)

### Community 9 - "ui_cfg.c"
Cohesion: 0.13
Nodes (37): appcfg_cache_peek(), appcfg_save(), f_switch(), app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user() (+29 more)

### Community 10 - "http_api.c"
Cohesion: 0.21
Nodes (17): device_state_t, state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+9 more)

### Community 11 - "example_eth_init"
Cohesion: 0.36
Nodes (10): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, esp_err_t, eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init() (+2 more)

### Community 12 - "ui_cfg_preview_brightness"
Cohesion: 0.47
Nodes (6): bright_cb(), clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.21
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 16 - "task_tracer.c"
Cohesion: 0.36
Nodes (6): eTaskState, state_str(), task_tracer_dump_now(), timer_cb(), track_get_and_update(), TaskHandle_t

### Community 17 - "ui_sensorEditScreen.c"
Cohesion: 0.21
Nodes (22): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+14 more)

### Community 18 - "ui.c"
Cohesion: 0.13
Nodes (24): ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_userEditScreen_screen_destroy(), lv_obj_t, lv_timer_t, fresh_confirm() (+16 more)

### Community 19 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.06
Nodes (67): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_write_reg(), esp_err_t (+59 more)

### Community 21 - "app_main"
Cohesion: 0.18
Nodes (12): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), alarm_mgr_init() (+4 more)

### Community 22 - "ui_edit_begin"
Cohesion: 0.18
Nodes (15): alarm_mgr_test_buzzer(), lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t (+7 more)

### Community 23 - "save_cb"
Cohesion: 0.25
Nodes (7): eth_mgr_is_up(), esp_event_base_t, log_dns_info(), wifi_event_handler(), lv_event_t, save_cb(), set_str()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.13
Nodes (14): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+6 more)

### Community 25 - "statusbar_timer_cb"
Cohesion: 0.19
Nodes (11): conn_status_t, cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), lv_timer_t, statusbar_timer_cb(), cpy() (+3 more)

### Community 26 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.25
Nodes (12): eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage() (+4 more)

### Community 28 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 29 - "ui_nav.c"
Cohesion: 0.23
Nodes (9): lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace() (+1 more)

### Community 30 - "ui_pinScreen.c"
Cohesion: 0.27
Nodes (8): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "save_and_refresh"
Cohesion: 0.33
Nodes (9): AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 33 - "sd_monitor_task"
Cohesion: 0.32
Nodes (6): sd_monitor_task(), unmount_sdcard_hotplug(), lv_event_cb_t, ui_sd_close(), ui_sd_finish_restart(), ui_sd_progress()

### Community 34 - "ui_loginScreen.c"
Cohesion: 0.29
Nodes (9): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+1 more)

### Community 35 - "state_on_net_available"
Cohesion: 0.31
Nodes (9): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), time_mgr_start_sntp(), transport_mqtt_on_net_down() (+1 more)

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 37 - "ui_statusbar_request_refresh"
Cohesion: 0.43
Nodes (6): lv_obj_t, set_glyph(), set_visible(), ui_statusbar_controller_init(), ui_statusbar_request_refresh(), ui_statusbar_set_enabled()

### Community 38 - "ui_cfg_pressure_unit"
Cohesion: 0.24
Nodes (10): general_loaded_cb(), ui_cfg_brightness(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit(), ui_cfg_press_fmt(), ui_cfg_press_from_disp(), ui_cfg_press_to_disp(), ui_cfg_pressure_unit() (+2 more)

### Community 39 - "on_eth_event"
Cohesion: 0.33
Nodes (7): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop()

### Community 40 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 41 - "ui_nav_load"
Cohesion: 0.18
Nodes (13): apply_authenticated_cb(), lv_event_t, ui_login_set_destination(), ui_loginScreen_screen_destroy(), lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color() (+5 more)

### Community 42 - "lv_event_t"
Cohesion: 0.33
Nodes (7): ui_infoScreen_screen_destroy(), lv_event_t, get_general(), get_info(), ui_open_general_authenticated_cb(), ui_open_general_cb(), ui_open_info_cb()

### Community 43 - "save_cb"
Cohesion: 0.50
Nodes (3): lv_event_t, save_cb(), set_str()

### Community 44 - "get_connectivity"
Cohesion: 0.67
Nodes (3): ui_connectivityScreen_screen_destroy(), get_connectivity(), ui_open_connectivity_cb()

### Community 45 - "get_general_simple"
Cohesion: 0.67
Nodes (3): ui_generalSimpleScreen_screen_destroy(), get_general_simple(), ui_open_general_simple_cb()

### Community 61 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.13
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 65 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 89 - "fresh_net_wifi"
Cohesion: 0.67
Nodes (3): ui_netWifiScreen_screen_destroy(), fresh_net_wifi(), ui_open_net_wifi_cb()

### Community 90 - "get_sensor_diag"
Cohesion: 0.67
Nodes (3): ui_sensorDiagScreen_screen_destroy(), get_sensor_diag(), ui_open_sensordiag_cb()

### Community 91 - "get_sensor_edit"
Cohesion: 0.67
Nodes (3): ui_sensorEditScreen_screen_destroy(), get_sensor_edit(), ui_open_sensor_cb()

### Community 92 - "get_users"
Cohesion: 0.67
Nodes (3): ui_usersScreen_screen_destroy(), get_users(), ui_open_users_cb()

## Knowledge Gaps
- **57 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+52 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `ui_cfg.c` to `ui_label`, `save_and_refresh`, `ui_mainScreen.c`, `transport_ble.c`, `appcfg_patch`, `fpm_ble_config.c`, `ui_cfg_pressure_unit`, `next`, `save_cb`, `ui_cfg_preview_brightness`, `cert_store.c`, `app_main`, `ui_edit_begin`, `save_cb`, `statusbar_timer_cb`, `gas_label_color`, `screen_init_task`?**
  _High betweenness centrality (0.240) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ui_wifi_main_icon.c`, `main.c`, `ui_statusbar_request_refresh`, `ui_cfg.c`, `bmp280.c`, `sensors_runtime.c`, `rtc_rv3028.c`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.125) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `main.c`, `state_on_net_available`, `appcfg_patch`, `on_eth_event`, `ui_cfg.c`, `fpm_ble_config.c`, `next`, `sensors_runtime.c`?**
  _High betweenness centrality (0.084) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._