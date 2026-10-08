# Graph Report - FPM-200  (2026-10-08)

## Corpus Check
- 186 files · ~357,276 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1408 nodes · 3540 edges · 94 communities (87 shown, 7 thin omitted)
- Extraction: 72% EXTRACTED · 28% INFERRED · 0% AMBIGUOUS · INFERRED: 1000 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `6467cdb2`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- save_cb
- esp_err_to_name
- ui_mainScreen.c
- lv_port_mem.c
- alarm_mgr.c
- ui_wifi_main_icon.c
- transport_ble.c
- driver_delay_ms
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- bmp280.c
- fpm_ble_config.c
- ui_sensorDiagScreen.c
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- esp_log.h
- ui_label
- Matriz de casos
- cert_store.c
- ui_sensorEditScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- ws_stream.c
- eth_mgr.c
- appmetrics_load
- ui_nav_load
- esp_eth_phy_ksz8851snl.c
- Migración HMI: SquareLine → Claude Design
- esp_err.h
- sensor_cfg_persist
- test_alarm.c
- ui_statusbar_controller.c
- ui_cfg.c
- general_loaded_cb
- ui_generalSimpleScreen.c
- vTaskDelay
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- ui_keypadScreen.c
- ui_loginScreen.c
- README.md
- transport_mqtt.c
- gen_ota_signing_key.sh script
- bsp_touch_new
- ui_usersScreen.c
- gas_label_color
- task_tracer.c
- ui_pinScreen.c
- Changelog
- flow_meter.c
- ui_auth_can
- ui_bleAppScreen.c
- ui.c
- Consumo y endurecimiento 1.5.5-dev
- ui.h
- at24c256.c
- esp_timer.h
- Microchip KSZ8851SNL Ethernet Driver
- AGENTS.md
- CLAUDE.md
- README_PARCHE.md
- app_main
- on_network_ready
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- wifi_apply_cb

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 56 edges
3. `esp_err_to_name()` - 45 edges
4. `ui_col()` - 44 edges
5. `ui_box()` - 41 edges
6. `_t()` - 35 edges
7. `ui_card()` - 33 edges
8. `appcfg_save()` - 32 edges
9. `ui_icon()` - 30 edges
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 26 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `app_main()` --calls--> `fpm_i2c_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/drivers/fpm_i2c_guard.c
- `sensors_acq_task()` --calls--> `sfm3300_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `sensors_acq_task()` --calls--> `sfm3300_get_last_rx()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c

## Import Cycles
- None detected.

## Communities (94 total, 7 thin omitted)

### Community 0 - "save_cb"
Cohesion: 0.33
Nodes (4): eth_mgr_is_up(), lv_event_t, save_cb(), set_str()

### Community 1 - "esp_err_to_name"
Cohesion: 0.15
Nodes (25): time_sync_worker(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals() (+17 more)

### Community 2 - "ui_mainScreen.c"
Cohesion: 0.10
Nodes (41): card_state_t, sensors_runtime_get_debug(), alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_silence_cb(), banner_open_cb(), alarm_clinical_state_t (+33 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.17
Nodes (7): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.16
Nodes (27): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+19 more)

### Community 5 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 6 - "transport_ble.c"
Cohesion: 0.11
Nodes (25): ble_cmd_handler_t, fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event() (+17 more)

### Community 7 - "driver_delay_ms"
Cohesion: 0.42
Nodes (11): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+3 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.13
Nodes (51): emac_ksz8851snl_t, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t, eth_duplex_t, eth_link_t (+43 more)

### Community 9 - "ads1115.c"
Cohesion: 0.44
Nodes (10): ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg() (+2 more)

### Community 10 - "bmp280.c"
Cohesion: 0.38
Nodes (9): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.22
Nodes (14): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume() (+6 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "esp_log.h"
Cohesion: 0.22
Nodes (3): time_mgr_start_sntp(), config_mode_enter(), log_heap()

### Community 16 - "ui_label"
Cohesion: 0.08
Nodes (93): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+85 more)

### Community 17 - "Matriz de casos"
Cohesion: 0.07
Nodes (26): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+18 more)

### Community 18 - "cert_store.c"
Cohesion: 0.21
Nodes (18): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+10 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.17
Nodes (27): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+19 more)

### Community 20 - "storage.c"
Cohesion: 0.09
Nodes (52): cfg_result_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate(), appcfg_take_pin_migration(), app_user_t (+44 more)

### Community 21 - "http_api.c"
Cohesion: 0.08
Nodes (42): eTaskState, diag_worker(), mem_diag_report(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+34 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.17
Nodes (27): ads1115_is_ready(), ms5803_is_ready(), AppConfig, sensor_sample_t, sensor_signal_stats_t, lock(), median3f(), median5f() (+19 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.39
Nodes (8): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.07
Nodes (26): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 6. Pendiente (después de estabilizar §2 y validar §3) (+18 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 27 - "eth_mgr.c"
Cohesion: 0.12
Nodes (25): esp_eth_handle_t, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init() (+17 more)

### Community 28 - "appmetrics_load"
Cohesion: 0.47
Nodes (4): app_metrics_t, appmetrics_load(), appmetrics_store(), esp_err_t

### Community 29 - "ui_nav_load"
Cohesion: 0.15
Nodes (15): bleapp_goroot_async(), get_general(), lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init() (+7 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (27): eth_phy_autoneg_cmd_t, eth_phy_config_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t (+19 more)

### Community 31 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 32 - "esp_err.h"
Cohesion: 0.07
Nodes (14): esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_err_t, i2c_master_bus_handle_t, fpm_new_panel_io_i2c(), panel_io_i2c_del() (+6 more)

### Community 33 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 34 - "test_alarm.c"
Cohesion: 0.18
Nodes (12): set_buzzer_acoustic(), appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config(), ledc_set_duty() (+4 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.32
Nodes (11): transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb() (+3 more)

### Community 36 - "ui_cfg.c"
Cohesion: 0.11
Nodes (33): lv_event_t, next(), app_user_t, AppConfig, lv_color_t, lv_obj_t, save_and_refresh(), set_str() (+25 more)

### Community 37 - "general_loaded_cb"
Cohesion: 0.50
Nodes (4): lv_event_t, general_loaded_cb(), protected_back_cb(), ui_auth_logout()

### Community 38 - "ui_generalSimpleScreen.c"
Cohesion: 0.33
Nodes (7): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 39 - "vTaskDelay"
Cohesion: 0.22
Nodes (13): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+5 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.17
Nodes (15): conn_status_t, cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_connected(), lv_timer_t, conn_signature(), cpy(), gather() (+7 more)

### Community 41 - "main.c"
Cohesion: 0.05
Nodes (58): esp_app_desc_t, esp_partition_t, esp_trace_open_params_t, FILE, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set() (+50 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.11
Nodes (27): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), bright_cb(), f_switch(), clamp_brightness(), ui_auth_set_factory_pin(), ui_auth_user_count() (+19 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.35
Nodes (16): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_init() (+8 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.24
Nodes (8): ui_confirmScreen_screen_destroy(), accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_edit_set_new(), fresh_confirm(), ui_open_confirm_cb()

### Community 45 - "ui_loginScreen.c"
Cohesion: 0.16
Nodes (16): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+8 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.14
Nodes (22): esp_mqtt_event_handle_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), mqtt_start_locked() (+14 more)

### Community 51 - "bsp_touch_new"
Cohesion: 0.18
Nodes (12): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+4 more)

### Community 52 - "ui_usersScreen.c"
Cohesion: 0.17
Nodes (17): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index() (+9 more)

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.31
Nodes (6): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 56 - "ui_pinScreen.c"
Cohesion: 0.27
Nodes (8): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "flow_meter.c"
Cohesion: 0.27
Nodes (7): flow_meter_get(), flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t, flow_meter_snapshot_t

### Community 60 - "ui_auth_can"
Cohesion: 0.24
Nodes (14): apply_authenticated_cb(), lv_event_t, app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user() (+6 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (9): bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_loaded_cb(), bleapp_pair_pin(), bleapp_tick_cb(), bleapp_unloaded_cb(), AppConfig, lv_event_t (+1 more)

### Community 62 - "ui.c"
Cohesion: 0.08
Nodes (43): ui_bleAppScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_infoScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy() (+35 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 67 - "at24c256.c"
Cohesion: 0.30
Nodes (13): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+5 more)

### Community 69 - "esp_timer.h"
Cohesion: 0.29
Nodes (4): device_state_t, state_build_json(), state_get(), state_update_sensors()

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 89 - "app_main"
Cohesion: 0.29
Nodes (7): transport_mqtt_runtime_init(), esp_err_t, flow_meter_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), task_tracer_start(), app_main()

### Community 90 - "on_network_ready"
Cohesion: 0.40
Nodes (3): esp_err_t, mdns_svc_start(), on_network_ready()

### Community 91 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 92 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

## Knowledge Gaps
- **128 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+123 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `save_cb`, `ui_mainScreen.c`, `alarm_mgr.c`, `transport_ble.c`, `fpm_ble_config.c`, `ui_sensorDiagScreen.c`, `ui_label`, `cert_store.c`, `ui_sensorEditScreen.c`, `storage.c`, `http_api.c`, `eth_mgr.c`, `ui_cfg.c`, `ui_connectivityScreen.c`, `main.c`, `transport_mqtt.c`, `gas_label_color`, `ui_auth_can`, `ui.c`, `app_main`, `on_network_ready`?**
  _High betweenness centrality (0.139) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `transport_ble.c`, `driver_delay_ms`, `ads1115.c`, `bmp280.c`, `fpm_ble_config.c`, `cert_store.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `esp_err.h`, `sensor_cfg_persist`, `vTaskDelay`, `main.c`, `fpm_i2c_guard.c`, `bsp_touch_new`, `at24c256.c`, `app_main`, `on_network_ready`?**
  _High betweenness centrality (0.055) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `main.c` to `at24c256.c`, `ui_statusbar_controller.c`, `ui_wifi_main_icon.c`, `vTaskDelay`, `appcfg_cache_peek`, `bsp_touch_new`, `ui.c`?**
  _High betweenness centrality (0.050) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 47 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 47 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 44 INFERRED edges - model-reasoned connections that need verification._
- **Are the 42 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 42 INFERRED edges - model-reasoned connections that need verification._