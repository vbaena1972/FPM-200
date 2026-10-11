# Graph Report - FPM-200  (2026-10-11)

## Corpus Check
- 187 files · ~423,258 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 1555 nodes · 4222 edges · 96 communities (88 shown, 8 thin omitted)
- Extraction: 76% EXTRACTED · 24% INFERRED · 0% AMBIGUOUS · INFERRED: 1028 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `0a012e1a`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- string
- esp_err_to_name
- transport_ble.c
- lv_port_mem.c
- alarm_mgr.c
- ui.c
- ui_loginScreen.c
- ms5803.c
- esp_eth_mac_ksz8851snl.c
- ads1115.c
- FreeRTOS.h
- fpm_ble_config.c
- step_cb
- Sesión — Migración HMI Axira (SquareLine → Claude Design)
- 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)
- time_mgr.c
- ui_label
- QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)
- ui_pin_hash.c
- ui_sensorEditScreen.c
- storage.c
- http_api.c
- sensors_runtime.c
- test_eeprom.c
- Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)
- Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)
- touch_io_bounded.c
- eth_mgr.c
- ui_wifi_main_icon.c
- task_tracer.c
- esp_eth_phy_ksz8851snl.c
- ui_mainScreen.c
- esp_log.h
- ethernet_init.c
- screen_init_task
- ui_statusbar_controller.c
- gas_label_color
- ui_generalSimpleScreen.c
- cert_store.c
- sensors_acq_task
- ui_connectivityScreen.c
- main.c
- appcfg_cache_peek
- fpm_i2c_guard.c
- accept_cb
- esp_lcd_st7796.h
- README.md
- transport_mqtt.c
- save_cb
- gen_ota_signing_key.sh script
- ft5x06.c
- sd_monitor_task
- ui_cfg_preview_brightness
- Migración HMI: SquareLine → Claude Design
- ui_nav_load
- Changelog
- mem_diag.c
- esp_eth_driver
- ui_cfg.c
- ui_bleAppScreen.c
- get_general
- fresh_login
- Consumo y endurecimiento 1.5.5-dev
- lvgl
- at24c256.c
- ui_statusbar_request_refresh
- app_main
- Microchip KSZ8851SNL Ethernet Driver
- Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes
- ui_auth_can
- AGENTS.md
- CLAUDE.md
- test_alarm.c
- esp_eth_com
- ui_netBleScreen.c
- Q: Revisa cada cuando el FPM esta enviando los datos a AWS
- config_mode.c
- ui_open_useredit_cb
- ws_stream.c
- get_general_simple
- flow_meter.c
- next
- ui_infoScreen.c
- README_PARCHE.md
- lv_event_t
- ui_pinScreen.c
- ui_i18n
- ota_update.c
- ui_cfg_apply_visual_mode
- ui_generalScreen.c
- apply_data_activity_visual
- on_network_ready
- ui_mainScreen_screen_destroy

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
10. `Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)` - 30 edges

## Surprising Connections (you probably didn't know these)
- `sensors_acq_task()` --calls--> `bmp280_is_ready()`  [INFERRED]
  firmware/components/sensors_runtime/sensors_runtime.c → firmware/components/drivers/bmp280.c
- `ui_refresh_task()` --calls--> `time_mgr_is_time_valid()`  [INFERRED]
  firmware/main/main.c → firmware/components/network_core/time_mgr.c
- `ui_main_update()` --calls--> `sensors_runtime_get_debug()`  [INFERRED]
  firmware/main/ui/screens/ui_mainScreen.c → firmware/components/sensors_runtime/sensors_runtime.c
- `get_bleapp()` --calls--> `ui_bleAppScreen_screen_destroy()`  [INFERRED]
  firmware/main/ui/ui.c → firmware/main/ui/screens/ui_bleAppScreen.c
- `ui_confirmScreen_screen_init()` --calls--> `ui_edit_old()`  [INFERRED]
  firmware/main/ui/screens/ui_confirmScreen.c → firmware/main/ui/ui_cfg.c

## Import Cycles
- None detected.

## Communities (96 total, 8 thin omitted)

### Community 0 - "string"
Cohesion: 0.14
Nodes (12): key_cb(), refresh_value(), cert_pills_refresh(), cert_presence(), set_str(), close_to(), main(), stdio (+4 more)

### Community 1 - "esp_err_to_name"
Cohesion: 0.13
Nodes (26): esp_task_wdt, esp_wifi_types, time_sync_worker(), apply_ip_mode(), AppConfig, esp_err_t, wifi_status_t, net_cfg_equals() (+18 more)

### Community 2 - "transport_ble.c"
Cohesion: 0.12
Nodes (13): ble_cmd_handler_t, ble_gap, ble_gatt, ble_hs, ble_svc_gap, ble_svc_gatt, esp_bt, append_json() (+5 more)

### Community 3 - "lv_port_mem.c"
Cohesion: 0.15
Nodes (8): esp_heap_caps, lv_mem_add_pool(), lv_mem_monitor_core(), lv_mem_remove_pool(), lv_mem_test_core(), lv_mem_monitor_t, lv_mem_pool_t, lv_result_t

### Community 4 - "alarm_mgr.c"
Cohesion: 0.13
Nodes (31): alarm_state_change_cb_t, eth_ksz8851snl_config_t, eth_mac_config_t, alarm_mgr_get_current_state(), alarm_mgr_get_current_state_impl(), alarm_mgr_get_sensor_faults(), alarm_mgr_get_sensor_faults_impl(), alarm_mgr_init() (+23 more)

### Community 5 - "ui.c"
Cohesion: 0.08
Nodes (46): ui_confirmScreen_screen_destroy(), ui_connectivityScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), ui_keypadScreen_screen_destroy(), ui_netBleScreen_screen_destroy(), ui_netCloudScreen_screen_destroy(), ui_netEthScreen_screen_destroy(), ui_netWifiScreen_screen_destroy() (+38 more)

### Community 6 - "ui_loginScreen.c"
Cohesion: 0.23
Nodes (16): app_user_role_t, app_user_t, lv_event_t, fac_close_cb(), fac_hide(), fac_kb_cb(), fac_show_err(), fac_try() (+8 more)

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
Cohesion: 0.25
Nodes (10): bmp280, bmp280_compensate_press(), bmp280_compensate_temp(), bmp280_init(), bmp280_is_ready(), bmp280_read(), bmp280_read_regs(), bmp280_write_reg() (+2 more)

### Community 11 - "fpm_ble_config.c"
Cohesion: 0.17
Nodes (36): apply_channel_obj(), apply_netif(), build_channel_flat(), build_channel_obj(), AppConfig, cJSON, cfg_dup(), color_to_gas() (+28 more)

### Community 12 - "step_cb"
Cohesion: 0.47
Nodes (6): step_cb(), volume_cb(), ui_cfg_alarm_volume(), ui_cfg_max_silence_minutes(), ui_cfg_reannounce_minutes(), ui_cfg_set_alarm_audio()

### Community 13 - "Sesión — Migración HMI Axira (SquareLine → Claude Design)"
Cohesion: 0.10
Nodes (20): 10. Backup, 1. Objetivo y estado, 2. Repositorios (GitHub, cuenta vbaena1972), 3. Arquitectura de la nueva HMI (`main/ui/`), 4. Design system (alineado a ClaudeHMI/MedGuard, dark), 5. Pantallas (16) y navegación, 6. Lógica / persistencia (hecho), 7. Simulador de PC (`ClaudeHMI-Sim`) (+12 more)

### Community 14 - "3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter)"
Cohesion: 0.11
Nodes (17): 1. GATT, 2. Dos esquemas de canal (importante), 3. Correspondencia de campos (firmware `AppConfig` ↔ JSON ↔ Flutter), 4. Límites de longitud (deben coincidir), 5. Secretos y versión, 6. Cambios aplicados en el firmware (esta tanda), 7. Pendiente, alarms ↔ `general.alarm` / `AlarmsConfig` (+9 more)

### Community 15 - "time_mgr.c"
Cohesion: 0.11
Nodes (21): esp_eth, esp_event, esp_netif_sntp, esp_sntp, esp_wifi, esp_err_t, esp_event_base_t, net_core_init() (+13 more)

### Community 16 - "ui_label"
Cohesion: 0.08
Nodes (99): bleapp_set_connected(), ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+91 more)

### Community 17 - "QA FORMAL — FPM-200 firmware **1.5.24-dev** (iniciado sobre 1.5.22)"
Cohesion: 0.07
Nodes (27): A. Arranque / versión / estabilidad, Alcance: qué cambia 1.5.22 respecto de 1.5.4 (último commit previo al ciclo), B. Calibración EEPROM, C. Medición presión / flujo, Cierre, D. Adquisición / temporización, E. Consumo, F. Alarmas (+19 more)

### Community 18 - "ui_pin_hash.c"
Cohesion: 0.23
Nodes (18): esp_random, derive(), from_hex(), hmac(), hmac_key_init(), random_salt(), sha256_block(), sha256_final() (+10 more)

### Community 19 - "ui_sensorEditScreen.c"
Cohesion: 0.20
Nodes (19): lv_event_t, dec_cb(), gas_cb(), is_press_unit(), limit_edit(), limit_step(), p_switch(), rebuild() (+11 more)

### Community 20 - "storage.c"
Cohesion: 0.11
Nodes (42): app_metrics_t, cfg_result_t, appmetrics_load(), appmetrics_store(), esp_err_t, appcfg_cache_get(), appcfg_cache_reload(), appcfg_defaults() (+34 more)

### Community 21 - "http_api.c"
Cohesion: 0.20
Nodes (22): esp_chip_info, esp_http_server, add_channel(), alarms_get_handler(), cJSON, esp_err_t, httpd_handle_t, httpd_req_t (+14 more)

### Community 22 - "sensors_runtime.c"
Cohesion: 0.10
Nodes (37): device_state_t, aws_telemetry_task(), flow_meter_get(), esp_err_t, sensor_sample_t, sensor_signal_stats_t, crc16_ccitt(), lock() (+29 more)

### Community 23 - "test_eeprom.c"
Cohesion: 0.33
Nodes (9): at24c256, esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_rm_device(), fpm_i2c_stop_read() (+1 more)

### Community 24 - "Handoff — ClaudeHMI-FW (FPM-200) · arranque de sesión (2026-08-01)"
Cohesion: 0.06
Nodes (30): 0. Contexto, 1. ⚠️ ESTADO — leer antes de nada, 2026-09-29 (cont.) — FW 1.5.29-dev: config remota por shadow NOMBRADO `config`, 2026-10-10 (tarde) — FW 1.5.32-dev (compilado, pendiente de HW): corte de energía + DEC-027, 2026-10-11 — 1.5.32 probado en HW; FW 1.5.33-dev (sin compilar, pendiente de HW), 2026-10-11 (noche) — 1.5.33 probado; FW 1.5.34-dev (sin compilar), 2. 🔴 BUG ABIERTO #1 (bloqueante): watchdog en `taskLVGL` — FIX A VALIDAR, 3. Contrato BLE FPM ↔ app Sensvax (commit `711b463`) — A VALIDAR EN HW (+22 more)

### Community 25 - "Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)"
Cohesion: 0.25
Nodes (7): 1. Estado actual: lo activado es 100 % reversible (NINGÚN eFuse), 2.1 Fix 2026-09-28: bloque de firma residual, 2. OTA firmada — cómo funciona y cómo operarla, 3. PIN con hash — cómo funciona, 4. Producción — Secure Boot V2 + cifrado de flash (IRREVERSIBLE), 5. Mientras tanto: mitigación en la nube (sin tocar el chip), Seguridad del firmware — MedGuard 12-IoT y FPM-200 (2026-09-26)

### Community 26 - "touch_io_bounded.c"
Cohesion: 0.20
Nodes (18): cdefs, esp_compiler, esp_lcd_panel_io_callbacks_t, esp_lcd_panel_io_handle_t, esp_lcd_panel_io_i2c_config_t, esp_lcd_panel_io_interface, esp_lcd_panel_io_t, esp_err_t (+10 more)

### Community 27 - "eth_mgr.c"
Cohesion: 0.15
Nodes (18): esp_eth_netif_glue, esp_netif, eth_view_t, appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t (+10 more)

### Community 28 - "ui_wifi_main_icon.c"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 29 - "task_tracer.c"
Cohesion: 0.27
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), trace_worker(), track_get_and_update(), task_tracer, TaskHandle_t

### Community 30 - "esp_eth_phy_ksz8851snl.c"
Cohesion: 0.15
Nodes (28): eth_phy_autoneg_cmd_t, eth_phy_config_t, esp_err_t, esp_eth_mediator_t, esp_eth_phy_t, eth_duplex_t, eth_link_t, eth_speed_t (+20 more)

### Community 31 - "ui_mainScreen.c"
Cohesion: 0.17
Nodes (30): card_state_t, alarm_condition(), alarm_overlay_open(), banner_open_cb(), alarm_clinical_state_t, AppConfig, lv_coord_t, lv_obj_t (+22 more)

### Community 32 - "esp_log.h"
Cohesion: 0.09
Nodes (9): esp_lcd_panel_ops, esp_lcd_touch, esp_lcd_types, esp_lvgl_port, stdbool, stddef, stdint, storage (+1 more)

### Community 33 - "ethernet_init.c"
Cohesion: 0.19
Nodes (16): esp_check, esp_eth_handle_t, esp_eth_mac_ksz8851snl, esp_eth_phy_ksz8851snl, esp_mac, ethernet_init, esp_err_t, esp_eth_mac_t (+8 more)

### Community 34 - "screen_init_task"
Cohesion: 0.24
Nodes (16): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+8 more)

### Community 35 - "ui_statusbar_controller.c"
Cohesion: 0.17
Nodes (18): eth_mgr_is_up(), cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_advertising(), transport_ble_is_connected(), activity_timer_cb(), apply_cloud_visual(), lv_obj_t (+10 more)

### Community 36 - "gas_label_color"
Cohesion: 0.31
Nodes (9): gas_label_color(), ui_cfg_color_code(), ui_cfg_color_is_iso(), ui_cfg_gas(), ui_cfg_gas_color_at(), ui_cfg_gas_count(), ui_cfg_gas_index(), ui_cfg_gas_key() (+1 more)

### Community 37 - "ui_generalSimpleScreen.c"
Cohesion: 0.42
Nodes (7): bright_cb(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general(), theme_cb()

### Community 38 - "cert_store.c"
Cohesion: 0.20
Nodes (19): cert_info_t, cert_kind_t, cert_store, crypto, fcntl, esp_err_t, cert_store_erase(), cert_store_erase_all() (+11 more)

### Community 39 - "sensors_acq_task"
Cohesion: 0.20
Nodes (16): esp_err_t, i2c_master_bus_handle_t, sfm3300_crc8(), sfm3300_get_last_rx(), sfm3300_init(), sfm3300_is_ready(), sfm3300_read(), sfm3300_send_cmd() (+8 more)

### Community 40 - "ui_connectivityScreen.c"
Cohesion: 0.28
Nodes (7): conn_status_t, lv_timer_t, conn_signature(), cpy(), live_tick(), rssi_pct(), str_hash()

### Community 41 - "main.c"
Cohesion: 0.09
Nodes (21): errno, esp_app_trace, esp_core_dump, esp_heap_trace, esp_trace, esp_trace_open_params_t, esp_vfs_fat, ui_statusbar_controller_deinit() (+13 more)

### Community 42 - "appcfg_cache_peek"
Cohesion: 0.12
Nodes (25): appcfg_cache_peek(), appcfg_save(), bleapp_off_cb(), save_cb(), lv_event_t, save_cb(), f_switch(), app_user_t (+17 more)

### Community 43 - "fpm_i2c_guard.c"
Cohesion: 0.17
Nodes (25): esp_err_t, i2c_device_config_t, i2c_master_bus_handle_t, i2c_master_dev_handle_t, fpm_i2c_bus_add_device(), fpm_i2c_bus_reset(), fpm_i2c_bus_rm_device(), fpm_i2c_multi_buffer_transmit() (+17 more)

### Community 44 - "accept_cb"
Cohesion: 0.67
Nodes (3): accept_cb(), lv_event_t, ui_edit_set_new()

### Community 45 - "esp_lcd_st7796.h"
Cohesion: 0.40
Nodes (4): esp_idf_version, esp_lcd_mipi_dsi, esp_lcd_panel_vendor, lcd_types

### Community 47 - "transport_mqtt.c"
Cohesion: 0.12
Nodes (23): esp_mqtt_event_handle_t, add_channel(), cJSON, esp_event_base_t, sensor_signal_stats_t, mqtt_event(), mqtt_start_locked(), mqtt_stop_locked() (+15 more)

### Community 49 - "save_cb"
Cohesion: 0.20
Nodes (17): fpm_ble_config_set_clock_cb(), ble_on_sync(), ble_start_adv(), esp_err_t, transport_ble_init(), transport_ble_set_enabled(), transport_ble_set_finish_cb(), transport_ble_set_mesh_enabled() (+9 more)

### Community 51 - "ft5x06.c"
Cohesion: 0.14
Nodes (16): bsp_touch_config_t, esp_lcd_panel_io, esp_lcd_touch_handle_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t (+8 more)

### Community 52 - "sd_monitor_task"
Cohesion: 0.21
Nodes (10): sd_flow_clear(), sd_monitor_task(), sd_ui_close_cb(), unmount_sdcard_hotplug(), lv_event_cb_t, lv_event_t, close_click_cb(), ui_sd_close() (+2 more)

### Community 54 - "ui_cfg_preview_brightness"
Cohesion: 0.50
Nodes (5): clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 55 - "Migración HMI: SquareLine → Claude Design"
Cohesion: 0.17
Nodes (11): 🩹 Arreglos preexistentes (NO relacionados con la HMI), 💾 Backup, ✅ Completado (Fases 0–2), 🔨 Cómo compilar (en tu terminal ESP-IDF), Fase 0 — Tooling y sistema de diseño, Fase 1 — Esqueleto + limpieza, Fase 2 — Pantalla principal + integración, Migración HMI: SquareLine → Claude Design (+3 more)

### Community 56 - "ui_nav_load"
Cohesion: 0.16
Nodes (17): bleapp_goroot_async(), apply_authenticated_cb(), lv_event_t, ui_connectivityScreen_refresh(), ui_sensorDiagScreen_refresh(), ui_sensorEditScreen_refresh(), lv_event_t, lv_obj_t (+9 more)

### Community 57 - "Changelog"
Cohesion: 0.25
Nodes (7): 1.0.0 (2025-09-24), [1.1.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.0.0...ksz8851snl@v1.1.0) (2026-06-25), [1.2.0](https://github.com/espressif/esp-eth-drivers/compare/ksz8851snl@v1.1.0...ksz8851snl@v1.2.0) (2026-07-29), Changelog, Features, Features, Features

### Community 58 - "mem_diag.c"
Cohesion: 0.25
Nodes (7): esp_system, eTaskState, diag_worker(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), task_state_str()

### Community 60 - "ui_cfg.c"
Cohesion: 0.18
Nodes (22): general_loaded_cb(), allow_decimal(), pressure_card(), ui_edit_target_t, ui_cfg_brightness(), ui_cfg_flow_decimals(), ui_cfg_flow_fmt(), ui_cfg_flow_to_disp() (+14 more)

### Community 61 - "ui_bleAppScreen.c"
Cohesion: 0.24
Nodes (8): config_mode, bleapp_is_connected(), bleapp_leave_to_dashboard(), bleapp_pair_pin(), bleapp_tick_cb(), AppConfig, lv_timer_t, ui_bleAppScreen_screen_destroy()

### Community 62 - "get_general"
Cohesion: 0.67
Nodes (3): ui_generalScreen_screen_destroy(), get_general(), ui_open_general_cb()

### Community 63 - "fresh_login"
Cohesion: 0.25
Nodes (9): ui_infoScreen_screen_destroy(), ui_login_set_destination(), ui_loginScreen_screen_destroy(), menu_item_cb(), fresh_login(), get_info(), ui_open_config_ble_cb(), ui_open_config_pin_cb() (+1 more)

### Community 64 - "Consumo y endurecimiento 1.5.5-dev"
Cohesion: 0.07
Nodes (27): 1.5.17 hardware result (log3, 2026-09-24): VALIDATED, 1.5.18 hardware result (log4): VALIDATED, committed a25efbf, 1.5.19 hardware result (log5), 1.5.20 hardware result (log6): OK, 1.5.21 hardware result (log7): VALIDATED, 1.5.22 hardware result (log8, after full erase-flash + SD import): VALIDATED, 1.5.23 boot check + 1.5.24-dev (2026-09-25), Audit and fixes: 1.5.18-dev (Claude, 2026-09-24) (+19 more)

### Community 66 - "lvgl"
Cohesion: 0.11
Nodes (5): lv_event_t, kb_event_cb(), password_toggle_cb(), ta_event_cb(), lvgl

### Community 67 - "at24c256.c"
Cohesion: 0.30
Nodes (13): at24c256_init(), at24c256_is_ready(), at24c256_read(), at24c256_self_test(), at24c256_write(), esp_err_t, i2c_master_bus_handle_t, write_verified() (+5 more)

### Community 68 - "ui_statusbar_request_refresh"
Cohesion: 0.17
Nodes (15): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_event_base_t, esp_netif_t, log_dns_info(), wifi_event_handler() (+7 more)

### Community 69 - "app_main"
Cohesion: 0.18
Nodes (11): mem_diag_start_periodic(), ads1115_is_ready(), fpm_i2c_init(), ms5803_is_ready(), transport_mqtt_runtime_init(), AppConfig, i2c_master_bus_handle_t, sensors_runtime_init() (+3 more)

### Community 70 - "Microchip KSZ8851SNL Ethernet Driver"
Cohesion: 0.50
Nodes (3): ESP-IDF Usage, Microchip KSZ8851SNL Ethernet Driver, Overview

### Community 71 - "Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Implement WiFi password visibility toggle, keep SSID/password above LVGL keyboard, refresh connectivity after WiFi save, suppress auth-screen buzzer, and publish device_status immediately on alarm changes, Source Nodes

### Community 72 - "ui_auth_can"
Cohesion: 0.22
Nodes (16): app_user_role_t, save_cb(), selected_role(), app_user_role_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage() (+8 more)

### Community 75 - "test_alarm.c"
Cohesion: 0.14
Nodes (14): alarm_mgr, assert, set_buzzer_acoustic(), appcfg_cache_get(), appcfg_cache_peek(), alarm_clinical_state_t, AppConfig, changed() (+6 more)

### Community 76 - "esp_eth_com"
Cohesion: 0.40
Nodes (3): esp_eth_com, esp_eth_mac_spi, esp_eth_phy

### Community 77 - "ui_netBleScreen.c"
Cohesion: 0.21
Nodes (7): eth_mgr, lv_event_t, save_cb(), set_str(), set_str(), ui_form, ui_nav

### Community 78 - "Q: Revisa cada cuando el FPM esta enviando los datos a AWS"
Cohesion: 0.40
Nodes (4): Answer, Outcome, Q: Revisa cada cuando el FPM esta enviando los datos a AWS, Source Nodes

### Community 79 - "config_mode.c"
Cohesion: 0.24
Nodes (9): transport_ble_stop_adv(), esp_err_t, config_mode_enter(), config_mode_exit(), log_heap(), bleapp_loaded_cb(), bleapp_unloaded_cb(), lv_event_t (+1 more)

### Community 80 - "ui_open_useredit_cb"
Cohesion: 0.60
Nodes (6): ui_userEditScreen_set_index(), add_cb(), lv_event_t, factory_pin_cb(), row_cb(), ui_open_useredit_cb()

### Community 81 - "ws_stream.c"
Cohesion: 0.31
Nodes (8): cjson, esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 82 - "get_general_simple"
Cohesion: 0.67
Nodes (3): ui_generalSimpleScreen_screen_destroy(), get_general_simple(), ui_open_general_simple_cb()

### Community 83 - "flow_meter.c"
Cohesion: 0.22
Nodes (7): esp_err_t, flow_meter_init(), flow_meter_record(), flow_integrator_push(), flow_integrator_t, flow_meter, time

### Community 84 - "next"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 85 - "ui_infoScreen.c"
Cohesion: 0.43
Nodes (5): lv_event_t, erase_nvs_namespace(), factory_btn_cb(), factory_reset_apply(), reboot_cb()

### Community 87 - "lv_event_t"
Cohesion: 0.40
Nodes (6): alarm_overlay_close(), alarm_overlay_close_cb(), alarm_overlay_silence_cb(), lv_event_t, menu_close(), menu_scrim_cb()

### Community 88 - "ui_pinScreen.c"
Cohesion: 0.47
Nodes (4): lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots()

### Community 89 - "ui_i18n"
Cohesion: 0.13
Nodes (13): lv_event_t, edit_cb(), rebuild(), test_cb(), lv_event_t, delete_cb(), flash_error(), app_user_role_t (+5 more)

### Community 90 - "ota_update.c"
Cohesion: 0.12
Nodes (21): dirent, esp_app_desc, esp_app_desc_t, esp_app_format, esp_ota_ops, esp_partition, esp_partition_t, FILE (+13 more)

### Community 91 - "ui_cfg_apply_visual_mode"
Cohesion: 0.33
Nodes (6): lv_color_t, lv_obj_t, ui_cfg_apply_visual_mode(), ui_cfg_set_theme(), ui_cfg_theme(), visual_color()

### Community 93 - "apply_data_activity_visual"
Cohesion: 0.50
Nodes (4): apply_data_activity_visual(), lv_timer_t, data_activity_timer_cb(), ui_main_signal_data_activity()

### Community 94 - "on_network_ready"
Cohesion: 0.67
Nodes (3): esp_err_t, mdns_svc_start(), on_network_ready()

## Knowledge Gaps
- **133 isolated node(s):** `gen_ota_signing_key.sh script`, `graphify`, `Estructura del repositorio (reorganizado 2026-09-15, mirror de MedGuard)`, `graphify`, `1. GATT` (+128 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **8 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Work-memory lessons

**Preferred sources** — corroborated by past sessions; start here.
- `transport_mqtt_publish_now()` (2× useful, score=1.99980777) _(code changed — re-verify)_

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `appcfg_cache_peek` to `alarm_mgr.c`, `fpm_ble_config.c`, `step_cb`, `ui_label`, `ui_sensorEditScreen.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `ui_mainScreen.c`, `screen_init_task`, `ui_statusbar_controller.c`, `gas_label_color`, `save_cb`, `ui_cfg_preview_brightness`, `ui_cfg.c`, `app_main`, `ui_auth_can`, `ui_netBleScreen.c`, `next`, `ui_cfg_apply_visual_mode`, `on_network_ready`?**
  _High betweenness centrality (0.047) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `screen_init_task` to `at24c256.c`, `ui_statusbar_request_refresh`, `sensors_acq_task`, `main.c`, `appcfg_cache_peek`, `fpm_i2c_guard.c`, `ui_label`, `ft5x06.c`, `ui_wifi_main_icon.c`?**
  _High betweenness centrality (0.033) - this node is a cross-community bridge._
- **Why does `esp_err_to_name()` connect `esp_err_to_name` to `ms5803.c`, `ads1115.c`, `FreeRTOS.h`, `fpm_ble_config.c`, `storage.c`, `http_api.c`, `sensors_runtime.c`, `eth_mgr.c`, `esp_log.h`, `screen_init_task`, `cert_store.c`, `sensors_acq_task`, `fpm_i2c_guard.c`, `save_cb`, `ft5x06.c`, `sd_monitor_task`, `at24c256.c`, `ui_statusbar_request_refresh`, `app_main`, `ui_infoScreen.c`, `ota_update.c`, `on_network_ready`?**
  _High betweenness centrality (0.024) - this node is a cross-community bridge._
- **Are the 73 inferred relationships involving `appcfg_cache_peek()` (e.g. with `alarm_mgr_press_mute_impl()` and `alarm_mgr_process_impl()`) actually correct?**
  _`appcfg_cache_peek()` has 73 INFERRED edges - model-reasoned connections that need verification._
- **Are the 49 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 49 INFERRED edges - model-reasoned connections that need verification._
- **Are the 46 inferred relationships involving `esp_err_to_name()` (e.g. with `cert_store_erase()` and `cert_store_import_from_sd()`) actually correct?**
  _`esp_err_to_name()` has 46 INFERRED edges - model-reasoned connections that need verification._
- **Are the 43 inferred relationships involving `ui_col()` (e.g. with `bleapp_set_connected()` and `ui_bleAppScreen_screen_init()`) actually correct?**
  _`ui_col()` has 43 INFERRED edges - model-reasoned connections that need verification._