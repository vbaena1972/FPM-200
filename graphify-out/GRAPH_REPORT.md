# Graph Report - FPM-200  (2026-10-12)

## Corpus Check
- 192 files · ~427,635 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1607 nodes · 4363 edges · 84 communities (77 shown, 7 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1060 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `908e3ede`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- app_main
- esp_err_to_name
- state_pub.c
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- time_mgr.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- FreeRTOS.h
- transport_ble.c
- appcfg_cache_peek
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_userEditScreen.c
- ethernet_init.c
- storage.c
- http_api.c
- sensors_runtime.c
- flow_meter.c
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- vTaskDelay
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- wifi_mgr.c
- esp_log.h
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- sd_monitor_task
- ui_generalSimpleScreen.c
- ui_wifi_main_icon_init
- sfm3300.c
- ui_connectivityScreen.c
- main.c
- save_and_refresh
- fpm_i2c_guard.c
- ui_sensorEditScreen.c
- lv_port_mem.c
- README.md
- ui_i18n
- display_idle_cb
- gen_ota_signing_key.sh script
- ui_pinScreen.c
- ui_keypadScreen.c
- Migración HMI: SquareLine → Claude Design
- at24c256.c
- ui_nav.c
- Changelog
- string
- esp_eth_driver
- apply_authenticated_cb
- ui_bleAppScreen.c
- ui_netWifiScreen.c
- gas_label_color
- esp_lcd_st7796.h
- lvgl
- ms5803.c
- ads1115.c
- test_eeprom.c
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_cfg.c
- AGENTS.md
- CLAUDE.md
- ws_stream.c
- ui_usersScreen.c
- ui_form.c
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- esp_eth_com
- save_cb
- on_eth_event
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
- `sensors_acq_task()` --calls--> `sfm3300_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `sensors_acq_task()` --calls--> `sfm3300_get_last_rx()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c

## Import Cycles
- None detected.

## Communities (84 total, 7 thin omitted)

### Community 0 - "app_main"
Cohesion: 0.15
Nodes (13): fpm_i2c_init(), transport_mqtt_runtime_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start() (+5 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.23
Nodes (17): esp_err_t, mdns_svc_start(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_pending(), wifi_apply_worker() (+9 more)

### Community 2 - "state_pub.c"
Cohesion: 0.29
Nodes (5): device_state_t, state_build_json(), state_get(), state_update_sensors(), state_pub

### Community 3 - "transport_mqtt.c"
Cohesion: 0.08
Nodes (41): alarm_mgr, cert_info_t, cert_kind_t, cert_store, crypto, esp_mqtt_event_handle_t, fcntl, esp_err_t (+33 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.07
Nodes (49): alarm_state_change_cb_t, alarm_tone_out_t, alarm_tone_prio_t, assert, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl() (+41 more)

### Community 5 - "ui.c"
Cohesion: 0.10
Nodes (53): menu_item_cb(), ui_mainScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), lv_event_t (+45 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.16
Nodes (23): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+15 more)

### Community 7 - "time_mgr.c"
Cohesion: 0.13
Nodes (19): esp_netif_sntp, esp_sntp, bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init() (+11 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.12
Nodes (53): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t (+45 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "FreeRTOS.h"
Cohesion: 0.27
Nodes (9): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "transport_ble.c"
Cohesion: 0.07
Nodes (41): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, fpm_ble_config_set_clock_cb() (+33 more)

### Community 12 - "appcfg_cache_peek"
Cohesion: 0.14
Nodes (21): appcfg_cache_peek(), step_cb(), volume_cb(), f_switch(), ui_edit_target_t, ui_auth_user_count(), ui_cfg_alarm_tone(), ui_cfg_alarm_volume() (+13 more)

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
Cohesion: 0.06
Nodes (131): card_state_t, bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init() (+123 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_userEditScreen.c"
Cohesion: 0.33
Nodes (8): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), ui_form

### Community 19 - "ethernet_init.c"
Cohesion: 0.19
Nodes (16): esp_check, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, ethernet_init, esp_err_t, esp_eth_mac_t (+8 more)

### Community 20 - "storage.c"
Cohesion: 0.11
Nodes (43): app_metrics_t, cfg_result_t, appmetrics_load(), appmetrics_store(), esp_err_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+35 more)

### Community 21 - "http_api.c"
Cohesion: 0.10
Nodes (33): esp_chip_info, esp_http_server, esp_system, eTaskState, diag_worker(), mem_diag_report(), mem_diag_report_full(), mem_diag_report_heap_full() (+25 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.11
Nodes (41): ads1115_is_ready(), bmp280_is_ready(), ms5803_is_ready(), aws_telemetry_task(), flow_meter_get(), AppConfig, esp_err_t, sensor_sample_t (+33 more)

### Community 23 - "flow_meter.c"
Cohesion: 0.25
Nodes (7): esp_err_t, flow_meter_init(), flow_meter_record(), flow_integrator_push(), flow_integrator_t, flow_meter, math

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.17
Nodes (11): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar), 2026-10-12 — Soak final de 1.5.36-dev APROBADO (~28 h): baseline de V1, 6. Pendiente (después de estabilizar §2 y validar §3) (+3 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.22
Nodes (17): cdefs, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t, esp_err_t (+9 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.14
Nodes (20): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+12 more)

### Community 28 - "vTaskDelay"
Cohesion: 0.13
Nodes (18): bsp_touch_config_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t (+10 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.15
Nodes (28): eth_phy_autoneg_cmd_t, eth_phy_config_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t (+20 more)

### Community 31 - "wifi_mgr.c"
Cohesion: 0.15
Nodes (13): esp_wifi_types, esp_event_base_t, wifi_status_t, log_dns_info(), notify(), wifi_apply_cb(), wifi_event_handler(), wifi_mgr_stop() (+5 more)

### Community 32 - "esp_log.h"
Cohesion: 0.06
Nodes (29): cjson, esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, esp_random, derive(), from_hex() (+21 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.17
Nodes (18): cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t (+10 more)

### Community 36 - "sd_monitor_task"
Cohesion: 0.15
Nodes (15): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), lv_event_t, restart_btn_event_cb(), sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), ui_main_deinit_cb() (+7 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.16
Nodes (19): base_card(), lv_event_cb_t, lv_event_t, lv_obj_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general() (+11 more)

### Community 38 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 39 - "sfm3300.c"
Cohesion: 0.29
Nodes (10): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+2 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.27
Nodes (10): conn_status_t, lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct(), str_hash() (+2 more)

### Community 41 - "main.c"
Cohesion: 0.10
Nodes (17): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+9 more)

### Community 42 - "save_and_refresh"
Cohesion: 0.27
Nodes (11): is_press_unit(), unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas() (+3 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "ui_sensorEditScreen.c"
Cohesion: 0.17
Nodes (26): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+18 more)

### Community 45 - "lv_port_mem.c"
Cohesion: 0.14
Nodes (9): esp_heap_caps, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t (+1 more)

### Community 47 - "ui_i18n"
Cohesion: 0.17
Nodes (10): ui_datetimeScreen_screen_destroy(), ui_generalScreen_screen_destroy(), lv_event_t, edit_cb(), rebuild(), test_cb(), ui_sensorDiagScreen_screen_destroy(), ui_cfg (+2 more)

### Community 49 - "display_idle_cb"
Cohesion: 0.24
Nodes (10): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness() (+2 more)

### Community 51 - "ui_pinScreen.c"
Cohesion: 0.32
Nodes (6): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry()

### Community 52 - "ui_keypadScreen.c"
Cohesion: 0.28
Nodes (7): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new(), ui

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "at24c256.c"
Cohesion: 0.30
Nodes (13): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+5 more)

### Community 56 - "ui_nav.c"
Cohesion: 0.17
Nodes (15): bleapp_goroot_async(), bleapp_off_cb(), lv_event_t, save_cb(), set_str(), lv_event_t, lv_obj_t, ui_nav_back() (+7 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "string"
Cohesion: 0.16
Nodes (9): cert_erase_cb(), cert_pills_refresh(), cert_presence(), ui_netCloudScreen_screen_destroy(), close_to(), main(), stdio, string (+1 more)

### Community 60 - "apply_authenticated_cb"
Cohesion: 0.18
Nodes (12): apply_authenticated_cb(), lv_event_t, allow_decimal(), ui_sensorDiagScreen_refresh(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit() (+4 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (8): config_mode, bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t, ui_bleAppScreen_screen_destroy()

### Community 62 - "ui_netWifiScreen.c"
Cohesion: 0.15
Nodes (12): eth_mgr, apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), ui_netEthScreen_screen_destroy() (+4 more)

### Community 63 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 64 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 67 - "ms5803.c"
Cohesion: 0.37
Nodes (12): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+4 more)

### Community 68 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 69 - "test_eeprom.c"
Cohesion: 0.33
Nodes (9): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read() (+1 more)

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_cfg.c"
Cohesion: 0.16
Nodes (27): appcfg_save(), lv_event_t, next(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+19 more)

### Community 75 - "ws_stream.c"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 76 - "ui_usersScreen.c"
Cohesion: 0.33
Nodes (4): app_user_role_t, role_color(), role_name(), ui_usersScreen_screen_destroy()

### Community 77 - "ui_form.c"
Cohesion: 0.38
Nodes (4): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 79 - "esp_eth_com"
Cohesion: 0.40
Nodes (3): esp_eth_com, esp_eth_mac_spi, esp_eth_phy

### Community 80 - "save_cb"
Cohesion: 0.67
Nodes (3): eth_mgr_is_up(), lv_event_t, save_cb()

### Community 83 - "on_eth_event"
Cohesion: 0.40
Nodes (6): esp_event_base_t, on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif(), state_on_net_available()

### Community 90 - "ui_infoScreen.c"
Cohesion: 0.06
Nodes (46): dirent, esp_app_desc, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE (+38 more)

## Knowledge Gaps
- **137 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+132 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `app_main`, `esp_err_to_name`, `alarm_mgr.c`, `ui_loginScreen.c`, `screen_init_task`, `transport_ble.c`, `fpm_ble_config.c`, `ui_label`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `ui_statusbar_controller.c`, `ui_generalSimpleScreen.c`, `ui_connectivityScreen.c`, `save_and_refresh`, `ui_sensorEditScreen.c`, `display_idle_cb`, `ui_nav.c`, `apply_authenticated_cb`, `gas_label_color`, `ui_cfg.c`, `save_cb`, `ui_infoScreen.c`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `app_main`, `transport_mqtt.c`, `time_mgr.c`, `screen_init_task`, `FreeRTOS.h`, `transport_ble.c`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `vTaskDelay`, `wifi_mgr.c`, `esp_log.h`, `sd_monitor_task`, `sfm3300.c`, `fpm_i2c_guard.c`, `at24c256.c`, `ms5803.c`, `ads1115.c`, `ui_infoScreen.c`?**
  _High betweenness centrality (0.042) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_statusbar_controller.c`, `ui_wifi_main_icon_init`, `time_mgr.c`, `main.c`, `appcfg_cache_peek`, `ui_label`, `at24c256.c`, `vTaskDelay`?**
  _High betweenness centrality (0.030) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._