idf_component_register(
    SRCS "st7796.c" "ft5x06.c" "touch_io_bounded.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        drivers
        driver 
        freertos 
        esp_driver_ledc
        esp_lcd
        esp_lcd_st7796
        esp_lcd_touch
        esp_lcd_touch_ft5x06
        lvgl
        esp_lvgl_port
        esp_driver_i2c
        sensors_runtime
    PRIV_REQUIRES
        esp_timer

        nvs_flash
)