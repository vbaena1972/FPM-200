# Graph Report - FPM-200  (2026-10-15)

## Corpus Check
- 215 files · ~445,311 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1754 nodes · 4607 edges · 102 communities (96 shown, 6 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1181 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `3c18856a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- transport_ble.c
- esp_err_to_name
- test_alarm.c
- transport_mqtt.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- sim_backend.c
- esp_eth_mac_ksz8851snl.c
- screen_init_task
- ui_netCloudScreen.c
- ethernet_init.c
- ui_sensorDiagScreen.c
- fpm_ble_config.c
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_cfg.c
- stdint
- ui_userEditScreen.c
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
- lv_port_mem.c
- HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- ui_statusbar_controller.c
- state_pub.c
- ui_generalSimpleScreen.c
- ui_connectivityScreen.c
- sensors_acq_task
- ui_sd.c
- main/main.c
- apply_authenticated_cb
- config_mode.c
- gas_label_color
- ui_mainScreen.c
- README.md
- ms5803.c
- ft5x06.h
- gen_ota_signing_key.sh script
- alarm_mgr_init
- ui_sensorEditScreen.c
- Migración HMI: SquareLine → Claude Design
- esp_timer.h
- ui_bleAppScreen.c
- Changelog
- on_eth_event
- esp_eth_driver
- driver_delay_ms
- ui_wifi_main_icon.c
- string
- esp_eth_com
- ui_i18n
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
- ui_infoScreen.c
- test_eeprom.c
- sensors_runtime_init
- time_mgr.c
- sensor_cfg_persist
- bmp280.c
- ui_wifi_main_icon_init
- alarm_mgr_get_current_state
- save_and_refresh
- math
- next

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 78 edges
2. `ui_label()` - 62 edges
3. `esp_err_to_name()` - 52 edges
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

## Communities (102 total, 6 thin omitted)

### Community 0 - "transport_ble.c"
Cohesion: 0.09
Nodes (32): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, fpm_ble_config_set_clock_cb() (+24 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (27): esp_wifi_types, apply_ip_mode(), AppConfig, esp_err_t, esp_event_base_t, wifi_status_t, log_dns_info(), net_cfg_equals() (+19 more)

### Community 2 - "test_alarm.c"
Cohesion: 0.13
Nodes (16): alarm_mgr_press_mute_impl(), set_buzzer_acoustic(), tone_out(), alarm_tone_get(), alarm_tone_init(), alarm_tone_set(), appcfg_cache_get(), appcfg_cache_peek() (+8 more)

### Community 3 - "transport_mqtt.c"
Cohesion: 0.11
Nodes (25): esp_mqtt_event_handle_t, esp_reset_reason_t, add_channel(), AppConfig, cJSON, esp_event_base_t, sensor_signal_stats_t, lwt_prepare() (+17 more)

### Community 4 - "alarm_mgr.c"
Cohesion: 0.19
Nodes (22): alarm_state_change_cb_t, alarm_mgr_get_active_conditions(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute(), alarm_mgr_process() (+14 more)

### Community 5 - "ui.c"
Cohesion: 0.10
Nodes (53): menu_item_cb(), ui_mainScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), lv_event_t (+45 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.18
Nodes (21): app_user_role_t, app_user_t, lv_event_t, eye_set(), fac_close_cb(), fac_eye_cb(), fac_hide(), fac_kb_cb() (+13 more)

### Community 7 - "sim_backend.c"
Cohesion: 0.05
Nodes (82): cert_info_t, cert_kind_t, cfg_result_t, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all() (+74 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.10
Nodes (60): BaseType_t, emac_ksz8851snl_t, esp_cpu, esp_rom_crc, eth_ksz8851snl_config_t, eth_mac_config_t, esp_err_t, esp_eth_mac_t (+52 more)

### Community 9 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 10 - "ui_netCloudScreen.c"
Cohesion: 0.29
Nodes (8): cert_store, lv_event_t, cert_erase_cb(), cert_pills_refresh(), cert_presence(), save_cb(), set_str(), ui_netCloudScreen_screen_destroy()

### Community 11 - "ethernet_init.c"
Cohesion: 0.18
Nodes (17): esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, eth_phy_config_t, ethernet_init, esp_eth_phy_new_ksz8851snl(), esp_err_t (+9 more)

### Community 12 - "ui_sensorDiagScreen.c"
Cohesion: 0.20
Nodes (15): lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb(), ui_edit_target_t (+7 more)

### Community 13 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (35): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+27 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "net_core.c"
Cohesion: 0.19
Nodes (14): esp_eth, esp_event, esp_wifi, esp_event_base_t, on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services() (+6 more)

### Community 16 - "ui_label"
Cohesion: 0.08
Nodes (98): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+90 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (28): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+20 more)

### Community 18 - "ui_cfg.c"
Cohesion: 0.18
Nodes (23): app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_factory() (+15 more)

### Community 19 - "stdint"
Cohesion: 0.05
Nodes (30): assert, cjson, esp_heap_caps, esp_random, flow_integrator_push(), derive(), from_hex(), hmac() (+22 more)

### Community 20 - "ui_userEditScreen.c"
Cohesion: 0.17
Nodes (12): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy(), app_user_role_t (+4 more)

### Community 21 - "http_api.c"
Cohesion: 0.09
Nodes (35): esp_app_desc, esp_chip_info, esp_http_server, esp_err_t, mdns_svc_start(), add_channel(), alarms_get_handler(), cJSON (+27 more)

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
Cohesion: 0.17
Nodes (19): cdefs, esp_check, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t (+11 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.16
Nodes (17): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+9 more)

### Community 28 - "display_idle_cb"
Cohesion: 0.38
Nodes (7): bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 29 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (26): eth_phy_autoneg_cmd_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t, ksz8851_update_link_duplex_speed() (+18 more)

### Community 31 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (15): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+7 more)

### Community 32 - "lv_port_mem.c"
Cohesion: 0.15
Nodes (8): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t, sdkconfig

### Community 33 - "HANDOFF FPM-200 — sesiones del 2026-08 al 2026-10-08"
Cohesion: 0.08
Nodes (24): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, 4. Cómo compilar / gotchas de build, 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41` (+16 more)

### Community 34 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.13
Nodes (24): conn_status_t, eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual() (+16 more)

### Community 36 - "state_pub.c"
Cohesion: 0.29
Nodes (3): device_state_t, state_get(), state_pub

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.21
Nodes (11): lv_event_t, general_loaded_cb(), protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild() (+3 more)

### Community 38 - "ui_connectivityScreen.c"
Cohesion: 0.17
Nodes (16): lv_timer_t, conn_signature(), live_tick(), rssi_pct(), str_hash(), ui_connectivityScreen_refresh(), ui_connectivityScreen_screen_destroy(), lv_event_t (+8 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.18
Nodes (17): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+9 more)

### Community 40 - "ui_sd.c"
Cohesion: 0.33
Nodes (3): lv_event_t, close_click_cb(), ui_sd_close()

### Community 41 - "main/main.c"
Cohesion: 0.07
Nodes (30): dirent, errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_task_wdt, esp_trace, esp_trace_open_params_t (+22 more)

### Community 42 - "apply_authenticated_cb"
Cohesion: 0.22
Nodes (9): apply_authenticated_cb(), lv_event_t, accept_cb(), lv_event_t, ui_sensorDiagScreen_refresh(), limit_step(), ui_edit_apply(), ui_edit_is_audio() (+1 more)

### Community 43 - "config_mode.c"
Cohesion: 0.19
Nodes (10): transport_ble_start_adv(), esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb(), lv_event_t (+2 more)

### Community 44 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 45 - "ui_mainScreen.c"
Cohesion: 0.06
Nodes (74): alarm_mgr, bsp_touch_config_t, card_state_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new() (+66 more)

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
Cohesion: 0.21
Nodes (21): lv_obj_t, ui_edit_target_t, card(), dec_cb(), dec_seg(), flow_card(), mini(), pressure_card() (+13 more)

### Community 54 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 55 - "esp_timer.h"
Cohesion: 0.24
Nodes (7): eTaskState, TaskHandle_t, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), task_tracer

### Community 56 - "ui_bleAppScreen.c"
Cohesion: 0.20
Nodes (10): config_mode, bleapp_goroot_async(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t (+2 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "on_eth_event"
Cohesion: 0.33
Nodes (6): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_netif_t, wifi_mgr_get_netif()

### Community 60 - "driver_delay_ms"
Cohesion: 0.28
Nodes (14): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+6 more)

### Community 61 - "ui_wifi_main_icon.c"
Cohesion: 0.36
Nodes (7): apply_icon(), lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), ui_statusbar_controller

### Community 62 - "string"
Cohesion: 0.10
Nodes (15): esp_system, eth_mgr, set_str(), ui_netBleScreen_screen_destroy(), lv_event_t, save_cb(), set_str(), ui_netEthScreen_screen_destroy() (+7 more)

### Community 63 - "esp_eth_com"
Cohesion: 0.40
Nodes (3): esp_eth_com, esp_eth_mac_spi, esp_eth_phy

### Community 64 - "ui_i18n"
Cohesion: 0.14
Nodes (13): ui_datetimeScreen_screen_destroy(), key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots() (+5 more)

### Community 66 - "lvgl"
Cohesion: 0.10
Nodes (6): ui_confirmScreen_screen_destroy(), lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), lvgl

### Community 67 - "xSemaphoreTake"
Cohesion: 0.20
Nodes (15): alarm_tone_get(), alarm_tone_set(), apply_step_locked(), alarm_tone_prio_t, timer_cb(), aws_telemetry_task(), publish_alarm_transition(), shadow_apply_pending() (+7 more)

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
Cohesion: 0.18
Nodes (12): mem_diag_start_periodic(), fpm_i2c_init(), esp_err_t, net_core_init(), transport_mqtt_runtime_init(), i2c_master_bus_handle_t, sensors_runtime_set_bus(), task_tracer_start() (+4 more)

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
Cohesion: 0.11
Nodes (23): appcfg_cache_peek(), appcfg_save(), f_switch(), lv_color_t, lv_obj_t, ui_auth_user_count(), ui_cfg_alarm_tone(), ui_cfg_apply_visual_mode() (+15 more)

### Community 88 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), task_state_str(), mem_diag

### Community 89 - "ads1115.c"
Cohesion: 0.38
Nodes (11): ads1115, ads1115_channel_t, ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover() (+3 more)

### Community 90 - "ui_infoScreen.c"
Cohesion: 0.08
Nodes (42): esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE, cJSON, esp_err_t, ota_manifest_t (+34 more)

### Community 91 - "test_eeprom.c"
Cohesion: 0.26
Nodes (11): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, TickType_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device() (+3 more)

### Community 92 - "sensors_runtime_init"
Cohesion: 0.50
Nodes (4): ads1115_is_ready(), ms5803_is_ready(), AppConfig, sensors_runtime_init()

### Community 93 - "time_mgr.c"
Cohesion: 0.14
Nodes (17): esp_netif_sntp, esp_sntp, bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init() (+9 more)

### Community 94 - "sensor_cfg_persist"
Cohesion: 0.48
Nodes (7): esp_err_t, crc16_ccitt(), sensor_cfg_persist(), sensors_runtime_cal_flow_point(), sensors_runtime_set_pressure_gauge(), sensors_runtime_tare_flow(), sensors_runtime_tare_pressure()

### Community 95 - "bmp280.c"
Cohesion: 0.33
Nodes (10): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+2 more)

### Community 96 - "ui_wifi_main_icon_init"
Cohesion: 0.50
Nodes (4): wifi_mgr_set_status_cb(), lv_obj_t, ui_wifi_main_icon_init(), wifi_status_cb_t

### Community 97 - "alarm_mgr_get_current_state"
Cohesion: 1.00
Nodes (3): alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_clinical_state_t

### Community 99 - "save_and_refresh"
Cohesion: 0.20
Nodes (15): lv_event_t, gas_cb(), is_press_unit(), limit_edit(), p_switch(), unit_cb(), AppConfig, save_and_refresh() (+7 more)

### Community 101 - "math"
Cohesion: 0.46
Nodes (6): mm24_t, mm24_get(), mm24_merge(), mm24_push(), main(), math

### Community 102 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

## Knowledge Gaps
- **149 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `Alarmas: silencio (1.6.7)`, `Consumo, totalizador y cortes de energía (1.6.8)` (+144 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **6 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `transport_ble.c`, `sim_backend.c`, `screen_init_task`, `fpm_ble_config.c`, `http_api.c`, `test_flow_meter.c`, `eth_mgr.c`, `fpm_i2c_guard.c`, `sensors_acq_task`, `main/main.c`, `ui_mainScreen.c`, `ms5803.c`, `driver_delay_ms`, `app_main`, `ads1115.c`, `ui_infoScreen.c`, `time_mgr.c`, `sensor_cfg_persist`, `bmp280.c`?**
  _High betweenness centrality (0.051) - this node is a cross-community bridge._
- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `transport_ble.c`, `test_alarm.c`, `alarm_mgr.c`, `sim_backend.c`, `screen_init_task`, `ui_netCloudScreen.c`, `ui_sensorDiagScreen.c`, `fpm_ble_config.c`, `ui_label`, `ui_cfg.c`, `http_api.c`, `display_idle_cb`, `ui_statusbar_controller.c`, `apply_authenticated_cb`, `gas_label_color`, `ui_mainScreen.c`, `ui_sensorEditScreen.c`, `string`, `xSemaphoreTake`, `app_main`, `ui_infoScreen.c`, `save_and_refresh`, `next`?**
  _High betweenness centrality (0.043) - this node is a cross-community bridge._
- **Why does `app_main()` connect `app_main` to `transport_ble.c`, `esp_err_to_name`, `alarm_mgr.c`, `next`, `sim_backend.c`, `sensors_acq_task`, `main/main.c`, `esp_eth_mac_ksz8851snl.c`, `appcfg_cache_peek`, `alarm_mgr_init`, `http_api.c`, `sensors_runtime.c`, `test_flow_meter.c`, `mem_diag.c`, `on_eth_event`, `sensors_runtime_init`?**
  _High betweenness centrality (0.034) - this node is a cross-community bridge._
- **Are the 75 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 75 INFERRED edges - model-reasoned connections that need verification._
- **Are the 53 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 53 INFERRED edges - model-reasoned connections that need verification._
- **Are the 50 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 50 INFERRED edges - model-reasoned connections that need verification._
- **Are the 45 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 45 INFERRED edges - model-reasoned connections that need verification._