#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// Estados de Alarma segÃƒÂºn prioridad NFPA 99
typedef enum {
    ALARM_STATE_NORMAL = 0,
    ALARM_STATE_WARNING,    // PrecauciÃƒÂ³n (Amarillo) - Fuera de rango ÃƒÂ³ptimo
    ALARM_STATE_ALERT       // CrÃƒÂ­tico (Rojo) - PresiÃƒÂ³n extrema o falla de lÃƒÂ­nea
} alarm_clinical_state_t;

// Inicializa el perifÃƒÂ©rico PWM (LEDC) para el Buzzer
esp_err_t alarm_mgr_init(int buzzer_gpio);

// Procesador central: EvalÃƒÂºa los sensores y maneja los timings de Mute
bool alarm_mgr_process(float current_pressure, float current_flow, uint32_t sensor_faults);

// AcciÃƒÂ³n fÃƒÂ­sica al presionar el botÃƒÂ³n de MUTE del HMI
void alarm_mgr_press_mute(void);
void alarm_mgr_test_buzzer(void);

// Callback opcional invocado cuando el estado clinico CAMBIA (NORMAL/WARNING/ALERT).
// Lo usa la capa de red para forzar un publish inmediato a la nube sin acoplar
// este componente con network_core. Registrar una sola vez tras el init.
typedef void (*alarm_state_change_cb_t)(alarm_clinical_state_t new_state);
void alarm_mgr_set_state_change_cb(alarm_state_change_cb_t cb);

// Inhibe el buzzer para estados NO criticos (WARNING/fallo tecnico); un ALERT
// critico de presion sigue sonando. La UI lo activa mientras muestra la pantalla
// de login/config para que no pite al ingresar el PIN (#4).
void alarm_mgr_set_audio_inhibit_noncritical(bool inhibit);

// Getters para que la pantalla (UI) sepa de quÃƒÂ© color pintarse
alarm_clinical_state_t alarm_mgr_get_current_state(void);
bool alarm_mgr_is_muted(void);
uint32_t alarm_mgr_get_sensor_faults(void);

#ifdef __cplusplus
}
#endif