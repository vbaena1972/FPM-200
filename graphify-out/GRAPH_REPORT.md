# Graph Report - FPM-200  (2026-09-16)

## Corpus Check
- 147 files · ~321,102 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1055 nodes · 2553 edges · 86 communities (83 shown, 3 thin omitted)
- Extraction: 68% EXTRACTED · 32% INFERRED · 0% AMBIGUOUS · INFERRED: 820 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `3ba6ad1d`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- ui_connectivityScreen_screen_init
- lv_port_mem.c
- ui_refresh_task
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
- rtc_rv3028.c
- bmp280.c
- ui.c
- apply_authenticated_cb
- sensors_runtime.c
- http_api.c
- gas_label_color
- ui_cfg_preview_brightness
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- on_eth_event
- ui_cfg.c
- eth_mgr.c
- ui_generalSimpleScreen.c
- ms5803.c
- fpm_ble_config.c
- Migración HMI: SquareLine → Claude Design
- ui_usersScreen.c
- ui_wifi_main_icon.c
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_statusbar_request_refresh
- ws_stream.c
- ui_form.c
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- ui_userEditScreen.c
- save_cb
- ui_loginScreen.c
- ui_main_deinit_cb
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
- `ui_main_update()` --calls--> `alarm_mgr_get_sensor_faults()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/drivers/alarm_mgr.c
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  firmware/main/ui/screens/ui_infoScreen.c → firmware/components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  firmware/main/main.c → firmware/components/storage/storage_extras.c
- `statusbar_timer_cb()` --calls--> `transport_ble_is_advertising()`  [INFERRED]
  firmware/components/ui_bind/ui_statusbar_controller.c → firmware/components/transport_ble/transport_ble.c

## Import Cycles
- None detected.

## Communities (86 total, 3 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.06
Nodes (114): card_state_t, sensors_runtime_get_debug(), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_obj_t, row(), ui_datetimeScreen_screen_init() (+106 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.15
Nodes (22): apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals(), notify() (+14 more)

### Community 2 - "ui_connectivityScreen_screen_init"
Cohesion: 0.18
Nodes (17): conn_status_t, eth_mgr_is_up(), cloud_mgr_connected(), transport_ble_is_connected(), lv_event_cb_t, lv_obj_t, lv_timer_t, conn_card() (+9 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "ui_refresh_task"
Cohesion: 0.06
Nodes (41): alarm_state_change_cb_t, app_metrics_t, eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic() (+33 more)

### Community 5 - "main.c"
Cohesion: 0.11
Nodes (15): esp_trace_open_params_t, transport_mqtt_publish_now(), ble_json_worker(), bsp_i2c_init(), alarm_clinical_state_t, esp_err_t, esp_trace_get_user_params(), mount_sdcard_hotplug() (+7 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.06
Nodes (43): ble_cmd_handler_t, transport_mqtt_stop(), fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer() (+35 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.11
Nodes (42): cfg_result_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), prepare_pem() (+34 more)

### Community 8 - "ui_sensorEditScreen.c"
Cohesion: 0.14
Nodes (33): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), f_switch(), flow_card() (+25 more)

### Community 9 - "appcfg_cache_peek"
Cohesion: 0.12
Nodes (28): appcfg_cache_peek(), appcfg_save(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_auth_user_count(), ui_auth_user_remove() (+20 more)

### Community 10 - "bsp_display_indev_init"
Cohesion: 0.21
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 11 - "ui.h"
Cohesion: 0.12
Nodes (3): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_infoScreen_screen_destroy()

### Community 12 - "ui_sensorDiagScreen_screen_init"
Cohesion: 0.27
Nodes (13): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_refresh(), ui_sensorDiagScreen_screen_destroy(), ui_sensorDiagScreen_screen_init() (+5 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "rtc_rv3028.c"
Cohesion: 0.21
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 17 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 18 - "ui.c"
Cohesion: 0.07
Nodes (59): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_mainScreen_screen_destroy(), lv_event_t, pin_key_cb() (+51 more)

### Community 19 - "apply_authenticated_cb"
Cohesion: 0.33
Nodes (6): apply_authenticated_cb(), lv_event_t, ui_sensorEditScreen_refresh(), ui_cfg_press_from_disp(), ui_edit_apply(), ui_edit_is_audio()

### Community 20 - "sensors_runtime.c"
Cohesion: 0.07
Nodes (56): ads1115_channel_t, ads1115_init(), ads1115_is_ready(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+48 more)

### Community 21 - "http_api.c"
Cohesion: 0.21
Nodes (17): device_state_t, state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler() (+9 more)

### Community 22 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 23 - "ui_cfg_preview_brightness"
Cohesion: 0.38
Nodes (7): bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.11
Nodes (17): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+9 more)

### Community 25 - "on_eth_event"
Cohesion: 0.17
Nodes (16): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event() (+8 more)

### Community 26 - "ui_cfg.c"
Cohesion: 0.14
Nodes (28): lv_event_t, next(), app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+20 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): esp_eth_handle_t, esp_eth_mac_t, esp_eth_phy_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t (+18 more)

### Community 28 - "ui_generalSimpleScreen.c"
Cohesion: 0.23
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild() (+3 more)

### Community 29 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_is_ready(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 30 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup() (+28 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "ui_usersScreen.c"
Cohesion: 0.24
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 33 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 34 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 35 - "ui_statusbar_request_refresh"
Cohesion: 0.36
Nodes (10): activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_controller_init() (+2 more)

### Community 36 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 37 - "ui_form.c"
Cohesion: 0.24
Nodes (4): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 38 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 39 - "ui_userEditScreen.c"
Cohesion: 0.39
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 40 - "save_cb"
Cohesion: 0.33
Nodes (4): lv_event_t, save_cb(), set_str(), ui_netWifiScreen_screen_destroy()

### Community 42 - "ui_loginScreen.c"
Cohesion: 0.21
Nodes (13): add_user(), app_user_role_t, app_user_t, lv_event_t, lv_obj_t, key_cb(), refresh(), role_color() (+5 more)

### Community 43 - "ui_main_deinit_cb"
Cohesion: 0.40
Nodes (5): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), lv_event_t, restart_btn_event_cb(), ui_main_deinit_cb()

### Community 44 - "ui_nav.c"
Cohesion: 0.22
Nodes (13): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color(), lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb() (+5 more)

### Community 62 - "screen_init_task"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

## Knowledge Gaps
- **66 isolated node(s):** `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT`, `2. Dos esquemas de canal (importante)` (+61 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **3 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `ui_connectivityScreen_screen_init`, `ui_refresh_task`, `transport_ble.c`, `appcfg_patch`, `save_cb`, `ui_sensorEditScreen.c`, `ui_sensorDiagScreen_screen_init`, `cert_store.c`, `apply_authenticated_cb`, `ui_cfg_preview_brightness`, `gas_label_color`, `fpm_ble_config.c`, `ui_cfg.c`, `eth_mgr.c`, `screen_init_task`?**
  _High betweenness centrality (0.252) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ui_wifi_main_icon.c`, `ui_statusbar_request_refresh`, `main.c`, `appcfg_cache_peek`, `bsp_display_indev_init`, `rtc_rv3028.c`, `bmp280.c`, `sensors_runtime.c`, `ms5803.c`?**
  _High betweenness centrality (0.157) - this node is a cross-community bridge._
- **Why does `app_main()` connect `ui_refresh_task` to `main.c`, `appcfg_patch`, `appcfg_cache_peek`, `sensors_runtime.c`, `on_eth_event`, `ui_cfg.c`, `fpm_ble_config.c`?**
  _High betweenness centrality (0.104) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_read_json()` and `fpm_ble_cloud_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 39 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 39 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 35 INFERRED edges - model-reasoned connections that need verification._