# Graph Report - FPM-200  (2026-09-16)

## Corpus Check
- 145 files · ~317,754 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1020 nodes · 2468 edges · 84 communities (81 shown, 3 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 803 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `75bd074e`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- screen_init_task
- lv_port_mem.c
- ui_mainScreen.c
- ui.h
- transport_ble.c
- appcfg_patch
- ui_sensorEditScreen.c
- ui_cfg.c
- http_api.c
- ui_usersScreen.c
- general_loaded_cb
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- ms5803.c
- pressure_card
- ui.c
- ui_generalSimpleScreen.c
- sensors_runtime.c
- app_main
- ui_sensorDiagScreen.c
- save_cb
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- statusbar_timer_cb
- gas_label_color
- eth_mgr.c
- ui_keypadScreen.c
- ui_userEditScreen.c
- fpm_ble_config.c
- Migración HMI: SquareLine → Claude Design
- appcfg_cache_peek
- ui_bleAppScreen.c
- ui_loginScreen.c
- state_on_net_available
- ws_stream.c
- ui_statusbar_request_refresh
- config_mode_enter
- on_eth_event
- next
- ui_cfg_apply_visual_mode
- README.md
- bsp_display_indev_init
- ui_wifi_main_icon.c
- AGENTS.md
- CLAUDE.md

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
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  firmware/main/ui/screens/ui_infoScreen.c → firmware/components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `sd_monitor_task()` --calls--> `ui_sd_close()`  [INFERRED]
  firmware/main/main.c → firmware/main/ui/ui_sd.c
- `get_bleapp()` --calls--> `ui_bleAppScreen_screen_destroy()`  [INFERRED]
  firmware/main/ui/ui.c → firmware/main/ui/screens/ui_bleAppScreen.c
- `ui_confirmScreen_screen_init()` --calls--> `ui_edit_old()`  [INFERRED]
  firmware/main/ui/screens/ui_confirmScreen.c → firmware/main/ui/ui_cfg.c

## Import Cycles
- None detected.

## Communities (84 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.08
Nodes (91): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+83 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.15
Nodes (21): apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals(), notify() (+13 more)

### Community 2 - "screen_init_task"
Cohesion: 0.05
Nodes (52): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+44 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.06
Nodes (53): alarm_state_change_cb_t, app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_press_mute() (+45 more)

### Community 5 - "ui.h"
Cohesion: 0.11
Nodes (4): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_sd_close(), ui_sd_progress()

### Community 6 - "transport_ble.c"
Cohesion: 0.12
Nodes (24): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+16 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.12
Nodes (40): cfg_result_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), prepare_pem() (+32 more)

### Community 8 - "ui_sensorEditScreen.c"
Cohesion: 0.25
Nodes (14): lv_event_t, dec_cb(), f_switch(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch() (+6 more)

### Community 9 - "ui_cfg.c"
Cohesion: 0.18
Nodes (24): appcfg_save(), app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user() (+16 more)

### Community 10 - "http_api.c"
Cohesion: 0.21
Nodes (17): device_state_t, state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+9 more)

### Community 11 - "ui_usersScreen.c"
Cohesion: 0.27
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 12 - "general_loaded_cb"
Cohesion: 0.25
Nodes (9): lv_event_t, general_loaded_cb(), protected_back_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness() (+1 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 17 - "pressure_card"
Cohesion: 0.13
Nodes (25): apply_authenticated_cb(), lv_event_t, allow_decimal(), lv_obj_t, ui_edit_target_t, card(), flow_card(), mini() (+17 more)

### Community 18 - "ui.c"
Cohesion: 0.07
Nodes (60): ui_infoScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), lv_event_t (+52 more)

### Community 19 - "ui_generalSimpleScreen.c"
Cohesion: 0.23
Nodes (10): ui_generalScreen_screen_destroy(), bright_cb(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb() (+2 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.07
Nodes (58): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+50 more)

### Community 21 - "app_main"
Cohesion: 0.12
Nodes (18): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), i2c_master_bus_handle_t (+10 more)

### Community 22 - "ui_sensorDiagScreen.c"
Cohesion: 0.24
Nodes (11): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb(), ui_cfg_alarm_volume() (+3 more)

### Community 23 - "save_cb"
Cohesion: 0.33
Nodes (5): eth_mgr_is_up(), lv_event_t, save_cb(), set_str(), ui_netWifiScreen_screen_destroy()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.12
Nodes (15): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+7 more)

### Community 25 - "statusbar_timer_cb"
Cohesion: 0.18
Nodes (12): conn_status_t, cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), lv_timer_t, statusbar_timer_cb(), cpy() (+4 more)

### Community 26 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+18 more)

### Community 28 - "ui_keypadScreen.c"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 29 - "ui_userEditScreen.c"
Cohesion: 0.33
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 30 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+28 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "appcfg_cache_peek"
Cohesion: 0.20
Nodes (16): appcfg_cache_peek(), AppConfig, save_and_refresh(), set_str(), ui_auth_user_count(), ui_cfg_alarm_tone(), ui_cfg_check_pin(), ui_cfg_decimals() (+8 more)

### Community 33 - "ui_bleAppScreen.c"
Cohesion: 0.18
Nodes (13): bleapp_goroot_async(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_loaded_cb(), bleapp_off_cb(), bleapp_pair_pin(), bleapp_tick_cb(), bleapp_unloaded_cb() (+5 more)

### Community 34 - "ui_loginScreen.c"
Cohesion: 0.26
Nodes (11): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+3 more)

### Community 35 - "state_on_net_available"
Cohesion: 0.24
Nodes (11): esp_event_base_t, on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event() (+3 more)

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 37 - "ui_statusbar_request_refresh"
Cohesion: 0.43
Nodes (6): lv_obj_t, set_glyph(), set_visible(), ui_statusbar_controller_init(), ui_statusbar_request_refresh(), ui_statusbar_set_enabled()

### Community 38 - "config_mode_enter"
Cohesion: 0.33
Nodes (4): transport_mqtt_stop(), esp_err_t, config_mode_enter(), log_heap()

### Community 39 - "on_eth_event"
Cohesion: 0.50
Nodes (5): mem_diag_report(), on_eth_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop()

### Community 40 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 41 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.13
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 65 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

## Knowledge Gaps
- **58 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+53 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `screen_init_task`, `ui_mainScreen.c`, `transport_ble.c`, `appcfg_patch`, `ui_sensorEditScreen.c`, `ui_cfg.c`, `general_loaded_cb`, `cert_store.c`, `pressure_card`, `app_main`, `ui_sensorDiagScreen.c`, `save_cb`, `statusbar_timer_cb`, `gas_label_color`, `eth_mgr.c`, `fpm_ble_config.c`, `next`, `ui_cfg_apply_visual_mode`?**
  _High betweenness centrality (0.253) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `appcfg_cache_peek`, `ui_wifi_main_icon.c`, `ui_label`, `ui_statusbar_request_refresh`, `ms5803.c`, `sensors_runtime.c`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.168) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `appcfg_cache_peek`, `screen_init_task`, `state_on_net_available`, `ui_mainScreen.c`, `on_eth_event`, `appcfg_patch`, `next`, `sensors_runtime.c`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.079) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._