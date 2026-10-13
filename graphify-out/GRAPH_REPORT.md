# Graph Report - FPM-200  (2026-10-12)

## Corpus Check
- 192 files · ~428,148 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1609 nodes · 4369 edges · 79 communities (73 shown, 6 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1062 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `165a35f1`
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
- mem_diag.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- FreeRTOS.h
- ui_bleAppScreen.c
- ui_edit_begin
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_mainScreen.c
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_userEditScreen.c
- ethernet_init.c
- ui_infoScreen.c
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
- sensors_acq_task
- ui_connectivityScreen.c
- main.c
- save_and_refresh
- fpm_i2c_guard.c
- apply_authenticated_cb
- lv_port_mem.c
- README.md
- ui_pin_hash.c
- ui_cfg_preview_brightness
- gen_ota_signing_key.sh script
- ui_i18n
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- driver_delay_ms
- ota_manifest.c
- Changelog
- ui_netCloudScreen.c
- esp_eth_driver
- sensor_cfg_persist
- ui_sensorDiagScreen.c
- string
- esp_eth_com
- lvgl
- ms5803.c
- ads1115.c
- test_eeprom.c
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- gas_label_color
- AGENTS.md
- CLAUDE.md
- appcfg_cache_peek
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- ui_cfg.c
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
10. `esp_timer_get_time()` - 27 edges

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

## Communities (79 total, 6 thin omitted)

### Community 0 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), task_tracer, TaskHandle_t

### Community 1 - "esp_err_to_name"
Cohesion: 0.12
Nodes (26): esp_wifi_types, esp_err_t, mdns_svc_start(), time_sync_worker(), apply_ip_mode(), AppConfig, esp_err_t, net_cfg_equals() (+18 more)

### Community 2 - "ui_label"
Cohesion: 0.07
Nodes (102): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+94 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.08
Nodes (41): cert_info_t, cert_kind_t, crypto, esp_mqtt_event_handle_t, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all() (+33 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.07
Nodes (51): alarm_state_change_cb_t, alarm_tone_out_t, alarm_tone_prio_t, assert, eth_ksz8851snl_config_t, eth_mac_config_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl() (+43 more)

### Community 5 - "ui.c"
Cohesion: 0.05
Nodes (80): bleapp_goroot_async(), ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_infoScreen_screen_destroy(), accept_cb(), lv_event_t (+72 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.19
Nodes (21): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+13 more)

### Community 7 - "mem_diag.c"
Cohesion: 0.21
Nodes (9): esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+1 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.13
Nodes (50): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t, eth_duplex_t, eth_link_t (+42 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "FreeRTOS.h"
Cohesion: 0.27
Nodes (9): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg(), esp_err_t (+1 more)

### Community 11 - "ui_bleAppScreen.c"
Cohesion: 0.12
Nodes (18): config_mode, transport_ble_is_connected(), esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_is_connected(), bleapp_leave_to_dashboard() (+10 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.27
Nodes (10): step_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes(), ui_cfg_pressure_max(), ui_cfg_pressure_min(), ui_cfg_reannounce_minutes() (+2 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.07
Nodes (63): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, apply_channel_obj() (+55 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.11
Nodes (21): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_err_t, esp_event_base_t, net_core_init() (+13 more)

### Community 16 - "ui_mainScreen.c"
Cohesion: 0.10
Nodes (45): alarm_mgr, card_state_t, ui_refresh_task(), alarm_condition(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_silence_cb() (+37 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_userEditScreen.c"
Cohesion: 0.48
Nodes (6): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role()

### Community 19 - "ethernet_init.c"
Cohesion: 0.16
Nodes (18): esp_check, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, eth_phy_config_t, ethernet_init, esp_eth_phy_new_ksz8851snl() (+10 more)

### Community 20 - "ui_infoScreen.c"
Cohesion: 0.08
Nodes (53): app_metrics_t, cfg_result_t, appmetrics_load(), appmetrics_store(), esp_err_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+45 more)

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.13
Nodes (33): ads1115_is_ready(), fpm_i2c_init(), ms5803_is_ready(), aws_telemetry_task(), transport_mqtt_runtime_init(), AppConfig, i2c_master_bus_handle_t, sensor_sample_t (+25 more)

### Community 23 - "flow_meter.c"
Cohesion: 0.12
Nodes (13): device_state_t, esp_err_t, flow_meter_get(), flow_meter_init(), flow_meter_record(), flow_integrator_push(), state_build_json(), state_get() (+5 more)

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.14
Nodes (13): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar), 2026-10-12 — FW 1.6.2-dev: corrección del buzzer continuo (sin compilar), 2026-10-12 noche — FW 1.6.3-dev en HW, soak de ~12 h en curso (+5 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.22
Nodes (17): cdefs, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t, esp_err_t (+9 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.16
Nodes (17): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+9 more)

### Community 28 - "ft5x06.c"
Cohesion: 0.15
Nodes (16): bsp_touch_config_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t (+8 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (26): eth_phy_autoneg_cmd_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t, ksz8851_update_link_duplex_speed() (+18 more)

### Community 31 - "ui_statusbar_request_refresh"
Cohesion: 0.16
Nodes (20): mem_diag_report(), esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, wifi_status_t (+12 more)

### Community 32 - "esp_log.h"
Cohesion: 0.06
Nodes (22): cjson, esp_http_server, esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_ops, esp_lcd_panel_vendor, esp_lcd_touch, esp_lcd_types (+14 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.22
Nodes (14): eth_mgr, transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible() (+6 more)

### Community 36 - "sd_monitor_task"
Cohesion: 0.20
Nodes (12): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), lv_event_t, restart_btn_event_cb(), sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), ui_main_deinit_cb() (+4 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.29
Nodes (8): ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_nav

### Community 38 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 39 - "sensors_acq_task"
Cohesion: 0.18
Nodes (17): bmp280_is_ready(), esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read() (+9 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.23
Nodes (12): conn_status_t, cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+4 more)

### Community 41 - "main.c"
Cohesion: 0.08
Nodes (20): errno, esp_app_desc, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t (+12 more)

### Community 42 - "save_and_refresh"
Cohesion: 0.33
Nodes (9): AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.17
Nodes (25): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+17 more)

### Community 44 - "apply_authenticated_cb"
Cohesion: 0.14
Nodes (18): apply_authenticated_cb(), lv_event_t, lv_event_t, general_loaded_cb(), protected_back_cb(), allow_decimal(), ui_cfg_brightness(), ui_cfg_dim_minutes() (+10 more)

### Community 45 - "lv_port_mem.c"
Cohesion: 0.14
Nodes (9): esp_heap_caps, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t (+1 more)

### Community 47 - "ui_pin_hash.c"
Cohesion: 0.24
Nodes (18): esp_random, derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final() (+10 more)

### Community 49 - "ui_cfg_preview_brightness"
Cohesion: 0.83
Nodes (4): bright_cb(), clamp_brightness(), ui_cfg_preview_brightness(), ui_cfg_set_brightness()

### Community 51 - "ui_i18n"
Cohesion: 0.20
Nodes (8): key_cb(), refresh_value(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_cfg, ui_i18n

### Community 52 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (23): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+15 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "driver_delay_ms"
Cohesion: 0.30
Nodes (14): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+6 more)

### Community 56 - "ota_manifest.c"
Cohesion: 0.27
Nodes (8): cJSON, esp_err_t, ota_manifest_t, copy_str(), ota_manifest_fetch(), ota_version_compare(), parse_version(), strings

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "ui_netCloudScreen.c"
Cohesion: 0.32
Nodes (6): cert_store, lv_event_t, cert_erase_cb(), cert_pills_refresh(), cert_presence(), set_str()

### Community 60 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 61 - "ui_sensorDiagScreen.c"
Cohesion: 0.21
Nodes (11): apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), lv_event_t, edit_cb() (+3 more)

### Community 62 - "string"
Cohesion: 0.12
Nodes (14): set_str(), lv_event_t, save_cb(), set_str(), set_str(), close_to(), main(), stdio (+6 more)

### Community 63 - "esp_eth_com"
Cohesion: 0.40
Nodes (3): esp_eth_com, esp_eth_mac_spi, esp_eth_phy

### Community 66 - "lvgl"
Cohesion: 0.11
Nodes (4): app_user_role_t, role_color(), role_name(), lvgl

### Community 67 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+3 more)

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

### Community 72 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 77 - "appcfg_cache_peek"
Cohesion: 0.10
Nodes (29): appcfg_cache_peek(), appcfg_save(), transport_ble_stop_adv(), bleapp_off_cb(), lv_event_t, save_cb(), save_cb(), f_switch() (+21 more)

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 81 - "ui_cfg.c"
Cohesion: 0.15
Nodes (27): lv_event_t, next(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user() (+19 more)

### Community 90 - "ota_update.c"
Cohesion: 0.16
Nodes (22): dirent, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE, esp_err_t (+14 more)

## Knowledge Gaps
- **139 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+134 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_err_to_name`, `ui_label`, `transport_mqtt.c`, `alarm_mgr.c`, `ui_loginScreen.c`, `screen_init_task`, `ui_edit_begin`, `fpm_ble_config.c`, `ui_mainScreen.c`, `ui_infoScreen.c`, `http_api.c`, `sensors_runtime.c`, `ui_statusbar_request_refresh`, `ui_statusbar_controller.c`, `ui_connectivityScreen.c`, `save_and_refresh`, `apply_authenticated_cb`, `ui_cfg_preview_brightness`, `string`, `gas_label_color`, `ui_cfg.c`?**
  _High betweenness centrality (0.047) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `transport_mqtt.c`, `screen_init_task`, `FreeRTOS.h`, `fpm_ble_config.c`, `ui_infoScreen.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `ft5x06.c`, `ui_statusbar_request_refresh`, `esp_log.h`, `sd_monitor_task`, `sensors_acq_task`, `fpm_i2c_guard.c`, `driver_delay_ms`, `ota_manifest.c`, `sensor_cfg_persist`, `ms5803.c`, `ads1115.c`, `ota_update.c`?**
  _High betweenness centrality (0.042) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_label`, `ui_statusbar_controller.c`, `ui_wifi_main_icon_init`, `sensors_acq_task`, `main.c`, `fpm_i2c_guard.c`, `appcfg_cache_peek`, `driver_delay_ms`, `ft5x06.c`?**
  _High betweenness centrality (0.030) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._