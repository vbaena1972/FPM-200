# Graph Report - FPM-200  (2026-10-10)

## Corpus Check
- 186 files · ~421,334 words
- Verdict: corpus is large enough that graph structure adds value.
- Unclassified: 10 file(s) not represented in the graph (top: (none) 5, .projbuild 1, .lock 1)

## Summary
- 1541 nodes · 4292 edges · 85 communities (73 shown, 12 thin omitted)
- Extraction: 74% EXTRACTED · 26% INFERRED · 0% AMBIGUOUS · INFERRED: 1120 edges (avg confidence: 0.86)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `45e74183`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- string
- wifi_mgr.c
- transport_ble.c
- lv_port_mem.c
- alarm_mgr.c
- ui_netEthScreen.c
- test_alarm.c
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- FreeRTOS.h
- fpm_ble_config.c
- ui_edit_begin
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- net_core.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- storage_extras.c
- ui_sensorEditScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW
- save_cb
- esp_eth_phy_ksz8851snl.c
- ui_pin_hash.c
- esp_log.h
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
- ui_i18n
- cert_store.c
- README.md
- transport_mqtt.c
- fpm_i2c_guard
- gen_ota_signing_key.sh script
- ft5x06.c
- sd_monitor_task
- time_mgr.c
- Migración HMI: SquareLine → Claude Design
- ui_pinScreen.c
- Changelog
- mem_diag.c
- ui_cfg_flow_unit
- ui_bleAppScreen.c
- ui.c
- ui_infoScreen.c
- Consumo y endurecimiento 1.5.5-dev
- lvgl
- esp_err_to_name
- wifi_event_handler
- flow_meter.c
- esp_eth_mac_new_ksz8851snl
- metrics_store.c
- ui_auth_can
- AGENTS.md
- CLAUDE.md
- mqtt_event
- ui_wifi_main_icon_init
- save_and_refresh
- on_alarm_state_change
- ui_userEditScreen.c
- ui_cfg_apply_visual_mode
- Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas
- ui_usersScreen.c
- ota_update.c

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 76 edges
2. `ui_label()` - 58 edges
3. `esp_err_to_name()` - 47 edges
4. `ui_col()` - 44 edges
5. `ui_box()` - 42 edges
6. `_t()` - 38 edges
7. `appcfg_save()` - 36 edges
8. `ui_card()` - 35 edges
9. `ui_icon()` - 32 edges
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 28 edges

## Surprising Connections (you probably didn't know these)
- `Fase 2 — Pantalla principal + integración` --references--> `ui_refresh_task()`  [INFERRED]
  docs/HMI_MIGRATION.md → firmware/main/main.c
- `Fase 1 — Esqueleto + limpieza` --references--> `ui_init()`  [INFERRED]
  docs/HMI_MIGRATION.md → firmware/main/ui/ui.c
- `Hecho en la sesión 2026-07-22` --references--> `cert_store_info()`  [INFERRED]
  docs/SESION_HMI.md → firmware/components/cert_store/cert_store.c
- `2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027` --references--> `cert_store_erase_all()`  [INFERRED]
  docs/HANDOFF.md → firmware/components/cert_store/cert_store.c
- `Hecho en la sesión 2026-07-22` --references--> `cert_store_erase_all()`  [INFERRED]
  docs/SESION_HMI.md → firmware/components/cert_store/cert_store.c

## Import Cycles
- None detected.

## Communities (85 total, 12 thin omitted)

### Community 0 - "string"
Cohesion: 0.12
Nodes (6): set_str(), ui_netBleScreen_screen_destroy(), set_str(), ui_netCloudScreen_screen_destroy(), close_to(), main()

### Community 1 - "wifi_mgr.c"
Cohesion: 0.15
Nodes (14): 8. Sesión 2026-09-16 — fixes de campo (#1 buzzer/I2C, #2 WiFi) — commit `a353c41`, apply_ip_mode(), net_cfg_equals(), wifi_apply_cb(), wifi_apply_pending(), wifi_apply_worker(), wifi_mgr_apply_from_cache(), wifi_mgr_init() (+6 more)

### Community 2 - "transport_ble.c"
Cohesion: 0.06
Nodes (30): Cierre final (misma sesión 2026-07-22), Sesión 2026-08-01 — contrato BLE con la app + saga del watchdog (rama `feature/ble-app-contract`), net_core_init(), appmetrics_service_min(), fpm_ble_config_set_clock_cb(), append_json(), ble_on_sync(), ble_start_adv() (+22 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.15
Nodes (4): lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core()

### Community 4 - "alarm_mgr.c"
Cohesion: 0.21
Nodes (17): alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_is_muted(), alarm_mgr_is_muted_impl(), alarm_mgr_press_mute(), alarm_mgr_process() (+9 more)

### Community 5 - "ui_netEthScreen.c"
Cohesion: 0.18
Nodes (7): apply_icon(), timer_cb(), ui_wifi_rssi_to_bars(), wifi_status_cb(), save_cb(), set_str(), ui_netEthScreen_screen_destroy()

### Community 6 - "test_alarm.c"
Cohesion: 0.12
Nodes (12): alarm_mgr_init(), alarm_mgr_press_mute_impl(), alarm_mgr_process_impl(), alarm_mgr_set_audio_inhibit_noncritical_impl(), set_buzzer_acoustic(), appcfg_cache_get(), appcfg_cache_peek(), changed() (+4 more)

### Community 7 - "ms5803.c"
Cohesion: 0.37
Nodes (9): driver_delay_ms(), ms5803_crc4(), ms5803_init(), ms5803_read(), ms5803_read_adc(), ms5803_read_prom_word(), ms5803_reset_retry(), ms5803_send_cmd() (+1 more)

### Community 8 - "esp_eth_mac_ksz8851snl.c"
Cohesion: 0.12
Nodes (41): ksz8851snl 1.2.0 — copia local PARCHEADA (MedGuard / FPM-200), emac_ksz8851_add_mac_filter(), emac_ksz8851_alloc_recv_buf(), emac_ksz8851_deinit(), emac_ksz8851_del(), emac_ksz8851_enable_flow_ctrl(), emac_ksz8851_flush_recv_queue(), emac_ksz8851_get_addr() (+33 more)

### Community 9 - "ads1115.c"
Cohesion: 0.38
Nodes (7): ads1115_init(), ads1115_read_raw(), ads1115_read_raw_once(), ads1115_read_reg(), ads1115_read_voltage(), ads1115_recover(), ads1115_write_reg()

### Community 10 - "FreeRTOS.h"
Cohesion: 0.25
Nodes (7): bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg()

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.16
Nodes (37): 2. Dos esquemas de canal (importante), 6. Cambios aplicados en el firmware (esta tanda), 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW, apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), cfg_dup() (+29 more)

### Community 12 - "ui_edit_begin"
Cohesion: 0.22
Nodes (11): edit_cb(), step_cb(), test_cb(), volume_cb(), ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes(), ui_cfg_pressure_max(), ui_cfg_pressure_min() (+3 more)

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.18
Nodes (10): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`), 8. Cómo compilar (+2 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.12
Nodes (15): 1. GATT, 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig`, bluetooth ↔ `bt` / `BluetoothConfig`, channels ↔ `sensors` / `ChannelConfig`/`BleChannelConfig` (+7 more)

### Community 15 - "net_core.c"
Cohesion: 0.13
Nodes (10): on_eth_event(), on_ip_event(), on_wifi_event(), rearm_net_services(), time_mgr_start_sntp(), transport_mqtt_on_net_down(), state_build_json(), state_get() (+2 more)

### Community 16 - "ui_label"
Cohesion: 0.06
Nodes (102): 3. Arquitectura de la nueva HMI (`main/ui/`), bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), conn_card(), ui_connectivityScreen_screen_init(), row(), ui_datetimeScreen_screen_init() (+94 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "storage_extras.c"
Cohesion: 0.29
Nodes (13): appcfg_calc_etag(), appcfg_patch(), appcfg_set(), appcfg_to_minified_json(), appcfg_validate(), fnv1a32(), json_merge_patch_obj(), other_slot() (+5 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.21
Nodes (13): accept_cb(), card(), dec_cb(), flow_card(), limit_step(), mini(), pressure_card(), rebuild() (+5 more)

### Community 20 - "storage.c"
Cohesion: 0.24
Nodes (17): 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 6. Pendiente (después de estabilizar §2 y validar §3), 7. Gotchas que no re-descubrir, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults(), appcfg_load(), appcfg_migrate() (+9 more)

### Community 21 - "http_api.c"
Cohesion: 0.14
Nodes (17): mdns_svc_start(), add_channel(), alarms_get_handler(), cfg_get_handler(), cfg_put_handler(), http_api_init(), http_api_start(), http_api__start_locked() (+9 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.13
Nodes (24): ads1115_is_ready(), ms5803_is_ready(), flow_meter_init(), lock(), sensors_runtime_flow_warming(), sensors_runtime_get_ages(), sensors_runtime_get_aws_telemetry(), sensors_runtime_get_debug() (+16 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (4): fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read(), fpm_i2c_transmit()

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.11
Nodes (18): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 4. Cómo compilar / gotchas de build, Actualización 2026-08-09 (estabilización en HW + app), Actualización 2026-08-18 (sensores reales — se elimina la simulación), Actualización 2026-08-21 — EEPROM persiste + recalibración FS7 estable, Actualización 2026-09-24 — EEPROM, consumo, cero de presión (FW 1.5.5 → 1.5.22-dev) (+10 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.20
Nodes (8): fpm_new_panel_io_i2c(), panel_io_i2c_del(), panel_io_i2c_register_event_callbacks(), panel_io_i2c_rx_buffer(), panel_io_i2c_rx_param(), panel_io_i2c_tx_buffer(), panel_io_i2c_tx_color(), panel_io_i2c_tx_param()

### Community 27 - "eth_mgr.c"
Cohesion: 0.16
Nodes (8): appcfg_to_eth_view(), apply_ip_config(), eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start(), eth_mgr_stop(), eth_view_from_appcfg(), eth_init_task()

### Community 28 - "Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW"
Cohesion: 0.29
Nodes (7): 5. Lo demás que ya se hizo (commiteado en `66014fa`, sin validar 100% en HW), 9. Estado de pendientes (actualizado 2026-07-31), Aún pendiente, Sesión 2026-07-30 (previa, ya commiteada en `c03ba34`), Sesión 2026-07-31 (parte 2 — RAM/estabilidad + modo config BLE) — SIN COMPILAR NI HW, mqtt_stop_locked(), transport_mqtt_stop()

### Community 29 - "save_cb"
Cohesion: 0.50
Nodes (3): ui_connectivityScreen_refresh(), save_cb(), set_str()

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.16
Nodes (17): ksz8851_update_link_duplex_speed(), phy_ksz8851_advertise_pause_ability(), phy_ksz8851_autonego_ctrl(), phy_ksz8851_deinit(), phy_ksz8851_del(), phy_ksz8851_get_addr(), phy_ksz8851_get_link(), phy_ksz8851_init() (+9 more)

### Community 31 - "ui_pin_hash.c"
Cohesion: 0.24
Nodes (15): derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final(), sha256_init() (+7 more)

### Community 32 - "esp_log.h"
Cohesion: 0.06
Nodes (4): ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 33 - "ethernet_init.c"
Cohesion: 0.19
Nodes (5): eth_init_internal(), eth_init_spi(), example_eth_deinit(), example_eth_init(), spi_bus_init()

### Community 34 - "screen_init_task"
Cohesion: 0.21
Nodes (14): Contrato de integración con el firmware (preservado), bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on() (+6 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.17
Nodes (14): Sesión 2026-07-31 (ESTA sesión — NO commiteada aún; sin compilar en este entorno, sin HW), eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual() (+6 more)

### Community 36 - "ui_cfg.c"
Cohesion: 0.14
Nodes (21): next(), gas_label_color(), ui_auth_factory(), ui_auth_login(), ui_auth_user_at(), ui_cfg_apply_timezone(), ui_cfg_color_code(), ui_cfg_color_is_iso() (+13 more)

### Community 37 - "Hecho en la sesión 2026-07-22"
Cohesion: 0.36
Nodes (8): Hecho en la sesión 2026-07-22, bright_cb(), clamp_brightness(), ui_cfg_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 38 - "ui_generalSimpleScreen.c"
Cohesion: 0.18
Nodes (9): ui_generalScreen_screen_destroy(), dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb(), ui_generalSimpleScreen_screen_destroy(), rebuild() (+1 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.16
Nodes (15): Actualización 2026-08-18 (tarde) — SFM leyendo, robustez I²C, calibración FS7 · **v1.4.0**, Actualización 2026-08-20 — BMP280 (ref. atmosférica), cero FS7, debug en dashboard, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+7 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.19
Nodes (8): conn_signature(), cpy(), gather(), live_tick(), rssi_pct(), str_hash(), ui_connectivityScreen_screen_destroy(), ui_netWifiScreen_screen_destroy()

### Community 41 - "main.c"
Cohesion: 0.10
Nodes (3): bsp_i2c_init(), esp_trace_get_user_params(), mount_sdcard_hotplug()

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.16
Nodes (17): appcfg_cache_peek(), appcfg_save(), f_switch(), ui_auth_set_factory_pin(), ui_auth_user_count(), ui_auth_user_remove(), ui_cfg_alarm_tone(), ui_cfg_check_pin() (+9 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.16
Nodes (15): fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_init(), fpm_i2c_multi_buffer_transmit(), fpm_i2c_receive(), fpm_i2c_stop_read(), fpm_i2c_transmit() (+7 more)

### Community 44 - "ui_i18n"
Cohesion: 0.24
Nodes (4): ui_datetimeScreen_screen_destroy(), key_cb(), refresh_value(), ui_keypadScreen_screen_destroy()

### Community 45 - "cert_store.c"
Cohesion: 0.26
Nodes (10): cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load(), cert_store_write(), key_for_kind(), read_file_to_buf() (+2 more)

### Community 47 - "transport_mqtt.c"
Cohesion: 0.19
Nodes (9): add_channel(), aws_telemetry_task(), mqtt_start_locked(), prepare_pem(), publish_alarm_transition(), shadow_apply_pending(), shadow_topics_init(), transport_mqtt_on_time_ready() (+1 more)

### Community 49 - "fpm_i2c_guard"
Cohesion: 0.25
Nodes (6): bcd2dec(), dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init()

### Community 51 - "ft5x06.c"
Cohesion: 0.15
Nodes (5): bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), ft5x06_lvgl_read_cb(), sensors_runtime_bus1_hung()

### Community 52 - "sd_monitor_task"
Cohesion: 0.15
Nodes (12): Actualización 2026-09-21 — microSD, AWS por SD, BT-off y splash (FW 1.5.4-dev), ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), restart_btn_event_cb(), sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), ui_main_deinit_cb() (+4 more)

### Community 54 - "time_mgr.c"
Cohesion: 0.17
Nodes (4): 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, time_mgr_is_time_valid(), time_mgr_set_datetime(), on_ble_set_clock()

### Community 55 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 56 - "ui_pinScreen.c"
Cohesion: 0.32
Nodes (5): pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry()

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "mem_diag.c"
Cohesion: 0.23
Nodes (6): diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str()

### Community 60 - "ui_cfg_flow_unit"
Cohesion: 0.20
Nodes (11): general_loaded_cb(), protected_back_cb(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit(), ui_cfg_press_decimals(), ui_cfg_press_fmt(), ui_cfg_press_from_disp(), ui_cfg_press_to_disp() (+3 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (5): bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), ui_bleAppScreen_screen_destroy()

### Community 62 - "ui.c"
Cohesion: 0.05
Nodes (72): apply_authenticated_cb(), allow_decimal(), add_user(), key(), key_cb(), refresh(), role_color(), role_name() (+64 more)

### Community 63 - "ui_infoScreen.c"
Cohesion: 0.36
Nodes (5): erase_nvs_namespace(), factory_btn_cb(), factory_reset_apply(), reboot_cb(), ui_infoScreen_screen_destroy()

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 66 - "lvgl"
Cohesion: 0.10
Nodes (4): ui_confirmScreen_screen_destroy(), kb_event_cb(), password_toggle_cb(), ta_event_cb()

### Community 67 - "esp_err_to_name"
Cohesion: 0.25
Nodes (16): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), write_verified(), crc16_ccitt(), sensor_cfg_persist() (+8 more)

### Community 68 - "wifi_event_handler"
Cohesion: 0.16
Nodes (10): mem_diag_report(), on_eth_event(), on_ip_event(), log_dns_info(), notify(), wifi_event_handler(), wifi_mgr_get_netif(), wifi_mgr_stop() (+2 more)

### Community 69 - "flow_meter.c"
Cohesion: 0.22
Nodes (5): time_sync_worker(), flow_meter_get(), flow_meter_record(), flow_meter_service(), flow_integrator_push()

### Community 70 - "esp_eth_mac_new_ksz8851snl"
Cohesion: 0.22
Nodes (5): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview, esp_eth_mac_new_ksz8851snl(), esp_eth_phy_new_ksz8851snl()

### Community 72 - "ui_auth_can"
Cohesion: 0.23
Nodes (13): cert_erase_cb(), save_cb(), ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_logout() (+5 more)

### Community 75 - "mqtt_event"
Cohesion: 0.38
Nodes (5): mqtt_event(), shadow_rx_chunk(), shadow_rx_reset(), topic_is(), ui_statusbar_signal_cloud_activity()

### Community 78 - "save_and_refresh"
Cohesion: 0.19
Nodes (14): gas_cb(), is_press_unit(), limit_edit(), p_switch(), std_cb(), unit_cb(), save_and_refresh(), set_str() (+6 more)

### Community 80 - "ui_userEditScreen.c"
Cohesion: 0.39
Nodes (5): delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 81 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (4): ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 84 - "Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Analisis de longFPM.log: bloqueo de interfaz, alarma persistente, pila, heap y tareas, Source Nodes

### Community 89 - "ui_usersScreen.c"
Cohesion: 0.33
Nodes (3): role_color(), role_name(), ui_usersScreen_screen_destroy()

### Community 90 - "ota_update.c"
Cohesion: 0.15
Nodes (7): ends_with_ci(), erase_stale_tail(), notify(), ota_update_find_file(), ota_update_from_file(), read_description(), version_cmp()

## Knowledge Gaps
- **12 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `transport_ble.c`, `ui_netEthScreen.c`, `test_alarm.c`, `fpm_ble_config.c`, `ui_edit_begin`, `ui_label`, `ui_sensorEditScreen.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `save_cb`, `screen_init_task`, `ui_cfg.c`, `Hecho en la sesión 2026-07-22`, `ui_connectivityScreen.c`, `transport_mqtt.c`, `ui_cfg_flow_unit`, `ui_auth_can`, `save_and_refresh`, `ui_cfg_apply_visual_mode`?**
  _High betweenness centrality (0.062) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Should `string` be split into smaller, more focused modules?**
  _Cohesion score 0.11594202898550725 - nodes in this community are weakly interconnected._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `wifi_mgr.c`, `transport_ble.c`, `ms5803.c`, `ads1115.c`, `FreeRTOS.h`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `esp_log.h`, `screen_init_task`, `sensors_acq_task`, `fpm_i2c_guard.c`, `cert_store.c`, `fpm_i2c_guard`, `ft5x06.c`, `sd_monitor_task`, `ui_infoScreen.c`, `wifi_event_handler`, `flow_meter.c`, `ota_update.c`?**
  _High betweenness centrality (0.037) - this node is a cross-community bridge._
- **Are the 49 inferred relationships involving `ui_label()` (e.g. with `3. Arquitectura de la nueva HMI (`main/ui/`)` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_label()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Should `wifi_mgr.c` be split into smaller, more focused modules?**
  _Cohesion score 0.14666666666666667 - nodes in this community are weakly interconnected._
- **Why does `screen_init_task()` connect `screen_init_task` to `ui_statusbar_controller.c`, `esp_err_to_name`, `sensors_acq_task`, `main.c`, `appcfg_cache_peek`, `ui_wifi_main_icon_init`, `ui_label`, `fpm_i2c_guard`, `ft5x06.c`?**
  _High betweenness centrality (0.036) - this node is a cross-community bridge._