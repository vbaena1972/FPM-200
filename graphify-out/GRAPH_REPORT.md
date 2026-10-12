# Graph Report - FPM-200  (2026-10-12)

## Corpus Check
- 190 files · ~426,489 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1594 nodes · 4335 edges · 82 communities (75 shown, 7 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1055 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `be52050a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- app_main
- esp_err_to_name
- time_mgr.c
- cert_store.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- FreeRTOS.h
- transport_ble.c
- ui_sensorDiagScreen.c
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_netCloudScreen.c
- ui_infoScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- string
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- ui_mainScreen.c
- esp_log.h
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- config_mode.c
- ui_generalSimpleScreen.c
- stdlib
- vTaskDelay
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- ui_sensorEditScreen.c
- ble_init_task
- README.md
- transport_mqtt.c
- display_idle_cb
- gen_ota_signing_key.sh script
- lv_event_t
- ui_widgets
- Migración HMI: SquareLine → Claude Design
- ui_usersScreen.c
- ui_nav_load
- Changelog
- ui_wifi_main_icon.c
- esp_eth_driver
- apply_authenticated_cb
- ui_bleAppScreen.c
- fresh_login
- gas_label_color
- esp_app_desc
- lvgl
- at24c256.c
- metrics_store.c
- save_and_refresh
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_cfg.c
- AGENTS.md
- CLAUDE.md
- fresh_net_eth
- get_sensor_edit
- fresh_confirm
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- ui_statusbar_request_refresh
- README_PARCHE.md
- ota_update.c

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

## Communities (82 total, 7 thin omitted)

### Community 0 - "app_main"
Cohesion: 0.13
Nodes (15): fpm_i2c_init(), transport_mqtt_runtime_init(), esp_err_t, flow_meter_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), eTaskState, state_str() (+7 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.11
Nodes (31): esp_wifi_types, esp_err_t, mdns_svc_start(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t (+23 more)

### Community 2 - "time_mgr.c"
Cohesion: 0.12
Nodes (19): esp_netif_sntp, esp_sntp, bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init() (+11 more)

### Community 3 - "cert_store.c"
Cohesion: 0.20
Nodes (19): cert_info_t, cert_kind_t, cert_store, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all() (+11 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.06
Nodes (61): alarm_state_change_cb_t, assert, esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init() (+53 more)

### Community 5 - "ui.c"
Cohesion: 0.09
Nodes (34): ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_pinScreen_screen_destroy() (+26 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.19
Nodes (21): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+13 more)

### Community 7 - "ms5803.c"
Cohesion: 0.37
Nodes (12): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+4 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.12
Nodes (53): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t (+45 more)

### Community 9 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 10 - "FreeRTOS.h"
Cohesion: 0.25
Nodes (10): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+2 more)

### Community 11 - "transport_ble.c"
Cohesion: 0.11
Nodes (17): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, append_json() (+9 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.22
Nodes (14): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume() (+6 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "net_core.c"
Cohesion: 0.17
Nodes (14): esp_eth, esp_event, esp_wifi, esp_err_t, esp_event_base_t, net_core_init(), on_eth_event(), on_ip_event() (+6 more)

### Community 16 - "ui_label"
Cohesion: 0.07
Nodes (104): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+96 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_netCloudScreen.c"
Cohesion: 0.21
Nodes (11): cert_pills_refresh(), cert_presence(), set_str(), app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb() (+3 more)

### Community 19 - "ui_infoScreen.c"
Cohesion: 0.24
Nodes (11): esp_ota_ops, lv_event_t, erase_nvs_namespace(), factory_btn_cb(), ota_channel_cb(), ota_check_cb(), ota_install_cb(), ota_screen_del_cb() (+3 more)

### Community 20 - "storage.c"
Cohesion: 0.15
Nodes (36): cfg_result_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate(), appcfg_take_pin_migration(), app_user_t (+28 more)

### Community 21 - "http_api.c"
Cohesion: 0.11
Nodes (32): esp_chip_info, esp_system, eTaskState, diag_worker(), mem_diag_report(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks() (+24 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.11
Nodes (40): ads1115_is_ready(), at24c256_is_ready(), ms5803_is_ready(), aws_telemetry_task(), flow_meter_get(), AppConfig, esp_err_t, sensor_sample_t (+32 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (9): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read() (+1 more)

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.18
Nodes (10): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — Soak final de 1.5.36-dev APROBADO (~28 h): baseline de V1, 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir (+2 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.06
Nodes (41): bsp_touch_config_t, cdefs, esp_compiler, esp_heap_caps, esp_lcd_panel_io, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t (+33 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.14
Nodes (20): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+12 more)

### Community 28 - "string"
Cohesion: 0.17
Nodes (8): lv_event_t, save_cb(), set_str(), close_to(), main(), stdio, string, tz_map

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.07
Nodes (47): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+39 more)

### Community 31 - "ui_mainScreen.c"
Cohesion: 0.10
Nodes (44): alarm_mgr, card_state_t, alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual() (+36 more)

### Community 32 - "esp_log.h"
Cohesion: 0.05
Nodes (37): cjson, esp_http_server, esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, esp_random, derive() (+29 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.20
Nodes (15): cloud_mgr_connected(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph() (+7 more)

### Community 36 - "config_mode.c"
Cohesion: 0.19
Nodes (10): transport_ble_start_adv(), transport_ble_stop_adv(), esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb() (+2 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.46
Nodes (6): lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 38 - "stdlib"
Cohesion: 0.24
Nodes (9): cJSON, esp_err_t, ota_manifest_t, copy_str(), ota_manifest_fetch(), ota_version_compare(), parse_version(), stdlib (+1 more)

### Community 39 - "vTaskDelay"
Cohesion: 0.24
Nodes (12): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+4 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.16
Nodes (14): conn_status_t, eth_mgr, lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+6 more)

### Community 41 - "main.c"
Cohesion: 0.07
Nodes (32): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+24 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.11
Nodes (24): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), f_switch(), lv_color_t, lv_obj_t, ui_auth_user_count(), ui_cfg_alarm_tone() (+16 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "ui_sensorEditScreen.c"
Cohesion: 0.18
Nodes (25): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+17 more)

### Community 45 - "ble_init_task"
Cohesion: 0.29
Nodes (10): fpm_ble_config_set_clock_cb(), esp_err_t, transport_ble_init(), transport_ble_set_enabled(), transport_ble_set_finish_cb(), transport_ble_set_mesh_enabled(), transport_ble_set_passkey_mode(), transport_ble_set_tx_power() (+2 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.08
Nodes (29): device_state_t, esp_mqtt_event_handle_t, add_channel(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), mqtt_start_locked() (+21 more)

### Community 49 - "display_idle_cb"
Cohesion: 0.28
Nodes (9): general_loaded_cb(), bright_cb(), clamp_brightness(), ui_auth_logout(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness() (+1 more)

### Community 51 - "lv_event_t"
Cohesion: 0.22
Nodes (10): ui_generalScreen_screen_destroy(), ui_pinScreen_set_config_entry(), ui_usersScreen_screen_destroy(), lv_event_t, get_general(), get_users(), ui_open_general_authenticated_cb(), ui_open_general_cb() (+2 more)

### Community 52 - "ui_widgets"
Cohesion: 0.15
Nodes (11): lv_event_t, protected_back_cb(), key_cb(), refresh_value(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots() (+3 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "ui_usersScreen.c"
Cohesion: 0.27
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 56 - "ui_nav_load"
Cohesion: 0.25
Nodes (10): bleapp_goroot_async(), lv_event_t, lv_obj_t, ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_load(), ui_nav_pop_to() (+2 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "ui_wifi_main_icon.c"
Cohesion: 0.19
Nodes (12): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+4 more)

### Community 60 - "apply_authenticated_cb"
Cohesion: 0.20
Nodes (10): apply_authenticated_cb(), lv_event_t, ui_sensorDiagScreen_refresh(), ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_from_disp(), ui_cfg_press_to_disp(), ui_cfg_pressure_unit() (+2 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (8): config_mode, bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t, ui_bleAppScreen_screen_destroy()

### Community 62 - "fresh_login"
Cohesion: 0.22
Nodes (10): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb() (+2 more)

### Community 63 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 64 - "esp_app_desc"
Cohesion: 0.40
Nodes (3): esp_app_desc, mdns, mdns_svc

### Community 67 - "at24c256.c"
Cohesion: 0.29
Nodes (13): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), crc16_ccitt() (+5 more)

### Community 68 - "metrics_store.c"
Cohesion: 0.28
Nodes (7): app_metrics_t, appmetrics_load(), appmetrics_store(), esp_err_t, metrics_store, nvs, nvs_flash

### Community 69 - "save_and_refresh"
Cohesion: 0.33
Nodes (9): AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_cfg.c"
Cohesion: 0.16
Nodes (25): lv_event_t, next(), save_cb(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+17 more)

### Community 75 - "fresh_net_eth"
Cohesion: 0.67
Nodes (3): ui_netEthScreen_screen_destroy(), fresh_net_eth(), ui_open_net_eth_cb()

### Community 76 - "get_sensor_edit"
Cohesion: 0.67
Nodes (3): ui_sensorEditScreen_screen_destroy(), get_sensor_edit(), ui_open_sensor_cb()

### Community 77 - "fresh_confirm"
Cohesion: 0.33
Nodes (6): ui_confirmScreen_screen_destroy(), accept_cb(), lv_event_t, ui_edit_set_new(), fresh_confirm(), ui_open_confirm_cb()

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 83 - "ui_statusbar_request_refresh"
Cohesion: 0.25
Nodes (11): esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), wifi_mgr_stop(), state_on_net_available() (+3 more)

### Community 90 - "ota_update.c"
Cohesion: 0.19
Nodes (20): dirent, esp_app_desc_t, esp_app_format, esp_partition, esp_partition_t, FILE, esp_err_t, ota_manifest_t (+12 more)

## Knowledge Gaps
- **136 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+131 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `app_main`, `esp_err_to_name`, `alarm_mgr.c`, `ui_loginScreen.c`, `ui_sensorDiagScreen.c`, `fpm_ble_config.c`, `ui_label`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `string`, `ui_mainScreen.c`, `ui_statusbar_controller.c`, `ui_connectivityScreen.c`, `ui_sensorEditScreen.c`, `display_idle_cb`, `apply_authenticated_cb`, `gas_label_color`, `save_and_refresh`, `ui_cfg.c`, `ui_statusbar_request_refresh`, `ota_update.c`?**
  _High betweenness centrality (0.051) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `app_main`, `time_mgr.c`, `cert_store.c`, `alarm_mgr.c`, `ms5803.c`, `ads1115.c`, `FreeRTOS.h`, `fpm_ble_config.c`, `ui_infoScreen.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `touch_io_bounded.c`, `eth_mgr.c`, `esp_log.h`, `stdlib`, `vTaskDelay`, `main.c`, `fpm_i2c_guard.c`, `ble_init_task`, `at24c256.c`, `ota_update.c`?**
  _High betweenness centrality (0.036) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `alarm_mgr.c` to `time_mgr.c`, `ui_statusbar_controller.c`, `ui_wifi_main_icon.c`, `at24c256.c`, `vTaskDelay`, `main.c`, `appcfg_cache_peek`, `ui_label`, `touch_io_bounded.c`?**
  _High betweenness centrality (0.027) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._