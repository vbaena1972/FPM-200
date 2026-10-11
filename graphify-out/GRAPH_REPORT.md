# Graph Report - FPM-200  (2026-10-11)

## Corpus Check
- 187 files · ~423,896 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1564 nodes · 4249 edges · 89 communities (82 shown, 7 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1031 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `68eaf048`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- string
- esp_err_to_name
- time_mgr.c
- ui_card
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- bmp280_init
- fpm_ble_config.c
- ui_edit_begin
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_pin_hash.c
- save_and_refresh
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- bsp_touch_new
- eth_mgr.c
- ui_wifi_main_icon.c
- task_tracer.c
- esp_eth_phy_ksz8851snl.c
- ui_mainScreen.c
- esp_err.h
- wifi_mgr.c
- screen_init_task
- ui_statusbar_controller.c
- gas_label_color
- ui_generalSimpleScreen.c
- cert_store.c
- sensors_acq_task
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- ui_sensorEditScreen.c
- ft5x06.h
- README.md
- transport_mqtt.c
- lv_obj_t
- gen_ota_signing_key.sh script
- esp_timer.h
- ui_col
- ui_cfg_preview_brightness
- Migración HMI: SquareLine → Claude Design
- ui_nav.c
- Changelog
- mem_diag.c
- esp_eth_driver
- apply_authenticated_cb
- get_bleapp
- ui_style_button
- lv_event_t
- Consumo y endurecimiento 1.5.5-dev
- lvgl
- vTaskDelay
- ui_statusbar_request_refresh
- app_main
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_cfg.c
- AGENTS.md
- CLAUDE.md
- esp_app_desc
- fac_try
- save_cb
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- fresh_pin
- ui_open_useredit_cb
- include/storage.h
- get_connectivity
- math
- next
- get_users
- README_PARCHE.md
- ota_update.c
- ui_nav_load

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 77 edges
2. `ui_label()` - 59 edges
3. `esp_err_to_name()` - 47 edges
4. `ui_col()` - 45 edges
5. `ui_box()` - 42 edges
6. `_t()` - 37 edges
7. `ui_card()` - 36 edges
8. `appcfg_save()` - 33 edges
9. `ui_icon()` - 33 edges
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 31 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c
- `state_build_json()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (89 total, 7 thin omitted)

### Community 0 - "string"
Cohesion: 0.14
Nodes (16): cdefs, config_mode, esp_compiler, esp_heap_caps, esp_lcd_panel_io, esp_lcd_panel_io_interface, esp_system, bleapp_pair_pin() (+8 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.18
Nodes (20): esp_err_t, mdns_svc_start(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_pending(), wifi_apply_worker() (+12 more)

### Community 2 - "time_mgr.c"
Cohesion: 0.13
Nodes (19): esp_netif_sntp, esp_sntp, bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init() (+11 more)

### Community 3 - "ui_card"
Cohesion: 0.22
Nodes (20): lv_obj_t, key(), ui_netBleScreen_screen_init(), ui_netCloudScreen_screen_init(), ui_netEthScreen_screen_init(), ui_netWifiScreen_screen_init(), ui_userEditScreen_screen_init(), lv_event_cb_t (+12 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.10
Nodes (39): alarm_state_change_cb_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl() (+31 more)

### Community 5 - "ui.c"
Cohesion: 0.14
Nodes (18): ui_confirmScreen_screen_destroy(), ui_mainScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_sensorDiagScreen_screen_destroy(), lv_timer_t, fresh_confirm(), fresh_net_eth(), get_general() (+10 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.24
Nodes (16): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+8 more)

### Community 7 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+3 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.12
Nodes (53): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t (+45 more)

### Community 9 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 10 - "bmp280_init"
Cohesion: 0.36
Nodes (8): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t, i2c_master_bus_handle_t

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.06
Nodes (71): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, apply_channel_obj() (+63 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.20
Nodes (14): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume() (+6 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "net_core.c"
Cohesion: 0.19
Nodes (14): esp_eth, esp_event, esp_wifi, esp_event_base_t, on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services() (+6 more)

### Community 16 - "ui_label"
Cohesion: 0.14
Nodes (46): ui_bleAppScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t, row(), ui_datetimeScreen_screen_init() (+38 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_pin_hash.c"
Cohesion: 0.23
Nodes (18): esp_random, derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final() (+10 more)

### Community 19 - "save_and_refresh"
Cohesion: 0.24
Nodes (12): is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas() (+4 more)

### Community 20 - "storage.c"
Cohesion: 0.11
Nodes (42): app_metrics_t, cfg_result_t, appmetrics_load(), appmetrics_store(), esp_err_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+34 more)

### Community 21 - "http_api.c"
Cohesion: 0.13
Nodes (29): esp_chip_info, esp_http_server, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t (+21 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.15
Nodes (29): esp_err_t, sensor_sample_t, sensor_signal_stats_t, crc16_ccitt(), lock(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_flow_warming() (+21 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (9): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read() (+1 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.06
Nodes (31): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR (+23 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "bsp_touch_new"
Cohesion: 0.11
Nodes (27): bsp_touch_config_t, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_t, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init() (+19 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.15
Nodes (19): esp_eth_netif_glue, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init() (+11 more)

### Community 28 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 29 - "task_tracer.c"
Cohesion: 0.31
Nodes (6): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), TaskHandle_t

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.05
Nodes (55): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+47 more)

### Community 31 - "ui_mainScreen.c"
Cohesion: 0.12
Nodes (43): card_state_t, alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual(), banner_open_cb() (+35 more)

### Community 32 - "esp_err.h"
Cohesion: 0.12
Nodes (5): esp_netif, stdbool, stddef, stdint, time

### Community 33 - "wifi_mgr.c"
Cohesion: 0.15
Nodes (13): esp_wifi_types, esp_event_base_t, wifi_status_t, log_dns_info(), notify(), wifi_apply_cb(), wifi_event_handler(), wifi_mgr_get_netinfo() (+5 more)

### Community 34 - "screen_init_task"
Cohesion: 0.22
Nodes (17): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+9 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.20
Nodes (12): eth_mgr, transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible() (+4 more)

### Community 36 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.46
Nodes (6): lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 38 - "cert_store.c"
Cohesion: 0.20
Nodes (19): cert_info_t, cert_kind_t, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd() (+11 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.17
Nodes (17): bmp280_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read() (+9 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.18
Nodes (14): conn_status_t, cloud_mgr_connected(), transport_ble_is_connected(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_tick_cb(), lv_timer_t, lv_timer_t (+6 more)

### Community 41 - "main.c"
Cohesion: 0.07
Nodes (31): bmp280, errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t (+23 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.11
Nodes (25): appcfg_cache_peek(), appcfg_save(), transport_ble_stop_adv(), bleapp_off_cb(), lv_event_t, save_cb(), set_str(), save_cb() (+17 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "ui_sensorEditScreen.c"
Cohesion: 0.18
Nodes (24): accept_cb(), lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card() (+16 more)

### Community 45 - "ft5x06.h"
Cohesion: 0.17
Nodes (8): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_ops, esp_lcd_panel_vendor, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, lcd_types

### Community 47 - "transport_mqtt.c"
Cohesion: 0.12
Nodes (24): esp_mqtt_event_handle_t, add_channel(), aws_telemetry_task(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), mqtt_start_locked() (+16 more)

### Community 49 - "lv_obj_t"
Cohesion: 0.17
Nodes (12): ui_datetimeScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), lv_obj_t, fresh_net_ble(), fresh_net_cloud(), fresh_net_wifi() (+4 more)

### Community 51 - "esp_timer.h"
Cohesion: 0.25
Nodes (5): device_state_t, state_build_json(), state_get(), state_update_sensors(), state_pub

### Community 52 - "ui_col"
Cohesion: 0.12
Nodes (25): bleapp_set_connected(), ui_confirmScreen_screen_init(), gas_label(), ui_keypadScreen_screen_init(), build_metric_card(), cert_pills_refresh(), lv_obj_t, pkey() (+17 more)

### Community 54 - "ui_cfg_preview_brightness"
Cohesion: 0.38
Nodes (7): bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 55 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 56 - "ui_nav.c"
Cohesion: 0.15
Nodes (17): bleapp_goroot_async(), ui_connectivityScreen_refresh(), lv_event_t, pin_key_cb(), pin_wrong_flash(), ui_sensorDiagScreen_refresh(), lv_event_t, lv_obj_t (+9 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "mem_diag.c"
Cohesion: 0.23
Nodes (8): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), mem_diag

### Community 60 - "apply_authenticated_cb"
Cohesion: 0.13
Nodes (18): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), allow_decimal(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt() (+10 more)

### Community 61 - "get_bleapp"
Cohesion: 0.67
Nodes (3): ui_bleAppScreen_screen_destroy(), get_bleapp(), ui_open_bleapp_cb()

### Community 62 - "ui_style_button"
Cohesion: 0.32
Nodes (8): base_card(), lv_event_cb_t, lv_obj_t, seg(), lv_obj_t, small_button(), stepper(), ui_style_button()

### Community 63 - "lv_event_t"
Cohesion: 0.23
Nodes (12): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), lv_event_t, fresh_login(), get_info(), ui_open_config_ble_cb() (+4 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 66 - "lvgl"
Cohesion: 0.07
Nodes (17): alarm_mgr, cert_store, ui_generalScreen_screen_destroy(), lv_event_t, key_cb(), refresh_value(), app_user_role_t, role_color() (+9 more)

### Community 67 - "vTaskDelay"
Cohesion: 0.20
Nodes (18): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), driver_delay_ms() (+10 more)

### Community 68 - "ui_statusbar_request_refresh"
Cohesion: 0.28
Nodes (9): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), http_api_stop(), ui_statusbar_request_refresh() (+1 more)

### Community 69 - "app_main"
Cohesion: 0.13
Nodes (15): ads1115_is_ready(), at24c256_is_ready(), fpm_i2c_init(), ms5803_is_ready(), esp_err_t, net_core_init(), transport_mqtt_runtime_init(), esp_err_t (+7 more)

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_cfg.c"
Cohesion: 0.19
Nodes (22): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), app_user_role_t, ui_auth_active() (+14 more)

### Community 75 - "esp_app_desc"
Cohesion: 0.40
Nodes (3): esp_app_desc, mdns, mdns_svc

### Community 76 - "fac_try"
Cohesion: 0.50
Nodes (5): fac_show_err(), fac_try(), app_user_t, ui_auth_factory(), ui_auth_login()

### Community 77 - "save_cb"
Cohesion: 0.50
Nodes (4): eth_mgr_is_up(), lv_event_t, save_cb(), set_str()

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 79 - "fresh_pin"
Cohesion: 0.50
Nodes (4): ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 80 - "ui_open_useredit_cb"
Cohesion: 0.39
Nodes (8): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), fresh_useredit(), ui_open_useredit_cb()

### Community 82 - "get_connectivity"
Cohesion: 0.67
Nodes (3): ui_connectivityScreen_screen_destroy(), get_connectivity(), ui_open_connectivity_cb()

### Community 83 - "math"
Cohesion: 0.28
Nodes (7): assert, flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t, math

### Community 84 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 85 - "get_users"
Cohesion: 0.67
Nodes (3): ui_usersScreen_screen_destroy(), get_users(), ui_open_users_cb()

### Community 90 - "ota_update.c"
Cohesion: 0.16
Nodes (18): dirent, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE, esp_err_t (+10 more)

### Community 91 - "ui_nav_load"
Cohesion: 0.18
Nodes (11): ui_generalSimpleScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), visual_color(), fresh_keypad(), get_general_simple() (+3 more)

## Knowledge Gaps
- **134 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+129 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_err_to_name`, `ui_card`, `alarm_mgr.c`, `fpm_ble_config.c`, `ui_edit_begin`, `ui_label`, `save_and_refresh`, `storage.c`, `http_api.c`, `eth_mgr.c`, `ui_mainScreen.c`, `screen_init_task`, `ui_statusbar_controller.c`, `gas_label_color`, `ui_connectivityScreen.c`, `transport_mqtt.c`, `ui_cfg_preview_brightness`, `apply_authenticated_cb`, `app_main`, `ui_cfg.c`, `fac_try`, `save_cb`, `next`?**
  _High betweenness centrality (0.050) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `time_mgr.c`, `ms5803.c`, `ads1115.c`, `bmp280_init`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `bsp_touch_new`, `eth_mgr.c`, `esp_err.h`, `wifi_mgr.c`, `screen_init_task`, `cert_store.c`, `sensors_acq_task`, `main.c`, `fpm_i2c_guard.c`, `vTaskDelay`, `app_main`, `ota_update.c`?**
  _High betweenness centrality (0.034) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `time_mgr.c`, `vTaskDelay`, `main.c`, `appcfg_cache_peek`, `bsp_touch_new`, `ui_wifi_main_icon.c`, `ui_mainScreen.c`?**
  _High betweenness centrality (0.028) - this node is a cross-community bridge._
- **Are the 74 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 74 INFERRED edges - model-reasoned connections that need verification._
- **Are the 50 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 50 INFERRED edges - model-reasoned connections that need verification._
- **Are the 46 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 46 INFERRED edges - model-reasoned connections that need verification._
- **Are the 43 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 43 INFERRED edges - model-reasoned connections that need verification._