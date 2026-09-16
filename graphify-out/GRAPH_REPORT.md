# Graph Report - FPM-200  (2026-09-15)

## Corpus Check
- 145 files · ~316,024 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1010 nodes · 2447 edges · 82 communities (79 shown, 3 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 797 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `797f04ce`
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
- appcfg_cache_peek
- save_cb
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_statusbar_request_refresh
- gas_label_color
- eth_mgr.c
- rtc_rv3028.c
- app_user_t
- Migración HMI: SquareLine → Claude Design
- ms5803.c
- ui_loginScreen.c
- state_on_net_available
- ws_stream.c
- ui_cfg_pressure_unit
- on_eth_event
- ui_cfg_apply_visual_mode
- save_cb
- README.md
- screen_init_task
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
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  firmware/main/ui/screens/ui_infoScreen.c → firmware/components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `app_main()` --calls--> `task_tracer_start()`  [INFERRED]
  firmware/main/main.c → firmware/components/task_tracer/task_tracer.c
- `get_bleapp()` --calls--> `ui_bleAppScreen_screen_destroy()`  [INFERRED]
  firmware/main/ui/ui.c → firmware/main/ui/screens/ui_bleAppScreen.c

## Import Cycles
- None detected.

## Communities (82 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.08
Nodes (95): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+87 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.24
Nodes (11): apply_ip_mode(), AppConfig, net_cfg_equals(), wifi_apply_cb(), wifi_mgr_apply_from_cache(), wifi_mgr_init(), wifi_schedule_apply(), wifi_view_from_appcfg() (+3 more)

### Community 2 - "main.c"
Cohesion: 0.11
Nodes (15): esp_trace_open_params_t, ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), ble_json_worker(), bsp_i2c_init(), esp_err_t, lv_event_t, esp_trace_get_user_params() (+7 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.07
Nodes (44): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process() (+36 more)

### Community 5 - "ui.h"
Cohesion: 0.06
Nodes (21): ui_confirmScreen_screen_destroy(), rssi_pct(), ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb() (+13 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (43): ble_cmd_handler_t, transport_mqtt_stop(), fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer() (+35 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.12
Nodes (40): cfg_result_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), prepare_pem() (+32 more)

### Community 8 - "fpm_ble_config.c"
Cohesion: 0.19
Nodes (34): appcfg_cache_reload(), appcfg_save(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON (+26 more)

### Community 9 - "ui_cfg.c"
Cohesion: 0.15
Nodes (30): lv_event_t, next(), app_user_role_t, AppConfig, save_and_refresh(), set_str(), ui_auth_active(), ui_auth_can() (+22 more)

### Community 10 - "http_api.c"
Cohesion: 0.21
Nodes (17): device_state_t, state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+9 more)

### Community 11 - "example_eth_init"
Cohesion: 0.36
Nodes (10): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, esp_err_t, eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init() (+2 more)

### Community 12 - "ui_cfg_preview_brightness"
Cohesion: 0.32
Nodes (8): bright_cb(), lv_timer_t, clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb(), splash_done_cb()

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "task_tracer.c"
Cohesion: 0.31
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), timer_cb(), track_get_and_update(), TaskHandle_t

### Community 17 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (23): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+15 more)

### Community 18 - "ui.c"
Cohesion: 0.05
Nodes (76): ui_infoScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), app_user_role_t, lv_event_t (+68 more)

### Community 19 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.07
Nodes (55): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_write_reg(), esp_err_t (+47 more)

### Community 21 - "app_main"
Cohesion: 0.20
Nodes (11): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), i2c_master_bus_handle_t (+3 more)

### Community 22 - "appcfg_cache_peek"
Cohesion: 0.09
Nodes (31): alarm_mgr_test_buzzer(), appcfg_cache_peek(), fpm_ble_cloud_read_json(), fpm_ble_info_json(), fpm_ble_wifi_read_json(), lv_event_t, edit_cb(), rebuild() (+23 more)

### Community 23 - "save_cb"
Cohesion: 0.23
Nodes (10): esp_err_t, wifi_status_t, notify(), wifi_mgr_start(), wifi_mgr_stop(), wifi_mgr_update_credentials(), lv_event_t, save_cb() (+2 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.13
Nodes (14): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+6 more)

### Community 25 - "ui_statusbar_request_refresh"
Cohesion: 0.17
Nodes (16): conn_status_t, eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), lv_obj_t, lv_timer_t, set_glyph() (+8 more)

### Community 26 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.25
Nodes (12): eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage() (+4 more)

### Community 28 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 29 - "app_user_t"
Cohesion: 0.67
Nodes (3): app_user_t, ui_auth_factory(), ui_auth_login()

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 33 - "ms5803.c"
Cohesion: 0.44
Nodes (10): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+2 more)

### Community 34 - "ui_loginScreen.c"
Cohesion: 0.26
Nodes (11): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+3 more)

### Community 35 - "state_on_net_available"
Cohesion: 0.31
Nodes (9): esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event(), time_mgr_start_sntp(), transport_mqtt_on_net_down() (+1 more)

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 38 - "ui_cfg_pressure_unit"
Cohesion: 0.15
Nodes (16): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), allow_decimal(), ui_cfg_brightness(), ui_cfg_flow_to_disp() (+8 more)

### Community 39 - "on_eth_event"
Cohesion: 0.22
Nodes (10): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, log_dns_info(), wifi_event_handler() (+2 more)

### Community 41 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 43 - "save_cb"
Cohesion: 0.40
Nodes (4): lv_event_t, save_cb(), set_str(), ui_netEthScreen_screen_destroy()

### Community 61 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.14
Nodes (11): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+3 more)

### Community 65 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

## Knowledge Gaps
- **57 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+52 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `ui_mainScreen.c`, `ui.h`, `transport_ble.c`, `appcfg_patch`, `fpm_ble_config.c`, `ui_cfg.c`, `ui_cfg_preview_brightness`, `cert_store.c`, `ui.c`, `app_main`, `save_cb`, `ui_statusbar_request_refresh`, `gas_label_color`, `app_user_t`, `ui_cfg_pressure_unit`, `ui_cfg_apply_visual_mode`, `save_cb`, `screen_init_task`?**
  _High betweenness centrality (0.247) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ms5803.c`, `main.c`, `ui_wifi_main_icon.c`, `bmp280.c`, `sensors_runtime.c`, `appcfg_cache_peek`, `ui_statusbar_request_refresh`, `rtc_rv3028.c`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.144) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `main.c`, `state_on_net_available`, `ui_mainScreen.c`, `on_eth_event`, `appcfg_patch`, `fpm_ble_config.c`, `ui_cfg.c`, `task_tracer.c`, `sensors_runtime.c`, `appcfg_cache_peek`?**
  _High betweenness centrality (0.089) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._