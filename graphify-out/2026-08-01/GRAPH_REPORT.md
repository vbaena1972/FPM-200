# Graph Report - .  (2026-08-01)

## Corpus Check
- 176 files · ~309,173 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 883 nodes · 2208 edges · 79 communities (77 shown, 2 thin omitted)
- Extraction: 65% EXTRACTED · 35% INFERRED · 0% AMBIGUOUS · INFERRED: 777 edges (avg confidence: 0.8)
- Token cost: 267,358 input · 0 output

## Community Hubs (Navigation)
- LVGL Screen Widgets
- PIN Entry Screen
- Alarm Manager & Metrics
- Network Core & Events
- Config Storage & MQTT
- BLE Config Bridge (fpm)
- Ethernet Manager
- BLE Transport (NimBLE)
- Screen Lifecycle Headers
- Graphify Pipeline Features
- Sensor Edit Screen
- Certificate Store
- HMI Mockup Screens
- WiFi Manager
- Role-Based Auth (ui_auth)
- UI Config Accessors
- Touch Driver (FT5x06)
- Display Driver (ST7796)
- Sensors Runtime & Telemetry
- Main Boot & SD Card
- Firmware Overview Docs
- ESP-IDF Components
- Memory Diagnostics
- Sensor Diagnostics Screen
- General/Edit Config Handlers
- UI Navigation Stack
- General Settings Screens
- RTC & Time Manager
- Users Management Screen
- MQTT Transport & Config Mode
- Connectivity Status Screen
- WiFi Status Icon
- Login Screen
- WiFi Network Settings
- Task Tracer
- Status Bar Controller
- User Edit Screen
- UI Form & i18n
- WebSocket Stream
- Keypad Screen
- Unit Config Setters
- Timezone Config
- BLE App Screen
- Brightness & Dim Control
- Main Deinit Callbacks
- Graphify Watch Mode
- clock_mgr Component

## God Nodes (most connected - your core abstractions)
1. `appcfg_cache_peek()` - 69 edges
2. `ui_label()` - 53 edges
3. `ui_box()` - 39 edges
4. `ui_col()` - 37 edges
5. `_t()` - 33 edges
6. `ui_card()` - 32 edges
7. `appcfg_save()` - 30 edges
8. `ui_icon()` - 30 edges
9. `ui_nav_load()` - 22 edges
10. `ui_screen_base()` - 20 edges

## Surprising Connections (you probably didn't know these)
- `ui_infoScreen_screen_init()` --calls--> `appmetrics_service_min()`  [INFERRED]
  main/ui/screens/ui_infoScreen.c → components/storage/metrics_store.c
- `ble_json_worker()` --calls--> `appcfg_patch()`  [INFERRED]
  main/main.c → components/storage/storage_extras.c
- `app_main()` --calls--> `task_tracer_start()`  [INFERRED]
  main/main.c → components/task_tracer/task_tracer.c
- `sd_monitor_task()` --calls--> `cert_store_import_from_sd()`  [INFERRED]
  main/main.c → components/cert_store/cert_store.c
- `app_main()` --calls--> `mem_diag_report()`  [INFERRED]
  main/main.c → components/diag/mem_diag.c

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **graphify extraction and analysis pipeline** — _claude_skills_graphify_skill_pipeline, _claude_skills_graphify_skill_ast_extraction, _claude_skills_graphify_skill_semantic_extraction, _claude_skills_graphify_skill_community_detection [EXTRACTED 1.00]
- **BLE app-config flow (contract, bridge, config mode)** — handoff_ble_contract, handoff_fpm_ble_config, handoff_config_mode [EXTRACTED 1.00]
- **FPM-200 HMI handoff documentation set** — handoff_fpm200_firmware, hmi_migration_squareline_to_claude, sesion_hmi_history [INFERRED 0.85]
- **network_core component dependency set** — components_network_core_cmakelists_network_core, components_storage_cmakelists_storage, components_drivers_cmakelists_drivers, components_state_pub_cmakelists_state_pub, components_cert_store_cmakelists_cert_store, components_ui_bind_cmakelists_ui_bind, components_diag_cmakelists_diag, components_transport_http_cmakelists_transport_http, components_transport_ble_cmakelists_transport_ble, components_sensors_runtime_cmakelists_sensors_runtime [EXTRACTED 1.00]
- **ui_bind component dependency set** — components_ui_bind_cmakelists_ui_bind, components_network_core_cmakelists_network_core, components_display_cmakelists_display, components_transport_ble_cmakelists_transport_ble, components_storage_cmakelists_storage, components_sensors_runtime_cmakelists_sensors_runtime, components_drivers_cmakelists_drivers [EXTRACTED 1.00]
- **main firmware entrypoint dependency set** — main_cmakelists_main, components_cert_store_cmakelists_cert_store, components_diag_cmakelists_diag, components_display_cmakelists_display, components_drivers_cmakelists_drivers, components_network_core_cmakelists_network_core, components_sensors_runtime_cmakelists_sensors_runtime, components_state_pub_cmakelists_state_pub, components_storage_cmakelists_storage, components_transport_ble_cmakelists_transport_ble, components_transport_http_cmakelists_transport_http, components_ui_bind_cmakelists_ui_bind, components_task_tracer_cmakelists_task_tracer [EXTRACTED 1.00]
- **mainScreen alarm-state variants (Normal/Alarma/Advertencia/Silenciada)** — mockups_3a_mainscreen_normal_screen, mockups_3b_mainscreen_alarma_screen, mockups_4b_mainscreen_advertencia_screen, mockups_4c_mainscreen_alarma_silenciada_screen [INFERRED 0.90]
- **Critical parameter change flow (edit umbral → confirm → PIN)** — mockups_5b_sensorscreen_editable_screen, mockups_5c_keypad_umbral_screen, mockups_5d_confirmar_cambio_screen, mockups_5a_pindialog_screen [INFERRED 0.85]
- **Settings screens sharing the back-header navigation pattern** — mockups_4d_sensorscreen_screen, mockups_4e_generalscreen_screen, mockups_4f_infoscreen_screen, mockups_4g_conectividad_screen, mockups_5b_sensorscreen_editable_screen, mockups_5e_generalscreen_simple_screen [INFERRED 0.90]

## Communities (79 total, 2 thin omitted)

### Community 0 - "LVGL Screen Widgets"
Cohesion: 0.10
Nodes (79): lv_font_t, ui_bleAppScreen_screen_init(), ui_confirmScreen_screen_init(), lv_event_cb_t, lv_obj_t, conn_card(), ui_connectivityScreen_screen_init(), lv_obj_t (+71 more)

### Community 1 - "PIN Entry Screen"
Cohesion: 0.09
Nodes (50): ui_infoScreen_screen_destroy(), lv_event_t, pin_key_cb(), pin_wrong_flash(), refresh_dots(), ui_pinScreen_screen_destroy(), ui_pinScreen_set_config_entry(), lv_event_t (+42 more)

### Community 2 - "Alarm Manager & Metrics"
Cohesion: 0.07
Nodes (49): app_metrics_t, card_state_t, alarm_mgr_get_current_state(), alarm_mgr_get_sensor_faults(), alarm_mgr_is_muted(), alarm_mgr_press_mute(), alarm_mgr_process(), alarm_clinical_state_t (+41 more)

### Community 3 - "Network Core & Events"
Cohesion: 0.10
Nodes (33): mem_diag_report(), esp_event_base_t, on_eth_event(), on_ip_event(), esp_err_t, esp_event_base_t, net_core_init(), on_eth_event() (+25 more)

### Community 4 - "Config Storage & MQTT"
Cohesion: 0.15
Nodes (34): cfg_result_t, esp_event_base_t, mqtt_event(), appcfg_cache_get(), appcfg_defaults(), appcfg_load(), appcfg_migrate(), app_user_t (+26 more)

### Community 5 - "BLE Config Bridge (fpm)"
Cohesion: 0.17
Nodes (33): appcfg_cache_reload(), apply_channel_obj(), apply_netif(), build_channel_obj(), AppConfig, cJSON, color_to_gas(), cpy() (+25 more)

### Community 6 - "Ethernet Manager"
Cohesion: 0.11
Nodes (26): appcfg_to_eth_view(), apply_ip_config(), AppConfig, esp_err_t, esp_netif_t, eth_mgr_init(), eth_mgr_init_from_storage(), eth_mgr_start() (+18 more)

### Community 7 - "BLE Transport (NimBLE)"
Cohesion: 0.13
Nodes (22): ble_cmd_handler_t, append_json(), ble_on_sync(), ble_start_adv(), esp_err_t, free_config_buffer(), gap_event(), transport_ble_init() (+14 more)

### Community 8 - "Screen Lifecycle Headers"
Cohesion: 0.10
Nodes (4): ui_confirmScreen_screen_destroy(), ui_datetimeScreen_screen_destroy(), lv_event_cb_t, ui_sd_finish_restart()

### Community 9 - "Graphify Pipeline Features"
Cohesion: 0.10
Nodes (23): graphify /graphify trigger instruction, graphify add URL ingest, Export targets (Neo4j, FalkorDB, SVG, GraphML, MCP, wiki), graphify MCP stdio server, Confidence taxonomy (EXTRACTED/INFERRED/AMBIGUOUS), Deterministic node ID format, Extraction subagent prompt spec, GitHub clone and cross-repo merge (+15 more)

### Community 10 - "Sensor Edit Screen"
Cohesion: 0.16
Nodes (22): lv_event_t, lv_obj_t, ui_edit_target_t, card(), f_switch(), flow_card(), limit_edit(), limit_step() (+14 more)

### Community 11 - "Certificate Store"
Cohesion: 0.19
Nodes (19): cert_info_t, cert_kind_t, esp_err_t, cert_store_erase(), cert_store_erase_all(), cert_store_import_from_sd(), cert_store_info(), cert_store_load() (+11 more)

### Community 12 - "HMI Mockup Screens"
Cohesion: 0.19
Nodes (21): Alert Banner (Normal/Alarma/Advertencia/Silenciada), Sensor Gauge Card (Presión / Flujo), mainScreen · Normal, Status Bar (marca + BLE/WiFi/red/nube + reloj + ajustes), mainScreen · Alarma, Splash / Boot, mainScreen · Advertencia, mainScreen · Alarma Silenciada (+13 more)

### Community 13 - "WiFi Manager"
Cohesion: 0.19
Nodes (17): apply_ip_mode(), AppConfig, esp_err_t, wifi_status_t, net_cfg_equals(), notify(), wifi_apply_cb(), wifi_mgr_apply_from_cache() (+9 more)

### Community 14 - "Role-Based Auth (ui_auth)"
Cohesion: 0.23
Nodes (19): app_user_role_t, app_user_t, ui_auth_active(), ui_auth_can(), ui_auth_can_edit_user(), ui_auth_can_manage(), ui_auth_current_user(), ui_auth_factory() (+11 more)

### Community 15 - "UI Config Accessors"
Cohesion: 0.14
Nodes (18): appcfg_cache_peek(), appcfg_save(), lv_color_t, lv_obj_t, ui_auth_user_count(), ui_cfg_alarm_tone(), ui_cfg_apply_visual_mode(), ui_cfg_check_pin() (+10 more)

### Community 16 - "Touch Driver (FT5x06)"
Cohesion: 0.15
Nodes (9): bsp_touch_config_t, bsp_display_get_input_dev(), bsp_display_indev_init(), bsp_touch_new(), esp_err_t, i2c_master_bus_handle_t, lv_display_t, esp_lcd_touch_handle_t (+1 more)

### Community 17 - "Display Driver (ST7796)"
Cohesion: 0.26
Nodes (15): bsp_display_backlight_off(), bsp_display_backlight_on(), bsp_display_brightness_init(), bsp_display_brightness_set(), bsp_display_lock(), bsp_display_off(), bsp_display_on(), bsp_display_rotate() (+7 more)

### Community 18 - "Sensors Runtime & Telemetry"
Cohesion: 0.31
Nodes (14): aws_telemetry_task(), AppConfig, sensor_sample_t, lock(), sensors_dummy_task(), sensors_runtime_get_faults(), sensors_runtime_get_last(), sensors_runtime_get_min_max() (+6 more)

### Community 19 - "Main Boot & SD Card"
Cohesion: 0.15
Nodes (10): esp_trace_open_params_t, ble_json_worker(), bsp_i2c_init(), esp_err_t, esp_trace_get_user_params(), mount_sdcard_hotplug(), sd_monitor_task(), unmount_sdcard_hotplug() (+2 more)

### Community 20 - "Firmware Overview Docs"
Cohesion: 0.20
Nodes (14): esp32s3_hmi_skeleton ESP-IDF project, BLE contract FPM <-> Sensvax app (GATT 9f3c1000), CONFIG_LV_USE_CLIB_MALLOC watchdog fix, config_mode (BLE provisioning: stop WiFi/AWS, raise BLE), ClaudeHMI-FW FPM-200 Axira firmware, fpm_ble_config AppConfig<->JSON bridge, taskLVGL watchdog bug (lv_realloc loop), ui_theme design tokens and Tabler icon fonts (+6 more)

### Community 21 - "ESP-IDF Components"
Cohesion: 0.36
Nodes (14): cert_store component, diag component, display component, WT32-SC01 Plus BSP board, drivers component, network_core component, sensors_runtime component, state_pub component (+6 more)

### Community 22 - "Memory Diagnostics"
Cohesion: 0.20
Nodes (11): eTaskState, diag_timer_cb(), mem_diag_report_full(), mem_diag_report_heap_full(), mem_diag_report_tasks(), mem_diag_start_periodic(), task_state_str(), alarm_mgr_init() (+3 more)

### Community 23 - "Sensor Diagnostics Screen"
Cohesion: 0.22
Nodes (12): alarm_mgr_test_buzzer(), lv_event_t, edit_cb(), rebuild(), step_cb(), test_cb(), ui_sensorDiagScreen_screen_destroy(), volume_cb() (+4 more)

### Community 24 - "General/Edit Config Handlers"
Cohesion: 0.18
Nodes (14): apply_authenticated_cb(), lv_event_t, general_loaded_cb(), allow_decimal(), ui_cfg_brightness(), ui_cfg_flow_to_disp(), ui_cfg_flow_unit(), ui_cfg_press_fmt() (+6 more)

### Community 25 - "UI Navigation Stack"
Cohesion: 0.23
Nodes (12): ble_finish_async(), lv_event_t, lv_obj_t, ui_nav_back(), ui_nav_back_event_cb(), ui_nav_current(), ui_nav_init(), ui_nav_pop_to() (+4 more)

### Community 26 - "General Settings Screens"
Cohesion: 0.24
Nodes (10): lv_event_t, protected_back_cb(), ui_generalScreen_screen_destroy(), lv_event_t, dim_cb(), lang_cb(), rebuild(), retranslate_parent_general() (+2 more)

### Community 27 - "RTC & Time Manager"
Cohesion: 0.24
Nodes (9): bcd2dec(), esp_err_t, i2c_master_bus_handle_t, dec2bcd(), rtc_rv3028_get_time(), rtc_rv3028_init(), rtc_rv3028_set_time(), time_mgr_init() (+1 more)

### Community 28 - "Users Management Screen"
Cohesion: 0.27
Nodes (11): ui_userEditScreen_set_index(), add_cb(), add_user_row(), app_user_role_t, lv_event_t, lv_obj_t, factory_pin_cb(), role_color() (+3 more)

### Community 29 - "MQTT Transport & Config Mode"
Cohesion: 0.25
Nodes (7): prepare_pem(), transport_mqtt_on_time_ready(), transport_mqtt_stop(), esp_err_t, config_mode_enter(), config_mode_exit(), log_heap()

### Community 30 - "Connectivity Status Screen"
Cohesion: 0.20
Nodes (9): cloud_mgr_connected(), wifi_mgr_get_netinfo(), transport_ble_is_connected(), conn_status_t, cpy(), gather(), rssi_pct(), ui_connectivityScreen_screen_destroy() (+1 more)

### Community 31 - "WiFi Status Icon"
Cohesion: 0.24
Nodes (10): wifi_mgr_set_status_cb(), apply_icon(), lv_obj_t, lv_timer_t, wifi_status_t, timer_cb(), ui_wifi_main_icon_init(), ui_wifi_rssi_to_bars() (+2 more)

### Community 32 - "Login Screen"
Cohesion: 0.29
Nodes (10): app_user_role_t, app_user_t, lv_event_t, key_cb(), refresh(), role_color(), role_name(), select_user() (+2 more)

### Community 33 - "WiFi Network Settings"
Cohesion: 0.22
Nodes (8): eth_mgr_is_up(), esp_event_base_t, log_dns_info(), wifi_event_handler(), lv_event_t, save_cb(), set_str(), ui_netWifiScreen_screen_destroy()

### Community 34 - "Task Tracer"
Cohesion: 0.31
Nodes (7): eTaskState, state_str(), task_tracer_dump_now(), task_tracer_start(), timer_cb(), track_get_and_update(), TaskHandle_t

### Community 35 - "Status Bar Controller"
Cohesion: 0.36
Nodes (8): lv_obj_t, lv_timer_t, set_glyph(), set_visible(), statusbar_timer_cb(), ui_statusbar_controller_init(), ui_statusbar_request_refresh(), ui_statusbar_set_enabled()

### Community 36 - "User Edit Screen"
Cohesion: 0.33
Nodes (7): app_user_role_t, lv_event_t, delete_cb(), flash_error(), save_cb(), selected_role(), ui_userEditScreen_screen_destroy()

### Community 37 - "UI Form & i18n"
Cohesion: 0.25
Nodes (3): lv_event_t, kb_event_cb(), ta_event_cb()

### Community 38 - "WebSocket Stream"
Cohesion: 0.36
Nodes (7): esp_err_t, httpd_handle_t, httpd_req_t, ws_emit_cfg_changed(), ws_handler(), ws_stream_broadcast(), ws_stream_start()

### Community 39 - "Keypad Screen"
Cohesion: 0.32
Nodes (6): accept_cb(), lv_event_t, key_cb(), refresh_value(), ui_keypadScreen_screen_destroy(), ui_edit_set_new()

### Community 40 - "Unit Config Setters"
Cohesion: 0.39
Nodes (8): unit_cb(), AppConfig, save_and_refresh(), set_str(), ui_cfg_set_flow_unit(), ui_cfg_set_gas(), ui_cfg_set_lang(), ui_cfg_set_pressure_unit()

### Community 41 - "Timezone Config"
Cohesion: 0.29
Nodes (7): lv_event_t, next(), ui_cfg_apply_timezone(), ui_cfg_set_tz_index(), ui_cfg_tz_count(), ui_cfg_tz_index(), ui_cfg_tz_label()

### Community 42 - "BLE App Screen"
Cohesion: 0.40
Nodes (4): bleapp_loaded_cb(), bleapp_unloaded_cb(), lv_event_t, ui_bleAppScreen_screen_destroy()

### Community 43 - "Brightness & Dim Control"
Cohesion: 0.47
Nodes (6): bright_cb(), clamp_brightness(), ui_cfg_dim_minutes(), ui_cfg_preview_brightness(), ui_cfg_set_brightness(), display_idle_cb()

### Community 44 - "Main Deinit Callbacks"
Cohesion: 0.40
Nodes (5): ui_statusbar_controller_deinit(), ui_wifi_main_icon_deinit(), lv_event_t, restart_btn_event_cb(), ui_main_deinit_cb()

## Knowledge Gaps
- **20 isolated node(s):** `graphify /graphify trigger instruction`, `Knowledge graph (nodes, edges, communities)`, `Structural AST extraction (Part A)`, `Community detection and god nodes`, `graphify add URL ingest` (+15 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **2 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `appcfg_cache_peek()` connect `UI Config Accessors` to `LVGL Screen Widgets`, `Alarm Manager & Metrics`, `Config Storage & MQTT`, `BLE Config Bridge (fpm)`, `Ethernet Manager`, `BLE Transport (NimBLE)`, `Sensor Edit Screen`, `Certificate Store`, `Role-Based Auth (ui_auth)`, `Display Driver (ST7796)`, `Memory Diagnostics`, `Sensor Diagnostics Screen`, `General/Edit Config Handlers`, `UI Navigation Stack`, `Connectivity Status Screen`, `WiFi Network Settings`, `Unit Config Setters`, `Timezone Config`, `Brightness & Dim Control`?**
  _High betweenness centrality (0.249) - this node is a cross-community bridge._
- **Why does `app_main()` connect `Memory Diagnostics` to `Task Tracer`, `Network Core & Events`, `Config Storage & MQTT`, `BLE Config Bridge (fpm)`, `Timezone Config`, `UI Config Accessors`, `Sensors Runtime & Telemetry`, `Main Boot & SD Card`?**
  _High betweenness centrality (0.095) - this node is a cross-community bridge._
- **Why does `screen_init_task()` connect `Display Driver (ST7796)` to `Status Bar Controller`, `UI Config Accessors`, `Touch Driver (FT5x06)`, `Main Boot & SD Card`, `UI Navigation Stack`, `RTC & Time Manager`, `WiFi Status Icon`?**
  _High betweenness centrality (0.087) - this node is a cross-community bridge._
- **Are the 66 inferred relationships involving `appcfg_cache_peek()` (e.g. with `fpm_ble_channel_apply_json()` and `fpm_ble_channel_read_json()`) actually correct?**
  _`appcfg_cache_peek()` has 66 INFERRED edges - model-reasoned connections that need verification._
- **Are the 44 inferred relationships involving `ui_label()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_label()` has 44 INFERRED edges - model-reasoned connections that need verification._
- **Are the 33 inferred relationships involving `ui_box()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_box()` has 33 INFERRED edges - model-reasoned connections that need verification._
- **Are the 35 inferred relationships involving `ui_col()` (e.g. with `ui_bleAppScreen_screen_init()` and `ui_confirmScreen_screen_init()`) actually correct?**
  _`ui_col()` has 35 INFERRED edges - model-reasoned connections that need verification._