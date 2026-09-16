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

    // Fuerza un publish INMEDIATO de telemetría/device_status (sin esperar el
    // periodo de 30 s). Se llama al cambiar el estado de alarma para que la nube
    // reciba la alarma al instante. Seguro de llamar desde otra tarea.
    void transport_mqtt_publish_now(void);

#ifdef __cplusplus
}
#endif