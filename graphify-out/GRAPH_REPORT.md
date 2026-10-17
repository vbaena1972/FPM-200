# Graph Report - FPM-200  (2026-10-16)

## Corpus Check
- 215 files · ~447,800 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1773 nodes · 4669 edges · 101 communities (95 shown, 6 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1194 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `0b0c4be7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- transport_ble.c
- esp_err_to_name
- test_alarm.c
- transport_mqtt.c
- alarm_mgr.c
- fresh_login
- ui_loginScreen.c
- sim_backend.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- ui.c
- fpm_ble_config.c
- ui_sensorDiagScreen.c
- string
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_auth_can
- stdint
- ui_open_useredit_cb
- http_api.c
- sensors_runtime.c
- test_flow_meter.c
- Handoff — FPM-200 (firmware + HMI)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- display_idle_cb
- Consumo y endurecimiento 1.5.5-dev
- esp_eth_phy_ksz8851snl.c
- fpm_i2c_guard.c
- xSemaphoreTake
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- esp_timer.h
- sha256_update
- ui_i18n
- ui_nav_load
- sensors_acq_task
- ui_infoScreen.c
- main/main.c
- ui_connectivityScreen.c
- ui_pin_hash.c
- gas_label_color
- ui_mainScreen.c
- README.md
- ms5803.c
- ft5x06.h
- gen_ota_signing_key.sh script
- alarm_mgr_init
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- ui_edit_begin
- ui_bleAppScreen.c
- Changelog
- math
- esp_eth_driver
- driver_delay_ms
- ui
- stdio
- ui_netCloudScreen.c
- app_main
- lvgl
- ws_stream.c
- PUBLICAR_FIRMWARE.md
- Simulador de PC — HMI FPM-200
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- sensor_cfg_persist
- AGENTS.md
- CLAUDE.md
- ui_userEditScreen.c
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- ui_cfg.c
- README_PARCHE.md
- mem_diag.c
- ads1115.c
- ota_update.c
- test_eeprom.c
- esp_lcd_st7796.h
- time_mgr.c
- ui_cfg_apply_visual_mode
- fpm_i2c_guard
- fresh_pin
- alarm_mgr_set_state_change_cb
- alarm_mgr_get_current_state
- get_users

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 80 edges
2. `ui_label()` - 62 edges
3. `esp_err_to_name()` - 53 edges
4. `ui_col()` - 47 edges
5. `_t()` - 46 edges
6. `ui_box()` - 43 edges
7. `ui_card()` - 38 edges
8. `appcfg_save()` - 36 edges
9. `ui_icon()` - 33 edges
10. `app_main()` - 26 edges

## Surprising Connections (you probably didn't know these)
- `open_login_cb()` --calls--> `ui_open_login_cb()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/ui.c
- `open_sensor_cb()` --calls--> `ui_open_sensor_cb()`  [INFERRED]
  Simulator/main.c → firmware/main/ui/ui.c
- `cert_store_erase()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/cert_store/cert_store.c → Simulator/sim_backend.c
- `cert_store_import_from_sd()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/cert_store/cert_store.c → Simulator/sim_backend.c
- `bsp_display_indev_init()` --calls--> `esp_err_to_name()`  [INFERRED]
  firmware/components/display/ft5x06.c → Simulator/sim_backend.c

## Import Cycles
- None detected.

## Communities (101 total, 6 thin omitted)

### Community 0 - "transport_ble.c"
Cohesion: 0.06
Nodes (49): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, fpm_ble_config_set_clock_cb() (+41 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.10
Nodes (35): esp_task_wdt, esp_wifi_types, mem_diag_report(), eth_mgr_is_up(), apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t (+27 more)

### Community 2 - "test_alarm.c"
Cohesion: 0.11
Nodes (16): assert, set_buzzer_acoustic(), tone_out(), alarm_tone_get(), alarm_tone_init(), alarm_tone_set(), appcfg_cache_get(), appcfg_cache_peek() (+8 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.11
Nodes (26): esp_mqtt_event_handle_t, esp_reset_reason_t, add_channel(), AppConfig, cJSON, esp_event_base_t, sensor_signal_stats_t, lwt_prepare() (+18 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.18
Nodes (20): alarm_mgr_get_active_conditions(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute(), alarm_mgr_press_mute_impl(), alarm_mgr_process() (+12 more)

### Community 5 - "fresh_login"
Cohesion: 0.22
Nodes (10): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb() (+2 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.20
Nodes (20): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+12 more)

### Community 7 - "sim_backend.c"
Cohesion: 0.05
Nodes (82): cert_info_t, cert_kind_t, cfg_result_t, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all() (+74 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.11
Nodes (56): emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t, esp_eth_mediator_t (+48 more)

### Community 9 - "screen_init_task"
Cohesion: 0.18
Nodes (20): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+12 more)

### Community 10 - "ui.c"
Cohesion: 0.09
Nodes (40): ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_generalSimpleScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy() (+32 more)

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.16
Nodes (19): accept_cb(), lv_event_t, lv_event_t, lv_timer_t, edit_cb(), refresh_values(), save_flush(), save_timer_cb() (+11 more)

### Community 13 - "string"
Cohesion: 0.24
Nodes (7): key_cb(), refresh_value(), close_to(), main(), stdlib, string, tz_map

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "net_core.c"
Cohesion: 0.10
Nodes (21): device_state_t, esp_eth, esp_event, esp_wifi, esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t (+13 more)

### Community 16 - "ui_label"
Cohesion: 0.07
Nodes (113): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+105 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (28): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+20 more)

### Community 18 - "ui_auth_can"
Cohesion: 0.35
Nodes (11): app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_max_assignable(), ui_auth_role() (+3 more)

### Community 19 - "stdint"
Cohesion: 0.09
Nodes (7): esp_heap_caps, esp_netif, flow_integrator_push(), flow_integrator_t, stdbool, stddef, stdint

### Community 20 - "ui_open_useredit_cb"
Cohesion: 0.39
Nodes (8): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), fresh_useredit(), ui_open_useredit_cb()

### Community 21 - "http_api.c"
Cohesion: 0.23
Nodes (21): esp_chip_info, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t, cfg_get_handler() (+13 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.17
Nodes (25): mm24_t, sensor_sample_t, sensor_signal_stats_t, lock(), mm24_clock_ms(), sensors_runtime_flow_warming(), sensors_runtime_flow_warming_elapsed_ms(), sensors_runtime_get_ages() (+17 more)

### Community 23 - "test_flow_meter.c"
Cohesion: 0.12
Nodes (22): esp_err_t, flow_meter_boot_checkpoint(), flow_meter_init(), flow_meter_record(), flow_meter_service(), mm24_service(), appmetrics_load(), appmetrics_load_blob() (+14 more)

### Community 24 - "Handoff — FPM-200 (firmware + HMI)"
Cohesion: 0.14
Nodes (13): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (cierre) — 1.5.34 probado; FW 1.5.35-dev (sin compilar), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2026-10-12 — FW 1.5.37-dev: OTA por RED (sin compilar), 2026-10-12 — FW 1.6.0-dev: buzzer IEC 60601-1-8 (sin compilar), 2026-10-12 — FW 1.6.2-dev: corrección del buzzer continuo (sin compilar), 2026-10-12 noche — FW 1.6.3-dev en HW, soak de ~12 h en curso (+5 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.20
Nodes (9): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4.1 Lecciones del ensayo en placa de desarrollo (2026-10-14), 4.2 Requisitos de producción pendientes (no se hacen en V1), 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip) (+1 more)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.06
Nodes (42): bsp_touch_config_t, cdefs, esp_compiler, esp_lcd_panel_io, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface (+34 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.15
Nodes (19): esp_eth_netif_glue, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init() (+11 more)

### Community 28 - "display_idle_cb"
Cohesion: 0.38
Nodes (7): bright_cb(), clamp_brightness(), ui_auth_logout(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.07
Nodes (48): esp_check, esp_eth_com, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_mac_spi, esp_eth_phy, esp_eth_phy_ksz8851snl, esp_mac (+40 more)

### Community 31 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 32 - "xSemaphoreTake"
Cohesion: 0.16
Nodes (20): alarm_tone_get(), alarm_tone_set(), apply_step_locked(), alarm_tone_prio_t, timer_cb(), fpm_i2c_init(), aws_telemetry_task(), publish_alarm_transition() (+12 more)

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "esp_timer.h"
Cohesion: 0.21
Nodes (8): eTaskState, TaskHandle_t, state_str(), task_tracer_dump_now(), task_tracer_start(), trace_worker(), track_get_and_update(), task_tracer

### Community 36 - "sha256_update"
Cohesion: 0.47
Nodes (9): hmac(), hmac_key_init(), sha256_block(), sha256_final(), sha256_init(), sha256_update(), ui_pbkdf2_sha256(), hmac_key_t (+1 more)

### Community 37 - "ui_i18n"
Cohesion: 0.20
Nodes (10): lv_event_t, protected_back_cb(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb() (+2 more)

### Community 38 - "ui_nav_load"
Cohesion: 0.19
Nodes (13): bleapp_goroot_async(), ui_generalScreen_screen_destroy(), get_general(), lv_event_t, lv_obj_t, ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init() (+5 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_infoScreen.c"
Cohesion: 0.33
Nodes (10): lv_event_t, factory_btn_cb(), ota_allowed(), ota_channel_cb(), ota_check_cb(), ota_install_cb(), ota_screen_del_cb(), ota_start() (+2 more)

### Community 41 - "main/main.c"
Cohesion: 0.06
Nodes (38): errno, esp_app_desc, esp_app_trace, esp_core_dump, esp_heap_trace, esp_trace, esp_trace_open_params_t, esp_vfs_fat (+30 more)

### Community 42 - "ui_connectivityScreen.c"
Cohesion: 0.23
Nodes (12): conn_status_t, cloud_mgr_connected(), transport_ble_is_connected(), lv_timer_t, conn_signature(), cpy(), gather(), live_tick() (+4 more)

### Community 43 - "ui_pin_hash.c"
Cohesion: 0.33
Nodes (9): esp_random, derive(), from_hex(), random_salt(), to_hex(), ui_pin_hash(), ui_pin_is_hashed(), ui_pin_normalize() (+1 more)

### Community 44 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 45 - "ui_mainScreen.c"
Cohesion: 0.07
Nodes (60): alarm_mgr, card_state_t, ui_statusbar_set_enabled(), ui_refresh_task(), alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_refresh(), alarm_overlay_silence_cb() (+52 more)

### Community 47 - "ms5803.c"
Cohesion: 0.38
Nodes (11): esp_err_t, i2c_master_bus_handle_t, ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry() (+3 more)

### Community 49 - "ft5x06.h"
Cohesion: 0.29
Nodes (4): esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port

### Community 51 - "alarm_mgr_init"
Cohesion: 0.22
Nodes (9): alarm_mgr_init(), esp_err_t, alarm_tone_init(), alarm_tone_out_t, esp_err_t, ledc_channel_config(), ledc_timer_config(), ledc_channel_config_t (+1 more)

### Community 52 - "ui_sensorEditScreen.c"
Cohesion: 0.27
Nodes (17): lv_event_t, dec_cb(), f_switch(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch() (+9 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "ui_edit_begin"
Cohesion: 0.12
Nodes (25): apply_authenticated_cb(), lv_event_t, general_loaded_cb(), allow_decimal(), value_text(), AppConfig, ui_edit_target_t, edit_store() (+17 more)

### Community 56 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (8): config_mode, bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t, ui_bleAppScreen_screen_destroy()

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "math"
Cohesion: 0.46
Nodes (6): mm24_t, mm24_get(), mm24_merge(), mm24_push(), main(), math

### Community 60 - "driver_delay_ms"
Cohesion: 0.28
Nodes (14): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+6 more)

### Community 61 - "ui"
Cohesion: 0.20
Nodes (11): apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), lv_event_t, pin_key_cb() (+3 more)

### Community 62 - "stdio"
Cohesion: 0.14
Nodes (10): eth_mgr, lv_event_t, save_cb(), set_str(), set_str(), stdio, storage, ui_nav (+2 more)

### Community 63 - "ui_netCloudScreen.c"
Cohesion: 0.29
Nodes (8): cert_store, lv_event_t, cert_erase_cb(), cert_pills_refresh(), cert_presence(), save_cb(), set_str(), ui_form

### Community 64 - "app_main"
Cohesion: 0.17
Nodes (13): BaseType_t, ads1115_is_ready(), ms5803_is_ready(), esp_err_t, net_core_init(), AppConfig, i2c_master_bus_handle_t, sensors_runtime_init() (+5 more)

### Community 66 - "lvgl"
Cohesion: 0.08
Nodes (8): app_user_role_t, role_color(), role_name(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), lvgl

### Community 67 - "ws_stream.c"
Cohesion: 0.19
Nodes (10): cjson, esp_http_server, esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast() (+2 more)

### Community 69 - "Simulador de PC — HMI FPM-200"
Cohesion: 0.22
Nodes (7): 1. Compilar, 2. Correr, Simulador FPM-200 — instrucciones rápidas, Compilar y correr, Cómo está armado, Notas, Simulador de PC — HMI FPM-200

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 74 - "CLAUDE.md"
Cohesion: 0.40
Nodes (4): Alarmas: silencio (1.6.7), Consumo, totalizador y cortes de energía (1.6.8), Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard), graphify

### Community 77 - "ui_userEditScreen.c"
Cohesion: 0.43
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_nav_back()

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 81 - "ui_cfg.c"
Cohesion: 0.12
Nodes (40): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), lv_event_t, next(), app_user_t, save_and_refresh(), set_str() (+32 more)

### Community 88 - "mem_diag.c"
Cohesion: 0.21
Nodes (9): esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str() (+1 more)

### Community 89 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 90 - "ota_update.c"
Cohesion: 0.11
Nodes (30): dirent, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE, cJSON, esp_err_t (+22 more)

### Community 91 - "test_eeprom.c"
Cohesion: 0.26
Nodes (11): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, TickType_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device() (+3 more)

### Community 92 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 93 - "time_mgr.c"
Cohesion: 0.13
Nodes (18): esp_netif_sntp, esp_sntp, bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init() (+10 more)

### Community 94 - "ui_cfg_apply_visual_mode"
Cohesion: 0.40
Nodes (5): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_theme(), visual_color()

### Community 95 - "fpm_i2c_guard"
Cohesion: 0.29
Nodes (11): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+3 more)

### Community 96 - "fresh_pin"
Cohesion: 0.50
Nodes (4): ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), fresh_pin(), ui_open_pin_cb()

### Community 98 - "alarm_mgr_set_state_change_cb"
Cohesion: 1.00
Nodes (3): alarm_state_change_cb_t, alarm_mgr_set_state_change_cb(), alarm_mgr_set_state_change_cb_impl()

### Community 99 - "alarm_mgr_get_current_state"
Cohesion: 1.00
Nodes (3): alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_clinical_state_t

### Community 100 - "get_users"
Cohesion: 0.67
Nodes (3): ui_usersScreen_screen_destroy(), get_users(), ui_open_users_cb()

## Knowledge Gaps
- **149 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `Alarmas: silencio (1.6.7)`, `Consumo, totalizador y cortes de energía (1.6.8)` (+144 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `ui_cfg.c` to `transport_ble.c`, `esp_err_to_name`, `transport_mqtt.c`, `alarm_mgr.c`, `sim_backend.c`, `screen_init_task`, `fpm_ble_config.c`, `ui_sensorDiagScreen.c`, `ui_label`, `ui_auth_can`, `http_api.c`, `eth_mgr.c`, `display_idle_cb`, `xSemaphoreTake`, `main/main.c`, `ui_connectivityScreen.c`, `gas_label_color`, `ui_mainScreen.c`, `ui_sensorEditScreen.c`, `ui_edit_begin`, `stdio`, `ui_netCloudScreen.c`, `app_main`, `ui_cfg_apply_visual_mode`?**
  _High betweenness centrality (0.057) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `transport_ble.c`, `transport_mqtt.c`, `sim_backend.c`, `screen_init_task`, `fpm_ble_config.c`, `http_api.c`, `test_flow_meter.c`, `touch_io_bounded.c`, `eth_mgr.c`, `fpm_i2c_guard.c`, `sensors_acq_task`, `main/main.c`, `ms5803.c`, `driver_delay_ms`, `app_main`, `sensor_cfg_persist`, `ads1115.c`, `ota_update.c`, `time_mgr.c`, `fpm_i2c_guard`?**
  _High betweenness centrality (0.041) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `xSemaphoreTake`, `esp_err_to_name`, `alarm_mgr_set_state_change_cb`, `esp_timer.h`, `alarm_mgr.c`, `sim_backend.c`, `sensors_acq_task`, `main/main.c`, `esp_eth_mac_ksz8851snl.c`, `ui_cfg.c`, `alarm_mgr_init`, `sensors_runtime.c`, `test_flow_meter.c`, `mem_diag.c`?**
  _High betweenness centrality (0.033) - this node is a cross-community bridge._
- **Are the 77 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 77 INFERRED edges - model-reasoned connections that need verification._
- **Are the 53 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 53 INFERRED edges - model-reasoned connections that need verification._
- **Are the 51 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 51 INFERRED edges - model-reasoned connections that need verification._
- **Are the 45 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 45 INFERRED edges - model-reasoned connections that need verification._