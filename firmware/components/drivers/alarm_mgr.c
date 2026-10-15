#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "alarm_mgr.h"
#include "alarm_tone.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "storage.h"
#include <math.h>

static SemaphoreHandle_t s_alarm_lock;
static const char *TAG = "alarm_mgr";

// Parámetros de PWM adaptados para el Driver PAM8906
#define LEDC_SPEED_MODE         LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT
#define BUZZER_FREQ_HZ          2000    // Resonancia óptima para máxima presión sonora
#define BUZZER_DUTY_50_PERCENT  512     // 50% de 1024 para onda cuadrada perfecta

// Variables operacionales de control
static alarm_clinical_state_t s_current_state = ALARM_STATE_NORMAL;
static bool s_is_muted = false;
static int64_t s_mute_end_time_us = 0;
/* 1.6.7: condiciones de alarma (bits) para que el silencio no tape una condicion NUEVA. */
#define COND_P_LOW      ALARM_COND_P_LOW
#define COND_P_HIGH     ALARM_COND_P_HIGH
#define COND_FLOW_DELTA ALARM_COND_FLOW_DELTA
#define COND_FLOW_HIGH  ALARM_COND_FLOW_HIGH
#define COND_SENSOR     ALARM_COND_SENSOR
#define COND_COUNT      5
#define COND_REARM_US   10000000LL   /* ausente 10 s = deja de estar silenciada */
static uint32_t s_active_conds;      /* condiciones de la ultima evaluacion */
static uint32_t s_muted_conds;       /* condiciones presentes al silenciar */
static uint32_t s_crit_conds;        /* 1.6.8: las que estan en banda critica */
static int64_t  s_cond_seen_us[COND_COUNT];
/* 1.6.0: la cadencia del pitido la lleva alarm_tone (patrones IEC 60601-1-8 por
 * esp_timer); aqui solo se decide la prioridad: ALERT = ALTA, WARNING clinico
 * (limite de presion o flujo) = MEDIA, solo fallo de sensor (tecnica) = BAJA. */
static bool s_technical_only = false;
static int64_t s_test_end_time_us = 0;
static int s_volume = 80;
static uint32_t s_sensor_faults = 0;
static alarm_state_change_cb_t s_state_cb = NULL;
// Cuando true, se silencia el buzzer para estados NO criticos (WARNING/fallo
// tecnico). Un ALERT critico de presion SIEMPRE sigue sonando. Lo activa la UI
// mientras se muestra la pantalla de login/config (no debe pitar al teclear PIN).
static bool s_audio_inhibit_noncritical = false;

static void set_buzzer_acoustic(bool turn_on);
static void tone_out(bool on, void *user) { (void)user; set_buzzer_acoustic(on); }

static void alarm_mgr_set_audio_inhibit_noncritical_impl(bool inhibit)
{
    s_audio_inhibit_noncritical = inhibit;
    if (inhibit && s_current_state != ALARM_STATE_ALERT)
        alarm_tone_set(ALARM_TONE_OFF);
}

static void alarm_mgr_set_state_change_cb_impl(alarm_state_change_cb_t cb)
{
    s_state_cb = cb;
}

// Variables para el análisis matemático diferencial de flujo (Delta)
static float s_baseline_flow = 0.0f;
static int64_t s_baseline_time_us = 0;

// Flow alarm confirmation (see alarm_mgr_process_impl).
#define FLOW_CONFIRM_US 500000LL
static bool s_delta_pending = false;
static float s_delta_base = 0.0f;
static int64_t s_delta_since_us = 0;
static int64_t s_high_since_us = 0;

esp_err_t alarm_mgr_init(int buzzer_gpio)
{
    if (!s_alarm_lock) s_alarm_lock = xSemaphoreCreateRecursiveMutex();
    if (!s_alarm_lock) return ESP_ERR_NO_MEM;
    ESP_LOGI(TAG, "Inicializando PWM para Driver PAM8906 en GPIO %d...", buzzer_gpio);

    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_SPEED_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = BUZZER_FREQ_HZ,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_SPEED_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = buzzer_gpio,
        .duty           = 0,
        .hpoint         = 0
    };
    esp_err_t err = ledc_channel_config(&ledc_channel);
    if (err != ESP_OK) return err;
    return alarm_tone_init(tone_out, NULL);
}

static void set_buzzer_acoustic(bool turn_on)
{
    // El PAM8906 amplifica la señal cuadrada diferencial cuando detecta pulsos en DIN
    uint32_t duty = turn_on ? (uint32_t)(102 + (s_volume - 30) * 410 / 70) : 0;
    ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL);
}

static void alarm_mgr_process_impl(float current_pressure, float current_flow, uint32_t sensor_faults)
{
    s_sensor_faults = sensor_faults;
    // 1. Copia rápida del snapshot en RAM (memcpy, SIN parseo JSON de NVS).
    //    Antes hacía appcfg_load() en CADA muestra -> re-parseaba AppConfig.json
    //    ~1-2 Hz (visible en el log) y fragmentaba el heap. La caché se mantiene
    //    fresca por el UI (peek+save) y por appcfg_set/patch (reload) en remoto.
    // Read the RAM snapshot in place: copying the whole AppConfig here cost
    // ~2 KB of stack every 50 ms (main task) and on the LVGL stack for mute.
    const AppConfig *cfg = appcfg_cache_peek();

    int64_t now = esp_timer_get_time();
    s_volume = cfg->general.alarm.volume;
    if (s_volume < 30) s_volume = 30;
    if (s_volume > 100) s_volume = 100;
    if (now < s_test_end_time_us) { alarm_tone_set(ALARM_TONE_HIGH); return; }

    // --- EVALUACIÓN DE PRESIÓN ---
    bool p_min_enabled = cfg->sensors.alarm_limits.pressure_min_enabled;
    bool p_max_enabled = cfg->sensors.alarm_limits.pressure_max_enabled;
    float p_min = cfg->sensors.alarm_limits.pressure_min;
    float p_max = cfg->sensors.alarm_limits.pressure_max;

    // --- EVALUACIÓN DE FLUJO (Análisis de Delta temporal) ---
    float flow_delta_thresh = cfg->sensors.alarm_limits.flow_delta_threshold;
    int flow_window_ms = cfg->sensors.alarm_limits.flow_delta_window_ms;
    bool flow_warning = false;
    bool flow_high = cfg->sensors.alarm_limits.flow_high_enabled && current_flow > cfg->sensors.alarm_limits.flow_high_limit;

    // Si la ventana de tiempo ya caducó o es el primer arranque, tomamos una nueva línea base de flujo
    if (!isfinite(current_flow)) {
        // Invalid flow: restart the delta window once valid data returns.
        s_baseline_time_us = 0;
    } else if (s_baseline_time_us == 0 || (now - s_baseline_time_us) >= (flow_window_ms * 1000LL)) {
        s_baseline_flow = current_flow;
        s_baseline_time_us = now;
    } else {
        // Evaluamos si el flujo tuvo un pico o caída abrupta dentro de la ventana de tiempo actual
        float delta = current_flow - s_baseline_flow;
        if (delta < 0) delta = -delta; // Convertimos a valor absoluto para detectar cambios hacia arriba o abajo

        if (cfg->sensors.alarm_limits.flow_delta_enabled && delta > flow_delta_thresh) {
            flow_warning = true;
        }
    }

    // 1.5.23: confirmation. The 18 h soak showed 100-200 ms Vain0 spikes right
    // after each MQTT publish (Wi-Fi TX noise) raising false leak alarms. A flow
    // condition must now hold FLOW_CONFIRM_US. The delta is re-checked against the
    // baseline captured at detection, so a real step is not lost if the window
    // rolls over meanwhile.
    if (flow_warning && !s_delta_pending) {
        s_delta_pending = true;
        s_delta_base = s_baseline_flow;
        s_delta_since_us = now;
    }
    if (s_delta_pending) {
        float d = isfinite(current_flow) ? current_flow - s_delta_base : 0.f;
        if (d < 0) d = -d;
        if (!cfg->sensors.alarm_limits.flow_delta_enabled || d <= flow_delta_thresh)
            s_delta_pending = false;               // transient: it came back
        // Same lifetime as before (about one window): a legitimate new flow level
        // must not latch the warning forever.
        if (now - s_delta_since_us > flow_window_ms * 1000LL + FLOW_CONFIRM_US)
            s_delta_pending = false;
        flow_warning = s_delta_pending && (now - s_delta_since_us) >= FLOW_CONFIRM_US;
    }
    if (flow_high) {
        if (!s_high_since_us) s_high_since_us = now;
        flow_high = (now - s_high_since_us) >= FLOW_CONFIRM_US;
    } else {
        s_high_since_us = 0;
    }

    // 2. Clasificación del peor escenario según norma hospitalaria
    alarm_clinical_state_t target_state = ALARM_STATE_NORMAL;

    // Escenario crítico (ALERT - Rojo): Variación severa de presión fuera de rangos vitales
    if ((p_min_enabled && current_pressure < (p_min - 15.0f)) ||
        (p_max_enabled && current_pressure > (p_max + 15.0f))) {
        target_state = ALARM_STATE_ALERT;
    }
    // Escenario moderado (WARNING - Amarillo): Cruce de límites de presión O variación súbita de flujo (Fuga)
    else if ((p_min_enabled && current_pressure < p_min) ||
             (p_max_enabled && current_pressure > p_max) || flow_warning || flow_high ||
             sensor_faults != 0) {
        // Fallo de comunicaciones de sensor = alarma tecnica SILENCIABLE (no ALERT permanente)
        target_state = ALARM_STATE_WARNING;
    }
    s_technical_only = target_state == ALARM_STATE_WARNING &&
                       !((p_min_enabled && current_pressure < p_min) ||
                         (p_max_enabled && current_pressure > p_max) || flow_warning || flow_high);

    // 3. Cláusula de Seguridad NFPA 99: Si pasa de Warning a Alert, se anula el Mute
    if (target_state == ALARM_STATE_ALERT && s_current_state == ALARM_STATE_WARNING) {
        if (s_is_muted) {
            ESP_LOGW(TAG, "Condicion medica escalo a CRITICA: se anula el silencio.");
            s_is_muted = false;
        }
    }

    // 3b. (1.6.7) El silencio vale SOLO para las condiciones presentes al silenciar
    //     (IEC 60601-1-8): una condicion NUEVA (p. ej. flujo con la presion baja ya
    //     silenciada) lo anula. Una condicion silenciada que desaparece por
    //     COND_REARM_US deja de estar reconocida: si vuelve, tambien suena.
    uint32_t conds = ((p_min_enabled && current_pressure < p_min) ? COND_P_LOW : 0u) |
                     ((p_max_enabled && current_pressure > p_max) ? COND_P_HIGH : 0u) |
                     (flow_warning ? COND_FLOW_DELTA : 0u) | (flow_high ? COND_FLOW_HIGH : 0u) |
                     (sensor_faults != 0 ? COND_SENSOR : 0u);
    s_active_conds = conds;
    s_crit_conds = ((p_min_enabled && current_pressure < (p_min - 15.0f)) ? COND_P_LOW : 0u) |
                   ((p_max_enabled && current_pressure > (p_max + 15.0f)) ? COND_P_HIGH : 0u);
    for (int b = 0; b < COND_COUNT; b++)
        if (conds & (1u << b)) s_cond_seen_us[b] = now;
    if (s_is_muted) {
        for (int b = 0; b < COND_COUNT; b++) {
            uint32_t bit = 1u << b;
            if ((s_muted_conds & bit) && !(conds & bit) && now - s_cond_seen_us[b] >= COND_REARM_US)
                s_muted_conds &= ~bit;
        }
        uint32_t fresh = conds & ~s_muted_conds;
        if (fresh) {
            ESP_LOGW(TAG, "Nueva condicion de alarma (0x%02lX) con el buzzer silenciado: se anula el silencio.",
                     (unsigned long)fresh);
            s_is_muted = false;
        }
    }

    // Actualizar primero y notificar SOLO si cambio: el consumidor que despierta
    // por el callback debe observar ya el nuevo estado al construir el payload.
    bool state_changed = target_state != s_current_state;
    // Diagnóstico de la "alarma fantasma": al ENTRAR en alarma registra el valor
    // exacto y QUÉ condición la disparó (el pico transitorio no se ve en el log
    // periódico de sensores cada ~2 s). Si es flujo con valor bajo -> glitch ADS.
    if (state_changed && target_state != ALARM_STATE_NORMAL) {
        ESP_LOGW(TAG, "ALARMA -> %d | P=%.2f kPa F=%.2f L/min | "
                      "p<min=%d p>max=%d flow_high=%d flow_warn=%d faults=0x%lX",
                 (int)target_state, (double)current_pressure, (double)current_flow,
                 (int)(p_min_enabled && current_pressure < p_min),
                 (int)(p_max_enabled && current_pressure > p_max),
                 (int)flow_high, (int)flow_warning, (unsigned long)sensor_faults);
    }
    s_current_state = target_state;
    if (state_changed && s_state_cb != NULL)
        s_state_cb(s_current_state);

    // 4. Evaluar fin del tiempo de silencio (Mute Timeout)
    if (s_is_muted && now >= s_mute_end_time_us) {
        ESP_LOGI(TAG, "Tiempo de silencio vencido. Reactivando senal sonora.");
        s_is_muted = false;
    }

    // Normal o silenciado: sin sonido.
    if (s_current_state == ALARM_STATE_NORMAL || s_is_muted) {
        alarm_tone_set(ALARM_TONE_OFF);
        return;
    }

    // Inhibicion de audio no-critico (p.ej. pantalla de login/config): un WARNING
    // o fallo tecnico NO debe pitar, pero un ALERT critico SI sigue sonando.
    if (s_audio_inhibit_noncritical && s_current_state != ALARM_STATE_ALERT) {
        alarm_tone_set(ALARM_TONE_OFF);
        return;
    }

    // 5. Senal acustica IEC 60601-1-8 segun prioridad (1.6.0; antes 150/150 ms y
    //    500/500 ms). Las banderas de NVS siguen pudiendo callar cada nivel.
    bool alert = s_current_state == ALARM_STATE_ALERT;
    bool tone_allowed = alert ? cfg->general.alarm.tone_alert : cfg->general.alarm.tone_warn;
    alarm_tone_set(!tone_allowed ? ALARM_TONE_OFF
                   : alert ? ALARM_TONE_HIGH
                   : s_technical_only ? ALARM_TONE_LOW
                   : ALARM_TONE_MEDIUM);
}

static void alarm_mgr_press_mute_impl(void)
{
    if (s_current_state == ALARM_STATE_NORMAL) return;

    const AppConfig *cfg = appcfg_cache_peek();   // snapshot en RAM (sin copiar)

    // Extraemos el tiempo exacto que el usuario configuró desde AWS o SD
    int timeout_seconds = 60 * ((s_current_state == ALARM_STATE_ALERT) ? cfg->general.alarm.max_silence_minutes : cfg->general.alarm.reannounce_minutes);

    s_is_muted = true;
    s_muted_conds = s_active_conds;   // solo estas quedan silenciadas (3b)
    alarm_tone_set(ALARM_TONE_OFF);
    s_mute_end_time_us = esp_timer_get_time() + ((int64_t)timeout_seconds * 1000000ULL);

    ESP_LOGW(TAG, "Buzzer silenciado por %d s (condiciones 0x%02lX).", timeout_seconds,
             (unsigned long)s_muted_conds);
    set_buzzer_acoustic(false); // Apagado instantáneo
}

/* 3 s: alcanza para una rafaga ALTA completa (2,6 s), como la prueba de MedGuard. */
static void alarm_mgr_test_buzzer_impl(void) { s_test_end_time_us = esp_timer_get_time() + 3000000LL; }

static alarm_clinical_state_t alarm_mgr_get_current_state_impl(void) { return s_current_state; }
static bool alarm_mgr_is_muted_impl(void) { return s_is_muted; }
static uint32_t alarm_mgr_get_sensor_faults_impl(void) { return s_sensor_faults; }

uint32_t alarm_mgr_get_active_conditions(uint32_t *critical)
{
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) {
        if (critical) *critical = 0;
        return 0;
    }
    uint32_t active = s_active_conds;
    if (critical) *critical = s_crit_conds & active;
    xSemaphoreGiveRecursive(s_alarm_lock);
    return active;
}

void alarm_mgr_set_audio_inhibit_noncritical(bool inhibit) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return;
    alarm_mgr_set_audio_inhibit_noncritical_impl(inhibit);
    xSemaphoreGiveRecursive(s_alarm_lock);
}

void alarm_mgr_set_state_change_cb(alarm_state_change_cb_t cb) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return;
    alarm_mgr_set_state_change_cb_impl(cb);
    xSemaphoreGiveRecursive(s_alarm_lock);
}

bool alarm_mgr_process(float current_pressure, float current_flow, uint32_t sensor_faults) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return false;
    alarm_mgr_process_impl(current_pressure, current_flow, sensor_faults);
    xSemaphoreGiveRecursive(s_alarm_lock);
    return true;
}

void alarm_mgr_press_mute(void) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return;
    alarm_mgr_press_mute_impl();
    xSemaphoreGiveRecursive(s_alarm_lock);
}

void alarm_mgr_test_buzzer(void) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return;
    alarm_mgr_test_buzzer_impl();
    xSemaphoreGiveRecursive(s_alarm_lock);
}

alarm_clinical_state_t alarm_mgr_get_current_state(void) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return ALARM_STATE_WARNING;
    alarm_clinical_state_t result = alarm_mgr_get_current_state_impl();
    xSemaphoreGiveRecursive(s_alarm_lock);
    return result;
}

bool alarm_mgr_is_muted(void) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return false;
    bool result = alarm_mgr_is_muted_impl();
    xSemaphoreGiveRecursive(s_alarm_lock);
    return result;
}

uint32_t alarm_mgr_get_sensor_faults(void) {
    if (!s_alarm_lock || xSemaphoreTakeRecursive(s_alarm_lock, pdMS_TO_TICKS(100)) != pdTRUE) return UINT32_MAX;
    uint32_t result = alarm_mgr_get_sensor_faults_impl();
    xSemaphoreGiveRecursive(s_alarm_lock);
    return result;
}
