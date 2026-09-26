#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "alarm_mgr.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/task.h"
#include "storage.h"
#include <math.h>

static SemaphoreHandle_t s_alarm_lock;
static const char *TAG = "alarm_mgr";

// ParÃƒÆ’Ã‚Â¡metros de PWM adaptados para el Driver PAM8906
#define LEDC_SPEED_MODE         LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT
#define BUZZER_FREQ_HZ          2000    // Resonancia ÃƒÆ’Ã‚Â³ptima para mÃƒÆ’Ã‚Â¡xima presiÃƒÆ’Ã‚Â³n sonora
#define BUZZER_DUTY_50_PERCENT  512     // 50% de 1024 para onda cuadrada perfecta

// Variables operacionales de control
static alarm_clinical_state_t s_current_state = ALARM_STATE_NORMAL;
static bool s_is_muted = false;
static int64_t s_mute_end_time_us = 0;
static int64_t s_last_toggle_time_us = 0;
static bool s_beep_active = false;
static int64_t s_test_end_time_us = 0;
static int s_volume = 80;
static uint32_t s_sensor_faults = 0;
static alarm_state_change_cb_t s_state_cb = NULL;
// Cuando true, se silencia el buzzer para estados NO criticos (WARNING/fallo
// tecnico). Un ALERT critico de presion SIEMPRE sigue sonando. Lo activa la UI
// mientras se muestra la pantalla de login/config (no debe pitar al teclear PIN).
static bool s_audio_inhibit_noncritical = false;

static void set_buzzer_acoustic(bool turn_on);

static void alarm_mgr_set_audio_inhibit_noncritical_impl(bool inhibit)
{
    s_audio_inhibit_noncritical = inhibit;
    if (inhibit && s_current_state != ALARM_STATE_ALERT) {
        s_beep_active = false;
        set_buzzer_acoustic(false);
    }
}

static void alarm_mgr_set_state_change_cb_impl(alarm_state_change_cb_t cb)
{
    s_state_cb = cb;
}

// Variables para el anÃƒÆ’Ã‚Â¡lisis matemÃƒÆ’Ã‚Â¡tico diferencial de flujo (Delta)
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
    return ledc_channel_config(&ledc_channel);
}

static void set_buzzer_acoustic(bool turn_on)
{
    // El PAM8906 amplifica la seÃƒÆ’Ã‚Â±al cuadrada diferencial cuando detecta pulsos en DIN
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
    if (now < s_test_end_time_us) { set_buzzer_acoustic(true); return; }

    // --- EVALUACIÃƒÆ’Ã¢â‚¬Å“N DE PRESIÃƒÆ’Ã¢â‚¬Å“N ---
    bool p_min_enabled = cfg->sensors.alarm_limits.pressure_min_enabled;
    bool p_max_enabled = cfg->sensors.alarm_limits.pressure_max_enabled;
    float p_min = cfg->sensors.alarm_limits.pressure_min;
    float p_max = cfg->sensors.alarm_limits.pressure_max;

    // --- EVALUACIÃƒÆ’Ã¢â‚¬Å“N DE FLUJO (AnÃƒÆ’Ã‚Â¡lisis de Delta temporal) ---
    float flow_delta_thresh = cfg->sensors.alarm_limits.flow_delta_threshold;
    int flow_window_ms = cfg->sensors.alarm_limits.flow_delta_window_ms;
    bool flow_warning = false;
    bool flow_high = cfg->sensors.alarm_limits.flow_high_enabled && current_flow > cfg->sensors.alarm_limits.flow_high_limit;

    // Si la ventana de tiempo ya caducÃƒÆ’Ã‚Â³ o es el primer arranque, tomamos una nueva lÃƒÆ’Ã‚Â­nea base de flujo
    if (!isfinite(current_flow)) {
        // Invalid flow: restart the delta window once valid data returns.
        s_baseline_time_us = 0;
    } else if (s_baseline_time_us == 0 || (now - s_baseline_time_us) >= (flow_window_ms * 1000LL)) {
        s_baseline_flow = current_flow;
        s_baseline_time_us = now;
    } else {
        // Evaluamos si el flujo tuvo un pico o caÃƒÆ’Ã‚Â­da abrupta dentro de la ventana de tiempo actual
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

    // 2. ClasificaciÃƒÆ’Ã‚Â³n del peor escenario segÃƒÆ’Ã‚Âºn norma hospitalaria
    alarm_clinical_state_t target_state = ALARM_STATE_NORMAL;

    // Escenario crÃƒÆ’Ã‚Â­tico (ALERT - Rojo): VariaciÃƒÆ’Ã‚Â³n severa de presiÃƒÆ’Ã‚Â³n fuera de rangos vitales
    if ((p_min_enabled && current_pressure < (p_min - 15.0f)) ||
        (p_max_enabled && current_pressure > (p_max + 15.0f))) {
        target_state = ALARM_STATE_ALERT;
    }
    // Escenario moderado (WARNING - Amarillo): Cruce de lÃƒÆ’Ã‚Â­mites de presiÃƒÆ’Ã‚Â³n O variaciÃƒÆ’Ã‚Â³n sÃƒÆ’Ã‚Âºbita de flujo (Fuga)
    else if ((p_min_enabled && current_pressure < p_min) ||
             (p_max_enabled && current_pressure > p_max) || flow_warning || flow_high ||
             sensor_faults != 0) {
        // Fallo de comunicaciones de sensor = alarma tecnica SILENCIABLE (no ALERT permanente)
        target_state = ALARM_STATE_WARNING;
    }

    // 3. ClÃƒÆ’Ã‚Â¡usula de Seguridad NFPA 99: Si pasa de Warning a Alert, se anula el Mute
    if (target_state == ALARM_STATE_ALERT && s_current_state == ALARM_STATE_WARNING) {
        if (s_is_muted) {
            ESP_LOGW(TAG, "Ãƒâ€šÃ‚Â¡CondiciÃƒÆ’Ã‚Â³n mÃƒÆ’Ã‚Â©dica escalÃƒÆ’Ã‚Â³ a CRÃƒÆ’Ã‚ÂTICA! Rompiendo Mute automÃƒÆ’Ã‚Â¡ticamente.");
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
    if (state_changed) {
        s_beep_active = false;
        s_last_toggle_time_us = now - (target_state == ALARM_STATE_ALERT ? 150000 : 500000);
    }
    if (state_changed && s_state_cb != NULL)
        s_state_cb(s_current_state);

    // 4. Evaluar fin del tiempo de silencio (Mute Timeout)
    if (s_is_muted && now >= s_mute_end_time_us) {
        ESP_LOGI(TAG, "Tiempo de Mute vencido. Reactivando seÃƒÆ’Ã‚Â±al sonora.");
        s_is_muted = false;
    }

    // Si todo estÃƒÆ’Ã‚Â¡ normal o el sistema estÃƒÆ’Ã‚Â¡ silenciado, apagamos el PWM de inmediato
    if (s_current_state == ALARM_STATE_NORMAL || s_is_muted) {
        s_beep_active = false;
        set_buzzer_acoustic(false);
        return;
    }

    // Inhibicion de audio no-critico (p.ej. pantalla de login/config): un WARNING
    // o fallo tecnico NO debe pitar, pero un ALERT critico SI sigue sonando.
    if (s_audio_inhibit_noncritical && s_current_state != ALARM_STATE_ALERT) {
        s_beep_active = false;
        set_buzzer_acoustic(false);
        return;
    }

    // 5. Cadencia del Beep segÃƒÆ’Ã‚Âºn Severidad
    // Alert: RÃƒÆ’Ã‚Â¡fagas muy rÃƒÆ’Ã‚Â¡pidas (150ms encendido / 150ms apagado)
    // Warning: RÃƒÆ’Ã‚Â¡fagas lentas y espaciadas (500ms encendido / 500ms apagado)
    int64_t toggle_interval_us = (s_current_state == ALARM_STATE_ALERT) ? 150000 : 500000;

    if (now - s_last_toggle_time_us >= toggle_interval_us) {
        s_beep_active = !s_beep_active;
        s_last_toggle_time_us = now;
        
        // Verificamos si las banderas en NVS tienen el sonido activo para este nivel
        bool tone_allowed = (s_current_state == ALARM_STATE_ALERT) ? cfg->general.alarm.tone_alert : cfg->general.alarm.tone_warn;
        
        set_buzzer_acoustic(s_beep_active && tone_allowed);
    }
}

static void alarm_mgr_press_mute_impl(void)
{
    if (s_current_state == ALARM_STATE_NORMAL) return;

    const AppConfig *cfg = appcfg_cache_peek();   // snapshot en RAM (sin copiar)

    // Extraemos el tiempo exacto que el usuario configurÃƒÆ’Ã‚Â³ desde AWS o SD
    int timeout_seconds = 60 * ((s_current_state == ALARM_STATE_ALERT) ? cfg->general.alarm.max_silence_minutes : cfg->general.alarm.reannounce_minutes);

    s_is_muted = true;
    s_beep_active = false;
    s_mute_end_time_us = esp_timer_get_time() + ((int64_t)timeout_seconds * 1000000ULL);

    ESP_LOGW(TAG, "Buzzer silenciado vÃƒÆ’Ã‚Â­a software por %d segundos.", timeout_seconds);
    set_buzzer_acoustic(false); // Apagado instantÃƒÆ’Ã‚Â¡neo
}

static void alarm_mgr_test_buzzer_impl(void) { s_test_end_time_us = esp_timer_get_time() + 2000000LL; }

static alarm_clinical_state_t alarm_mgr_get_current_state_impl(void) { return s_current_state; }
static bool alarm_mgr_is_muted_impl(void) { return s_is_muted; }
static uint32_t alarm_mgr_get_sensor_faults_impl(void) { return s_sensor_faults; }

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
