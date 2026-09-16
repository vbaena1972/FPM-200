idf_component_register(
    SRCS "sensors_runtime.c"
    INCLUDE_DIRS "include"
    REQUIRES
        freertos
        esp_timer
        esp_driver_i2c
        storage        # sensors_runtime.h expone AppConfig (storage.h) en su API publica
    PRIV_REQUIRES
        drivers
)
