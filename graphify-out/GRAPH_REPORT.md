# Graph Report - ClaudeHMI-FW  (2026-08-20)

## Corpus Check
- 145 files · ~314,394 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 998 nodes · 2428 edges · 82 communities (78 shown, 4 thin omitted)
- Extraction: 67% EXTRACTED · 33% INFERRED · 0% AMBIGUOUS · INFERRED: 797 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `730f11b9`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- screen_init_task
- lv_port_mem.c
- ui_mainScreen.c
- ui_nav.c
- transport_ble.c
- appcfg_patch
- ui_nav_back
- ui_pinScreen.c
- http_api.c
- eth_mgr.c
- ui_sensorEditScreen_screen_init
- pressure_card
- cert_store.c
- app_main
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- general_loaded_cb
- Migración HMI: SquareLine → Claude Design
- sensors_runtime.c
- ui_cfg.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- ws_stream.c
- statusbar_timer_cb
- ui_generalSimpleScreen.c
- README.md
- ui.c
- bsp_display_indev_init
- fpm_ble_config.c
- ui_wifi_main_icon.c
- save_cb
- state_on_net_available
- ui_statusbar_request_refresh
- ui_cfg_apply_visual_mode
- on_eth_event
- ui.h
- ui_form.c
- AGENTS.md
- CLAUDE.md
- ui_loginScreen.c
- ui_keypadScreen.c
- ui_sensorEditScreen.c
- ui_edit_begin
- ui_connectivityScreen.c

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

## Communities (82 total, 4 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.09
Nodes (88): lv_font_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init() (+80 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.16
Nodes (20): apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals(), notify() (+12 more)

### Community 2 - "screen_init_task"
Cohesion: 0.06
Nodes (51): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+43 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_monitor_t, lv_mem_pool_t, lv_result_t, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core()

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.08
Nodes (42): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_clinical_state_t (+34 more)

### Community 5 - "ui_nav.c"
Cohesion: 0.33
Nodes (8): lv_event_t, lv_obj_t, ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace(), ui_nav_swap()

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (42): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+34 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.13
Nodes (37): cfg_result_t, esp_event_base_t, mqtt_event(), prepare_pem(), transport_mqtt_on_time_ready(), transport_mqtt_stop(), appcfg_cache_get(), appcfg_defaults() (+29 more)

### Community 8 - "ui_nav_back"
Cohesion: 0.36
Nodes (8): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), ui_nav_back()

### Community 9 - "ui_pinScreen.c"
Cohesion: 0.32
Nodes (6): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry()

### Community 10 - "http_api.c"
Cohesion: 0.21
Nodes (17): state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler(), http_api_init() (+9 more)

### Community 11 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start() (+18 more)

### Community 13 - "ui_sensorEditScreen_screen_init"
Cohesion: 0.14
Nodes (24): gas_label_color(), lv_obj_t, ui_edit_target_t, card(), dec_seg(), flow_card(), mini(), seg() (+16 more)

### Community 14 - "pressure_card"
Cohesion: 0.21
Nodes (12): apply_authenticated_cb(), lv_event_t, allow_decimal(), pressure_card(), ui_cfg_press_fmt(), ui_cfg_press_from_disp(), ui_cfg_press_to_disp(), ui_cfg_pressure_limit_enabled() (+4 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "app_main"
Cohesion: 0.11
Nodes (20): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), alarm_mgr_init() (+12 more)

### Community 17 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 18 - "general_loaded_cb"
Cohesion: 0.24
Nodes (10): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness() (+2 more)

### Community 19 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 20 - "sensors_runtime.c"
Cohesion: 0.06
Nodes (59): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_write_reg(), esp_err_t (+51 more)

### Community 21 - "ui_cfg.c"
Cohesion: 0.10
Nodes (51): appcfg_cache_peek(), appcfg_save(), lv_event_t, next(), f_switch(), app_user_role_t, app_user_t, AppConfig (+43 more)

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
Nodes (10): eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), lv_timer_t, statusbar_timer_cb(), conn_status_t, cpy() (+2 more)

### Community 26 - "ui_generalSimpleScreen.c"
Cohesion: 0.31
Nodes (8): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy()

### Community 60 - "ui.c"
Cohesion: 0.08
Nodes (55): ui_infoScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name() (+47 more)

### Community 62 - "bsp_display_indev_init"
Cohesion: 0.14
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
Cohesion: 0.50
Nodes (4): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color()

### Community 70 - "on_eth_event"
Cohesion: 0.50
Nodes (5): mem_diag_report(), on_eth_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop()

### Community 71 - "ui.h"
Cohesion: 0.11
Nodes (4): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), lv_event_cb_t, ui_sd_finish_restart()

### Community 72 - "ui_form.c"
Cohesion: 0.25
Nodes (3): lv_event_t, kb_event_cb(), ta_event_cb()

### Community 76 - "ui_loginScreen.c"
Cohesion: 0.23
Nodes (11): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+3 more)

### Community 77 - "ui_keypadScreen.c"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 79 - "ui_sensorEditScreen.c"
Cohesion: 0.33
Nodes (11): lv_event_t, dec_cb(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch(), rebuild() (+3 more)

### Community 80 - "ui_edit_begin"
Cohesion: 0.17
Nodes (16): alarm_mgr_test_buzzer(), lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb() (+8 more)

## Knowledge Gaps
- **54 isolated node(s):** `graphify`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)`, `display ↔ `general` / `DisplayConfig`` (+49 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **4 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `ui_cfg.c` to `ui_label`, `screen_init_task`, `save_cb`, `ui_mainScreen.c`, `transport_ble.c`, `appcfg_patch`, `eth_mgr.c`, `ui_sensorEditScreen_screen_init`, `pressure_card`, `cert_store.c`, `app_main`, `ui_edit_begin`, `general_loaded_cb`, `statusbar_timer_cb`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.248) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ui_wifi_main_icon.c`, `ui_statusbar_request_refresh`, `sensors_runtime.c`, `ui_cfg.c`, `bsp_display_indev_init`?**
  _High betweenness centrality (0.140) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `screen_init_task`, `state_on_net_available`, `on_eth_event`, `appcfg_patch`, `sensors_runtime.c`, `ui_cfg.c`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.094) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 34 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 34 INFERRED edges - model-reasoned connections that need verification._
- **Are the 37 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 37 INFERRED edges - model-reasoned connections that need verification._