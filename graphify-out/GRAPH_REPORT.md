# Graph Report - FPM-200  (2026-10-10)

## Corpus Check
- 185 files · ~357,660 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 10 file(s) not represented in the graph (top: (none) 5, .projbuild 1, .lock 1)

## Summary
- 1534 nodes · 4256 edges · 91 communities (80 shown, 11 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1101 edges (avg confidence: 0.86)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `7073f567`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- stdio
- esp_err_to_name
- transport_ble.c
- lv_port_mem.c
- alarm_mgr.c
- ui_wifi_main_icon.c
- save_cb
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- FreeRTOS.h
- fpm_ble_config.c
- ui_netBleScreen.c
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- cert_store.c
- ui_sensorEditScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- 9. Estado de pendientes (actualizado 2026-07-31)
- ui_nav.c
- esp_eth_phy_ksz8851snl.c
- ui_pin_hash.c
- ethernet_init.c
- screen_init_task
- ui_statusbar_controller.c
- ui_cfg.c
- Hecho en la sesión 2026-07-22
- ui_generalSimpleScreen.c
- sensors_acq_task
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- string
- ui_loginScreen.c
- README.md
- transport_mqtt.c
- fpm_i2c_guard
- gen_ota_signing_key.sh script
- ft5x06.c
- ui_open_useredit_cb
- gas_label_color
- task_tracer.c
- ui_pinScreen.c
- Changelog
- mem_diag.c
- ui_cfg_flow_unit
- ui_bleAppScreen.c
- ui.c
- lv_obj_t
- Consumo y endurecimiento 1.5.5-dev
- lvgl
- driver_delay_ms
- save_cb
- flow_meter.c
- esp_eth_mac_new_ksz8851snl
- config_mode.c
- ui_netCloudScreen.c
- AGENTS.md
- CLAUDE.md
- build_channel_flat
- lv_event_t
- ui_nav_load
- save_and_refresh
- on_eth_event
- ui_userEditScreen.c
- ui_cfg_apply_visual_mode
- bleapp_off_cb
- get_general
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- ui_open_confirm_cb
- get_datetime
- get_general_simple
- fresh_net_cloud
- get_users
- ota_update.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 57 edges
3. `esp_err_to_name()` - 45 edges
4. `ui_col()` - 44 edges
5. `ui_box()` - 42 edges
6. `_t()` - 36 edges
7. `appcfg_save()` - 35 edges
8. `ui_card()` - 34 edges
9. `ui_icon()` - 31 edges
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 27 edges

## Surprising Connections (you probably didn't know these)
- `Fase 1 — Esqueleto + limpieza` --references--> `ui_init()`  [INFERRED]
  docs/HMI_MIGRATION.md → firmware/main/ui/ui.c
- `Hecho en la sesión 2026-07-22` --references--> `cert_store_info()`  [INFERRED]
  docs/SESION_HMI.md → firmware/components/cert_store/cert_store.c
- `Hecho en la sesión 2026-07-22` --references--> `cert_store_erase_all()`  [INFERRED]
  docs/SESION_HMI.md → firmware/components/cert_store/cert_store.c
- `Actualización 2026-09-21 — microSD, AWS por SD, BT-off y splash (FW 1.5.4-dev)` --references--> `cert_store_import_from_sd()`  [INFERRED]
  docs/HANDOFF.md → firmware/components/cert_store/cert_store.c
- `Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW` --references--> `bsp_display_lock()`  [INFERRED]
  docs/SESION_HMI.md → firmware/components/display/st7796.c

## Import Cycles
- None detected.

## Communities (91 total, 11 thin omitted)

### Community 0 - "stdio"
Cohesion: 0.17
Nodes (3): set_str(), close_to(), main()

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (18): 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41`, mdns_svc_start(), apply_ip_mode(), net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache() (+10 more)

### Community 2 - "transport_ble.c"
Cohesion: 0.12
Nodes (6): append_json(), ble_on_sync(), ble_start_adv(), free_config_buffer(), gap_event(), transport_ble_set_cmd_handler()

### Community 3 - "lv_port_mem.c"
Cohesion: 0.15
Nodes (4): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core()

### Community 4 - "alarm_mgr.c"
Cohesion: 0.07
Nodes (42): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+34 more)

### Community 5 - "ui_wifi_main_icon.c"
Cohesion: 0.21
Nodes (6): wifi_mgr_set_status_cb(), apply_icon(), timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars(), wifi_status_cb()

### Community 6 - "save_cb"
Cohesion: 0.31
Nodes (10): fpm_ble_config_set_clock_cb(), transport_ble_init(), transport_ble_set_enabled(), transport_ble_set_name(), transport_ble_set_passkey_mode(), transport_ble_set_tx_power(), transport_ble_start_adv(), transport_ble_stop_adv() (+2 more)

### Community 7 - "ms5803.c"
Cohesion: 0.38
Nodes (8): ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry(), ms5803_send_cmd(), ms5803_try_addr()

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.10
Nodes (41): ksz8851snl 1.2.0 — copia local PARCHEADA (MedGuard / FPM-200), emac_ksz8851_add_mac_filter(), emac_ksz8851_alloc_recv_buf(), emac_ksz8851_deinit(), emac_ksz8851_del(), emac_ksz8851_enable_flow_ctrl(), emac_ksz8851_flush_recv_queue(), emac_ksz8851_get_addr() (+33 more)

### Community 9 - "ads1115.c"
Cohesion: 0.38
Nodes (7): ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg()

### Community 10 - "FreeRTOS.h"
Cohesion: 0.25
Nodes (7): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg()

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.20
Nodes (28): 6. Cambios aplicados en el firmware (esta tanda), 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, apply_channel_obj(), apply_netif(), cfg_dup(), color_to_gas(), cpy(), flow_from_disp() (+20 more)

### Community 12 - "ui_netBleScreen.c"
Cohesion: 0.19
Nodes (4): set_str(), edit_cb(), rebuild(), test_cb()

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.18
Nodes (10): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`), 8. Cómo compilar (+2 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.12
Nodes (16): 1. GATT, 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig`, bluetooth ↔ `bt` / `BluetoothConfig`, channels ↔ `sensors` / `ChannelConfig`/`BleChannelConfig` (+8 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.13
Nodes (6): on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), time_mgr_start_sntp(), transport_mqtt_on_net_down()

### Community 16 - "ui_label"
Cohesion: 0.05
Nodes (103): 3. Arquitectura de la nueva HMI (`main/ui/`), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), conn_card(), ui_connectivityScreen_screen_init(), row(), ui_datetimeScreen_screen_init() (+95 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "cert_store.c"
Cohesion: 0.11
Nodes (24): cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load(), cert_store_write(), key_for_kind(), read_file_to_buf() (+16 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (20): card(), dec_cb(), dec_seg(), flow_card(), gas_cb(), is_press_unit(), limit_edit(), limit_step() (+12 more)

### Community 20 - "storage.c"
Cohesion: 0.24
Nodes (17): 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir, Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+9 more)

### Community 21 - "http_api.c"
Cohesion: 0.12
Nodes (19): add_channel(), alarms_get_handler(), cfg_get_handler(), cfg_put_handler(), http_api_init(), http_api_start(), http_api__start_locked(), http_bearer_ok() (+11 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.10
Nodes (33): Actualización 2026-08-18 (sensores reales — se elimina la simulación), ads1115_is_ready(), fpm_i2c_init(), ms5803_is_ready(), aws_telemetry_task(), flow_meter_get(), flow_meter_init(), crc16_ccitt() (+25 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (4): fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.11
Nodes (18): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 4. Cómo compilar / gotchas de build, Actualización 2026-08-09 (estabilización en HW + app), Actualización 2026-08-21 — EEPROM persiste + recalibración FS7 estable, Actualización 2026-09-24 — EEPROM, consumo, cero de presión (FW 1.5.5 → 1.5.22-dev) (+10 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.20
Nodes (8): fpm_new_panel_io_i2c(), panel_io_i2c_del(), panel_io_i2c_register_event_callbacks(), panel_io_i2c_rx_buffer(), panel_io_i2c_rx_param(), panel_io_i2c_tx_buffer(), panel_io_i2c_tx_color(), panel_io_i2c_tx_param()

### Community 27 - "eth_mgr.c"
Cohesion: 0.14
Nodes (10): appcfg_to_eth_view(), apply_ip_config(), eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start(), eth_mgr_stop(), eth_view_from_appcfg(), eth_init_task() (+2 more)

### Community 28 - "9. Estado de pendientes (actualizado 2026-07-31)"
Cohesion: 0.25
Nodes (7): 9. Estado de pendientes (actualizado 2026-07-31), Aún pendiente, Cierre final (misma sesión 2026-07-22), Sesión 2026-07-30 (previa, ya commiteada en `c03ba34`), net_core_init(), appmetrics_service_min(), transport_ble_set_mesh_enabled()

### Community 29 - "ui_nav.c"
Cohesion: 0.18
Nodes (11): 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), apply_authenticated_cb(), ui_connectivityScreen_refresh(), ui_sensorDiagScreen_refresh(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to() (+3 more)

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (17): ksz8851_update_link_duplex_speed(), phy_ksz8851_advertise_pause_ability(), phy_ksz8851_autonego_ctrl(), phy_ksz8851_deinit(), phy_ksz8851_del(), phy_ksz8851_get_addr(), phy_ksz8851_get_link(), phy_ksz8851_init() (+9 more)

### Community 31 - "ui_pin_hash.c"
Cohesion: 0.24
Nodes (15): derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final(), sha256_init() (+7 more)

### Community 33 - "ethernet_init.c"
Cohesion: 0.19
Nodes (5): eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init(), spi_bus_init()

### Community 34 - "screen_init_task"
Cohesion: 0.24
Nodes (12): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+4 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.28
Nodes (9): transport_ble_is_advertising(), activity_timer_cb(), apply_cloud_visual(), set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_controller_init(), ui_statusbar_request_refresh() (+1 more)

### Community 36 - "ui_cfg.c"
Cohesion: 0.13
Nodes (28): Sesión 2026-07-31 (ESTA sesión — NO commiteada aún; sin compilar en este entorno, sin HW), appcfg_save(), next(), ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user() (+20 more)

### Community 37 - "Hecho en la sesión 2026-07-22"
Cohesion: 0.24
Nodes (10): Hecho en la sesión 2026-07-22, general_loaded_cb(), protected_back_cb(), bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness() (+2 more)

### Community 38 - "ui_generalSimpleScreen.c"
Cohesion: 0.24
Nodes (5): dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 39 - "sensors_acq_task"
Cohesion: 0.14
Nodes (16): Actualización 2026-08-18 (tarde) — SFM leyendo, robustez I²C, calibración FS7 · **v1.4.0**, Actualización 2026-08-20 — BMP280 (ref. atmosférica), cero FS7, debug en dashboard, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.27
Nodes (7): cloud_mgr_connected(), conn_signature(), cpy(), gather(), live_tick(), rssi_pct(), str_hash()

### Community 41 - "main.c"
Cohesion: 0.07
Nodes (15): Actualización 2026-09-21 — microSD, AWS por SD, BT-off y splash (FW 1.5.4-dev), ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), bsp_i2c_init(), esp_trace_get_user_params(), mount_sdcard_hotplug(), restart_btn_event_cb(), sd_flow_clear() (+7 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.13
Nodes (21): appcfg_cache_peek(), step_cb(), volume_cb(), f_switch(), ui_auth_user_count(), ui_cfg_alarm_tone(), ui_cfg_alarm_volume(), ui_cfg_check_pin() (+13 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.38
Nodes (10): fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit(), fpm_i2c_receive(), fpm_i2c_stop_read(), fpm_i2c_transmit(), fpm_i2c_transmit_receive() (+2 more)

### Community 45 - "ui_loginScreen.c"
Cohesion: 0.29
Nodes (5): refresh(), role_color(), role_name(), select_user(), user_cb()

### Community 47 - "transport_mqtt.c"
Cohesion: 0.10
Nodes (17): add_channel(), mqtt_event(), mqtt_start_locked(), mqtt_stop_locked(), prepare_pem(), publish_alarm_transition(), shadow_apply_pending(), shadow_rx_chunk() (+9 more)

### Community 49 - "fpm_i2c_guard"
Cohesion: 0.21
Nodes (8): bcd2dec(), dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init(), time_mgr_set_datetime(), on_ble_set_clock()

### Community 51 - "ft5x06.c"
Cohesion: 0.15
Nodes (5): bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), ft5x06_lvgl_read_cb(), sensors_runtime_bus1_hung()

### Community 52 - "ui_open_useredit_cb"
Cohesion: 0.39
Nodes (7): ui_userEditScreen_screen_destroy(), ui_userEditScreen_set_index(), add_cb(), factory_pin_cb(), row_cb(), fresh_useredit(), ui_open_useredit_cb()

### Community 54 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 55 - "task_tracer.c"
Cohesion: 0.27
Nodes (4): state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update()

### Community 56 - "ui_pinScreen.c"
Cohesion: 0.47
Nodes (3): pin_key_cb(), pin_wrong_flash(), refresh_dots()

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "mem_diag.c"
Cohesion: 0.23
Nodes (6): diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str()

### Community 60 - "ui_cfg_flow_unit"
Cohesion: 0.17
Nodes (13): allow_decimal(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit(), ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_from_disp() (+5 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.22
Nodes (6): transport_ble_is_connected(), bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), ui_bleAppScreen_screen_destroy()

### Community 62 - "ui.c"
Cohesion: 0.15
Nodes (15): ui_keypadScreen_screen_destroy(), ui_mainScreen_screen_destroy(), ui_netWifiScreen_screen_destroy(), ui_sensorEditScreen_screen_destroy(), fresh_keypad(), fresh_net_wifi(), get_bleapp(), get_sensor_edit() (+7 more)

### Community 63 - "lv_obj_t"
Cohesion: 0.18
Nodes (10): ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_infoScreen_screen_destroy(), ui_pinScreen_screen_destroy(), fresh_confirm(), fresh_pin(), get_connectivity(), get_info() (+2 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 67 - "driver_delay_ms"
Cohesion: 0.37
Nodes (10): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), write_verified(), driver_delay_ms(), sensors_runtime_preload_calibration() (+2 more)

### Community 68 - "save_cb"
Cohesion: 0.22
Nodes (7): eth_mgr_is_up(), log_dns_info(), notify(), wifi_event_handler(), wifi_mgr_stop(), wifi_schedule_retry(), save_cb()

### Community 69 - "flow_meter.c"
Cohesion: 0.12
Nodes (7): time_sync_worker(), flow_meter_record(), flow_meter_service(), flow_integrator_push(), state_build_json(), state_get(), state_update_sensors()

### Community 70 - "esp_eth_mac_new_ksz8851snl"
Cohesion: 0.22
Nodes (5): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview, esp_eth_mac_new_ksz8851snl(), esp_eth_phy_new_ksz8851snl()

### Community 71 - "config_mode.c"
Cohesion: 0.27
Nodes (5): config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb()

### Community 72 - "ui_netCloudScreen.c"
Cohesion: 0.33
Nodes (5): cert_erase_cb(), cert_pills_refresh(), cert_presence(), save_cb(), set_str()

### Community 75 - "build_channel_flat"
Cohesion: 0.39
Nodes (8): 2. Dos esquemas de canal (importante), build_channel_flat(), build_channel_obj(), flow_to_disp(), flow_unit_app(), gas_to_color(), press_to_disp(), press_unit_app()

### Community 76 - "lv_event_t"
Cohesion: 0.25
Nodes (8): ui_login_set_destination(), ui_loginScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), fresh_login(), fresh_net_ble(), ui_open_config_ble_cb(), ui_open_config_pin_cb(), ui_open_net_ble_cb()

### Community 77 - "ui_nav_load"
Cohesion: 0.22
Nodes (9): ui_netEthScreen_screen_destroy(), ui_pinScreen_set_config_entry(), ui_sensorDiagScreen_screen_destroy(), fresh_net_eth(), get_sensor_diag(), ui_nav_load(), ui_open_net_eth_cb(), ui_open_pin_cb() (+1 more)

### Community 78 - "save_and_refresh"
Cohesion: 0.33
Nodes (8): save_and_refresh(), set_str(), ui_cfg_set_color_code(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_limit_enabled(), ui_cfg_set_pressure_unit()

### Community 79 - "on_eth_event"
Cohesion: 0.32
Nodes (6): mem_diag_report(), on_eth_event(), on_ip_event(), wifi_mgr_get_netif(), state_on_net_available(), http_api_stop()

### Community 80 - "ui_userEditScreen.c"
Cohesion: 0.43
Nodes (5): delete_cb(), flash_error(), save_cb(), selected_role(), ui_nav_back()

### Community 81 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (4): ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 82 - "bleapp_off_cb"
Cohesion: 0.40
Nodes (5): Sesión 2026-08-01 — contrato BLE con la app + saga del watchdog (rama `feature/ble-app-contract`), transport_ble_set_finish_cb(), bleapp_goroot_async(), bleapp_off_cb(), ui_nav_show_root()

### Community 83 - "get_general"
Cohesion: 0.40
Nodes (5): ui_generalScreen_screen_destroy(), key_cb(), get_general(), ui_open_general_authenticated_cb(), ui_open_general_cb()

### Community 84 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 85 - "ui_open_confirm_cb"
Cohesion: 0.50
Nodes (3): accept_cb(), ui_edit_set_new(), ui_open_confirm_cb()

### Community 86 - "get_datetime"
Cohesion: 0.67
Nodes (3): ui_datetimeScreen_screen_destroy(), get_datetime(), ui_open_datetime_cb()

### Community 87 - "get_general_simple"
Cohesion: 0.67
Nodes (3): ui_generalSimpleScreen_screen_destroy(), get_general_simple(), ui_open_general_simple_cb()

### Community 88 - "fresh_net_cloud"
Cohesion: 0.67
Nodes (3): ui_netCloudScreen_screen_destroy(), fresh_net_cloud(), ui_open_net_cloud_cb()

### Community 89 - "get_users"
Cohesion: 0.67
Nodes (3): ui_usersScreen_screen_destroy(), get_users(), ui_open_users_cb()

### Community 90 - "ota_update.c"
Cohesion: 0.11
Nodes (7): ends_with_ci(), erase_stale_tail(), notify(), ota_update_find_file(), ota_update_from_file(), read_description(), version_cmp()

## Knowledge Gaps
- **11 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `esp_err_to_name`, `alarm_mgr.c`, `save_cb`, `fpm_ble_config.c`, `ui_label`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `screen_init_task`, `ui_cfg.c`, `Hecho en la sesión 2026-07-22`, `ui_connectivityScreen.c`, `gas_label_color`, `ui_cfg_flow_unit`, `save_cb`, `ui_netCloudScreen.c`, `save_and_refresh`, `ui_cfg_apply_visual_mode`, `bleapp_off_cb`?**
  _High betweenness centrality (0.054) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Should `esp_err_to_name` be split into smaller, more focused modules?**
  _Cohesion score 0.1310344827586207 - nodes in this community are weakly interconnected._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `save_cb`, `ms5803.c`, `ads1115.c`, `FreeRTOS.h`, `fpm_ble_config.c`, `cert_store.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `esp_log.h`, `screen_init_task`, `sensors_acq_task`, `main.c`, `fpm_i2c_guard.c`, `fpm_i2c_guard`, `ft5x06.c`, `driver_delay_ms`, `save_cb`, `flow_meter.c`, `ota_update.c`?**
  _High betweenness centrality (0.036) - this node is a cross-community bridge._
- **Are the 48 inferred relationships involving `ui_label()` (e.g. with `3. Arquitectura de la nueva HMI (`main/ui/`)` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_label()` has 48 INFERRED edges - model-reasoned connections that need verification._
- **Should `transport_ble.c` be split into smaller, more focused modules?**
  _Cohesion score 0.11578947368421053 - nodes in this community are weakly interconnected._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_statusbar_controller.c`, `driver_delay_ms`, `ui_wifi_main_icon.c`, `sensors_acq_task`, `main.c`, `appcfg_cache_peek`, `ui_label`, `fpm_i2c_guard`, `ft5x06.c`?**
  _High betweenness centrality (0.035) - this node is a cross-community bridge._