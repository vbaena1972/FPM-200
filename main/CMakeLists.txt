#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "ui/*.c")

idf_component_register(SRCS "main.c" "config_mode.c" "lv_port_mem.c" ${SRC_UI}
                    INCLUDE_DIRS "." "ui"
                    REQUIRES
                         app_update
                         bt
                         cert_store
                         cjson
                         diag
                         display 
                         drivers
                         esp_driver_ledc
                         esp_event 
                         esp_http_server
                         esp_sysview 
                         esp_trace
                         esp_lcd
                         esp_netif
                         lvgl 
                         network_core
                         nvs_flash 
                         sensors_runtime
                         state_pub
                         storage 
                         transport_ble
                         transport_http
                         ui_bind
                         task_tracer
                         )

# LVGL (componente gestionado, modo CUSTOM malloc) llama a lv_malloc_core/etc.,
# que definimos en main/lv_port_mem.c. Es una referencia "hacia atrás" (LVGL no
# depende de main), así que el linker no jala ese objeto del archivo por sí solo.
# Forzamos su inclusión pidiendo un símbolo como indefinido en el enlace final.
target_link_libraries(${COMPONENT_LIB} INTERFACE "-Wl,-u,lv_malloc_core")
