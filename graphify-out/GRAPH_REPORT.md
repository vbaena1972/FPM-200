# Graph Report - FPM-200  (2026-10-11)

## Corpus Check
- 187 files · ~422,697 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1553 nodes · 4216 edges · 89 communities (82 shown, 7 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1026 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `b316c2dc`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- string
- esp_err_to_name
- transport_ble.c
- lv_port_mem.c
- alarm_mgr.c
- lv_obj_t
- ui_loginScreen.c
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- FreeRTOS.h
- fpm_ble_config.c
- ui_edit_begin
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- config_mode.c
- ui_sensorEditScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- ble_init_task
- state_pub.c
- esp_eth_phy_ksz8851snl.c
- mdns_svc.c
- esp_log.h
- ethernet_init.c
- screen_init_task
- ui_statusbar_controller.c
- gas_label_color
- ui_generalSimpleScreen.c
- sensor_cfg_persist
- sensors_acq_task
- ui_connectivityScreen.c
- main.c
- ui_cfg.c
- fpm_i2c_guard.c
- ui_keypadScreen.c
- esp_lcd_st7796.h
- README.md
- transport_mqtt.c
- fpm_i2c_guard
- gen_ota_signing_key.sh script
- ft5x06.c
- sd_monitor_task
- save_cb
- Migración HMI: SquareLine → Claude Design
- ui_nav.c
- Changelog
- mem_diag.c
- esp_eth_driver
- apply_authenticated_cb
- ui_bleAppScreen.c
- ui.c
- ui_nav_load
- Consumo y endurecimiento 1.5.5-dev
- lvgl
- at24c256.c
- ui_statusbar_request_refresh
- flow_meter.c
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_auth_can
- AGENTS.md
- CLAUDE.md
- mqtt_event
- esp_eth_com
- ui_wifi_main_icon_init
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- on_alarm_state_change
- ui_open_useredit_cb
- get_bleapp
- get_connectivity
- fresh_net_cloud
- get_sensor_edit
- get_users
- README_PARCHE.md
- ui_widgets
- ota_update.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 58 edges
3. `esp_err_to_name()` - 47 edges
4. `ui_col()` - 45 edges
5. `ui_box()` - 42 edges
6. `_t()` - 37 edges
7. `ui_card()` - 35 edges
8. `appcfg_save()` - 33 edges
9. `ui_icon()` - 33 edges
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 29 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `app_main()` --calls--> `fpm_i2c_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/drivers/fpm_i2c_guard.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c
- `app_main()` --calls--> `transport_mqtt_runtime_init()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/transport_mqtt.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (89 total, 7 thin omitted)

### Community 0 - "string"
Cohesion: 0.13
Nodes (13): cert_store, esp_app_desc, eth_mgr, lv_event_t, factory_btn_cb(), reboot_cb(), stdio, stdlib (+5 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.12
Nodes (29): esp_wifi_types, apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals() (+21 more)

### Community 2 - "transport_ble.c"
Cohesion: 0.11
Nodes (17): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, append_json() (+9 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.14
Nodes (9): esp_heap_caps, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t (+1 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.09
Nodes (42): alarm_state_change_cb_t, eth_ksz8851snl_config_t, eth_mac_config_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init() (+34 more)

### Community 5 - "lv_obj_t"
Cohesion: 0.18
Nodes (11): ui_confirmScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_pinScreen_screen_destroy(), lv_obj_t, fresh_confirm(), fresh_net_eth(), fresh_pin() (+3 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.19
Nodes (17): app_user_role_t, app_user_t, lv_event_t, fac_close_cb(), fac_hide(), fac_kb_cb(), fac_show_err(), fac_try() (+9 more)

### Community 7 - "ms5803.c"
Cohesion: 0.37
Nodes (12): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+4 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.13
Nodes (50): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t, eth_duplex_t, eth_link_t (+42 more)

### Community 9 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 10 - "FreeRTOS.h"
Cohesion: 0.27
Nodes (9): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.22
Nodes (13): lv_event_t, edit_cb(), rebuild(), step_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes() (+5 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.11
Nodes (19): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_err_t, esp_event_base_t, net_core_init() (+11 more)

### Community 16 - "ui_label"
Cohesion: 0.05
Nodes (130): card_state_t, sensors_runtime_flow_warming(), sensors_runtime_get_debug(), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t (+122 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "config_mode.c"
Cohesion: 0.24
Nodes (8): esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb(), lv_event_t, transport_mqtt

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.18
Nodes (25): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+17 more)

### Community 20 - "storage.c"
Cohesion: 0.08
Nodes (60): app_metrics_t, cert_info_t, cert_kind_t, cfg_result_t, crypto, fcntl, esp_err_t, cert_store_erase() (+52 more)

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.14
Nodes (30): ads1115_is_ready(), ms5803_is_ready(), aws_telemetry_task(), flow_meter_get(), AppConfig, i2c_master_bus_handle_t, sensor_sample_t, sensor_signal_stats_t (+22 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (9): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read() (+1 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.07
Nodes (29): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build (+21 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.22
Nodes (17): cdefs, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t, esp_err_t (+9 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.18
Nodes (16): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+8 more)

### Community 28 - "ble_init_task"
Cohesion: 0.29
Nodes (10): fpm_ble_config_set_clock_cb(), esp_err_t, transport_ble_init(), transport_ble_set_enabled(), transport_ble_set_finish_cb(), transport_ble_set_mesh_enabled(), transport_ble_set_passkey_mode(), transport_ble_set_tx_power() (+2 more)

### Community 29 - "state_pub.c"
Cohesion: 0.29
Nodes (5): device_state_t, state_build_json(), state_get(), state_update_sensors(), state_pub

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (26): eth_phy_autoneg_cmd_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t, ksz8851_update_link_duplex_speed() (+18 more)

### Community 31 - "mdns_svc.c"
Cohesion: 0.29
Nodes (5): esp_err_t, mdns_svc_start(), on_network_ready(), mdns, mdns_svc

### Community 32 - "esp_log.h"
Cohesion: 0.05
Nodes (36): cjson, esp_http_server, esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, esp_random, derive() (+28 more)

### Community 33 - "ethernet_init.c"
Cohesion: 0.16
Nodes (18): esp_check, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, eth_phy_config_t, ethernet_init, esp_eth_phy_new_ksz8851snl() (+10 more)

### Community 34 - "screen_init_task"
Cohesion: 0.21
Nodes (18): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+10 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.17
Nodes (18): transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb() (+10 more)

### Community 36 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.13
Nodes (25): general_loaded_cb(), base_card(), bright_cb(), lv_event_cb_t, lv_event_t, lv_obj_t, dim_cb(), lang_cb() (+17 more)

### Community 38 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 39 - "sensors_acq_task"
Cohesion: 0.18
Nodes (17): bmp280_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read() (+9 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.26
Nodes (11): conn_status_t, cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+3 more)

### Community 41 - "main.c"
Cohesion: 0.09
Nodes (19): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+11 more)

### Community 42 - "ui_cfg.c"
Cohesion: 0.12
Nodes (36): appcfg_cache_peek(), appcfg_save(), lv_event_t, next(), f_switch(), is_press_unit(), unit_cb(), AppConfig (+28 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.16
Nodes (23): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_init() (+15 more)

### Community 44 - "ui_keypadScreen.c"
Cohesion: 0.38
Nodes (5): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_edit_set_new()

### Community 45 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 47 - "transport_mqtt.c"
Cohesion: 0.14
Nodes (17): time_mgr_set_datetime(), add_channel(), cJSON, sensor_signal_stats_t, mqtt_start_locked(), mqtt_stop_locked(), prepare_pem(), publish_alarm_transition() (+9 more)

### Community 49 - "fpm_i2c_guard"
Cohesion: 0.21
Nodes (12): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+4 more)

### Community 51 - "ft5x06.c"
Cohesion: 0.16
Nodes (15): bsp_touch_config_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t (+7 more)

### Community 52 - "sd_monitor_task"
Cohesion: 0.20
Nodes (13): lv_event_t, restart_btn_event_cb(), sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), unmount_sdcard_hotplug(), lv_event_cb_t, lv_event_t (+5 more)

### Community 54 - "save_cb"
Cohesion: 0.40
Nodes (5): transport_ble_stop_adv(), bleapp_off_cb(), lv_event_t, save_cb(), set_str()

### Community 55 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 56 - "ui_nav.c"
Cohesion: 0.14
Nodes (17): bleapp_goroot_async(), ui_generalScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), get_general(), lv_event_t, lv_obj_t (+9 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "mem_diag.c"
Cohesion: 0.23
Nodes (8): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), mem_diag

### Community 60 - "apply_authenticated_cb"
Cohesion: 0.18
Nodes (12): apply_authenticated_cb(), lv_event_t, allow_decimal(), ui_sensorDiagScreen_refresh(), ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_from_disp(), ui_cfg_press_to_disp() (+4 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.20
Nodes (10): config_mode, esp_system, transport_ble_is_connected(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig (+2 more)

### Community 62 - "ui.c"
Cohesion: 0.13
Nodes (19): ui_datetimeScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_mainScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_sensorDiagScreen_screen_destroy(), lv_timer_t, fresh_keypad() (+11 more)

### Community 63 - "ui_nav_load"
Cohesion: 0.22
Nodes (15): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), ui_pinScreen_set_config_entry(), lv_event_t, fresh_login(), get_info() (+7 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 67 - "at24c256.c"
Cohesion: 0.30
Nodes (13): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+5 more)

### Community 68 - "ui_statusbar_request_refresh"
Cohesion: 0.15
Nodes (18): mem_diag_report(), esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), wifi_mgr_stop() (+10 more)

### Community 69 - "flow_meter.c"
Cohesion: 0.21
Nodes (9): assert, esp_err_t, flow_meter_init(), flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t (+1 more)

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_auth_can"
Cohesion: 0.13
Nodes (26): lv_event_t, protected_back_cb(), lv_event_t, cert_erase_cb(), save_cb(), set_str(), lv_event_t, delete_cb() (+18 more)

### Community 75 - "mqtt_event"
Cohesion: 0.38
Nodes (7): esp_mqtt_event_handle_t, esp_event_base_t, mqtt_event(), shadow_rx_chunk(), shadow_rx_reset(), topic_is(), ui_statusbar_signal_cloud_activity()

### Community 76 - "esp_eth_com"
Cohesion: 0.40
Nodes (3): esp_eth_com, esp_eth_mac_spi, esp_eth_phy

### Community 77 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 79 - "on_alarm_state_change"
Cohesion: 0.67
Nodes (3): transport_mqtt_publish_now(), alarm_clinical_state_t, on_alarm_state_change()

### Community 80 - "ui_open_useredit_cb"
Cohesion: 0.39
Nodes (8): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), fresh_useredit(), ui_open_useredit_cb()

### Community 81 - "get_bleapp"
Cohesion: 0.67
Nodes (3): ui_bleAppScreen_screen_destroy(), get_bleapp(), ui_open_bleapp_cb()

### Community 82 - "get_connectivity"
Cohesion: 0.67
Nodes (3): ui_connectivityScreen_screen_destroy(), get_connectivity(), ui_open_connectivity_cb()

### Community 83 - "fresh_net_cloud"
Cohesion: 0.67
Nodes (3): ui_netCloudScreen_screen_destroy(), fresh_net_cloud(), ui_open_net_cloud_cb()

### Community 84 - "get_sensor_edit"
Cohesion: 0.67
Nodes (3): ui_sensorEditScreen_screen_destroy(), get_sensor_edit(), ui_open_sensor_cb()

### Community 85 - "get_users"
Cohesion: 0.67
Nodes (3): ui_usersScreen_screen_destroy(), get_users(), ui_open_users_cb()

### Community 89 - "ui_widgets"
Cohesion: 0.16
Nodes (11): alarm_mgr, app_user_role_t, selected_role(), app_user_role_t, role_color(), role_name(), ui, ui_cfg (+3 more)

### Community 90 - "ota_update.c"
Cohesion: 0.15
Nodes (19): dirent, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE, esp_err_t (+11 more)

## Knowledge Gaps
- **132 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+127 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `ui_cfg.c` to `screen_init_task`, `alarm_mgr.c`, `ui_statusbar_request_refresh`, `ui_loginScreen.c`, `ui_generalSimpleScreen.c`, `ui_connectivityScreen.c`, `ui_auth_can`, `gas_label_color`, `fpm_ble_config.c`, `ui_edit_begin`, `ui_label`, `ui_sensorEditScreen.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `save_cb`, `apply_authenticated_cb`, `mdns_svc.c`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `at24c256.c`, `sensors_acq_task`, `main.c`, `ui_cfg.c`, `ui_wifi_main_icon_init`, `ui_label`, `fpm_i2c_guard`, `ft5x06.c`?**
  _High betweenness centrality (0.034) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `ms5803.c`, `ads1115.c`, `FreeRTOS.h`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `ble_init_task`, `mdns_svc.c`, `esp_log.h`, `screen_init_task`, `sensor_cfg_persist`, `sensors_acq_task`, `fpm_i2c_guard.c`, `fpm_i2c_guard`, `ft5x06.c`, `sd_monitor_task`, `at24c256.c`, `ota_update.c`?**
  _High betweenness centrality (0.030) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 46 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 46 INFERRED edges - model-reasoned connections that need verification._
- **Are the 43 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 43 INFERRED edges - model-reasoned connections that need verification._