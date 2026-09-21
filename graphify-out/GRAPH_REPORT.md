# Graph Report - FPM-200  (2026-09-21)

## Corpus Check
- 147 files · ~325,700 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1068 nodes · 2600 edges · 78 communities (74 shown, 4 thin omitted)
- Extraction: 68% EXTRACTED · 32% INFERRED · 0% AMBIGUOUS · INFERRED: 837 edges (avg confidence: 0.85)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `a40fe325`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- ui_connectivityScreen.c
- lv_port_mem.c
- ui_mainScreen.c
- main.c
- transport_ble.c
- appcfg_patch
- ui_sensorEditScreen.c
- appcfg_cache_peek
- bsp_display_indev_init
- ui.h
- ui_sensorDiagScreen_screen_init
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- cert_store.c
- task_tracer.c
- bmp280.c
- ui.c
- apply_authenticated_cb
- sensors_runtime.c
- http_api.c
- gas_label_color
- save_and_refresh
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- state_on_net_available
- ui_cfg.c
- eth_mgr.c
- state_pub.c
- ms5803.c
- fpm_ble_config.c
- Migración HMI: SquareLine → Claude Design
- ui_wifi_main_icon.c
- statusbar_timer_cb
- ws_stream.c
- ui_statusbar_request_refresh
- ui_loginScreen.c
- README.md
- AGENTS.md
- CLAUDE.md

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 73 edges
2. `ui_label()` - 56 edges
3. `ui_col()` - 44 edges
4. `ui_box()` - 41 edges
5. `_t()` - 35 edges
6. `ui_card()` - 33 edges
7. `appcfg_save()` - 32 edges
8. `ui_icon()` - 30 edges
9. `ui_sensorEditScreen_screen_init()` - 24 edges
10. `screen_init_task()` - 23 edges

## Surprising Connections (you probably didn't know these)
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `mqtt_event()` --calls--> `ui_statusbar_signal_cloud_activity()`  [INFERRED]
  firmware/components/network_core/transport_mqtt.c → firmware/components/ui_bind/ui_statusbar_controller.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `app_main()` --calls--> `task_tracer_start()`  [INFERRED]
  firmware/main/main.c → firmware/components/task_tracer/task_tracer.c
- `statusbar_timer_cb()` --calls--> `transport_ble_is_advertising()`  [INFERRED]
  firmware/components/ui_bind/ui_statusbar_controller.c → firmware/components/transport_ble/transport_ble.c

## Import Cycles
- None detected.

## Communities (78 total, 4 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.09
Nodes (87): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+79 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.19
Nodes (16): apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb(), wifi_mgr_apply_from_cache(), wifi_mgr_get_netinfo(), wifi_mgr_init() (+8 more)

### Community 2 - "ui_connectivityScreen.c"
Cohesion: 0.21
Nodes (13): conn_status_t, cloud_mgr_connected(), transport_ble_is_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick() (+5 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.06
Nodes (58): alarm_state_change_cb_t, app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_press_mute() (+50 more)

### Community 5 - "main.c"
Cohesion: 0.05
Nodes (52): esp_trace_open_params_t, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+44 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (41): ble_cmd_handler_t, transport_mqtt_stop(), fpm_ble_config_set_clock_cb(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+33 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.08
Nodes (52): cfg_result_t, eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+44 more)

### Community 8 - "ui_sensorEditScreen.c"
Cohesion: 0.18
Nodes (25): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), f_switch(), flow_card() (+17 more)

### Community 9 - "appcfg_cache_peek"
Cohesion: 0.10
Nodes (29): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), bright_cb(), lv_color_t, lv_obj_t, clamp_brightness(), ui_auth_factory() (+21 more)

### Community 10 - "bsp_display_indev_init"
Cohesion: 0.13
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 11 - "ui.h"
Cohesion: 0.05
Nodes (25): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general() (+17 more)

### Community 12 - "ui_sensorDiagScreen_screen_init"
Cohesion: 0.21
Nodes (16): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), ui_sensorDiagScreen_screen_init(), volume_cb() (+8 more)

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

### Community 17 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 18 - "ui.c"
Cohesion: 0.06
Nodes (70): menu_item_cb(), app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy() (+62 more)

### Community 19 - "apply_authenticated_cb"
Cohesion: 0.14
Nodes (17): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), allow_decimal(), ui_sensorDiagScreen_refresh(), ui_cfg_flow_to_disp() (+9 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.07
Nodes (57): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+49 more)

### Community 21 - "http_api.c"
Cohesion: 0.18
Nodes (22): esp_err_t, mdns_svc_start(), add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t (+14 more)

### Community 22 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 23 - "save_and_refresh"
Cohesion: 0.25
Nodes (9): is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.10
Nodes (19): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+11 more)

### Community 25 - "state_on_net_available"
Cohesion: 0.26
Nodes (11): esp_event_base_t, on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event(), on_wifi_event() (+3 more)

### Community 26 - "ui_cfg.c"
Cohesion: 0.16
Nodes (26): lv_event_t, next(), app_user_role_t, set_str(), ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+18 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+18 more)

### Community 29 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_is_ready(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 30 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (37): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+29 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 33 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 35 - "statusbar_timer_cb"
Cohesion: 0.33
Nodes (10): activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_set_enabled() (+2 more)

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 40 - "ui_statusbar_request_refresh"
Cohesion: 0.17
Nodes (16): mem_diag_report(), eth_mgr_is_up(), on_eth_event(), esp_event_base_t, esp_netif_t, wifi_status_t, log_dns_info(), notify() (+8 more)

### Community 42 - "ui_loginScreen.c"
Cohesion: 0.18
Nodes (16): add_user(), app_user_role_t, app_user_t, lv_event_t, lv_obj_t, key_cb(), refresh(), role_color() (+8 more)

## Knowledge Gaps
- **62 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+57 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **4 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `ui_connectivityScreen.c`, `ui_mainScreen.c`, `main.c`, `transport_ble.c`, `appcfg_patch`, `ui_sensorEditScreen.c`, `ui_sensorDiagScreen_screen_init`, `cert_store.c`, `apply_authenticated_cb`, `http_api.c`, `gas_label_color`, `save_and_refresh`, `ui_cfg.c`, `eth_mgr.c`, `fpm_ble_config.c`, `statusbar_timer_cb`, `ui_statusbar_request_refresh`, `ui_loginScreen.c`?**
  _High betweenness centrality (0.251) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `ui_wifi_main_icon.c`, `statusbar_timer_cb`, `ui_statusbar_request_refresh`, `appcfg_cache_peek`, `bsp_display_indev_init`, `bmp280.c`, `sensors_runtime.c`, `ms5803.c`?**
  _High betweenness centrality (0.155) - this node is a cross-community bridge._
- **Why does `app_main()` connect `appcfg_patch` to `ui_mainScreen.c`, `main.c`, `ui_statusbar_request_refresh`, `appcfg_cache_peek`, `task_tracer.c`, `sensors_runtime.c`, `http_api.c`, `state_on_net_available`, `ui_cfg.c`?**
  _High betweenness centrality (0.088) - this node is a cross-community bridge._
- **Are the 70 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 70 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 35 INFERRED edges - model-reasoned connections that need verification._