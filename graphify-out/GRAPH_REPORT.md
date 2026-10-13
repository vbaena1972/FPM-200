# Graph Report - FPM-200  (2026-10-12)

## Corpus Check
- 192 files · ~427,807 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1608 nodes · 4366 edges · 82 communities (75 shown, 7 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1062 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `c2913c2e`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- task_tracer.c
- esp_err_to_name
- ui_label
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- fpm_i2c_guard
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- FreeRTOS.h
- transport_ble.c
- ui_sensorDiagScreen.c
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_mainScreen.c
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
- ft5x06.c
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- ui_statusbar_request_refresh
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
- appcfg_cache_peek
- fpm_i2c_guard.c
- pressure_card
- lv_port_mem.c
- README.md
- ui_i18n
- display_idle_cb
- gen_ota_signing_key.sh script
- ui
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- at24c256.c
- mem_diag.c
- Changelog
- ui_netCloudScreen.c
- esp_eth_driver
- ui_card
- ui_wifi_main_icon.c
- string
- next
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
- vTaskDelay
- ui_usersScreen.c
- ui_col
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- stepper
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
10. `esp_timer_get_time()` - 27 edges

## Surprising Connections (you probably didn't know these)
- `app_main()` --calls--> `mem_diag_start_periodic()`  [INFERRED]
  firmware/main/main.c → firmware/components/diag/mem_diag.c
- `sensors_acq_task()` --calls--> `sfm3300_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `sensors_acq_task()` --calls--> `sfm3300_get_last_rx()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/sfm3300.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c
- `state_update_sensors()` --calls--> `esp_timer_get_time()`  [INFERRED]
  firmware/components/state_pub/state_pub.c → firmware/tests/host/test_alarm.c

## Import Cycles
- None detected.

## Communities (82 total, 7 thin omitted)

### Community 0 - "task_tracer.c"
Cohesion: 0.14
Nodes (11): device_state_t, state_build_json(), state_get(), state_update_sensors(), eTaskState, state_str(), task_tracer_dump_now(), trace_worker() (+3 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (25): esp_wifi_types, esp_err_t, mdns_svc_start(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals(), wifi_apply_cb() (+17 more)

### Community 2 - "ui_label"
Cohesion: 0.16
Nodes (42): ui_bleAppScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t, row(), ui_datetimeScreen_screen_init() (+34 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.09
Nodes (28): alarm_mgr, esp_mqtt_event_handle_t, time_mgr_set_datetime(), add_channel(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event() (+20 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.08
Nodes (49): alarm_state_change_cb_t, alarm_tone_out_t, alarm_tone_prio_t, assert, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl() (+41 more)

### Community 5 - "ui.c"
Cohesion: 0.07
Nodes (67): apply_authenticated_cb(), lv_event_t, ui_connectivityScreen_refresh(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), ui_mainScreen_screen_destroy(), ui_sensorDiagScreen_refresh(), ui_sensorEditScreen_refresh() (+59 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.15
Nodes (28): add_user(), app_user_role_t, app_user_t, lv_event_cb_t, lv_event_t, lv_obj_t, eye_button(), eye_set() (+20 more)

### Community 7 - "fpm_i2c_guard"
Cohesion: 0.25
Nodes (10): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+2 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.10
Nodes (56): emac_ksz8851snl_t, esp_cpu, esp_eth_com, esp_eth_mac_spi, esp_eth_phy, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t (+48 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "FreeRTOS.h"
Cohesion: 0.27
Nodes (9): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "transport_ble.c"
Cohesion: 0.06
Nodes (50): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, config_mode, esp_bt (+42 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.19
Nodes (15): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb(), ui_edit_target_t (+7 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.11
Nodes (45): cJSON, esp_err_t, ota_manifest_t, copy_str(), ota_manifest_fetch(), ota_version_compare(), parse_version(), apply_channel_obj() (+37 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.11
Nodes (20): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_err_t, esp_event_base_t, net_core_init() (+12 more)

### Community 16 - "ui_mainScreen.c"
Cohesion: 0.11
Nodes (46): card_state_t, sensors_runtime_get_debug(), alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb(), apply_data_activity_visual() (+38 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_userEditScreen.c"
Cohesion: 0.33
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 19 - "ethernet_init.c"
Cohesion: 0.19
Nodes (16): esp_check, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, ethernet_init, esp_err_t, esp_eth_mac_t (+8 more)

### Community 20 - "storage.c"
Cohesion: 0.08
Nodes (58): app_metrics_t, cert_info_t, cert_kind_t, cfg_result_t, crypto, fcntl, esp_err_t, cert_store_erase() (+50 more)

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.11
Nodes (40): ads1115_is_ready(), bmp280_is_ready(), fpm_i2c_init(), ms5803_is_ready(), aws_telemetry_task(), transport_mqtt_runtime_init(), flow_meter_get(), AppConfig (+32 more)

### Community 23 - "flow_meter.c"
Cohesion: 0.18
Nodes (10): time_sync_worker(), esp_err_t, flow_meter_init(), flow_meter_record(), flow_meter_service(), flow_integrator_push(), flow_integrator_t, flow_meter (+2 more)

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.15
Nodes (12): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar), 2026-10-12 — FW 1.6.2-dev: corrección del buzzer continuo (sin compilar), 2026-10-12 — Soak final de 1.5.36-dev APROBADO (~28 h): baseline de V1 (+4 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.22
Nodes (17): cdefs, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t, esp_err_t (+9 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.14
Nodes (20): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+12 more)

### Community 28 - "ft5x06.c"
Cohesion: 0.15
Nodes (16): bsp_touch_config_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t (+8 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (26): eth_phy_config_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t, esp_eth_phy_new_ksz8851snl() (+18 more)

### Community 31 - "ui_statusbar_request_refresh"
Cohesion: 0.13
Nodes (21): mem_diag_report(), esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, wifi_status_t (+13 more)

### Community 32 - "esp_log.h"
Cohesion: 0.05
Nodes (38): cjson, esp_http_server, esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, esp_random, derive() (+30 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.35
Nodes (10): transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb() (+2 more)

### Community 36 - "sd_monitor_task"
Cohesion: 0.21
Nodes (10): sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), unmount_sdcard_hotplug(), lv_event_cb_t, lv_event_t, close_click_cb(), ui_sd_close() (+2 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.26
Nodes (9): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy() (+1 more)

### Community 38 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 39 - "sfm3300.c"
Cohesion: 0.29
Nodes (10): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+2 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.27
Nodes (10): conn_status_t, cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+2 more)

### Community 41 - "main.c"
Cohesion: 0.08
Nodes (22): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+14 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.16
Nodes (24): appcfg_cache_peek(), appcfg_save(), AppConfig, save_and_refresh(), set_str(), ui_auth_set_factory_pin(), ui_auth_user_remove(), ui_cfg_alarm_tone() (+16 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 44 - "pressure_card"
Cohesion: 0.17
Nodes (19): lv_obj_t, ui_edit_target_t, card(), flow_card(), mini(), pressure_card(), value_button(), ui_cfg_flow_decimals() (+11 more)

### Community 45 - "lv_port_mem.c"
Cohesion: 0.14
Nodes (9): esp_heap_caps, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t (+1 more)

### Community 47 - "ui_i18n"
Cohesion: 0.24
Nodes (6): ui_datetimeScreen_screen_destroy(), key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_cfg, ui_i18n

### Community 49 - "display_idle_cb"
Cohesion: 0.20
Nodes (12): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), lv_timer_t, clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes() (+4 more)

### Community 51 - "ui"
Cohesion: 0.28
Nodes (7): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), ui

### Community 52 - "ui_sensorEditScreen.c"
Cohesion: 0.23
Nodes (15): accept_cb(), lv_event_t, lv_event_t, dec_cb(), f_switch(), gas_cb(), is_press_unit(), limit_edit() (+7 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "at24c256.c"
Cohesion: 0.19
Nodes (20): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+12 more)

### Community 56 - "mem_diag.c"
Cohesion: 0.21
Nodes (9): esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+1 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "ui_netCloudScreen.c"
Cohesion: 0.32
Nodes (6): cert_store, lv_event_t, cert_erase_cb(), save_cb(), set_str(), ui_netCloudScreen_screen_destroy()

### Community 60 - "ui_card"
Cohesion: 0.23
Nodes (16): ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, factory_btn(), kv_card(), ota_button(), ota_card(), ui_infoScreen_screen_init() (+8 more)

### Community 61 - "ui_wifi_main_icon.c"
Cohesion: 0.36
Nodes (7): apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), ui_statusbar_controller

### Community 62 - "string"
Cohesion: 0.11
Nodes (14): eth_mgr, set_str(), ui_netBleScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), set_str(), ui_netWifiScreen_screen_destroy(), close_to(), main() (+6 more)

### Community 63 - "next"
Cohesion: 0.40
Nodes (5): lv_event_t, next(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

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
Cohesion: 0.20
Nodes (22): gas_label_color(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_logout() (+14 more)

### Community 75 - "vTaskDelay"
Cohesion: 0.50
Nodes (4): eth_phy_autoneg_cmd_t, phy_ksz8851_autonego_ctrl(), TickType_t, vTaskDelay()

### Community 76 - "ui_usersScreen.c"
Cohesion: 0.31
Nodes (9): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb() (+1 more)

### Community 77 - "ui_col"
Cohesion: 0.11
Nodes (38): bleapp_set_connected(), base_card(), lv_event_cb_t, lv_obj_t, seg(), ui_generalSimpleScreen_screen_init(), lv_timer_t, gas_label() (+30 more)

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 79 - "stepper"
Cohesion: 1.00
Nodes (3): lv_obj_t, small_button(), stepper()

### Community 90 - "ui_infoScreen.c"
Cohesion: 0.08
Nodes (37): dirent, esp_app_desc, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE (+29 more)

## Knowledge Gaps
- **138 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+133 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **7 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_err_to_name`, `ui_label`, `alarm_mgr.c`, `ui.c`, `ui_loginScreen.c`, `screen_init_task`, `transport_ble.c`, `ui_sensorDiagScreen.c`, `fpm_ble_config.c`, `ui_mainScreen.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `ui_statusbar_request_refresh`, `esp_log.h`, `ui_statusbar_controller.c`, `ui_connectivityScreen.c`, `pressure_card`, `display_idle_cb`, `ui_netCloudScreen.c`, `ui_card`, `next`, `ui_cfg.c`, `ui_col`, `ui_infoScreen.c`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `fpm_i2c_guard`, `screen_init_task`, `FreeRTOS.h`, `transport_ble.c`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `flow_meter.c`, `eth_mgr.c`, `ft5x06.c`, `ui_statusbar_request_refresh`, `esp_log.h`, `sd_monitor_task`, `sfm3300.c`, `fpm_i2c_guard.c`, `at24c256.c`, `ms5803.c`, `ads1115.c`, `ui_infoScreen.c`?**
  _High betweenness centrality (0.042) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_wifi_main_icon_init`, `fpm_i2c_guard`, `main.c`, `appcfg_cache_peek`, `vTaskDelay`, `ui_mainScreen.c`, `at24c256.c`, `ft5x06.c`, `ui_statusbar_request_refresh`?**
  _High betweenness centrality (0.030) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._