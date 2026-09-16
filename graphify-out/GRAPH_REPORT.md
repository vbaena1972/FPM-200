# Graph Report - FPM-200  (2026-09-16)

## Corpus Check
- 147 files · ~320,921 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1053 nodes · 2547 edges · 82 communities (79 shown, 3 thin omitted)
- Extraction: 68% EXTRACTED · 32% INFERRED · 0% AMBIGUOUS · INFERRED: 818 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `72af71c1`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- app_main
- lv_port_mem.c
- ui_mainScreen.c
- main.c
- transport_ble.c
- appcfg_patch
- ui_sensorEditScreen.c
- appcfg_cache_peek
- bsp_display_indev_init
- ui.h
- ui_edit_begin
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- rtc_rv3028.c
- bmp280.c
- ui.c
- ui_cfg.c
- sensors_runtime.c
- http_api.c
- gas_label_color
- general_loaded_cb
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_nav_load
- ui_auth_can
- eth_mgr.c
- ui_keypadScreen.c
- task_tracer.c
- fpm_ble_config.c
- Migración HMI: SquareLine → Claude Design
- sd_monitor_task
- ui_confirmScreen.c
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- fresh_net_cloud
- ws_stream.c
- get_sensor_diag
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- ui_loginScreen.c
- ui_nav.c
- README.md
- screen_init_task
- AGENTS.md
- CLAUDE.md

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 69 edges
2. `ui_label()` - 56 edges
3. `ui_col()` - 41 edges
4. `ui_box()` - 41 edges
5. `_t()` - 33 edges
6. `ui_card()` - 33 edges
7. `appcfg_save()` - 31 edges
8. `ui_icon()` - 30 edges
9. `ui_sensorEditScreen_screen_init()` - 24 edges
10. `screen_init_task()` - 23 edges

## Surprising Connections (you probably didn't know these)
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  firmware/main/ui/screens/ui_infoScreen.c → firmware/components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `app_main()` --calls--> `task_tracer_start()`  [INFERRED]
  firmware/main/main.c → firmware/components/task_tracer/task_tracer.c

## Import Cycles
- None detected.

## Communities (82 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.08
Nodes (93): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), lv_obj_t, row() (+85 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.05
Nodes (59): conn_status_t, eth_mgr_is_up(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info() (+51 more)

### Community 2 - "app_main"
Cohesion: 0.13
Nodes (16): alarm_state_change_cb_t, eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+8 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.07
Nodes (53): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_mgr_set_audio_inhibit_noncritical() (+45 more)

### Community 5 - "main.c"
Cohesion: 0.13
Nodes (11): esp_trace_open_params_t, ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), ble_json_worker(), bsp_i2c_init(), esp_err_t, lv_event_t, esp_trace_get_user_params() (+3 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (41): ble_cmd_handler_t, transport_mqtt_stop(), fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer() (+33 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.10
Nodes (46): cfg_result_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, cloud_mgr_connected(), mqtt_event() (+38 more)

### Community 8 - "ui_sensorEditScreen.c"
Cohesion: 0.19
Nodes (24): lv_event_t, lv_obj_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb(), is_press_unit() (+16 more)

### Community 9 - "appcfg_cache_peek"
Cohesion: 0.14
Nodes (20): appcfg_cache_peek(), appcfg_save(), f_switch(), app_user_t, ui_auth_factory(), ui_auth_login(), ui_auth_set_factory_pin(), ui_auth_user_at() (+12 more)

### Community 10 - "bsp_display_indev_init"
Cohesion: 0.13
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 11 - "ui.h"
Cohesion: 0.06
Nodes (32): ui_datetimeScreen_screen_destroy(), ui_generalScreen_screen_destroy(), base_card(), lv_event_t, lv_obj_t, dim_cb(), lang_cb(), rebuild() (+24 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.22
Nodes (14): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume() (+6 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.21
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 16 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 17 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 18 - "ui.c"
Cohesion: 0.11
Nodes (35): ui_connectivityScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_pinScreen_screen_destroy(), ui_sensorEditScreen_screen_destroy(), ui_userEditScreen_screen_destroy() (+27 more)

### Community 19 - "ui_cfg.c"
Cohesion: 0.13
Nodes (28): lv_event_t, next(), AppConfig, lv_color_t, lv_obj_t, save_and_refresh(), set_str(), ui_cfg_apply_visual_mode() (+20 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.06
Nodes (68): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+60 more)

### Community 21 - "http_api.c"
Cohesion: 0.10
Nodes (33): device_state_t, mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_err_t, esp_event_base_t, net_core_init() (+25 more)

### Community 22 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 23 - "general_loaded_cb"
Cohesion: 0.18
Nodes (13): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), lv_timer_t, clamp_brightness(), ui_auth_logout(), ui_cfg_brightness() (+5 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.12
Nodes (16): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+8 more)

### Community 25 - "ui_nav_load"
Cohesion: 0.23
Nodes (12): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_nav_load(), ui_open_config_ble_cb() (+4 more)

### Community 26 - "ui_auth_can"
Cohesion: 0.35
Nodes (11): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+3 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.12
Nodes (25): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+17 more)

### Community 28 - "ui_keypadScreen.c"
Cohesion: 0.24
Nodes (8): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new(), fresh_keypad(), ui_open_keypad_cb()

### Community 29 - "task_tracer.c"
Cohesion: 0.31
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), timer_cb(), track_get_and_update(), TaskHandle_t

### Community 30 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+28 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "sd_monitor_task"
Cohesion: 0.32
Nodes (6): sd_monitor_task(), unmount_sdcard_hotplug(), lv_event_cb_t, ui_sd_close(), ui_sd_finish_restart(), ui_sd_progress()

### Community 33 - "ui_confirmScreen.c"
Cohesion: 0.33
Nodes (3): ui_confirmScreen_screen_destroy(), fresh_confirm(), ui_open_confirm_cb()

### Community 34 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 35 - "fresh_net_cloud"
Cohesion: 0.67
Nodes (3): ui_netCloudScreen_screen_destroy(), fresh_net_cloud(), ui_open_net_cloud_cb()

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 37 - "get_sensor_diag"
Cohesion: 0.67
Nodes (3): ui_sensorDiagScreen_screen_destroy(), get_sensor_diag(), ui_open_sensordiag_cb()

### Community 38 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 42 - "ui_loginScreen.c"
Cohesion: 0.29
Nodes (9): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+1 more)

### Community 44 - "ui_nav.c"
Cohesion: 0.20
Nodes (10): apply_authenticated_cb(), lv_event_t, ui_sensorEditScreen_refresh(), ui_edit_is_audio(), lv_obj_t, ui_nav_current(), ui_nav_init(), ui_nav_pop_to() (+2 more)

### Community 62 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

## Knowledge Gaps
- **65 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+60 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `wifi_mgr.c`, `app_main`, `ui_mainScreen.c`, `transport_ble.c`, `appcfg_patch`, `ui_sensorEditScreen.c`, `ui_edit_begin`, `cert_store.c`, `ui_cfg.c`, `general_loaded_cb`, `gas_label_color`, `fpm_ble_config.c`, `ui_auth_can`, `eth_mgr.c`, `screen_init_task`?**
  _High betweenness centrality (0.252) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `wifi_mgr.c`, `main.c`, `appcfg_cache_peek`, `bsp_display_indev_init`, `rtc_rv3028.c`, `bmp280.c`, `sensors_runtime.c`?**
  _High betweenness centrality (0.157) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `main.c`, `appcfg_patch`, `appcfg_cache_peek`, `sensors_runtime.c`, `http_api.c`, `task_tracer.c`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.104) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 39 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 39 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 35 INFERRED edges - model-reasoned connections that need verification._