#include "sensors_runtime.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "driver/i2c_master.h" // i2c_master_bus_reset (recuperacion de bus)

// Drivers reales de sensores/memoria
#include "ms5803.h"
#include "ads1115.h"
#include "at24c256.h"
#include "sfm3300.h" // caudalimetro de referencia (calibracion del FS7)
#include "bmp280.h"  // barometro de referencia atmosferica (cero de presion)

//------------------------------------------------------------------
// ParÃ¡metros del buffer y tarea de adquisiciÃ³n
//------------------------------------------------------------------

#define SENSORS_BUFFER_LEN 600      // ~600 muestras
#define SENSORS_DEFAULT_PERIOD_MS 100 // 10 Hz de adquisiciÃ³n real
#define SENSORS_TASK_STACK 4096
#define SENSORS_TASK_PRIO 5

// AntirrebÃ³te de fault de I2C: un NACK puntual en el bus compartido
// (MS5803/ADS/EEPROM) es comÃºn y NO debe disparar la alarma crÃ­tica ni
// parpadear la lectura a 0. Solo declaramos fault tras N ciclos seguidos
// fallidos (a 100 ms/ciclo, 3 = ~300 ms). Una pÃ©rdida sostenida sÃ­ alarma.
#define SENSORS_FAULT_DEBOUNCE 3

// Si el SFM3300 (bus 2) falla lecturas de forma sostenida, reintentamos
// arrancar la medicion continua cada N ciclos (a 100 ms, 50 = ~5 s).
#define SFM_RESTART_AFTER_MISSES 50

// Recuperacion del bus I2C 1: si MS5803 y ADS fallan JUNTOS de forma sostenida
// el bus quedo colgado (un esclavo con SDA en bajo). Reseteamos el controlador
// cada N ciclos mientras siga caido (a 100 ms, 5 = ~500 ms). Antes eran 30 (~3 s),
// demasiado lento: mientras tanto la tarea LVGL martillaba el touch (mismo bus 1)
// y re-colgaba el bus antes de que el reset lo recuperara. Con el touch ahora
// respetando el flag bus1_hung (ver ft5x06.c), un reset frecuente sÃ­ recupera.
#define BUS_RECOVER_EVERY 5

// Hook de prueba: si es 1, hace una tara automÃ¡tica ~2 s despuÃ©s del arranque
// (con la lÃ­nea a la atmÃ³sfera) para validar el modo gauge por log. Ponlo a 0
// en producciÃ³n: la tara real se dispara bajo demanda (BLE / servicio).
#define SENSORS_TARE_ON_BOOT 0

static const char *TAG = "sensors_runtime";

//------------------------------------------------------------------
// ConfiguraciÃ³n de calibraciÃ³n persistida en EEPROM AT24C256C
//------------------------------------------------------------------

#define SENS_CFG_EEPROM_ADDR 0x0000
#define SENS_CFG_MAGIC 0x53454E31u // "SEN1"
#define SENS_CFG_VERSION 5 // v5: recalibracion FS7 (2026-08-21). Sube al cambiar defaults

// Modos de presiÃ³n
#define PRESSURE_MODE_ABS   0 // absoluta (por defecto)
#define PRESSURE_MODE_GAUGE 1 // manomÃ©trica: absoluta - referencia tarada

// Modelo de flujo FS7 (CTA / ley de King): U = U0 + k * v^n
//   U       = salida CTA reconstruida = V_ain0 / divider
//   v       = velocidad (m/s)  ->  flujo_lpm = v * flow_scale + flow_offset
// Valores por defecto tomados del datasheet FS7 (AFFS7_E) y del divisor
// R1(240k)/R2(100k) del esquematico: ratio = 100/(240+100) = 0.294118
typedef struct __attribute__((packed))
{
    uint32_t magic;
    uint16_t version;
    uint16_t size;

    // PresiÃ³n (MS5803) -> ajuste lineal opcional sobre kPa
    float pressure_offset_kpa; // sumado
    float pressure_scale;      // multiplicativo (1.0 = sin cambio)
    float pressure_ref_kpa;    // referencia gauge (atmosfÃ©rica tarada, cero manomÃ©trico)

    // Flujo (FS7 va ADS1115 AIN0)
    float fs7_divider;   // V_ain0 = U * divider
    float fs7_u0;        // salida CTA a flujo cero (V)
    float fs7_k;         // constante fluÃ­dica
    float fs7_n;         // exponente (~0.5)
    float flow_scale;    // v(m/s) -> L/min
    float flow_offset;   // sumado (L/min)

    uint8_t flow_enabled;      // 0 = FS7 no conectado -> flujo=0 (evita ruido)
    uint8_t pressure_mode;     // PRESSURE_MODE_ABS / PRESSURE_MODE_GAUGE
    uint16_t sample_period_ms; // periodo de adquisiciÃ³n
    uint16_t crc16;            // CRC16-CCITT de todo lo anterior
} sensor_eeprom_cfg_t;

static sensor_eeprom_cfg_t s_scfg;
static bool s_scfg_valid = false;

// Ãšltima presiÃ³n ABSOLUTA calibrada (kPa), la usa la tara. NAN si aÃºn no hay.
static volatile float s_last_pressure_abs_kpa = NAN;

// Ãšltimo voltaje CTA reconstruido del FS7 (U = Vain0/divider). Lo usa la tara
// de flujo (auto-cero) y la calibraciÃ³n de 1 punto. NAN si aÃºn no hay lectura.
static volatile float s_last_ucta = NAN;

// Valores de DIAGNOSTICO en vivo para mostrar en el dashboard (temporales):
// presiÃ³n atmosfÃ©rica del BMP280, referencia SFM3300 y flujo del FS7.
static volatile float s_dbg_atm_kpa = NAN;
static volatile float s_dbg_sfm_slm = NAN;
static volatile float s_dbg_fs7_lpm = NAN;

//------------------------------------------------------------------
// Estado interno
//------------------------------------------------------------------

static sensor_sample_t s_buf[SENSORS_BUFFER_LEN];
static size_t s_head = 0;  // prÃ³xima posiciÃ³n de escritura
static size_t s_count = 0; // nÂº de elementos vÃ¡lidos en buffer

static SemaphoreHandle_t s_lock = NULL;

// Config cachÃ© (por si luego quieres usar cal.* aquÃ­)
static AppConfig s_cfg_cache;
static bool s_cfg_valid = false;
static uint32_t s_reported_faults = SENSOR_FAULT_NONE;

// Handle del bus I2C 1 para la recuperacion de bus (NULL = deshabilitada).
static i2c_master_bus_handle_t s_i2c_bus = NULL;

// Flag "bus I2C 1 colgado": lo consulta el touch (ft5x06.c, mismo bus fisico)
// para NO lanzar su lectura I2C mientras el bus esta caido y evitar bloquear el
// mutex de display. volatile: escrito por sensors_acq, leido por la tarea LVGL.
static volatile bool s_bus1_hung = false;

// Tarea de adquisiciÃ³n real
static TaskHandle_t s_acq_task_handle = NULL;

static void sensors_acq_task(void *arg);

//------------------------------------------------------------------
// Helpers de sincronizaciÃ³n
//------------------------------------------------------------------

static inline void lock(void)
{
    if (s_lock)
        xSemaphoreTake(s_lock, portMAX_DELAY);
}

static inline void unlock(void)
{
    if (s_lock)
        xSemaphoreGive(s_lock);
}

//------------------------------------------------------------------
// ConfiguraciÃ³n de calibraciÃ³n (EEPROM)
//------------------------------------------------------------------

static uint16_t crc16_ccitt(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; i++)
    {
        crc ^= (uint16_t)data[i] << 8;
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000) ? (uint16_t)((crc << 1) ^ 0x1021) : (uint16_t)(crc << 1);
    }
    return crc;
}

static void sensor_cfg_set_defaults(sensor_eeprom_cfg_t *c)
{
    memset(c, 0, sizeof(*c));
    c->magic = SENS_CFG_MAGIC;
    c->version = SENS_CFG_VERSION;
    c->size = sizeof(*c);

    c->pressure_offset_kpa = 0.0f;
    c->pressure_scale = 1.0f;
    c->pressure_ref_kpa = 0.0f; // sin tarar todavÃ­a

    // Divisor R2/(R1+R2) = 100k/340k
    c->fs7_divider = 100.0f / (240.0f + 100.0f); // 0.294118
    // Calibracion (2026-08-21) contra el patron SFM3300 con datos ESTABLES
    // (~180 muestras, 8-37 slm). El voltaje CTA RECONSTRUIDO (Ucta=Vain0/divisor)
    // a flujo CERO mide 3.466 V (muy estable, n=21). Respuesta convexa (el CTA
    // satura): flujo[slm] = 209*(Ucta-3.466)^1.17, que en el modelo
    // ((U-u0)/k)^(1/n)*scale es k=1, n=0.857, scale=209. RMS del ajuste ~2.6 slm.
    // Refinar arriba de ~37 slm (no cubierto); la auto-tara corrige la deriva del cero.
    c->fs7_u0 = 3.466f;     // cero reconstruido (Ucta), medido estable
    c->fs7_k = 1.0f;        // la ganancia va en flow_scale
    c->fs7_n = 0.857f;      // exponente (1/n = 1.17: respuesta convexa)
    c->flow_scale = 209.0f; // ley de potencia vs SFM3300 (RMS ~2.6 slm)
    c->flow_offset = 0.0f;

    c->flow_enabled = 1;   // FS7 conectado y calibrado (salida analog. a 3.6 V @ 0 flujo)
    // Gauge por defecto: presiÃ³n de lÃ­nea RELATIVA a la atmosfÃ©rica. Con el BMP280
    // presente resta la atmosfÃ©rica en vivo (abs - atm); si no hay BMP cae a la
    // tara (pressure_ref_kpa, 0 por defecto -> muestra absoluta).
    c->pressure_mode = PRESSURE_MODE_GAUGE;
    c->sample_period_ms = SENSORS_DEFAULT_PERIOD_MS;
}

// Carga la configuraciÃ³n desde la EEPROM. Si no es vÃ¡lida (primera vez o CRC
// incorrecto) escribe los valores por defecto y los deja activos.
static void sensor_cfg_load_or_init(void)
{
    sensor_cfg_set_defaults(&s_scfg); // base segura
    s_scfg_valid = false;

    if (!at24c256_is_ready())
    {
        ESP_LOGW(TAG, "EEPROM no disponible: uso calibraciÃ³n por defecto (RAM)");
        s_scfg_valid = true; // seguimos con defaults en RAM
        return;
    }

    sensor_eeprom_cfg_t tmp;
    esp_err_t err = at24c256_read(SENS_CFG_EEPROM_ADDR, (uint8_t *)&tmp, sizeof(tmp));
    if (err == ESP_OK &&
        tmp.magic == SENS_CFG_MAGIC &&
        tmp.version == SENS_CFG_VERSION &&
        tmp.size == sizeof(tmp))
    {
        uint16_t crc = crc16_ccitt((const uint8_t *)&tmp, sizeof(tmp) - sizeof(tmp.crc16));
        if (crc == tmp.crc16)
        {
            s_scfg = tmp;
            s_scfg_valid = true;
            ESP_LOGI(TAG, "CalibraciÃ³n cargada de EEPROM (v%u): U0=%.2f k=%.3f n=%.3f div=%.4f "
                          "flow_scale=%.3f period=%u ms",
                     s_scfg.version, s_scfg.fs7_u0, s_scfg.fs7_k, s_scfg.fs7_n,
                     s_scfg.fs7_divider, s_scfg.flow_scale, s_scfg.sample_period_ms);
            return;
        }
        ESP_LOGW(TAG, "EEPROM: CRC de calibraciÃ³n invÃ¡lido, reescribo defaults");
    }
    else
    {
        ESP_LOGW(TAG, "EEPROM sin calibraciÃ³n vÃ¡lida (magic/size), inicializo defaults");
    }

    // Escribimos los defaults en la EEPROM
    sensor_cfg_set_defaults(&s_scfg);
    s_scfg.crc16 = crc16_ccitt((const uint8_t *)&s_scfg, sizeof(s_scfg) - sizeof(s_scfg.crc16));
    err = at24c256_write(SENS_CFG_EEPROM_ADDR, (const uint8_t *)&s_scfg, sizeof(s_scfg));
    if (err == ESP_OK)
        ESP_LOGI(TAG, "CalibraciÃ³n por defecto escrita en EEPROM");
    else
        ESP_LOGW(TAG, "No se pudo escribir calibraciÃ³n en EEPROM: %s", esp_err_to_name(err));
    s_scfg_valid = true;
}

// Convierte el voltaje leÃ­do en AIN0 a flujo (L/min) usando el modelo FS7.
static float sensor_flow_from_voltage(float v_ain0, float *v_out_cta)
{
    float divider = (s_scfg.fs7_divider > 1e-4f) ? s_scfg.fs7_divider : 0.294118f;
    float u = v_ain0 / divider; // salida CTA reconstruida
    s_last_ucta = u;            // para la tara de flujo / calibraciÃ³n
    if (v_out_cta)
        *v_out_cta = u;

    float diff = u - s_scfg.fs7_u0;
    if (diff <= 0.0f)
        return s_scfg.flow_offset; // por debajo de U0 -> flujo cero (+ offset)

    float n = (fabsf(s_scfg.fs7_n) > 1e-3f) ? s_scfg.fs7_n : 0.51f;
    float k = (fabsf(s_scfg.fs7_k) > 1e-3f) ? s_scfg.fs7_k : 0.91f;

    float velocity = powf(diff / k, 1.0f / n); // m/s
    return velocity * s_scfg.flow_scale + s_scfg.flow_offset;
}

// Recalcula el CRC y persiste s_scfg en la EEPROM. Devuelve ESP_OK si se guardÃ³.
static esp_err_t sensor_cfg_persist(void)
{
    s_scfg.crc16 = crc16_ccitt((const uint8_t *)&s_scfg, sizeof(s_scfg) - sizeof(s_scfg.crc16));
    if (!at24c256_is_ready())
    {
        ESP_LOGW(TAG, "EEPROM no disponible: cambios de calibraciÃ³n solo en RAM");
        return ESP_ERR_INVALID_STATE;
    }
    esp_err_t err = at24c256_write(SENS_CFG_EEPROM_ADDR, (const uint8_t *)&s_scfg, sizeof(s_scfg));
    if (err != ESP_OK)
        ESP_LOGW(TAG, "No se pudo persistir calibraciÃ³n en EEPROM: %s", esp_err_to_name(err));
    return err;
}

//------------------------------------------------------------------
// Tara / modo de presiÃ³n (opciÃ³n B: gauge por referencia local)
//------------------------------------------------------------------

esp_err_t sensors_runtime_tare_pressure(void)
{
    float p_abs = s_last_pressure_abs_kpa;
    if (!isfinite(p_abs))
    {
        ESP_LOGW(TAG, "Tara abortada: aÃºn no hay lectura de presiÃ³n vÃ¡lida");
        return ESP_ERR_INVALID_STATE;
    }

    // Fijamos primero la referencia y luego el modo (para que la tarea de
    // adquisiciÃ³n nunca reste una referencia a medio actualizar).
    s_scfg.pressure_ref_kpa = p_abs;
    s_scfg.pressure_mode = PRESSURE_MODE_GAUGE;

    esp_err_t err = sensor_cfg_persist();
    ESP_LOGI(TAG, "TARA aplicada: referencia=%.2f kPa (%.2f psi). Modo=gauge. Persist=%s",
             p_abs, p_abs * 0.145038f, esp_err_to_name(err));
    return ESP_OK; // la tara queda activa aunque la persistencia falle
}

// Auto-cero del flujo (tara): fija el voltaje CTA ACTUAL como u0 (flujo cero).
// El cero del FS7 deriva con temperatura/alimentaciÃ³n, asÃ­ que esto se dispara
// con la lÃ­nea SIN flujo (por BLE al comisionar, como la tara de presiÃ³n).
esp_err_t sensors_runtime_tare_flow(void)
{
    float u = s_last_ucta;
    if (!isfinite(u))
    {
        ESP_LOGW(TAG, "Tara de flujo abortada: aÃºn no hay lectura del FS7");
        return ESP_ERR_INVALID_STATE;
    }
    s_scfg.fs7_u0 = u;
    esp_err_t err = sensor_cfg_persist();
    ESP_LOGI(TAG, "TARA de flujo: u0=%.4f V (nuevo cero). Persist=%s",
             u, esp_err_to_name(err));
    return ESP_OK; // queda activa aunque falle la persistencia
}

// CalibraciÃ³n de 1 punto: con un caudal CONOCIDO (p.ej. el patrÃ³n SFM3300),
// ajusta flow_scale para que la lectura del FS7 coincida con ref_slm.
esp_err_t sensors_runtime_cal_flow_point(float ref_slm)
{
    float u = s_last_ucta;
    if (!isfinite(u))
        return ESP_ERR_INVALID_STATE;

    float diff = u - s_scfg.fs7_u0;
    if (diff <= 1e-4f || ref_slm <= 0.0f)
    {
        ESP_LOGW(TAG, "Cal de flujo abortada: aplica un caudal conocido > 0 (diff=%.4f, ref=%.2f)",
                 diff, ref_slm);
        return ESP_ERR_INVALID_ARG;
    }

    float n = (fabsf(s_scfg.fs7_n) > 1e-3f) ? s_scfg.fs7_n : 1.0f;
    float k = (fabsf(s_scfg.fs7_k) > 1e-3f) ? s_scfg.fs7_k : 1.0f;
    float velocity = powf(diff / k, 1.0f / n);
    if (velocity <= 1e-6f)
        return ESP_ERR_INVALID_ARG;

    s_scfg.flow_scale = (ref_slm - s_scfg.flow_offset) / velocity;
    esp_err_t err = sensor_cfg_persist();
    ESP_LOGI(TAG, "CAL flujo 1 punto: ref=%.2f slm @ U=%.4f -> flow_scale=%.2f. Persist=%s",
             ref_slm, u, s_scfg.flow_scale, esp_err_to_name(err));
    return ESP_OK;
}

esp_err_t sensors_runtime_set_pressure_gauge(bool gauge)
{
    // La atmosfÃ©rica siempre es > 0; ref_kpa <= 0 significa "nunca tarado".
    if (gauge && s_scfg.pressure_ref_kpa <= 0.0f)
    {
        ESP_LOGW(TAG, "No hay referencia tarada; usa sensors_runtime_tare_pressure() primero");
        return ESP_ERR_INVALID_STATE;
    }
    s_scfg.pressure_mode = gauge ? PRESSURE_MODE_GAUGE : PRESSURE_MODE_ABS;
    esp_err_t err = sensor_cfg_persist();
    ESP_LOGI(TAG, "Modo de presiÃ³n = %s. Persist=%s",
             gauge ? "gauge" : "absoluta", esp_err_to_name(err));
    return ESP_OK;
}

bool sensors_runtime_is_pressure_gauge(void)
{
    return s_scfg.pressure_mode == PRESSURE_MODE_GAUGE;
}

//------------------------------------------------------------------
// API pÃºblica
//------------------------------------------------------------------

bool sensors_runtime_init(const AppConfig *cfg)
{
    ESP_LOGI(TAG, "Iniciando sensors_runtime...");
    if (!s_lock)
    {
        s_lock = xSemaphoreCreateMutex();
        if (!s_lock)
        {
            ESP_LOGI(TAG, "No se pudo crear mutex");
            return false;
        }
    }

    lock();
    s_head = 0;
    s_count = 0;

    if (cfg)
    {
        memcpy(&s_cfg_cache, cfg, sizeof(AppConfig));
        s_cfg_valid = true;
    }
    else
    {
        memset(&s_cfg_cache, 0, sizeof(AppConfig));
        s_cfg_valid = false;
    }
    unlock();

    // Cargamos la calibraciÃ³n desde la EEPROM (o escribimos defaults la 1a vez).
    sensor_cfg_load_or_init();

    // Estado de los sensores fÃ­sicos (los drivers se inicializan en main.c).
    ESP_LOGI(TAG, "Sensores: MS5803=%s  ADS1115=%s  EEPROM=%s",
             ms5803_is_ready() ? "OK" : "NO",
             ads1115_is_ready() ? "OK" : "NO",
             at24c256_is_ready() ? "OK" : "NO");

    // Arrancamos la tarea de adquisiciÃ³n real.
    if (!s_acq_task_handle)
    {
        BaseType_t ok = xTaskCreatePinnedToCore(
            /* pxTaskCode   */ sensors_acq_task,
            /* pcName       */ "sensors_acq",
            /* usStackDepth */ SENSORS_TASK_STACK,
            /* pvParameters */ NULL,
            /* uxPriority   */ SENSORS_TASK_PRIO,
            /* pxCreatedTask*/ &s_acq_task_handle,
            /* xCoreID      */ tskNO_AFFINITY);
        if (ok != pdPASS)
        {
            ESP_LOGE(TAG, "No se pudo crear la tarea de adquisiciÃ³n de sensores");
            s_acq_task_handle = NULL;
        }
    }

    ESP_LOGI(TAG, "Init ok (tarea adquisiciÃ³n %s)",
             s_acq_task_handle ? "ON" : "OFF");
    return true;
}

void sensors_runtime_set_bus(i2c_master_bus_handle_t bus)
{
    s_i2c_bus = bus;
}

bool sensors_runtime_bus1_hung(void)
{
    return s_bus1_hung;
}

void sensors_runtime_get_debug(float *atm_kpa, float *sfm_slm, float *fs7_lpm)
{
    if (atm_kpa) *atm_kpa = s_dbg_atm_kpa;
    if (sfm_slm) *sfm_slm = s_dbg_sfm_slm;
    if (fs7_lpm) *fs7_lpm = s_dbg_fs7_lpm;
}

void sensors_runtime_update_config(const AppConfig *cfg)
{
    if (!cfg)
        return;

    lock();
    memcpy(&s_cfg_cache, cfg, sizeof(AppConfig));
    s_cfg_valid = true;
    unlock();

    ESP_LOGI(TAG, "Config actualizada en sensors_runtime");
}

void sensors_runtime_push_sample(const sensor_sample_t *s)
{
    if (!s)
        return;

    lock();

    s_buf[s_head] = *s;
    s_head = (s_head + 1) % SENSORS_BUFFER_LEN;
    if (s_count < SENSORS_BUFFER_LEN)
        s_count++;

    unlock();
}

void sensors_runtime_report_faults(uint32_t faults)
{
    lock(); s_reported_faults = faults; unlock();
}

uint32_t sensors_runtime_get_faults(void)
{
    uint32_t faults; sensor_sample_t last = {0}; bool have = false; bool cfg_ok;
    lock();
    faults = s_reported_faults; cfg_ok = s_cfg_valid;
    if (s_count) { size_t i=(s_head+SENSORS_BUFFER_LEN-1)%SENSORS_BUFFER_LEN; last=s_buf[i]; have=true; }
    unlock();
    if (!cfg_ok) faults |= SENSOR_FAULT_CONFIG;
    if (!have) return faults | SENSOR_FAULT_NO_DATA;
    int64_t age_ms = esp_timer_get_time()/1000 - last.ts_ms;
    if (age_ms < 0 || age_ms > 1000) faults |= SENSOR_FAULT_STALE;
    float pmax = 1379.0f * 1.10f;
    float fmax = s_cfg_cache.sensors.flow_fullscale_lpm > 0 ? s_cfg_cache.sensors.flow_fullscale_lpm * 1.20f : 120.0f;
    if (!isfinite(last.pressure_kpa) || !isfinite(last.flow_lpm) || last.pressure_kpa < -5.f || last.pressure_kpa > pmax || last.flow_lpm < -1.f || last.flow_lpm > fmax) faults |= SENSOR_FAULT_INVALID;
    return faults;
}
bool sensors_runtime_get_last(sensor_sample_t *out)
{
    if (!out)
        return false;

    lock();

    if (s_count == 0)
    {
        unlock();
        return false;
    }

    size_t last_idx = (s_head + SENSORS_BUFFER_LEN - 1) % SENSORS_BUFFER_LEN;
    *out = s_buf[last_idx];

    unlock();
    return true;
}

bool sensors_runtime_get_min_max(int64_t window_ms,
                                 sensor_sample_t *min_out,
                                 sensor_sample_t *max_out)
{
    if (window_ms <= 0)
        return false;

    lock();

    if (s_count == 0)
    {
        unlock();
        return false;
    }

    int64_t now_ms = esp_timer_get_time() / 1000;
    int64_t from_ms = now_ms - window_ms;

    bool has = false;
    sensor_sample_t min_s = {0}, max_s = {0};

    // Recorremos desde la muestra mÃ¡s reciente hacia atrÃ¡s
    for (size_t i = 0; i < s_count; ++i)
    {
        size_t idx = (s_head + SENSORS_BUFFER_LEN - 1 - i) % SENSORS_BUFFER_LEN;
        const sensor_sample_t *s = &s_buf[idx];

        if (s->ts_ms < from_ms)
            break; // mÃ¡s allÃ¡ de la ventana

        if (!has)
        {
            min_s = max_s = *s;
            has = true;
        }
        else
        {
            // PresiÃ³n
            if (s->pressure_kpa < min_s.pressure_kpa)
                min_s.pressure_kpa = s->pressure_kpa;
            if (s->pressure_kpa > max_s.pressure_kpa)
                max_s.pressure_kpa = s->pressure_kpa;

            // Flujo
            if (s->flow_lpm < min_s.flow_lpm)
                min_s.flow_lpm = s->flow_lpm;
            if (s->flow_lpm > max_s.flow_lpm)
                max_s.flow_lpm = s->flow_lpm;

            // Temperatura
            if (s->temp_c < min_s.temp_c)
                min_s.temp_c = s->temp_c;
            if (s->temp_c > max_s.temp_c)
                max_s.temp_c = s->temp_c;
        }
    }

    unlock();

    if (!has)
        return false;

    if (min_out)
        *min_out = min_s;
    if (max_out)
        *max_out = max_s;

    return true;
}

// Acumulador de una senal para la estadistica de ventana (v2).
typedef struct {
    uint32_t n;
    double   sum;
    double   sumsq;
    float    min;
    float    max;
} sig_acc_t;

static inline void sig_acc_add(sig_acc_t *a, float v)
{
    if (!isfinite(v))
        return; // ignora NaN/inf senal por senal
    if (a->n == 0) {
        a->min = a->max = v;
    } else {
        if (v < a->min) a->min = v;
        if (v > a->max) a->max = v;
    }
    a->sum   += (double)v;
    a->sumsq += (double)v * (double)v;
    a->n++;
}

static void sig_acc_finish(const sig_acc_t *a, sensor_signal_stats_t *out)
{
    out->n = a->n;
    if (a->n == 0) {
        out->min = out->max = out->mean = out->std = 0.f;
        return;
    }
    double mean = a->sum / (double)a->n;
    double var  = a->sumsq / (double)a->n - mean * mean; // poblacional
    if (var < 0.0) var = 0.0; // saneo de error numerico
    out->min  = a->min;
    out->max  = a->max;
    out->mean = (float)mean;
    out->std  = (float)sqrt(var);
}

bool sensors_runtime_get_window_stats(int64_t window_ms,
                                      sensor_window_stats_t *out)
{
    if (!out || window_ms <= 0)
        return false;

    sig_acc_t ap = {0}, af = {0}, at = {0};

    lock();

    if (s_count == 0) {
        unlock();
        return false;
    }

    int64_t now_ms  = esp_timer_get_time() / 1000;
    int64_t from_ms = now_ms - window_ms;

    // Recorremos desde la muestra mas reciente hacia atras hasta salir de la ventana.
    for (size_t i = 0; i < s_count; ++i) {
        size_t idx = (s_head + SENSORS_BUFFER_LEN - 1 - i) % SENSORS_BUFFER_LEN;
        const sensor_sample_t *s = &s_buf[idx];
        if (s->ts_ms < from_ms)
            break;
        sig_acc_add(&ap, s->pressure_kpa);
        sig_acc_add(&af, s->flow_lpm);
        sig_acc_add(&at, s->temp_c);
    }

    unlock();

    sig_acc_finish(&ap, &out->pressure);
    sig_acc_finish(&af, &out->flow);
    sig_acc_finish(&at, &out->temp);

    // true si al menos una senal tuvo muestras en la ventana.
    return (out->pressure.n || out->flow.n || out->temp.n);
}

bool sensors_runtime_get_series(int64_t window_ms,
                                sensor_sample_t *out,
                                size_t max,
                                size_t *out_count)
{
    if (!out || max == 0 || window_ms <= 0)
        return false;

    lock();

    if (s_count == 0)
    {
        unlock();
        if (out_count)
            *out_count = 0;
        return false;
    }

    int64_t now_ms = esp_timer_get_time() / 1000;
    int64_t from_ms = now_ms - window_ms;

    // Primero contamos cuÃ¡ntos entran en la ventana (desde el mÃ¡s antiguo hacia arriba)
    // Para devolverlos en orden cronolÃ³gico ascendente.
    size_t n = 0;
    sensor_sample_t tmp[SENSORS_BUFFER_LEN]; // buffer temporal en stack (16 bytes * 600 = ~9.6 kB)

    size_t idx = (s_head + SENSORS_BUFFER_LEN - s_count) % SENSORS_BUFFER_LEN; // Ã­ndice del mÃ¡s antiguo
    for (size_t i = 0; i < s_count; ++i)
    {
        const sensor_sample_t *s = &s_buf[idx];

        if (s->ts_ms >= from_ms)
        {
            if (n < SENSORS_BUFFER_LEN)
            {
                tmp[n++] = *s;
            }
        }

        idx = (idx + 1) % SENSORS_BUFFER_LEN;
    }

    if (n == 0)
    {
        unlock();
        if (out_count)
            *out_count = 0;
        return false;
    }

    // Copiamos hasta max elementos al buffer de salida
    size_t copy_n = (n < max) ? n : max;
    memcpy(out, tmp, copy_n * sizeof(sensor_sample_t));

    unlock();

    if (out_count)
        *out_count = copy_n;

    return true;
}

//------------------------------------------------------------------
// Tarea de adquisiciÃ³n real (MS5803 + ADS1115/FS7)
//------------------------------------------------------------------

static void sensors_acq_task(void *arg)
{
    (void)arg;

    uint32_t period_ms = s_scfg.sample_period_ms ? s_scfg.sample_period_ms
                                                 : SENSORS_DEFAULT_PERIOD_MS;
    ESP_LOGI(TAG, "Tarea de adquisiciÃ³n iniciada (periodo %u ms)", period_ms);

    uint32_t log_decim = 0; // logueamos ~1 vez por segundo para no saturar
    uint32_t log_every = (period_ms > 0) ? (1000 / period_ms) : 10;
    if (log_every == 0)
        log_every = 1;

#if SENSORS_TARE_ON_BOOT
    bool boot_tare_done = false;
    uint32_t boot_ticks = 0;
#endif

    // Estado persistente entre ciclos para el antirrebÃ³te de fault de I2C.
    // Retenemos el Ãºltimo valor bueno para no parpadear a 0 ante un NACK puntual.
    float last_good_pressure = NAN;
    float last_good_temp = NAN;
    float last_good_flow = NAN;
    int ms5803_miss = 0;
    int ads_miss = 0;
    int sfm_miss = 0;
    int bus_recover_wait = 0;

    while (1)
    {
        int64_t now_ms = esp_timer_get_time() / 1000;
        uint32_t faults = SENSOR_FAULT_NONE;

        float pressure_kpa = NAN;
        float temp_c = NAN;
        float v_ain0 = NAN;
        float u_cta = NAN;
        float flow_lpm = NAN;
        float sfm_slm = NAN; // caudalimetro de referencia SFM3300 (slm)
        float atm_kpa = NAN; // presiÃ³n atmosfÃ©rica de referencia (BMP280)
        bool ms5803_read_ok = false;
        bool ads_read_ok = false;

        // --- Referencia atmosfÃ©rica (BMP280) para el cero de presiÃ³n de lÃ­nea ---
        if (bmp280_is_ready())
        {
            float ap = 0.0f;
            if (bmp280_read(&ap, NULL) == ESP_OK)
                atm_kpa = ap;
        }
        s_dbg_atm_kpa = atm_kpa;

        // --- PresiÃ³n / temperatura (MS5803-14BA) ---
        if (ms5803_is_ready())
        {
            float p = 0.0f, t = 0.0f;
            esp_err_t err = ms5803_read(&p, &t);
            if (err != ESP_OK && ms5803_miss < SENSORS_FAULT_DEBOUNCE)
            {
                // Un NACK puntual en el bus compartido es comÃºn: reintentamos 1 vez.
                // Solo en la fase transitoria: si el fault ya esta latcheado (bus
                // colgado) NO duplicamos trafico -> deja actuar al recovery.
                vTaskDelay(pdMS_TO_TICKS(2));
                err = ms5803_read(&p, &t);
            }
            if (err == ESP_OK)
            {
                // PresiÃ³n absoluta calibrada (para la tara y el modo absoluto)
                float p_abs = p * s_scfg.pressure_scale + s_scfg.pressure_offset_kpa;
                s_last_pressure_abs_kpa = p_abs;
                temp_c = t;

                ms5803_read_ok = true;
                ms5803_miss = 0;

                // Modo gauge: cero por referencia atmosfÃ©rica. Preferimos el
                // barÃ³metro en vivo (BMP280); si no hay, caemos a la tara (opciÃ³n B).
                if (s_scfg.pressure_mode == PRESSURE_MODE_GAUGE)
                {
                    float ref = isfinite(atm_kpa) ? atm_kpa : s_scfg.pressure_ref_kpa;
                    pressure_kpa = p_abs - ref;
                }
                else
                    pressure_kpa = p_abs;

                last_good_pressure = pressure_kpa;
                last_good_temp = temp_c;
            }
            else
            {
                // Fallo transitorio: retenemos el Ãºltimo valor bueno y solo
                // declaramos fault tras varios ciclos seguidos (antirrebÃ³te).
                if (++ms5803_miss >= SENSORS_FAULT_DEBOUNCE)
                    faults |= SENSOR_FAULT_MODULE_I2C;
                if (isfinite(last_good_pressure)) pressure_kpa = last_good_pressure;
                if (isfinite(last_good_temp)) temp_c = last_good_temp;
            }
        }
        else
        {
            faults |= SENSOR_FAULT_MODULE_I2C;
        }

        // --- Flujo (FS7 va ADS1115 AIN0) ---
        // Si el FS7 no esta habilitado (no conectado), no leemos: la entrada
        // AIN0 flota y daria ruido/valores aleatorios. Reportamos flujo 0.
        if (!s_scfg.flow_enabled)
        {
            flow_lpm = 0.0f;
        }
        else if (ads1115_is_ready())
        {
            float v = 0.0f;
            esp_err_t err = ads1115_read_voltage(ADS1115_MUX_AIN0, &v);
            if (err != ESP_OK && ads_miss < SENSORS_FAULT_DEBOUNCE)
            {
                // Reintento Ãºnico ante un NACK puntual (solo fase transitoria).
                vTaskDelay(pdMS_TO_TICKS(2));
                err = ads1115_read_voltage(ADS1115_MUX_AIN0, &v);
            }
            if (err == ESP_OK)
            {
                ads_read_ok = true;
                ads_miss = 0;
                v_ain0 = v;
                flow_lpm = sensor_flow_from_voltage(v, &u_cta);
                last_good_flow = flow_lpm;
            }
            else
            {
                // Fallo transitorio: retenemos el Ãºltimo flujo y solo declaramos
                // fault tras varios ciclos seguidos (antirrebÃ³te anti-alarma).
                if (++ads_miss >= SENSORS_FAULT_DEBOUNCE)
                    faults |= SENSOR_FAULT_MODULE_I2C;
                if (isfinite(last_good_flow)) flow_lpm = last_good_flow;
            }
        }
        else
        {
            faults |= SENSOR_FAULT_MODULE_I2C;
        }

        // --- Flujo de referencia (Sensirion SFM3300-D, para calibrar el FS7) ---
        // Guardamos el codigo de error para diagnosticar en el log por que falla
        // (NACK/timeout -> pull-ups del bus 2; CRC -> ruido; 0xFFFF -> warm-up).
        esp_err_t sfm_err = ESP_OK;
        if (sfm3300_is_ready())
        {
            float ref = 0.0f;
            sfm_err = sfm3300_read(&ref);
            if (sfm_err == ESP_OK)
            {
                sfm_slm = ref;
                sfm_miss = 0;
            }
            else if (++sfm_miss >= SFM_RESTART_AFTER_MISSES)
            {
                // Fallo sostenido: reintentamos arrancar la medicion continua por
                // si el sensor se reinicio o nunca entro en modo continuo.
                sfm_miss = 0;
                (void)sfm3300_start_measurement();
            }
            // Un NACK/CRC puntual no es fault: solo significa "sin dato nuevo".
        }

        // Diagnostico en vivo para el dashboard (temporal, se quita en producciÃ³n).
        s_dbg_sfm_slm = sfm_slm;
        s_dbg_fs7_lpm = flow_lpm;

        // Publicamos la muestra (usamos 0 cuando un canal no tiene lectura vÃ¡lida
        // para no romper a los consumidores que esperan floats finitos).
        sensor_sample_t s = {
            .ts_ms = now_ms,
            .pressure_kpa = isfinite(pressure_kpa) ? pressure_kpa : 0.0f,
            .flow_lpm = isfinite(flow_lpm) ? flow_lpm : 0.0f,
            .temp_c = isfinite(temp_c) ? temp_c : 0.0f,
        };
        sensors_runtime_push_sample(&s);
        sensors_runtime_report_faults(faults);

        // --- Recuperacion del bus I2C 1 (bus colgado) ---
        // Si MS5803 y ADS fallan JUNTOS de forma sostenida, el bus quedo
        // colgado. Reseteamos el controlador cada BUS_RECOVER_EVERY ciclos
        // mientras siga caido. (Si el flujo esta deshabilitado no probamos el
        // ADS, asi que basta con el MS5803 sostenido para inferir el cuelgue.)
        bool ads_down = s_scfg.flow_enabled ? (ads_miss >= SENSORS_FAULT_DEBOUNCE) : true;
        bool bus1_down = (ms5803_miss >= SENSORS_FAULT_DEBOUNCE) && ads_down;
        s_bus1_hung = bus1_down;   // visible al touch (ft5x06.c) para que se aparte del bus
        if (bus1_down && s_i2c_bus)
        {
            if (++bus_recover_wait >= BUS_RECOVER_EVERY)
            {
                bus_recover_wait = 0;
                esp_err_t r = i2c_master_bus_reset(s_i2c_bus);
                ESP_LOGW(TAG, "Bus I2C 1 colgado (MS5803+ADS): i2c_master_bus_reset -> %s",
                         esp_err_to_name(r));
            }
        }
        else
        {
            bus_recover_wait = 0;
        }

#if SENSORS_TARE_ON_BOOT
        // Tara automÃ¡tica de prueba tras ~2 s con lectura vÃ¡lida.
        if (!boot_tare_done)
        {
            boot_ticks++;
            if (boot_ticks > (2000 / period_ms) && isfinite(s_last_pressure_abs_kpa))
            {
                sensors_runtime_tare_pressure();
                boot_tare_done = true;
            }
        }
#endif

        // Log periÃ³dico (por defecto ~1 Hz). Ãštil mientras no hay pantalla.
        if (++log_decim >= log_every)
        {
            log_decim = 0;
            const char *pmode = (s_scfg.pressure_mode == PRESSURE_MODE_GAUGE) ? "gauge" : "abs";

            // Referencia SFM3300: distinguimos "cero real" de "sin dato" (n/d)
            char sfm_str[72];
            if (!sfm3300_is_ready())
                snprintf(sfm_str, sizeof(sfm_str), " | SFM=noInit");
            else if (isfinite(sfm_slm))
                snprintf(sfm_str, sizeof(sfm_str), " | SFMref=%.2f slm", sfm_slm);
            else
            {
                // Volcamos los bytes crudos para diagnosticar: FF FF FF = bus sin
                // dato (0xFFFF), bytes reales con CRC malo = ruido/framing.
                uint8_t rx[3];
                sfm3300_get_last_rx(rx);
                snprintf(sfm_str, sizeof(sfm_str), " | SFMref=n/d (%s) rx=%02X%02X%02X",
                         esp_err_to_name(sfm_err), rx[0], rx[1], rx[2]);
            }

            // Estado por sensor cuando hay fault: noInit (no arrancó) o rdErr (I2C read falló)
            char diag[56];
            if (faults)
                snprintf(diag, sizeof(diag), " [FAULT MS5803:%s ADS:%s]",
                         ms5803_read_ok ? "ok" : (ms5803_is_ready() ? "rdErr" : "noInit"),
                         ads_read_ok ? "ok" : (ads1115_is_ready() ? "rdErr" : "noInit"));
            else
                diag[0] = '\0';

            if (!s_scfg.flow_enabled)
            {
                ESP_LOGI(TAG,
                         "P=%.2f kPa (%s) | T=%.2f C | Flujo=OFF%s%s",
                         s.pressure_kpa, pmode, isfinite(temp_c) ? temp_c : 0.0f,
                         sfm_str, diag);
            }
            else
            {
                ESP_LOGI(TAG,
                         "P=%.2f kPa (%s) | T=%.2f C | Flujo=%.2f L/min | Vain0=%.4f V (Ucta=%.3f V)%s%s",
                         s.pressure_kpa, pmode, isfinite(temp_c) ? temp_c : 0.0f,
                         s.flow_lpm, isfinite(v_ain0) ? v_ain0 : 0.0f,
                         isfinite(u_cta) ? u_cta : 0.0f,
                         sfm_str, diag);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(period_ms));
    }
}

bool sensors_runtime_get_aws_telemetry(int64_t window_ms,
                                       float *p_min, float *p_max, float *p_avg,
                                       float *f_min, float *f_max, float *f_avg)
{
    if (!s_lock) return false;

    int64_t now_ms = esp_timer_get_time() / 1000;
    int64_t start_ms = now_ms - window_ms;

    xSemaphoreTake(s_lock, portMAX_DELAY);

    if (s_count == 0) {
        xSemaphoreGive(s_lock);
        return false;
    }

    float pmin = 999999.0f, pmax = -999999.0f, psum = 0.0f;
    float fmin = 999999.0f, fmax = -999999.0f, fsum = 0.0f;
    int valid_samples = 0;

    // Recorremos el buffer circular buscando muestras en la ventana de tiempo
    for (size_t i = 0; i < s_count; i++) {
        size_t idx = (s_head + SENSORS_BUFFER_LEN - s_count + i) % SENSORS_BUFFER_LEN;
        
        if (s_buf[idx].ts_ms >= start_ms) {
            float p = s_buf[idx].pressure_kpa;
            float f = s_buf[idx].flow_lpm;

            if (p < pmin) pmin = p;
            if (p > pmax) pmax = p;
            psum += p;

            if (f < fmin) fmin = f;
            if (f > fmax) fmax = f;
            fsum += f;

            valid_samples++;
        }
    }
    xSemaphoreGive(s_lock);

    if (valid_samples == 0) return false;

    // Asignamos resultados
    if (p_min) *p_min = pmin;
    if (p_max) *p_max = pmax;
    if (p_avg) *p_avg = psum / valid_samples;

    if (f_min) *f_min = fmin;
    if (f_max) *f_max = fmax;
    if (f_avg) *f_avg = fsum / valid_samples;

    return true;
}