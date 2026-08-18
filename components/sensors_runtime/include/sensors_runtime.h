#ifndef SENSORS_RUNTIME_H
#define SENSORS_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "storage.h"  // Para AppConfig (cfg->sensors.*)
#include "driver/i2c_master.h" // Para el handle del bus (recuperacion de bus)

/**
 * Muestra bÃƒÂ¡sica de sensores en unidades internas (SI):
 *  - PresiÃƒÂ³n: kPa
 *  - Flujo:   L/min
 */
typedef enum {
    SENSOR_FAULT_NONE        = 0,
    SENSOR_FAULT_NO_DATA     = 1u << 0,
    SENSOR_FAULT_STALE       = 1u << 1,
    SENSOR_FAULT_INVALID     = 1u << 2,
    SENSOR_FAULT_MODULE_I2C  = 1u << 3,
    SENSOR_FAULT_EEPROM_CRC  = 1u << 4,
    SENSOR_FAULT_CONFIG      = 1u << 5
} sensor_fault_t;
typedef struct
{
    int64_t ts_ms;        ///< timestamp en milisegundos (esp_timer_get_time()/1000)
    float   pressure_kpa; ///< presiÃƒÂ³n en kPa
    float   flow_lpm;     ///< flujo en L/min
    float   temp_c;       ///< temperatura del sensor de presiÃ³n (MS5803) en Â°C
} sensor_sample_t;

/**
 * Inicializa el runtime de sensores:
 *  - Crea el buffer circular
 *  - Inicializa mutex
 *  - Arranca la tarea dummy de generaciÃƒÂ³n de muestras (opcional, para pruebas)
 *
 * IMPORTANTE:
 *  - cfg puede ser NULL; en ese caso se usan defaults internos para la tarea dummy.
 *  - Cuando luego tengas el driver real, podrÃƒÂ¡s llamar a sensors_runtime_push_sample()
 *    desde tu propia tarea en vez de usar la dummy.
 */
bool sensors_runtime_init(const AppConfig *cfg);

/**
 * Registra el handle del bus I2C 1 (sensores) para permitir la recuperacion
 * del bus (reset del controlador) cuando se detecta que quedo colgado.
 * Llamar antes de sensors_runtime_init. Opcional: si no se registra, la
 * recuperacion de bus queda deshabilitada.
 */
void sensors_runtime_set_bus(i2c_master_bus_handle_t bus);

/**
 * Actualiza la configuraciÃƒÂ³n (por si cambias calibraciÃƒÂ³n, etc).
 * Por ahora la guardamos solo por si luego quieres usar cal.* aquÃƒÂ­.
 */
void sensors_runtime_update_config(const AppConfig *cfg);

/**
 * Inserta una muestra en el buffer circular.
 * Se puede llamar:
 *  - desde la tarea dummy interna
 *  - o desde tu driver real de sensores.
 */
void sensors_runtime_push_sample(const sensor_sample_t *s);

/**
 * Obtiene la ÃƒÂºltima muestra disponible (la mÃƒÂ¡s reciente).
 * Devuelve true si hay datos vÃƒÂ¡lidos, false si el buffer estÃƒÂ¡ vacÃƒÂ­o.
 */
bool sensors_runtime_get_last(sensor_sample_t *out);
uint32_t sensors_runtime_get_faults(void);
void sensors_runtime_report_faults(uint32_t faults);

/**
 * Tara de presiÃ³n (opciÃ³n B, gauge por offset local):
 * captura la presiÃ³n absoluta actual (atmosfÃ©rica, con la lÃ­nea venteada)
 * como el cero manomÃ©trico, activa el modo gauge y lo persiste en la EEPROM.
 * A partir de ahÃ­, la presiÃ³n reportada es: absoluta - referencia_tarada.
 *
 * Debe llamarse con la lÃ­nea DESPRESURIZADA (abierta a la atmÃ³sfera).
 * Devuelve ESP_OK si habÃ­a una lectura vÃ¡lida y se guardÃ³.
 */
esp_err_t sensors_runtime_tare_pressure(void);

/**
 * Cambia el modo de presiÃ³n: false = absoluta, true = gauge (usa la referencia
 * ya tarada). Persiste en EEPROM. Ãštil para volver a absoluta sin re-tarar.
 */
esp_err_t sensors_runtime_set_pressure_gauge(bool gauge);

/** true si el modo actual es gauge (manomÃ©trica). */
bool sensors_runtime_is_pressure_gauge(void);

/**
 * Auto-cero (tara) del flujo: fija el voltaje CTA actual del FS7 como el cero
 * (u0). Dispara con la lÃ­nea SIN flujo. El cero del FS7 deriva, asÃ­ que esto lo
 * corrige sin reflashear. Devuelve ESP_OK si habÃ­a lectura vÃ¡lida.
 */
esp_err_t sensors_runtime_tare_flow(void);

/**
 * CalibraciÃ³n de 1 punto del flujo: con un caudal CONOCIDO (ref_slm, p.ej. del
 * patrÃ³n SFM3300) ajusta flow_scale para que el FS7 coincida. Hacer la tara de
 * flujo primero (cero), luego este a un caudal estable. Persiste en EEPROM.
 */
esp_err_t sensors_runtime_cal_flow_point(float ref_slm);

/**
 * Calcula min y max de presiÃƒÂ³n y flujo en una ventana de tiempo [now - window_ms, now].
 * - window_ms: ventana en milisegundos (ej: 60000 para 1 minuto).
 * - min_out / max_out pueden ser NULL si no te interesa alguno.
 * Devuelve true si encontrÃƒÂ³ muestras en la ventana, false si no hay datos.
 */
bool sensors_runtime_get_min_max(int64_t window_ms,
                                 sensor_sample_t *min_out,
                                 sensor_sample_t *max_out);

/**
 * Extrae una serie de muestras dentro de la ventana de tiempo (para grÃƒÂ¡ficas).
 * - window_ms: ventana hacia atrÃƒÂ¡s en ms desde "ahora".
 * - out: buffer donde se copiarÃƒÂ¡n las muestras (en orden cronolÃƒÂ³gico ascendente).
 * - max: capacidad del buffer out.
 * - out_count: cantidad real copiada.
 * Devuelve true si hay al menos 1 muestra copiada.
 */
bool sensors_runtime_get_series(int64_t window_ms,
                                sensor_sample_t *out,
                                size_t max,
                                size_t *out_count);

                                /**
 * Extrae Min, Max y Promedio (Avg) de la ventana de tiempo especificada.
 * Ideal para construir el Payload JSON de AWS IoT.
 */
bool sensors_runtime_get_aws_telemetry(int64_t window_ms,
                                       float *p_min, float *p_max, float *p_avg,
                                       float *f_min, float *f_max, float *f_avg);
#endif // SENSORS_RUNTIME_H
