/* Prueba de host de alarm_mgr (logica de estados, silencio y prioridad sonora).
 * alarm_tone se sustituye por un stub que registra la ultima prioridad pedida:
 * aqui se prueba QUE debe sonar, no la cadencia (esa la cubre
 * MedGuard test/alarm_tone/host, modulo identico). */
#include <assert.h>
#include <stdio.h>
#include "alarm_mgr.h"
#include "alarm_tone.h"
#include "storage.h"
#include "driver/ledc.h"

static int64_t now;
static unsigned transitions;
static AppConfig cfg;
static alarm_tone_prio_t tone = ALARM_TONE_OFF;

int64_t esp_timer_get_time(void) { return now; }
int appcfg_cache_get(AppConfig *out) { *out = cfg; return ESP_OK; }
AppConfig *appcfg_cache_peek(void) { return &cfg; }
int ledc_timer_config(const ledc_timer_config_t *c) { (void)c; return ESP_OK; }
int ledc_channel_config(const ledc_channel_config_t *c) { (void)c; return ESP_OK; }
int ledc_set_duty(int m, int c, uint32_t d) { (void)m; (void)c; (void)d; return ESP_OK; }
int ledc_update_duty(int m, int c) { (void)m; (void)c; return ESP_OK; }
esp_err_t alarm_tone_init(alarm_tone_out_t out, void *user) { (void)out; (void)user; return ESP_OK; }
void alarm_tone_set(alarm_tone_prio_t p) { tone = p; }
alarm_tone_prio_t alarm_tone_get(void) { return tone; }
static void changed(alarm_clinical_state_t s) { (void)s; ++transitions; }

#define S(sec) ((int64_t)((sec) * 1000000.0))
static void at(double sec, float p, float f, uint32_t faults) { now = S(sec); alarm_mgr_process(p, f, faults); }

int main(void)
{
    cfg.general.alarm.volume = 80; cfg.general.alarm.tone_alert = true; cfg.general.alarm.tone_warn = true;
    cfg.general.alarm.max_silence_minutes = 2; cfg.general.alarm.reannounce_minutes = 15;
    cfg.sensors.alarm_limits.pressure_min_enabled = true; cfg.sensors.alarm_limits.pressure_min = 6.9f;  /* 1 psi */
    cfg.sensors.alarm_limits.pressure_max_enabled = true; cfg.sensors.alarm_limits.pressure_max = 20.7f; /* 3 psi */
    cfg.sensors.alarm_limits.flow_delta_window_ms = 2000;
    cfg.sensors.alarm_limits.flow_high_limit = 50.0f;
    assert(alarm_mgr_init(36) == ESP_OK);
    alarm_mgr_set_state_change_cb(changed);

    /* Fallo de sensor solo = alarma tecnica: WARNING con prioridad BAJA. */
    at(1, 10, 0, 8);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_WARNING && tone == ALARM_TONE_LOW);
    at(2, 10, 0, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_NORMAL && tone == ALARM_TONE_OFF);

    /* P2: presion bajo el minimo = WARNING (MEDIA); silenciar; subir el minimo
     * hasta que quede > 15 kPa por encima -> ALERT rompe el silencio (ALTA). */
    at(10, 0.1f, 0, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_WARNING && tone == ALARM_TONE_MEDIUM);
    alarm_mgr_press_mute();
    assert(alarm_mgr_is_muted() && tone == ALARM_TONE_OFF);
    at(11, 0.1f, 0, 0);
    assert(alarm_mgr_is_muted() && tone == ALARM_TONE_OFF);        /* misma condicion: sigue en silencio */
    cfg.sensors.alarm_limits.pressure_min = 20.7f; cfg.sensors.alarm_limits.pressure_max = 34.5f;  /* 3/5 psi */
    at(12, 0.1f, 0, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_ALERT);
    assert(!alarm_mgr_is_muted() && tone == ALARM_TONE_HIGH);
    {   /* 1.6.8: la presion baja esta en banda critica */
        uint32_t c = 0;
        assert(alarm_mgr_get_active_conditions(&c) == ALARM_COND_P_LOW && c == ALARM_COND_P_LOW);
    }

    /* 1.6.7: con la presion baja silenciada, una condicion NUEVA de flujo rompe el silencio. */
    cfg.sensors.alarm_limits.pressure_min = 6.9f; cfg.sensors.alarm_limits.pressure_max = 20.7f;
    at(20, 0.1f, 0, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_WARNING);
    alarm_mgr_press_mute();
    at(21, 0.1f, 0, 0);
    assert(alarm_mgr_is_muted() && tone == ALARM_TONE_OFF);
    cfg.sensors.alarm_limits.flow_high_enabled = true;
    at(21.2, 0.1f, 60, 0);                                         /* aun sin confirmar (< 0,5 s) */
    assert(alarm_mgr_is_muted());
    at(21.8, 0.1f, 60, 0);                                         /* flujo alto confirmado */
    assert(!alarm_mgr_is_muted() && tone == ALARM_TONE_MEDIUM);
    /* 1.6.8: la UI ve las DOS condiciones (presion baja + flujo alto), ninguna critica. */
    uint32_t crit = 0xFF;
    assert(alarm_mgr_get_active_conditions(&crit) == (ALARM_COND_P_LOW | ALARM_COND_FLOW_HIGH) && crit == 0);
    cfg.sensors.alarm_limits.flow_high_enabled = false;

    /* Fallo de sensor nuevo con la presion silenciada: tambien rompe el silencio. */
    at(30, 0.1f, 0, 0);
    alarm_mgr_press_mute();
    at(31, 0.1f, 0, 4);
    assert(!alarm_mgr_is_muted());

    /* La misma condicion que parpadea (< 10 s ausente) NO rompe el silencio;
     * si estuvo ausente >= 10 s es un episodio nuevo y SI suena. */
    at(40, 0.1f, 0, 0);
    alarm_mgr_press_mute();
    at(41, 10, 0, 0);                                              /* normal 3 s */
    assert(alarm_mgr_get_current_state() == ALARM_STATE_NORMAL && tone == ALARM_TONE_OFF);
    at(44, 0.1f, 0, 0);
    assert(alarm_mgr_is_muted() && tone == ALARM_TONE_OFF);
    at(45, 10, 0, 0);
    at(56, 10, 0, 0);                                              /* normal > 10 s */
    at(57, 0.1f, 0, 0);
    assert(!alarm_mgr_is_muted() && tone == ALARM_TONE_MEDIUM);

    /* El silencio vence solo (reannounce_minutes). */
    alarm_mgr_press_mute();
    at(58 + 15 * 60 + 1, 0.1f, 0, 0);
    assert(!alarm_mgr_is_muted() && tone == ALARM_TONE_MEDIUM);

    /* Pantalla de login/config: calla lo no critico, nunca un ALERT. */
    alarm_mgr_set_audio_inhibit_noncritical(true);
    at(2000, 0.1f, 0, 0);
    assert(tone == ALARM_TONE_OFF);
    cfg.sensors.alarm_limits.pressure_min = 20.7f;
    at(2001, 0.1f, 0, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_ALERT && tone == ALARM_TONE_HIGH);
    alarm_mgr_set_audio_inhibit_noncritical(false);

    /* Prueba del buzzer: ALTA acotada a 3 s. */
    cfg.sensors.alarm_limits.pressure_min = 6.9f;
    at(3000, 10, 0, 0);
    assert(tone == ALARM_TONE_OFF);
    alarm_mgr_test_buzzer();
    at(3001, 10, 0, 0);
    assert(tone == ALARM_TONE_HIGH);
    at(3004, 10, 0, 0);
    assert(tone == ALARM_TONE_OFF);

    /* 1.5.23: un pico de flujo de 200 ms (ruido Wi-Fi) no levanta fuga; un escalon sostenido si. */
    cfg.sensors.alarm_limits.flow_delta_enabled = true; cfg.sensors.alarm_limits.flow_delta_threshold = 5;
    at(4000, 10, 0, 0);
    at(4000.1, 10, 12, 0); at(4000.2, 10, 12, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_NORMAL);
    at(4000.3, 10, 0, 0);
    for (int i = 1; i <= 4; i++) { at(4000.3 + i * 0.1, 10, 12, 0); assert(alarm_mgr_get_current_state() == ALARM_STATE_NORMAL); }
    at(4000.9, 10, 12, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_WARNING && tone == ALARM_TONE_MEDIUM);
    at(4005, 10, 12, 0);
    assert(alarm_mgr_get_current_state() == ALARM_STATE_NORMAL);

    assert(transitions >= 8);
    puts("PASS: alarma tecnica BAJA, P2 (WARNING->ALERT rompe el silencio), condicion nueva rompe el silencio,"
         " parpadeo < 10 s no, vencimiento del silencio, inhibicion UI, prueba de buzzer, confirmacion de flujo");
    return 0;
}
