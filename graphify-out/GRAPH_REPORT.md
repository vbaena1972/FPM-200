# Graph Report - ClaudeHMI-FW  (2026-08-01)

## Corpus Check
- 132 files · ~298,180 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 854 nodes · 2133 edges · 60 communities (59 shown, 1 thin omitted)
- Extraction: 66% EXTRACTED · 34% INFERRED · 0% AMBIGUOUS · INFERRED: 732 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `59df261f`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- ui_label
- wifi_mgr.c
- main.c
- ui.h
- ui_mainScreen.c
- ui.c
- transport_ble.c
- appcfg_patch
- ui_generalSimpleScreen.c
- app_main
- fpm_ble_config.c
- eth_mgr.c
- ui_cfg.c
- state_on_net_available
- appcfg_cache_peek
- cert_store.c
- http_api.c
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- ui_loginScreen.c
- ui_edit_begin
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- ui_auth_can
- ws_stream.c
- ui_generalSimpleScreen_screen_init
- ui_cfg_preview_brightness
- README.md

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 69 edges
2. `ui_label()` - 53 edges
3. `ui_box()` - 39 edges
4. `ui_col()` - 37 edges
5. `_t()` - 33 edges
6. `ui_card()` - 32 edges
7. `appcfg_save()` - 30 edges
8. `ui_icon()` - 30 edges
9. `ui_nav_load()` - 22 edges
10. `ui_screen_base()` - 20 edges

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

## Communities (60 total, 1 thin omitted)

### Community 0 - "ui_label"
Cohesion: 0.11
Nodes (76): lv_font_t, ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+68 more)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.06
Nodes (55): mem_diag_report(), eth_mgr_is_up(), on_eth_event(), cloud_mgr_connected(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t (+47 more)

### Community 2 - "main.c"
Cohesion: 0.05
Nodes (41): bsp_touch_config_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t, bsp_display_backlight_off() (+33 more)

### Community 3 - "ui.h"
Cohesion: 0.05
Nodes (26): alarm_mgr_test_buzzer(), ui_confirmScreen_screen_destroy(), rssi_pct(), ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_infoScreen_screen_destroy(), accept_cb(), lv_event_t (+18 more)

### Community 4 - "ui_mainScreen.c"
Cohesion: 0.07
Nodes (50): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_clinical_state_t (+42 more)

### Community 5 - "ui.c"
Cohesion: 0.10
Nodes (48): lv_event_t, pin_key_cb(), pin_wrong_flash(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), lv_event_t, lv_obj_t, lv_timer_t (+40 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.08
Nodes (30): ble_cmd_handler_t, append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event(), transport_ble_init() (+22 more)

### Community 7 - "appcfg_patch"
Cohesion: 0.13
Nodes (38): cfg_result_t, aws_telemetry_task(), esp_event_base_t, mqtt_event(), prepare_pem(), transport_mqtt_on_time_ready(), transport_mqtt_stop(), appcfg_cache_get() (+30 more)

### Community 8 - "ui_generalSimpleScreen.c"
Cohesion: 0.07
Nodes (32): ble_finish_async(), lv_event_t, protected_back_cb(), ui_generalScreen_screen_destroy(), base_card(), lv_event_cb_t, lv_event_t, lv_obj_t (+24 more)

### Community 9 - "app_main"
Cohesion: 0.09
Nodes (31): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), alarm_mgr_init() (+23 more)

### Community 10 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (33): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_obj(), AppConfig, cJSON, color_to_gas(), cpy() (+25 more)

### Community 11 - "eth_mgr.c"
Cohesion: 0.11
Nodes (26): appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start() (+18 more)

### Community 12 - "ui_cfg.c"
Cohesion: 0.11
Nodes (29): apply_authenticated_cb(), lv_event_t, lv_event_t, next(), general_loaded_cb(), allow_decimal(), app_user_t, ui_auth_active() (+21 more)

### Community 13 - "state_on_net_available"
Cohesion: 0.12
Nodes (20): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), esp_event_base_t (+12 more)

### Community 14 - "appcfg_cache_peek"
Cohesion: 0.15
Nodes (23): appcfg_cache_peek(), appcfg_save(), f_switch(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_auth_user_count() (+15 more)

### Community 15 - "cert_store.c"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 16 - "http_api.c"
Cohesion: 0.21
Nodes (17): state_build_json(), state_get(), esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler(), cfg_put_handler(), http_api_init() (+9 more)

### Community 17 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 18 - "ui_sensorEditScreen.c"
Cohesion: 0.19
Nodes (18): lv_event_t, lv_obj_t, ui_edit_target_t, card(), flow_card(), limit_edit(), limit_step(), mini() (+10 more)

### Community 19 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 20 - "ui_loginScreen.c"
Cohesion: 0.26
Nodes (10): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+2 more)

### Community 21 - "ui_edit_begin"
Cohesion: 0.24
Nodes (12): lv_event_t, edit_cb(), step_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes(), ui_cfg_pressure_max() (+4 more)

### Community 22 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.20
Nodes (9): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+1 more)

### Community 23 - "ui_auth_can"
Cohesion: 0.44
Nodes (9): app_user_role_t, ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_max_assignable(), ui_auth_role(), ui_auth_user_add(), ui_auth_user_update() (+1 more)

### Community 24 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 25 - "ui_generalSimpleScreen_screen_init"
Cohesion: 0.29
Nodes (8): ui_generalSimpleScreen_screen_init(), lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_lang(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 26 - "ui_cfg_preview_brightness"
Cohesion: 0.47
Nodes (6): bright_cb(), clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

## Knowledge Gaps
- **34 isolated node(s):** `0. Contexto`, `1. ⚠️ ESTADO — leer antes de nada`, `2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR`, `3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW`, `4. Cómo compilar / gotchas de build` (+29 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `ui_label`, `wifi_mgr.c`, `main.c`, `ui_mainScreen.c`, `transport_ble.c`, `appcfg_patch`, `app_main`, `fpm_ble_config.c`, `eth_mgr.c`, `ui_cfg.c`, `cert_store.c`, `ui_sensorEditScreen.c`, `ui_edit_begin`, `ui_auth_can`, `ui_generalSimpleScreen_screen_init`, `ui_cfg_preview_brightness`?**
  _High betweenness centrality (0.266) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `wifi_mgr.c`, `main.c`, `appcfg_patch`, `fpm_ble_config.c`, `ui_cfg.c`, `state_on_net_available`, `appcfg_cache_peek`?**
  _High betweenness centrality (0.102) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `wifi_mgr.c`, `ui_mainScreen.c`, `state_on_net_available`, `appcfg_cache_peek`?**
  _High betweenness centrality (0.093) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_apply_json()` and `fpm_ble_channel_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 44 INFERRED edges - model-reasoned connections that need verification._
- **Are the 33 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 33 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_col()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_col()` has 35 INFERRED edges - model-reasoned connections that need verification._