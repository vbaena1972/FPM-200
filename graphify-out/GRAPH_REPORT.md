# Graph Report - ClaudeHMI-FW  (2026-08-18)

## Corpus Check
- 143 files · ~322,943 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 973 nodes · 2370 edges · 82 communities (79 shown, 3 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 781 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `4c483060`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- main.c
- lv_port_mem.c
- ui_refresh_task
- ui.c
- transport_ble.c
- appcfg_patch
- ui.h
- ms5803.c
- fpm_ble_config.c
- eth_mgr.c
- ui_cfg.c
- rtc_rv3028.c
- save_and_refresh
- cert_store.c
- app_main
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- sensors_runtime.c
- appcfg_cache_peek
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- ws_stream.c
- screen_init_task
- general_loaded_cb
- README.md
- ui_usersScreen.c
- apply_authenticated_cb
- bsp_display_indev_init
- ui_nav_load
- appmetrics_load
- ui_cfg_apply_visual_mode
- get_connectivity
- fresh_net_ble
- fresh_net_cloud
- lv_event_t
- get_sensor_diag
- ui_pinScreen.c
- AGENTS.md
- CLAUDE.md
- ui_nav.c
- gas_label_color
- lv_obj_t
- ui_keypadScreen.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 67 edges
2. `ui_label()` - 55 edges
3. `ui_box()` - 40 edges
4. `ui_col()` - 39 edges
5. `_t()` - 33 edges
6. `ui_card()` - 32 edges
7. `appcfg_save()` - 30 edges
8. `ui_icon()` - 30 edges
9. `ui_nav_load()` - 23 edges
10. `screen_init_task()` - 22 edges

## Surprising Connections (you probably didn't know these)
- `ui_main_update()` --calls--> `alarm_mgr_get_sensor_faults()`  [INFERRED]
  main/ui/screens/ui_mainScreen.c → components/drivers/alarm_mgr.c
- `config_mode_enter()` --calls--> `transport_mqtt_stop()`  [INFERRED]
  main/config_mode.c → components/network_core/transport_mqtt.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  main/ui/screens/ui_infoScreen.c → components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  main/main.c → components/storage/storage_extras.c
- `sd_monitor_task()` --calls--> `cert_store_import_from_sd()`  [INFERRED]
  main/main.c → components/cert_store/cert_store.c

## Import Cycles
- None detected.

## Communities (82 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.07
Nodes (107): card_state_t, lv_font_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card() (+99 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.06
Nodes (53): eth_mgr_is_up(), cloud_mgr_connected(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info() (+45 more)

### Community 2 - "main.c"
Cohesion: 0.14
Nodes (11): esp_trace_open_params_t, ble_json_worker(), bsp_i2c_init(), esp_err_t, esp_trace_get_user_params(), mount_sdcard_hotplug(), on_network_ready(), sd_monitor_task() (+3 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_monitor_t, lv_mem_pool_t, lv_result_t, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core()

### Community 4 - "ui_refresh_task"
Cohesion: 0.19
Nodes (13): alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_mgr_test_buzzer(), alarm_clinical_state_t, set_buzzer_acoustic() (+5 more)

### Community 5 - "ui.c"
Cohesion: 0.17
Nodes (15): ui_mainScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_sensorEditScreen_screen_destroy(), lv_timer_t, fresh_net_eth(), get_bleapp(), get_datetime(), get_sensor_edit() (+7 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.07
Nodes (41): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+33 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.13
Nodes (38): cfg_result_t, aws_telemetry_task(), esp_event_base_t, mqtt_event(), prepare_pem(), transport_mqtt_on_time_ready(), transport_mqtt_stop(), appcfg_cache_get() (+30 more)

### Community 8 - "ui.h"
Cohesion: 0.05
Nodes (26): cpy(), rssi_pct(), ui_datetimeScreen_screen_destroy(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild() (+18 more)

### Community 9 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 10 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+28 more)

### Community 11 - "eth_mgr.c"
Cohesion: 0.12
Nodes (25): appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start() (+17 more)

### Community 12 - "ui_cfg.c"
Cohesion: 0.14
Nodes (30): appcfg_save(), lv_event_t, next(), app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user() (+22 more)

### Community 13 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 14 - "save_and_refresh"
Cohesion: 0.25
Nodes (11): gas_cb(), p_switch(), AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas() (+3 more)

### Community 15 - "cert_store.c"
Cohesion: 0.21
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 16 - "app_main"
Cohesion: 0.06
Nodes (50): eTaskState, diag_timer_cb(), mem_diag_report(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+42 more)

### Community 17 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 18 - "ui_sensorEditScreen.c"
Cohesion: 0.13
Nodes (33): lv_event_t, lv_obj_t, ui_edit_target_t, card(), f_switch(), flow_card(), is_press_unit(), limit_edit() (+25 more)

### Community 19 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.08
Nodes (45): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_write_reg(), esp_err_t (+37 more)

### Community 21 - "appcfg_cache_peek"
Cohesion: 0.15
Nodes (18): appcfg_cache_peek(), lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_auth_user_count() (+10 more)

### Community 22 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.17
Nodes (11): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+3 more)

### Community 23 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 24 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 25 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

### Community 26 - "general_loaded_cb"
Cohesion: 0.24
Nodes (10): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness() (+2 more)

### Community 60 - "ui_usersScreen.c"
Cohesion: 0.14
Nodes (20): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index() (+12 more)

### Community 61 - "apply_authenticated_cb"
Cohesion: 0.22
Nodes (7): apply_authenticated_cb(), lv_event_t, ui_confirmScreen_screen_destroy(), allow_decimal(), ui_edit_is_audio(), fresh_confirm(), ui_open_login_cb()

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.15
Nodes (9): bsp_touch_config_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t, esp_lcd_touch_handle_t (+1 more)

### Community 63 - "ui_nav_load"
Cohesion: 0.32
Nodes (8): ui_infoScreen_screen_destroy(), ui_login_set_destination(), menu_item_cb(), get_info(), ui_nav_load(), ui_open_config_ble_cb(), ui_open_config_pin_cb(), ui_open_info_cb()

### Community 64 - "appmetrics_load"
Cohesion: 0.47
Nodes (5): app_metrics_t, appmetrics_load(), appmetrics_service_min(), appmetrics_store(), esp_err_t

### Community 65 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 66 - "get_connectivity"
Cohesion: 0.67
Nodes (3): ui_connectivityScreen_screen_destroy(), get_connectivity(), ui_open_connectivity_cb()

### Community 67 - "fresh_net_ble"
Cohesion: 0.67
Nodes (3): ui_netBleScreen_screen_destroy(), fresh_net_ble(), ui_open_net_ble_cb()

### Community 68 - "fresh_net_cloud"
Cohesion: 0.67
Nodes (3): ui_netCloudScreen_screen_destroy(), fresh_net_cloud(), ui_open_net_cloud_cb()

### Community 69 - "lv_event_t"
Cohesion: 0.33
Nodes (7): ui_generalSimpleScreen_screen_destroy(), lv_event_t, get_general(), get_general_simple(), ui_open_general_authenticated_cb(), ui_open_general_cb(), ui_open_general_simple_cb()

### Community 70 - "get_sensor_diag"
Cohesion: 0.67
Nodes (3): ui_sensorDiagScreen_screen_destroy(), get_sensor_diag(), ui_open_sensordiag_cb()

### Community 72 - "ui_pinScreen.c"
Cohesion: 0.27
Nodes (8): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 75 - "ui_nav.c"
Cohesion: 0.48
Nodes (6): lv_obj_t, ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace(), ui_nav_swap()

### Community 79 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 83 - "lv_obj_t"
Cohesion: 0.22
Nodes (9): ui_loginScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_usersScreen_screen_destroy(), lv_obj_t, fresh_login(), fresh_net_wifi(), get_users(), ui_open_net_wifi_cb() (+1 more)

### Community 88 - "ui_keypadScreen.c"
Cohesion: 0.22
Nodes (9): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new(), fresh_keypad(), ui_open_confirm_cb() (+1 more)

## Knowledge Gaps
- **53 isolated node(s):** `graphify`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)`, `display ↔ `general` / `DisplayConfig`` (+48 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `wifi_mgr.c`, `ui_cfg_apply_visual_mode`, `transport_ble.c`, `appcfg_patch`, `fpm_ble_config.c`, `eth_mgr.c`, `ui_cfg.c`, `save_and_refresh`, `cert_store.c`, `app_main`, `gas_label_color`, `ui_sensorEditScreen.c`, `screen_init_task`, `general_loaded_cb`?**
  _High betweenness centrality (0.247) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `wifi_mgr.c`, `main.c`, `ms5803.c`, `rtc_rv3028.c`, `sensors_runtime.c`, `appcfg_cache_peek`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.126) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `main.c`, `appcfg_patch`, `fpm_ble_config.c`, `ui_cfg.c`, `sensors_runtime.c`, `appcfg_cache_peek`?**
  _High betweenness centrality (0.097) - this node is a cross-community bridge._
- **Are the 64 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 64 INFERRED edges - model-reasoned connections that need verification._
- **Are the 46 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 46 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._