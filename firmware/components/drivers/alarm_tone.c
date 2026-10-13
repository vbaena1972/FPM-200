/* alarm_tone — patrones IEC 60601-1-8 (ver alarm_tone.h). IDENTICO en MedGuard
 * (firmware/system/alarm_tone.c) y FPM-200 (components/drivers/alarm_tone.c).
 *
 * Tiempos elegidos (IEC 60601-1-8, tabla de caracteristicas de la rafaga):
 *   ALTA   10 pulsos td=100 ms (75-200), separacion x=100 ms (50-125);
 *          3er-4o = 2x+td = 300 ms; 5o-6o = 400 ms (0,35-1,30 s);
 *          entre rafagas tb = 3 s (2,5-15 s).          Ritmo: *** **  *** **
 *   MEDIA  3 pulsos td=200 ms (125-250), separacion 200 ms (125-250);
 *          tb = 5 s (2,5-30 s).
 *   BAJA   2 pulsos td=200 ms, separacion 200 ms; repite cada 16 s (> 15 s).
 * Cada tabla alterna ENCENDIDO, APAGADO, ... y termina en APAGADO (= tb).
 * NO se cumple con el transductor de V1: frecuencia fundamental 150-1000 Hz con
 * >= 4 armonicos (V1 es un tono puro de 2-4 kHz) ni la rampa de subida/bajada del
 * pulso (10-20 % de td); ver PRUEBAS_HW_1.8.0_1.6.0.md. */
#include "alarm_tone.h"

#include <stddef.h>
#include <stdint.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static const char *TAG = "alarm_tone";

static const uint16_t PATTERN_HIGH[] = {
    100, 100, 100, 100, 100, 300, 100, 100, 100, 400,
    100, 100, 100, 100, 100, 300, 100, 100, 100, 3000,
};
static const uint16_t PATTERN_MEDIUM[] = { 200, 200, 200, 200, 200, 5000 };
static const uint16_t PATTERN_LOW[] = { 200, 200, 200, 16000 };

typedef struct {
    const uint16_t *steps;
    size_t count;
    const char *name;
} pattern_t;

#define PATTERN(a, n) { (a), sizeof(a) / sizeof((a)[0]), (n) }
static const pattern_t PATTERNS[] = {
    [ALARM_TONE_OFF] = { NULL, 0, "apagado" },
    [ALARM_TONE_LOW] = PATTERN(PATTERN_LOW, "BAJA"),
    [ALARM_TONE_MEDIUM] = PATTERN(PATTERN_MEDIUM, "MEDIA"),
    [ALARM_TONE_HIGH] = PATTERN(PATTERN_HIGH, "ALTA"),
};

static esp_timer_handle_t s_timer;
static SemaphoreHandle_t s_lock;
static alarm_tone_out_t s_out;
static void *s_user;
static alarm_tone_prio_t s_prio = ALARM_TONE_OFF;
static size_t s_step;
static int64_t s_due_us;   /* cuando vence el paso programado */

/* Con s_lock tomado: aplica el paso actual y programa el siguiente. */
static void apply_step_locked(void)
{
    const pattern_t *p = &PATTERNS[s_prio];
    const uint64_t us = (uint64_t)p->steps[s_step] * 1000ULL;
    s_out((s_step % 2U) == 0U, s_user);
    s_due_us = esp_timer_get_time() + (int64_t)us;
    esp_timer_start_once(s_timer, us);
}

static void timer_cb(void *arg)
{
    (void)arg;
    xSemaphoreTake(s_lock, portMAX_DELAY);
    /* Si alarm_tone_set cambio la prioridad mientras este disparo esperaba el
     * lock, ya programo un paso nuevo que vence mas tarde (>= 100 ms): este
     * disparo es viejo y se ignora. NO usar esp_timer_is_active(): en ESP-IDF
     * 6.1 da true mientras corre el propio callback (1.8.0/1.6.0 se quedaban
     * en el primer pulso = tono continuo, HW 2026-10-12). */
    if (s_prio != ALARM_TONE_OFF && esp_timer_get_time() + 1000 >= s_due_us) {
        s_step = (s_step + 1U) % PATTERNS[s_prio].count;
        apply_step_locked();
    }
    xSemaphoreGive(s_lock);
}

esp_err_t alarm_tone_init(alarm_tone_out_t out, void *user)
{
    if (!out) return ESP_ERR_INVALID_ARG;
    if (s_lock) return ESP_OK;
    s_lock = xSemaphoreCreateMutex();
    if (!s_lock) return ESP_ERR_NO_MEM;
    const esp_timer_create_args_t args = {
        .callback = timer_cb,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "alarm_tone",
    };
    esp_err_t err = esp_timer_create(&args, &s_timer);
    if (err != ESP_OK) {
        vSemaphoreDelete(s_lock);
        s_lock = NULL;
        return err;
    }
    s_out = out;
    s_user = user;
    s_out(false, s_user);
    return ESP_OK;
}

void alarm_tone_set(alarm_tone_prio_t prio)
{
    if (!s_lock) return;
    if ((int)prio < ALARM_TONE_OFF || prio > ALARM_TONE_HIGH) prio = ALARM_TONE_HIGH;
    xSemaphoreTake(s_lock, portMAX_DELAY);
    if (prio != s_prio) {
        esp_timer_stop(s_timer);
        s_prio = prio;
        s_step = 0;
        if (prio == ALARM_TONE_OFF) s_out(false, s_user);
        else apply_step_locked();
        ESP_LOGI(TAG, "Senal acustica: %s", PATTERNS[prio].name);
    }
    xSemaphoreGive(s_lock);
}

alarm_tone_prio_t alarm_tone_get(void)
{
    return s_prio;
}
