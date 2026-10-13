# Graph Report - FPM-200  (2026-10-12)

## Corpus Check
- 190 files · ~426,738 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1595 nodes · 4339 edges · 77 communities (71 shown, 6 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1056 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `994282f9`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- task_tracer.c
- esp_err_to_name
- esp_timer_get_time
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- save_cb
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- esp_log.h
- transport_ble.c
- ui_sensorDiagScreen.c
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_userEditScreen.c
- sensor_cfg_persist
- storage.c
- http_api.c
- sensors_runtime.c
- flow_meter.c
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- ft5x06.c
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- ui_mainScreen.c
- esp_err.h
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- sd_monitor_task
- ui_generalSimpleScreen.c
- ui_wifi_main_icon_init
- vTaskDelay
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- ui_sensorEditScreen.c
- ui_cfg_flow_limit
- README.md
- test_alarm.c
- display_idle_cb
- gen_ota_signing_key.sh script
- ui_i18n
- ui_keypadScreen.c
- Migración HMI: SquareLine → Claude Design
- mem_diag.c
- ui_nav.c
- Changelog
- string
- esp_eth_driver
- apply_authenticated_cb
- ui_bleAppScreen.c
- ui_wifi_main_icon.c
- gas_label_color
- esp_lcd_st7796.h
- lvgl
- on_network_ready
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_cfg.c
- AGENTS.md
- CLAUDE.md
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- wifi_event_handler
- README_PARCHE.md
- ui_infoScreen.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 78 edges
2. `ui_label()` - 61 edges
3. `esp_err_to_name()` - 50 edges
4. `ui_col()` - 46 edges
5. `ui_box()` - 43 edges
6. `_t()` - 39 edges
7. `ui_card()` - 38 edges
8. `appcfg_save()` - 33 edges
9. `ui_icon()` - 33 edges
10. `esp_timer_get_time()` - 25 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `sensors_acq_task()` --calls--> `sfm3300_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `sensors_acq_task()` --calls--> `sfm3300_get_last_rx()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c

## Import Cycles
- None detected.

## Communities (77 total, 6 thin omitted)

### Community 0 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), task_tracer, TaskHandle_t

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (27): esp_wifi_types, time_sync_worker(), apply_ip_mode(), AppConfig, esp_err_t, wifi_status_t, net_cfg_equals(), notify() (+19 more)

### Community 2 - "esp_timer_get_time"
Cohesion: 0.12
Nodes (16): device_state_t, fpm_i2c_init(), transport_mqtt_runtime_init(), esp_err_t, flow_meter_init(), i2c_master_bus_handle_t, sensors_runtime_flow_warming(), sensors_runtime_flow_warming_elapsed_ms() (+8 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.11
Nodes (26): esp_mqtt_event_handle_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), mqtt_start_locked() (+18 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.16
Nodes (27): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+19 more)

### Community 5 - "ui.c"
Cohesion: 0.10
Nodes (50): ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), lv_event_t, lv_obj_t, lv_timer_t (+42 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.16
Nodes (23): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+15 more)

### Community 7 - "save_cb"
Cohesion: 0.19
Nodes (18): fpm_ble_config_set_clock_cb(), esp_err_t, transport_ble_init(), transport_ble_set_enabled(), transport_ble_set_finish_cb(), transport_ble_set_mesh_enabled(), transport_ble_set_passkey_mode(), transport_ble_set_tx_power() (+10 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.12
Nodes (53): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t (+45 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "esp_log.h"
Cohesion: 0.29
Nodes (3): set_str(), ui_netBleScreen_screen_destroy(), transport_ble

### Community 11 - "transport_ble.c"
Cohesion: 0.11
Nodes (16): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, append_json() (+8 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.18
Nodes (16): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_refresh(), ui_sensorDiagScreen_screen_destroy(), volume_cb() (+8 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.10
Nodes (22): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_err_t, esp_event_base_t, net_core_init() (+14 more)

### Community 16 - "ui_label"
Cohesion: 0.08
Nodes (99): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+91 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_userEditScreen.c"
Cohesion: 0.33
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 19 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 20 - "storage.c"
Cohesion: 0.11
Nodes (42): app_metrics_t, cfg_result_t, appmetrics_load(), appmetrics_store(), esp_err_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+34 more)

### Community 21 - "http_api.c"
Cohesion: 0.25
Nodes (19): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+11 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.16
Nodes (26): ads1115_is_ready(), ms5803_is_ready(), AppConfig, sensor_sample_t, sensor_signal_stats_t, lock(), median3f(), median5f() (+18 more)

### Community 23 - "flow_meter.c"
Cohesion: 0.22
Nodes (7): flow_meter_get(), flow_meter_record(), flow_integrator_push(), flow_integrator_t, flow_meter, flow_meter_snapshot_t, math

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.18
Nodes (10): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — Soak final de 1.5.36-dev APROBADO (~28 h): baseline de V1, 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+2 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.09
Nodes (27): cdefs, esp_compiler, esp_heap_caps, esp_lcd_panel_io, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface (+19 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.16
Nodes (17): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+9 more)

### Community 28 - "ft5x06.c"
Cohesion: 0.21
Nodes (13): bsp_touch_config_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t (+5 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.07
Nodes (47): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+39 more)

### Community 31 - "ui_mainScreen.c"
Cohesion: 0.10
Nodes (47): alarm_mgr, card_state_t, alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual() (+39 more)

### Community 32 - "esp_err.h"
Cohesion: 0.06
Nodes (37): cjson, esp_http_server, esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, esp_random, derive() (+29 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.14
Nodes (21): eth_mgr_is_up(), cloud_mgr_connected(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t (+13 more)

### Community 36 - "sd_monitor_task"
Cohesion: 0.20
Nodes (12): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), lv_event_t, restart_btn_event_cb(), sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), ui_main_deinit_cb() (+4 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.21
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild() (+3 more)

### Community 38 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 39 - "vTaskDelay"
Cohesion: 0.24
Nodes (12): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+4 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.27
Nodes (10): conn_status_t, lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct(), str_hash() (+2 more)

### Community 41 - "main.c"
Cohesion: 0.09
Nodes (19): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+11 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.12
Nodes (24): appcfg_cache_peek(), AppConfig, lv_color_t, lv_obj_t, save_and_refresh(), set_str(), ui_auth_user_count(), ui_cfg_alarm_tone() (+16 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.05
Nodes (79): ads1115, ads1115_channel_t, at24c256, bmp280, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg() (+71 more)

### Community 44 - "ui_sensorEditScreen.c"
Cohesion: 0.16
Nodes (27): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+19 more)

### Community 45 - "ui_cfg_flow_limit"
Cohesion: 0.67
Nodes (3): f_switch(), ui_cfg_flow_limit(), ui_cfg_set_flow_alarm()

### Community 47 - "test_alarm.c"
Cohesion: 0.16
Nodes (13): assert, set_buzzer_acoustic(), appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed(), ledc_channel_config() (+5 more)

### Community 49 - "display_idle_cb"
Cohesion: 0.38
Nodes (7): bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 51 - "ui_i18n"
Cohesion: 0.13
Nodes (14): ui_datetimeScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), app_user_role_t (+6 more)

### Community 52 - "ui_keypadScreen.c"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "mem_diag.c"
Cohesion: 0.21
Nodes (9): esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+1 more)

### Community 56 - "ui_nav.c"
Cohesion: 0.33
Nodes (9): lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace() (+1 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "string"
Cohesion: 0.10
Nodes (16): eth_mgr, set_str(), ui_netCloudScreen_screen_destroy(), lv_event_t, save_cb(), set_str(), ui_netEthScreen_screen_destroy(), ui_netWifiScreen_screen_destroy() (+8 more)

### Community 60 - "apply_authenticated_cb"
Cohesion: 0.20
Nodes (10): apply_authenticated_cb(), lv_event_t, ui_sensorEditScreen_refresh(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit(), ui_cfg_press_from_disp() (+2 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.17
Nodes (14): config_mode, bleapp_goroot_async(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_loaded_cb(), bleapp_off_cb(), bleapp_pair_pin(), bleapp_tick_cb() (+6 more)

### Community 62 - "ui_wifi_main_icon.c"
Cohesion: 0.36
Nodes (7): apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), wifi_mgr

### Community 63 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 64 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 66 - "lvgl"
Cohesion: 0.09
Nodes (6): ui_confirmScreen_screen_destroy(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), lvgl

### Community 67 - "on_network_ready"
Cohesion: 0.50
Nodes (4): esp_err_t, mdns_svc_start(), http_api_init(), on_network_ready()

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_cfg.c"
Cohesion: 0.15
Nodes (29): appcfg_save(), lv_event_t, next(), save_cb(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user() (+21 more)

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 83 - "wifi_event_handler"
Cohesion: 0.19
Nodes (13): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, log_dns_info(), wifi_event_handler() (+5 more)

### Community 90 - "ui_infoScreen.c"
Cohesion: 0.05
Nodes (65): cert_info_t, cert_kind_t, cert_store, crypto, dirent, esp_app_desc, esp_app_desc_t, esp_app_format (+57 more)

## Knowledge Gaps
- **136 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+131 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_timer_get_time`, `transport_mqtt.c`, `alarm_mgr.c`, `ui_loginScreen.c`, `save_cb`, `screen_init_task`, `ui_sensorDiagScreen.c`, `fpm_ble_config.c`, `ui_label`, `storage.c`, `http_api.c`, `ui_mainScreen.c`, `ui_statusbar_controller.c`, `ui_connectivityScreen.c`, `ui_sensorEditScreen.c`, `ui_cfg_flow_limit`, `display_idle_cb`, `string`, `apply_authenticated_cb`, `ui_bleAppScreen.c`, `gas_label_color`, `on_network_ready`, `ui_cfg.c`?**
  _High betweenness centrality (0.046) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `esp_err.h`, `esp_timer_get_time`, `on_network_ready`, `sd_monitor_task`, `transport_mqtt.c`, `vTaskDelay`, `save_cb`, `screen_init_task`, `fpm_i2c_guard.c`, `fpm_ble_config.c`, `wifi_event_handler`, `sensor_cfg_persist`, `storage.c`, `sensors_runtime.c`, `http_api.c`, `ui_infoScreen.c`, `eth_mgr.c`, `ft5x06.c`?**
  _High betweenness centrality (0.036) - this node is a cross-community bridge._
- **Why does `app_main()` connect `esp_timer_get_time` to `esp_err_to_name`, `on_network_ready`, `alarm_mgr.c`, `vTaskDelay`, `ui_cfg.c`, `main.c`, `appcfg_cache_peek`, `time_mgr.c`, `wifi_event_handler`, `storage.c`, `sensors_runtime.c`, `mem_diag.c`?**
  _High betweenness centrality (0.027) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._