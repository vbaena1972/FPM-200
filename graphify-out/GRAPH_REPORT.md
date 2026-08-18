# Graph Report - ClaudeHMI-FW  (2026-08-18)

## Corpus Check
- 143 files · ~312,858 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 986 nodes · 2406 edges · 87 communities (81 shown, 6 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 793 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `6e8e0203`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- main.c
- lv_port_mem.c
- ui_mainScreen.c
- screen_init_task
- transport_ble.c
- appcfg_patch
- add_user_row
- ms5803.c
- http_api.c
- eth_mgr.c
- ui_cfg.c
- gas_label_color
- pressure_card
- cert_store.c
- app_main
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_cfg_flow_unit
- Migración HMI: SquareLine → Claude Design
- sensors_runtime.c
- appcfg_cache_peek
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- ws_stream.c
- statusbar_timer_cb
- ui_generalSimpleScreen.c
- README.md
- ui.c
- rtc_rv3028.c
- bsp_display_indev_init
- fpm_ble_config.c
- ui_wifi_main_icon.c
- save_cb
- state_on_net_available
- ui_statusbar_request_refresh
- ui_cfg_apply_visual_mode
- on_eth_event
- ui.h
- lv_event_t
- AGENTS.md
- CLAUDE.md
- ui_loginScreen.c
- ui_keypadScreen.c
- ui_sensorEditScreen.c
- ui_sensorDiagScreen.c
- sd_monitor_task
- ui_connectivityScreen.c
- next
- ui_datetimeScreen.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 69 edges
2. `ui_label()` - 56 edges
3. `ui_box()` - 40 edges
4. `ui_col()` - 39 edges
5. `_t()` - 33 edges
6. `ui_card()` - 32 edges
7. `appcfg_save()` - 31 edges
8. `ui_icon()` - 30 edges
9. `ui_sensorEditScreen_screen_init()` - 23 edges
10. `ui_nav_load()` - 23 edges

## Surprising Connections (you probably didn't know these)
- `config_mode_enter()` --calls--> `transport_mqtt_stop()`  [INFERRED]
  main/config_mode.c → components/network_core/transport_mqtt.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  main/ui/screens/ui_infoScreen.c → components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  main/main.c → components/storage/storage_extras.c
- `sd_monitor_task()` --calls--> `cert_store_import_from_sd()`  [INFERRED]
  main/main.c → components/cert_store/cert_store.c
- `app_main()` --calls--> `mem_diag_report()`  [INFERRED]
  main/main.c → components/diag/mem_diag.c

## Import Cycles
- None detected.

## Communities (87 total, 6 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.08
Nodes (97): lv_font_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init() (+89 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.16
Nodes (20): apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals(), notify() (+12 more)

### Community 2 - "main.c"
Cohesion: 0.13
Nodes (11): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), esp_trace_open_params_t, ble_json_worker(), bsp_i2c_init(), esp_err_t, lv_event_t, esp_trace_get_user_params() (+3 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_monitor_t, lv_mem_pool_t, lv_result_t, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core()

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.08
Nodes (40): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_clinical_state_t (+32 more)

### Community 5 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (41): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+33 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.13
Nodes (37): cfg_result_t, esp_event_base_t, mqtt_event(), prepare_pem(), transport_mqtt_on_time_ready(), transport_mqtt_stop(), appcfg_cache_get(), appcfg_defaults() (+29 more)

### Community 8 - "add_user_row"
Cohesion: 0.14
Nodes (18): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index() (+10 more)

### Community 9 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 10 - "http_api.c"
Cohesion: 0.21
Nodes (17): state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler(), http_api_init() (+9 more)

### Community 11 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start() (+18 more)

### Community 12 - "ui_cfg.c"
Cohesion: 0.24
Nodes (18): app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_factory() (+10 more)

### Community 13 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 14 - "pressure_card"
Cohesion: 0.33
Nodes (9): pressure_card(), ui_edit_target_t, ui_cfg_press_fmt(), ui_cfg_press_to_disp(), ui_cfg_pressure_limit_enabled(), ui_cfg_pressure_max(), ui_cfg_pressure_min(), ui_cfg_pressure_unit() (+1 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "app_main"
Cohesion: 0.11
Nodes (20): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), alarm_mgr_init() (+12 more)

### Community 17 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 18 - "ui_cfg_flow_unit"
Cohesion: 0.12
Nodes (19): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), allow_decimal(), clamp_brightness() (+11 more)

### Community 19 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.08
Nodes (50): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_write_reg(), esp_err_t (+42 more)

### Community 21 - "appcfg_cache_peek"
Cohesion: 0.14
Nodes (25): appcfg_cache_peek(), appcfg_save(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_auth_user_count(), ui_cfg_alarm_tone() (+17 more)

### Community 22 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.15
Nodes (12): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+4 more)

### Community 23 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 24 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 25 - "statusbar_timer_cb"
Cohesion: 0.24
Nodes (11): eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), lv_timer_t, statusbar_timer_cb(), conn_status_t (+3 more)

### Community 26 - "ui_generalSimpleScreen.c"
Cohesion: 0.27
Nodes (8): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy()

### Community 60 - "ui.c"
Cohesion: 0.07
Nodes (61): menu_item_cb(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), lv_event_t (+53 more)

### Community 61 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.23
Nodes (11): bsp_touch_config_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t, ft5x06_lvgl_read_cb() (+3 more)

### Community 63 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+28 more)

### Community 65 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 66 - "save_cb"
Cohesion: 0.40
Nodes (4): lv_event_t, save_cb(), set_str(), ui_netWifiScreen_screen_destroy()

### Community 67 - "state_on_net_available"
Cohesion: 0.24
Nodes (11): esp_event_base_t, on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event() (+3 more)

### Community 68 - "ui_statusbar_request_refresh"
Cohesion: 0.43
Nodes (6): lv_obj_t, set_glyph(), set_visible(), ui_statusbar_controller_init(), ui_statusbar_request_refresh(), ui_statusbar_set_enabled()

### Community 69 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 70 - "on_eth_event"
Cohesion: 0.50
Nodes (5): mem_diag_report(), on_eth_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop()

### Community 72 - "lv_event_t"
Cohesion: 0.67
Nodes (3): lv_event_t, kb_event_cb(), ta_event_cb()

### Community 76 - "ui_loginScreen.c"
Cohesion: 0.23
Nodes (11): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+3 more)

### Community 77 - "ui_keypadScreen.c"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 79 - "ui_sensorEditScreen.c"
Cohesion: 0.23
Nodes (14): lv_event_t, dec_cb(), f_switch(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch() (+6 more)

### Community 80 - "ui_sensorDiagScreen.c"
Cohesion: 0.22
Nodes (12): alarm_mgr_test_buzzer(), lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb() (+4 more)

### Community 84 - "sd_monitor_task"
Cohesion: 0.32
Nodes (6): sd_monitor_task(), unmount_sdcard_hotplug(), lv_event_cb_t, ui_sd_close(), ui_sd_finish_restart(), ui_sd_progress()

### Community 87 - "next"
Cohesion: 0.40
Nodes (5): lv_event_t, next(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

## Knowledge Gaps
- **54 isolated node(s):** `graphify`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)`, `display ↔ `general` / `DisplayConfig`` (+49 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `ui_mainScreen.c`, `screen_init_task`, `transport_ble.c`, `appcfg_patch`, `eth_mgr.c`, `ui_cfg.c`, `gas_label_color`, `pressure_card`, `cert_store.c`, `app_main`, `ui_cfg_flow_unit`, `statusbar_timer_cb`, `fpm_ble_config.c`, `save_cb`, `ui_cfg_apply_visual_mode`, `ui_sensorEditScreen.c`, `ui_sensorDiagScreen.c`, `next`?**
  _High betweenness centrality (0.248) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ui_wifi_main_icon.c`, `main.c`, `ui_statusbar_request_refresh`, `ms5803.c`, `sensors_runtime.c`, `appcfg_cache_peek`, `rtc_rv3028.c`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.129) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `main.c`, `state_on_net_available`, `on_eth_event`, `appcfg_patch`, `sensors_runtime.c`, `appcfg_cache_peek`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.099) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._