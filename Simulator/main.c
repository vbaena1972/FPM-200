/*
 * main.c — Simulador de PC de la HMI del FPM-200 (LVGL 9 + driver de Windows de LVGL).
 *
 * Compila los MISMOS firmware/main/ui/**.c que corren en el ESP32-S3 (pantalla ST7796 480x320,
 * aqui con zoom 200 %) y cicla 4 escenarios de demostracion. Ver README.md.
 */
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "lvgl.h"
#include "ui.h"
#include "screens/ui_mainScreen.h"
#include "sensors_runtime.h"
#include "storage.h"
#include "alarm_mgr.h"

AppConfig *appcfg_cache_peek(void);

/* Presion en kPa coherente con los limites de sim_backend.c (500 psi = 3447 kPa); flujo en L/min. */
typedef struct {
    const char *label;
    float pressure_kpa;
    float flow_lpm;
    alarm_clinical_state_t state;
    bool muted;
} scenario_t;

static const scenario_t s_scen[] = {
    { "NORMAL", 965.3f, 59.0f, ALARM_STATE_NORMAL, false },
    { "ADVERTENCIA (flujo)", 951.5f, 86.0f, ALARM_STATE_WARNING, false },
    { "ADVERTENCIA (presion baja)", 330.0f, 59.0f, ALARM_STATE_WARNING, false },
    { "ALARMA (presion baja)", 289.6f, 44.0f, ALARM_STATE_ALERT, false },
    { "ALARMA SILENCIADA", 289.6f, 44.0f, ALARM_STATE_ALERT, true },
};
static int s_idx;

static void apply_scenario(int i)
{
    const scenario_t *s = &s_scen[i];
    int64_t now_ms = (int64_t)lv_tick_get();
    sensor_sample_t last = { .ts_ms = now_ms, .pressure_kpa = s->pressure_kpa, .flow_lpm = s->flow_lpm };
    sensor_sample_t mn = { .ts_ms = now_ms, .pressure_kpa = s->pressure_kpa * 0.92f,
                           .flow_lpm = s->flow_lpm * 0.85f };
    sensor_sample_t mx = { .ts_ms = now_ms, .pressure_kpa = s->pressure_kpa * 1.05f,
                           .flow_lpm = s->flow_lpm * 1.12f };
    ui_main_update(&last, true, &mn, &mx, true, appcfg_cache_peek(), s->state, s->muted);

    SYSTEMTIME st;
    char hhmm[8], date[16];
    GetLocalTime(&st);
    snprintf(hhmm, sizeof hhmm, "%02u:%02u", st.wHour, st.wMinute);
    snprintf(date, sizeof date, "%02u/%02u/%04u", st.wDay, st.wMonth, st.wYear);
    ui_main_set_clock(hhmm);
    ui_main_set_date(date);
}

static void scenario_timer_cb(lv_timer_t *t)
{
    (void)t;
    s_idx = (s_idx + 1) % (int)(sizeof s_scen / sizeof s_scen[0]);
    apply_scenario(s_idx);
}

static void data_activity_cb(lv_timer_t *t)
{
    (void)t;
    ui_main_signal_data_activity();
}

/* Opcional: fpm200_sim.exe login -> abre el login a los 4 s (tras el splash), para revisarlo. */
static void open_login_cb(lv_timer_t *t)
{
    lv_timer_delete(t);
    ui_open_login_cb(NULL);
}

int main(int argc, char **argv)
{
    lv_init();
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    lv_display_t *display = lv_windows_create_display(L"FPM-200 — HMI (simulador, 480x320)", 480, 320,
                                                      200, false, true);
    if (display == NULL || lv_windows_acquire_pointer_indev(display) == NULL) {
        return -1;
    }

    ui_init();
    apply_scenario(0);
    lv_timer_create(scenario_timer_cb, 2500, NULL);   /* NORMAL -> ADVERTENCIA (flujo, presion) -> ALARMA -> SILENCIADA */
    lv_timer_create(data_activity_cb, 1000, NULL);
    if (argc > 1 && strcmp(argv[1], "login") == 0) {
        lv_timer_create(open_login_cb, 4000, NULL);
    }

    /* El driver de Windows asigna el buffer de dibujo DIFERIDO: bombear timers hasta que exista y
     * forzar un refresco sincrono (si no, una pantalla estatica puede quedar en blanco). */
    for (int guard = 0; guard < 2000 && !lv_display_get_buf_active(display); ++guard) {
        lv_timer_handler();
        lv_delay_ms(1);
    }
    lv_refr_now(display);

    while (1) {
        uint32_t next = lv_timer_handler();
        lv_delay_ms(next < 5 ? next : 5);
    }
    return 0;
}
