# Graph Report - FPM-200  (2026-10-15)

## Corpus Check
- 215 files · ~443,879 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1741 nodes · 4563 edges · 110 communities (101 shown, 9 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1169 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `dcfc7585`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- stdbool
- esp_err_to_name
- test_alarm.c
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- sim_backend.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- cert_store.c
- storage.c
- ui_edit_begin
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_auth_can
- ui_pin_hash.c
- ui_usersScreen.c
- http_api.c
- sensors_runtime.c
- test_flow_meter.c
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- ui_cfg.c
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- fpm_i2c_guard.c
- stdint
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- nvs_open
- ui_generalSimpleScreen.c
- ui_nav.c
- sensors_acq_task
- ui_connectivityScreen.c
- main/main.c
- apply_authenticated_cb
- config_mode.c
- gas_label_color
- ui_mainScreen.c
- README.md
- driver_delay_ms
- ft5x06.h
- gen_ota_signing_key.sh script
- alarm_mgr_init
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- esp_timer.h
- ui_bleAppScreen.c
- Changelog
- ui_nav_load
- esp_eth_driver
- at24c256.c
- storage
- string
- storage_extras.c
- ui_keypadScreen.c
- lvgl
- xSemaphoreTake
- PUBLICAR_FIRMWARE.md
- Simulador de PC — HMI FPM-200
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- app_main
- AGENTS.md
- CLAUDE.md
- esp_lcd_st7796.h
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- appcfg_cache_peek
- README_PARCHE.md
- mem_diag.c
- ads1115.c
- ota_update.c
- test_eeprom.c
- ui_infoScreen.c
- rtc_rv3028.c
- sensor_cfg_persist
- bmp280.c
- ws_stream.c
- lv_obj_t
- lv_event_t
- save_and_refresh
- on_network_ready
- math
- next
- metrics_store.c
- alarm_mgr_set_state_change_cb
- get_bleapp
- get_datetime
- get_general_simple
- fresh_net_wifi
- get_sensor_diag

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 78 edges
2. `ui_label()` - 62 edges
3. `esp_err_to_name()` - 51 edges
4. `ui_col()` - 47 edges
5. `_t()` - 44 edges
6. `ui_box()` - 44 edges
7. `ui_card()` - 38 edges
8. `appcfg_save()` - 36 edges
9. `ui_icon()` - 33 edges
10. `app_main()` - 26 edges

## Surprising Connections (you probably didn't know these)
- `open_login_cb()` --calls--> `ui_open_login_cb()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/ui.c
- `main()` --calls--> `ui_init()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/ui.c
- `cert_store_erase()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/cert_store/cert_store.c → Simulator/sim_backend.c
- `cert_store_import_from_sd()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/cert_store/cert_store.c → Simulator/sim_backend.c
- `bsp_touch_new()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/display/ft5x06.c → Simulator/sim_backend.c

## Import Cycles
- None detected.

## Communities (110 total, 9 thin omitted)

### Community 1 - "esp_err_to_name"
Cohesion: 0.09
Nodes (42): esp_wifi_types, esp_event_base_t, eth_mgr_is_up(), on_eth_event(), on_ip_event(), time_sync_worker(), apply_ip_mode(), AppConfig (+34 more)

### Community 2 - "test_alarm.c"
Cohesion: 0.12
Nodes (17): alarm_mgr, alarm_mgr_press_mute_impl(), set_buzzer_acoustic(), tone_out(), alarm_tone_get(), alarm_tone_init(), alarm_tone_set(), appcfg_cache_get() (+9 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.11
Nodes (25): esp_mqtt_event_handle_t, esp_reset_reason_t, add_channel(), AppConfig, cJSON, esp_event_base_t, sensor_signal_stats_t, lwt_prepare() (+17 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.17
Nodes (22): alarm_mgr_get_active_conditions(), alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute() (+14 more)

### Community 5 - "ui.c"
Cohesion: 0.15
Nodes (16): ui_connectivityScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_userEditScreen_screen_destroy(), lv_timer_t, fresh_net_cloud(), fresh_net_eth(), fresh_useredit() (+8 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.16
Nodes (23): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+15 more)

### Community 7 - "sim_backend.c"
Cohesion: 0.11
Nodes (16): ota_update, appcfg_cache_get(), appcfg_cache_peek(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_save(), AppConfig (+8 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.10
Nodes (60): BaseType_t, emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t (+52 more)

### Community 9 - "screen_init_task"
Cohesion: 0.18
Nodes (20): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+12 more)

### Community 10 - "cert_store.c"
Cohesion: 0.26
Nodes (16): cert_info_t, cert_kind_t, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd() (+8 more)

### Community 11 - "storage.c"
Cohesion: 0.32
Nodes (16): appcfg_cache_get(), appcfg_defaults(), appcfg_load(), appcfg_migrate(), appcfg_take_pin_migration(), app_user_t, AppConfig, cJSON (+8 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.20
Nodes (14): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), volume_cb(), ui_edit_target_t, ui_cfg_alarm_volume() (+6 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.07
Nodes (63): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, appcfg_cache_reload() (+55 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.13
Nodes (17): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_event_base_t, on_eth_event(), on_ip_event() (+9 more)

### Community 16 - "ui_label"
Cohesion: 0.07
Nodes (101): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+93 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (28): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+20 more)

### Community 18 - "ui_auth_can"
Cohesion: 0.15
Nodes (22): lv_event_t, cert_erase_cb(), save_cb(), set_str(), app_user_role_t, lv_event_t, delete_cb(), flash_error() (+14 more)

### Community 19 - "ui_pin_hash.c"
Cohesion: 0.23
Nodes (18): esp_random, derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final() (+10 more)

### Community 20 - "ui_usersScreen.c"
Cohesion: 0.31
Nodes (8): ui_userEditScreen_set_index(), add_cb(), app_user_role_t, lv_event_t, factory_pin_cb(), role_color(), role_name(), row_cb()

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.22
Nodes (19): sensor_sample_t, sensor_signal_stats_t, lock(), sensors_runtime_get_ages(), sensors_runtime_get_debug(), sensors_runtime_get_faults(), sensors_runtime_get_last(), sensors_runtime_get_min_max() (+11 more)

### Community 23 - "test_flow_meter.c"
Cohesion: 0.11
Nodes (18): assert, flow_meter_boot_checkpoint(), flow_meter_record(), flow_integrator_push(), close_to(), main(), appmetrics_load(), appmetrics_store() (+10 more)

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
Cohesion: 0.15
Nodes (19): esp_eth_netif_glue, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init() (+11 more)

### Community 28 - "ui_cfg.c"
Cohesion: 0.15
Nodes (25): lv_event_t, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_auth_active(), ui_auth_current_user(), ui_auth_logout() (+17 more)

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.07
Nodes (48): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+40 more)

### Community 31 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.24
Nodes (13): transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb() (+5 more)

### Community 36 - "nvs_open"
Cohesion: 0.25
Nodes (16): appmetrics_load(), appmetrics_store(), app_metrics_t, esp_err_t, esp_err_t, slot_key(), write_active(), write_slot_json() (+8 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.46
Nodes (6): lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 38 - "ui_nav.c"
Cohesion: 0.15
Nodes (17): bleapp_goroot_async(), lv_event_t, pin_key_cb(), pin_wrong_flash(), get_general(), lv_event_t, lv_obj_t, ui_nav_back() (+9 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.31
Nodes (9): conn_status_t, cloud_mgr_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick(), rssi_pct() (+1 more)

### Community 41 - "main/main.c"
Cohesion: 0.06
Nodes (36): dirent, errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_netif, esp_ota_ops, esp_task_wdt (+28 more)

### Community 42 - "apply_authenticated_cb"
Cohesion: 0.25
Nodes (9): apply_authenticated_cb(), lv_event_t, ui_connectivityScreen_refresh(), allow_decimal(), ui_sensorDiagScreen_refresh(), ui_sensorEditScreen_refresh(), ui_edit_is_audio(), ui_nav_swap() (+1 more)

### Community 43 - "config_mode.c"
Cohesion: 0.21
Nodes (8): esp_heap_caps, esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb(), lv_event_t

### Community 44 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 45 - "ui_mainScreen.c"
Cohesion: 0.08
Nodes (61): card_state_t, sensors_runtime_flow_warming(), sensors_runtime_flow_warming_elapsed_ms(), ui_refresh_task(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_open(), alarm_overlay_refresh() (+53 more)

### Community 47 - "driver_delay_ms"
Cohesion: 0.34
Nodes (12): driver_delay_ms(), esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word() (+4 more)

### Community 49 - "ft5x06.h"
Cohesion: 0.29
Nodes (4): esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port

### Community 51 - "alarm_mgr_init"
Cohesion: 0.22
Nodes (9): alarm_mgr_init(), esp_err_t, alarm_tone_init(), alarm_tone_out_t, esp_err_t, ledc_channel_config(), ledc_timer_config(), ledc_channel_config_t (+1 more)

### Community 52 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (23): lv_event_t, lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), gas_cb() (+15 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "esp_timer.h"
Cohesion: 0.12
Nodes (11): device_state_t, state_get(), eTaskState, TaskHandle_t, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker() (+3 more)

### Community 56 - "ui_bleAppScreen.c"
Cohesion: 0.22
Nodes (9): config_mode, transport_ble_is_connected(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t (+1 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "ui_nav_load"
Cohesion: 0.25
Nodes (8): ui_keypadScreen_screen_destroy(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_keypad(), fresh_pin(), ui_nav_load(), ui_open_keypad_cb(), ui_open_pin_cb()

### Community 60 - "at24c256.c"
Cohesion: 0.53
Nodes (8): at24c256_init(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified(), main()

### Community 61 - "storage"
Cohesion: 0.17
Nodes (10): eth_mgr, apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), storage (+2 more)

### Community 62 - "string"
Cohesion: 0.14
Nodes (13): cert_store, esp_system, ui_generalScreen_screen_destroy(), stdio, stdlib, string, tz_map, ui (+5 more)

### Community 63 - "storage_extras.c"
Cohesion: 0.36
Nodes (13): cfg_result_t, appcfg_calc_etag(), appcfg_patch(), appcfg_set(), appcfg_to_minified_json(), appcfg_validate(), AppConfig, cJSON (+5 more)

### Community 64 - "ui_keypadScreen.c"
Cohesion: 0.38
Nodes (5): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_edit_set_new()

### Community 67 - "xSemaphoreTake"
Cohesion: 0.17
Nodes (17): mem_diag_report(), alarm_tone_get(), alarm_tone_set(), apply_step_locked(), alarm_tone_prio_t, timer_cb(), aws_telemetry_task(), publish_alarm_transition() (+9 more)

### Community 69 - "Simulador de PC — HMI FPM-200"
Cohesion: 0.22
Nodes (7): 1. Compilar, 2. Correr, Simulador FPM-200 — instrucciones rápidas, Compilar y correr, Cómo está armado, Notas, Simulador de PC — HMI FPM-200

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "app_main"
Cohesion: 0.16
Nodes (14): mem_diag_start_periodic(), fpm_i2c_init(), esp_err_t, net_core_init(), transport_mqtt_runtime_init(), esp_err_t, flow_meter_init(), i2c_master_bus_handle_t (+6 more)

### Community 74 - "CLAUDE.md"
Cohesion: 0.40
Nodes (4): Alarmas: silencio (1.6.7), Consumo, totalizador y cortes de energía (1.6.8), Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard), graphify

### Community 77 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 81 - "appcfg_cache_peek"
Cohesion: 0.10
Nodes (29): appcfg_cache_peek(), appcfg_save(), transport_ble_stop_adv(), bleapp_off_cb(), lv_event_t, save_cb(), set_str(), f_switch() (+21 more)

### Community 88 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), task_state_str(), mem_diag

### Community 89 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 90 - "ota_update.c"
Cohesion: 0.12
Nodes (30): esp_app_format, esp_partition, esp_partition_t, FILE, cJSON, esp_err_t, ota_manifest_t, copy_str() (+22 more)

### Community 91 - "test_eeprom.c"
Cohesion: 0.23
Nodes (12): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, TickType_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device() (+4 more)

### Community 92 - "ui_infoScreen.c"
Cohesion: 0.35
Nodes (9): lv_event_t, factory_btn_cb(), ota_channel_cb(), ota_check_cb(), ota_install_cb(), ota_screen_del_cb(), ota_start(), reboot_cb() (+1 more)

### Community 93 - "rtc_rv3028.c"
Cohesion: 0.23
Nodes (11): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+3 more)

### Community 94 - "sensor_cfg_persist"
Cohesion: 0.19
Nodes (15): ads1115_is_ready(), at24c256_is_ready(), ms5803_is_ready(), AppConfig, esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensor_cfg_set_defaults() (+7 more)

### Community 95 - "bmp280.c"
Cohesion: 0.29
Nodes (11): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+3 more)

### Community 96 - "ws_stream.c"
Cohesion: 0.19
Nodes (10): cjson, esp_http_server, esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast() (+2 more)

### Community 97 - "lv_obj_t"
Cohesion: 0.22
Nodes (9): ui_confirmScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_usersScreen_screen_destroy(), lv_obj_t, fresh_confirm(), fresh_net_ble(), get_users(), ui_open_net_ble_cb() (+1 more)

### Community 98 - "lv_event_t"
Cohesion: 0.31
Nodes (9): ui_infoScreen_screen_destroy(), menu_item_cb(), lv_event_t, fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb(), ui_open_confirm_cb() (+1 more)

### Community 99 - "save_and_refresh"
Cohesion: 0.33
Nodes (9): AppConfig, save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled() (+1 more)

### Community 100 - "on_network_ready"
Cohesion: 0.29
Nodes (5): esp_err_t, mdns_svc_start(), on_network_ready(), mdns, mdns_svc

### Community 101 - "math"
Cohesion: 0.48
Nodes (5): mm24_get(), mm24_push(), main(), math, mm24_t

### Community 102 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 103 - "metrics_store.c"
Cohesion: 0.40
Nodes (3): metrics_store, nvs, nvs_flash

### Community 104 - "alarm_mgr_set_state_change_cb"
Cohesion: 1.00
Nodes (3): alarm_state_change_cb_t, alarm_mgr_set_state_change_cb(), alarm_mgr_set_state_change_cb_impl()

### Community 105 - "get_bleapp"
Cohesion: 0.67
Nodes (3): ui_bleAppScreen_screen_destroy(), get_bleapp(), ui_open_bleapp_cb()

### Community 106 - "get_datetime"
Cohesion: 0.67
Nodes (3): ui_datetimeScreen_screen_destroy(), get_datetime(), ui_open_datetime_cb()

### Community 107 - "get_general_simple"
Cohesion: 0.67
Nodes (3): ui_generalSimpleScreen_screen_destroy(), get_general_simple(), ui_open_general_simple_cb()

### Community 108 - "fresh_net_wifi"
Cohesion: 0.67
Nodes (3): ui_netWifiScreen_screen_destroy(), fresh_net_wifi(), ui_open_net_wifi_cb()

### Community 109 - "get_sensor_diag"
Cohesion: 0.67
Nodes (3): ui_sensorDiagScreen_screen_destroy(), get_sensor_diag(), ui_open_sensordiag_cb()

## Knowledge Gaps
- **149 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `Alarmas: silencio (1.6.7)`, `Consumo, totalizador y cortes de energía (1.6.8)` (+144 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **9 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `sim_backend.c`, `screen_init_task`, `cert_store.c`, `storage.c`, `fpm_ble_config.c`, `http_api.c`, `touch_io_bounded.c`, `eth_mgr.c`, `fpm_i2c_guard.c`, `nvs_open`, `sensors_acq_task`, `main/main.c`, `driver_delay_ms`, `at24c256.c`, `app_main`, `ads1115.c`, `ota_update.c`, `rtc_rv3028.c`, `sensor_cfg_persist`, `bmp280.c`, `on_network_ready`?**
  _High betweenness centrality (0.048) - this node is a cross-community bridge._
- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_err_to_name`, `test_alarm.c`, `alarm_mgr.c`, `ui_loginScreen.c`, `screen_init_task`, `storage.c`, `ui_edit_begin`, `fpm_ble_config.c`, `ui_label`, `ui_auth_can`, `http_api.c`, `eth_mgr.c`, `ui_cfg.c`, `ui_statusbar_controller.c`, `ui_connectivityScreen.c`, `gas_label_color`, `ui_mainScreen.c`, `xSemaphoreTake`, `app_main`, `ota_update.c`, `save_and_refresh`, `on_network_ready`, `next`?**
  _High betweenness centrality (0.042) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `esp_err_to_name`, `xSemaphoreTake`, `alarm_mgr.c`, `on_network_ready`, `next`, `sensors_acq_task`, `alarm_mgr_set_state_change_cb`, `main/main.c`, `esp_eth_mac_ksz8851snl.c`, `storage.c`, `fpm_ble_config.c`, `appcfg_cache_peek`, `alarm_mgr_init`, `sensors_runtime.c`, `esp_timer.h`, `mem_diag.c`, `sensor_cfg_persist`?**
  _High betweenness centrality (0.032) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 53 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 53 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 45 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 45 INFERRED edges - model-reasoned connections that need verification._