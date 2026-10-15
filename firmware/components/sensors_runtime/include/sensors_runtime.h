#ifndef SENSORS_RUNTIME_H
#define SENSORS_RUNTIME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "storage.h"  // Para AppConfig (cfg->sensors.*)
#include "driver/i2c_master.h" // Para el handle del bus (recuperacion de bus)

/**
 * Muestra básica de sensores en unidades internas (SI):
 *  - Presión: kPa
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
#define SENSOR_INVALID_PRESSURE 1u
#define SENSOR_INVALID_FLOW 2u
#define SENSOR_INVALID_TEMP 4u
typedef struct
{
    int64_t ts_ms;        ///< timestamp en milisegundos (esp_timer_get_time()/1000)
    float   pressure_kpa; ///< presión en kPa
    float   flow_lpm;     ///< flujo en L/min
    float   temp_c;       ///< temperatura del sensor de presión (MS5803) en °C
    uint32_t invalid_mask; // Retained values are not fresh measurements.
} sensor_sample_t;

/**
 * Inicializa el runtime de sensores:
 *  - Crea el buffer circular
 *  - Inicializa mutex
 *  - Arranca la tarea dummy de generación de muestras (opcional, para pruebas)
 *
 * IMPORTANTE:
 *  - cfg puede ser NULL; en ese caso se usan defaults internos para la tarea dummy.
 *  - Cuando luego tengas el driver real, podrás llamar a sensors_runtime_push_sample()
 *    desde tu propia tarea en vez de usar la dummy.
 */
// Call once after EEPROM init, before touch/display and acquisition tasks.
void sensors_runtime_preload_calibration(void);
bool sensors_runtime_init(const AppConfig *cfg);

/**
 * Registra el handle del bus I2C 1 (sensores) para permitir la recuperacion
 * del bus (reset del controlador) cuando se detecta que quedo colgado.
 * Llamar antes de sensors_runtime_init. Opcional: si no se registra, la
 * recuperacion de bus queda deshabilitada.
 */
void sensors_runtime_set_bus(i2c_master_bus_handle_t bus);

/**
 * true si el bus I2C 1 (compartido: MS5803/ADS/EEPROM/RTC/BMP280 + touch FT5x06)
 * se detecto colgado y sigue caido. El driver de touch lo consulta para NO lanzar
 * su lectura I2C mientras el bus esta caido (esa lectura usa timeout infinito en
 * esp_lcd y bloquearia el mutex de display para siempre -> UI congelada, buzzer
 * atascado). Se limpia solo cuando el bus se recupera.
 */
bool sensors_runtime_bus1_hung(void);

/**
 * Actualiza la configuración (por si cambias calibración, etc).
 * Por ahora la guardamos solo por si luego quieres usar cal.* aquí.
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
 * Obtiene la última muestra disponible (la más reciente).
 * Devuelve true si hay datos válidos, false si el buffer está vacío.
 */
bool sensors_runtime_get_last(sensor_sample_t *out);
void sensors_runtime_get_ages(int64_t *pressure_ms, int64_t *flow_ms);
uint32_t sensors_runtime_get_faults(void);
void sensors_runtime_report_faults(uint32_t faults);
/* true mientras el FS7 (sensor termico) se estabiliza tras arrancar o habilitarse:
 * el flujo viene marcado invalido pero no es una falla (no hay STALE). */
bool sensors_runtime_flow_warming(void);
/* Progreso del calentamiento para la HMI (1.5.33): ms desde que empezo (0 si no
 * esta calentando). */
uint32_t sensors_runtime_flow_warming_elapsed_ms(void);

/**
 * Tara de presión (opción B, gauge por offset local):
 * captura la presión absoluta actual (atmosférica, con la línea venteada)
 * como el cero manométrico, activa el modo gauge y lo persiste en la EEPROM.
 * A partir de ahí, la presión reportada es: absoluta - referencia_tarada.
 *
 * Debe llamarse con la línea DESPRESURIZADA (abierta a la atmósfera).
 * Devuelve ESP_OK si había una lectura válida y se guardó.
 */
esp_err_t sensors_runtime_tare_pressure(void);

/**
 * Cambia el modo de presión: false = absoluta, true = gauge (usa la referencia
 * ya tarada). Persiste en EEPROM. Útil para volver a absoluta sin re-tarar.
 */
esp_err_t sensors_runtime_set_pressure_gauge(bool gauge);

/** true si el modo actual es gauge (manométrica). */
bool sensors_runtime_is_pressure_gauge(void);

/**
 * Auto-cero (tara) del flujo: fija el voltaje CTA actual del FS7 como el cero
 * (u0). Dispara con la línea SIN flujo. El cero del FS7 deriva, así que esto lo
 * corrige sin reflashear. Devuelve ESP_OK si había lectura válida.
 */
esp_err_t sensors_runtime_tare_flow(void);

/**
 * Calibración de 1 punto del flujo: con un caudal CONOCIDO (ref_slm, p.ej. del
 * patrón SFM3300) ajusta flow_scale para que el FS7 coincida. Hacer la tara de
 * flujo primero (cero), luego este a un caudal estable. Persiste en EEPROM.
 */
esp_err_t sensors_runtime_cal_flow_point(float ref_slm);

/**
 * Valores de DIAGNOSTICO en vivo para el dashboard (temporales, se quitan en
 * producción): presión atmosférica del BMP280 (kPa), referencia SFM3300 (slm)
 * y flujo del FS7 (L/min). Cualquier puntero puede ser NULL. Devuelve NAN si
 * aún no hay lectura del sensor correspondiente.
 */
void sensors_runtime_get_debug(float *atm_kpa, float *sfm_slm, float *fs7_lpm);

/**
 * Calcula min y max de presión y flujo en una ventana de tiempo [now - window_ms, now].
 * - window_ms: ventana en milisegundos (ej: 60000 para 1 minuto).
 * - min_out / max_out pueden ser NULL si no te interesa alguno.
 * Devuelve true si encontró muestras en la ventana, false si no hay datos.
 */
bool sensors_runtime_get_min_max(int64_t window_ms,
                                 sensor_sample_t *min_out,
                                 sensor_sample_t *max_out);

/**
 * 1.6.8: min/max de las ULTIMAS 24 h (cubetas horarias, minmax24.h). Rellena
 * pressure_kpa y flow_lpm de min_out/max_out; NAN en la senal que no tuvo dato
 * valido. Devuelve true si alguna de las dos tiene datos.
 */
bool sensors_runtime_get_min_max_24h(sensor_sample_t *min_out, sensor_sample_t *max_out);

/* 1.6.9: copia / restauracion (fusion) de las cubetas de 24 h, para que flow_meter
 * las guarde en NVS y las recupere tras un reinicio. */
#include "minmax24.h"
void sensors_runtime_mm24_copy(mm24_t *out);
void sensors_runtime_mm24_restore(const mm24_t *in);

/**
 * Extrae una serie de muestras dentro de la ventana de tiempo (para gráficas).
 * - window_ms: ventana hacia atrás en ms desde "ahora".
 * - out: buffer donde se copiarán las muestras (en orden cronológico ascendente).
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

/**
 * Estadistica de ventana por senal (contrato AWS v2, base para ML).
 * n == 0 => no hubo muestras validas en la ventana (no publicar stats).
 * std es la desviacion estandar POBLACIONAL (sqrt(sumsq/n - mean^2)).
 * mean es el promedio REAL (suma/conteo), NO (min+max)/2.
 */
typedef struct {
    uint32_t n;    ///< muestras validas en la ventana
    float    min;
    float    max;
    float    mean; ///< promedio real
    float    std;  ///< desviacion estandar poblacional
} sensor_signal_stats_t;

typedef struct {
    sensor_signal_stats_t pressure; ///< kPa
    sensor_signal_stats_t flow;     ///< L/min
    sensor_signal_stats_t temp;     ///< C
} sensor_window_stats_t;

/**
 * Calcula, en una sola pasada sobre el buffer, la estadistica de ventana
 * [now - window_ms, now] por senal (presion/flujo/temperatura): n, min, max,
 * mean y std. Espejo del collect_window de MedGuard para el contrato AWS v2.
 * Ignora muestras no finitas (NaN/inf) senal por senal.
 * Devuelve true si hubo al menos una muestra en la ventana; false si no.
 */
bool sensors_runtime_get_window_stats(int64_t window_ms,
                                      sensor_window_stats_t *out);
#endif // SENSORS_RUNTIME_H
