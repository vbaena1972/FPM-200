# Graph Report - FPM-200  (2026-10-14)

## Corpus Check
- 210 files · ~437,363 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1691 nodes · 4438 edges · 96 communities (88 shown, 8 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1136 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `30c17129`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- stdbool
- wifi_mgr.c
- ui_infoScreen.c
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- sim_backend.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- ui_netCloudScreen.c
- transport_ble.c
- pressure_card
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_auth_can
- ui_pin_hash.c
- lv_obj_t
- http_api.c
- sensors_runtime.c
- math
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- display_idle_cb
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- ui_nav_load
- stdint
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- sha256_update
- ui_generalSimpleScreen.c
- ui_nav.c
- sensors_acq_task
- ui_connectivityScreen.c
- main/main.c
- apply_authenticated_cb
- get_general_simple
- ui_cfg.c
- storage
- README.md
- ms5803.c
- fresh_net_cloud
- gen_ota_signing_key.sh script
- fresh_pin
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- get_sensor_diag
- ui_bleAppScreen.c
- Changelog
- string
- esp_eth_driver
- driver_delay_ms
- ui_wifi_main_icon_init
- stdio
- ui_userEditScreen.c
- ui_widgets
- lvgl
- get_sensor_edit
- PUBLICAR_FIRMWARE.md
- Simulador de PC — HMI FPM-200
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- AGENTS.md
- CLAUDE.md
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- appcfg_cache_peek
- README_PARCHE.md
- state_pub.c
- ads1115.c
- ota_update.c
- test_eeprom.c
- fpm_i2c_guard
- esp_err_to_name
- bmp280.c
- ui_open_useredit_cb
- ui_open_confirm_cb
- fresh_net_eth

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 78 edges
2. `ui_label()` - 61 edges
3. `esp_err_to_name()` - 51 edges
4. `ui_col()` - 46 edges
5. `ui_box()` - 43 edges
6. `_t()` - 40 edges
7. `ui_card()` - 38 edges
8. `appcfg_save()` - 36 edges
9. `ui_icon()` - 33 edges
10. `esp_timer_get_time()` - 27 edges

## Surprising Connections (you probably didn't know these)
- `apply_scenario()` --calls--> `ui_main_set_clock()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/screens/ui_mainScreen.c
- `apply_scenario()` --calls--> `ui_main_set_date()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/screens/ui_mainScreen.c
- `apply_scenario()` --calls--> `ui_main_update()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/screens/ui_mainScreen.c
- `data_activity_cb()` --calls--> `ui_main_signal_data_activity()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/screens/ui_mainScreen.c
- `main()` --calls--> `ui_init()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/ui.c

## Import Cycles
- None detected.

## Communities (96 total, 8 thin omitted)

### Community 1 - "wifi_mgr.c"
Cohesion: 0.10
Nodes (33): esp_task_wdt, esp_wifi_types, eth_mgr_is_up(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t (+25 more)

### Community 2 - "ui_infoScreen.c"
Cohesion: 0.27
Nodes (10): esp_ota_ops, lv_event_t, factory_btn_cb(), ota_channel_cb(), ota_check_cb(), ota_install_cb(), ota_screen_del_cb(), ota_start() (+2 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.07
Nodes (49): esp_mqtt_event_handle_t, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device() (+41 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.05
Nodes (55): alarm_state_change_cb_t, alarm_tone_out_t, alarm_tone_prio_t, esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_ops, esp_lcd_panel_vendor, esp_lcd_touch (+47 more)

### Community 5 - "ui.c"
Cohesion: 0.16
Nodes (16): ui_connectivityScreen_screen_destroy(), ui_infoScreen_screen_destroy(), ui_mainScreen_screen_destroy(), lv_timer_t, get_bleapp(), get_connectivity(), get_general(), get_info() (+8 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.19
Nodes (21): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+13 more)

### Community 7 - "sim_backend.c"
Cohesion: 0.06
Nodes (64): app_metrics_t, cert_info_t, cert_kind_t, cfg_result_t, crypto, fcntl, esp_err_t, cert_store_erase() (+56 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.10
Nodes (60): BaseType_t, emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t (+52 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "ui_netCloudScreen.c"
Cohesion: 0.33
Nodes (7): cert_store, lv_event_t, cert_erase_cb(), cert_pills_refresh(), cert_presence(), save_cb(), set_str()

### Community 11 - "transport_ble.c"
Cohesion: 0.07
Nodes (37): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, fpm_ble_config_set_clock_cb() (+29 more)

### Community 12 - "pressure_card"
Cohesion: 0.29
Nodes (10): pressure_card(), ui_edit_target_t, ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_to_disp(), ui_cfg_pressure_limit_enabled(), ui_cfg_pressure_max(), ui_cfg_pressure_min() (+2 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.11
Nodes (54): appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate(), appcfg_take_pin_migration(), app_user_t, AppConfig (+46 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.09
Nodes (27): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, mem_diag_report(), esp_event_base_t, on_eth_event() (+19 more)

### Community 16 - "ui_label"
Cohesion: 0.05
Nodes (146): alarm_cause_t, card_state_t, ui_refresh_task(), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t (+138 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (28): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+20 more)

### Community 18 - "ui_auth_can"
Cohesion: 0.24
Nodes (14): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+6 more)

### Community 19 - "ui_pin_hash.c"
Cohesion: 0.26
Nodes (10): esp_random, derive(), from_hex(), random_salt(), to_hex(), ui_pin_hash(), ui_pin_is_hashed(), ui_pin_normalize() (+2 more)

### Community 20 - "lv_obj_t"
Cohesion: 0.17
Nodes (12): ui_confirmScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_usersScreen_screen_destroy(), lv_obj_t, fresh_confirm(), fresh_keypad(), fresh_net_wifi() (+4 more)

### Community 21 - "http_api.c"
Cohesion: 0.08
Nodes (38): esp_chip_info, esp_http_server, esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks() (+30 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.11
Nodes (36): ads1115_is_ready(), ms5803_is_ready(), aws_telemetry_task(), esp_err_t, flow_meter_snapshot_t, flow_meter_get(), flow_meter_init(), AppConfig (+28 more)

### Community 23 - "math"
Cohesion: 0.20
Nodes (7): assert, flow_meter_record(), flow_integrator_push(), close_to(), main(), flow_integrator_t, math

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.14
Nodes (13): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar), 2026-10-12 — FW 1.6.2-dev: corrección del buzzer continuo (sin compilar), 2026-10-12 noche — FW 1.6.3-dev en HW, soak de ~12 h en curso (+5 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.20
Nodes (9): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4.1 Lecciones del ensayo en placa de desarrollo (2026-10-14), 4.2 Requisitos de producción pendientes (no se hacen en V1), 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip) (+1 more)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.06
Nodes (41): bsp_touch_config_t, cdefs, esp_compiler, esp_lcd_panel_io, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface (+33 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.18
Nodes (16): esp_eth_netif_glue, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init() (+8 more)

### Community 28 - "display_idle_cb"
Cohesion: 0.24
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_auth_logout(), ui_cfg_brightness(), ui_cfg_dim_minutes() (+3 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.07
Nodes (48): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+40 more)

### Community 31 - "ui_nav_load"
Cohesion: 0.19
Nodes (15): ui_datetimeScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), lv_event_t, fresh_login(), fresh_net_ble(), get_datetime() (+7 more)

### Community 32 - "stdint"
Cohesion: 0.10
Nodes (5): cjson, esp_heap_caps, esp_netif, stdint, ui_pin_hash

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.14
Nodes (23): transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible() (+15 more)

### Community 36 - "sha256_update"
Cohesion: 0.47
Nodes (9): hmac(), hmac_key_init(), sha256_block(), sha256_final(), sha256_init(), sha256_update(), ui_pbkdf2_sha256(), hmac_key_t (+1 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.16
Nodes (18): base_card(), lv_event_cb_t, lv_event_t, lv_obj_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general() (+10 more)

### Community 38 - "ui_nav.c"
Cohesion: 0.27
Nodes (9): bleapp_goroot_async(), lv_event_t, lv_obj_t, ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to(), ui_nav_replace() (+1 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.31
Nodes (9): conn_status_t, cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+1 more)

### Community 41 - "main/main.c"
Cohesion: 0.06
Nodes (36): dirent, errno, esp_app_desc, esp_app_trace, esp_core_dump, esp_heap_trace, esp_trace, esp_trace_open_params_t (+28 more)

### Community 42 - "apply_authenticated_cb"
Cohesion: 0.40
Nodes (6): apply_authenticated_cb(), lv_event_t, ui_connectivityScreen_refresh(), ui_sensorDiagScreen_refresh(), ui_sensorEditScreen_refresh(), ui_nav_swap()

### Community 43 - "get_general_simple"
Cohesion: 0.67
Nodes (3): ui_generalSimpleScreen_screen_destroy(), get_general_simple(), ui_open_general_simple_cb()

### Community 44 - "ui_cfg.c"
Cohesion: 0.13
Nodes (26): lv_event_t, next(), allow_decimal(), gas_label_color(), ui_cfg_apply_timezone(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_flow_decimals() (+18 more)

### Community 45 - "storage"
Cohesion: 0.29
Nodes (7): alarm_mgr, apply_scenario(), lv_timer_t, data_activity_cb(), main(), scenario_timer_cb(), storage

### Community 47 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+3 more)

### Community 49 - "fresh_net_cloud"
Cohesion: 0.67
Nodes (3): ui_netCloudScreen_screen_destroy(), fresh_net_cloud(), ui_open_net_cloud_cb()

### Community 51 - "fresh_pin"
Cohesion: 0.50
Nodes (4): ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 52 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (19): lv_event_t, dec_cb(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch(), rebuild() (+11 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "get_sensor_diag"
Cohesion: 0.67
Nodes (3): ui_sensorDiagScreen_screen_destroy(), get_sensor_diag(), ui_open_sensordiag_cb()

### Community 56 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (8): config_mode, bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t, ui_bleAppScreen_screen_destroy()

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "string"
Cohesion: 0.20
Nodes (6): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), string, tz_map

### Community 60 - "driver_delay_ms"
Cohesion: 0.28
Nodes (14): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+6 more)

### Community 61 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 62 - "stdio"
Cohesion: 0.21
Nodes (6): eth_mgr, lv_event_t, save_cb(), set_str(), stdio, ui_form

### Community 63 - "ui_userEditScreen.c"
Cohesion: 0.43
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_nav_back()

### Community 64 - "ui_widgets"
Cohesion: 0.12
Nodes (15): ui_generalScreen_screen_destroy(), key_cb(), refresh_value(), lv_event_t, save_cb(), set_str(), lv_event_t, edit_cb() (+7 more)

### Community 66 - "lvgl"
Cohesion: 0.09
Nodes (8): app_user_role_t, role_color(), role_name(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), lvgl

### Community 67 - "get_sensor_edit"
Cohesion: 0.67
Nodes (3): ui_sensorEditScreen_screen_destroy(), get_sensor_edit(), ui_open_sensor_cb()

### Community 69 - "Simulador de PC — HMI FPM-200"
Cohesion: 0.22
Nodes (7): 1. Compilar, 2. Correr, Simulador FPM-200 — instrucciones rápidas, Compilar y correr, Cómo está armado, Notas, Simulador de PC — HMI FPM-200

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 81 - "appcfg_cache_peek"
Cohesion: 0.14
Nodes (22): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), step_cb(), volume_cb(), f_switch(), ui_auth_user_count(), ui_cfg_alarm_tone() (+14 more)

### Community 88 - "state_pub.c"
Cohesion: 0.29
Nodes (5): device_state_t, state_build_json(), state_get(), state_update_sensors(), state_pub

### Community 89 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 90 - "ota_update.c"
Cohesion: 0.12
Nodes (30): esp_app_format, esp_partition, esp_partition_t, FILE, cJSON, esp_err_t, ota_manifest_t, copy_str() (+22 more)

### Community 91 - "test_eeprom.c"
Cohesion: 0.26
Nodes (11): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, TickType_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device() (+3 more)

### Community 93 - "fpm_i2c_guard"
Cohesion: 0.29
Nodes (9): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+1 more)

### Community 94 - "esp_err_to_name"
Cohesion: 0.28
Nodes (13): esp_err_t, mdns_svc_start(), time_sync_worker(), flow_meter_service(), esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point() (+5 more)

### Community 95 - "bmp280.c"
Cohesion: 0.33
Nodes (10): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+2 more)

### Community 96 - "ui_open_useredit_cb"
Cohesion: 0.39
Nodes (8): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), fresh_useredit(), ui_open_useredit_cb()

### Community 102 - "ui_open_confirm_cb"
Cohesion: 0.50
Nodes (4): accept_cb(), lv_event_t, ui_edit_set_new(), ui_open_confirm_cb()

### Community 104 - "fresh_net_eth"
Cohesion: 0.67
Nodes (3): ui_netEthScreen_screen_destroy(), fresh_net_eth(), ui_open_net_eth_cb()

## Knowledge Gaps
- **147 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. Compilar` (+142 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **8 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `wifi_mgr.c`, `alarm_mgr.c`, `ui_loginScreen.c`, `screen_init_task`, `ui_netCloudScreen.c`, `pressure_card`, `fpm_ble_config.c`, `ui_label`, `ui_auth_can`, `http_api.c`, `sensors_runtime.c`, `display_idle_cb`, `ui_statusbar_controller.c`, `ui_generalSimpleScreen.c`, `ui_connectivityScreen.c`, `ui_cfg.c`, `ui_sensorEditScreen.c`, `stdio`, `ui_widgets`, `ota_update.c`, `esp_err_to_name`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `wifi_mgr.c`, `transport_mqtt.c`, `ota_update.c`, `sim_backend.c`, `sensors_acq_task`, `screen_init_task`, `main/main.c`, `transport_ble.c`, `fpm_ble_config.c`, `ms5803.c`, `http_api.c`, `sensors_runtime.c`, `ads1115.c`, `touch_io_bounded.c`, `eth_mgr.c`, `driver_delay_ms`, `fpm_i2c_guard`, `bmp280.c`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Why does `app_main()` connect `sensors_runtime.c` to `transport_mqtt.c`, `alarm_mgr.c`, `sensors_acq_task`, `esp_eth_mac_ksz8851snl.c`, `main/main.c`, `ui_cfg.c`, `fpm_ble_config.c`, `time_mgr.c`, `appcfg_cache_peek`, `http_api.c`, `esp_err_to_name`?**
  _High betweenness centrality (0.025) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 52 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 52 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 44 INFERRED edges - model-reasoned connections that need verification._