#pragma once
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // ¡Nombre corregido!
    void transport_mqtt_on_time_ready(void);
    void transport_mqtt_on_net_down(void);

    // Detiene y libera el cliente MQTT (modo configuración BLE: libera RAM).
    // Idempotente. Se re-inicializa con transport_mqtt_on_time_ready().
    void transport_mqtt_stop(void);

    bool cloud_mgr_connected(void);

#ifdef __cplusplus
}
#endif