#pragma once
/* alarm_tone — patrones de senal acustica de alarma IEC 60601-1-8 (tabla de
 * rafagas por prioridad). Modulo IDENTICO en MedGuard (firmware/system/) y
 * FPM-200 (components/drivers/); si se cambia uno, copiar al otro.
 *
 * El motor solo decide CUANDO suena cada pulso (esp_timer, independiente de LVGL
 * y del lazo principal); el hardware (frecuencia, volumen, PWM) lo pone cada
 * producto con su callback `out`. Ver docs/entrega/PRUEBAS_HW_1.8.0_1.6.0.md. */
#include <stdbool.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ALARM_TONE_OFF = 0,
    ALARM_TONE_LOW,      /* 2 pulsos, repite cada ~16 s              */
    ALARM_TONE_MEDIUM,   /* 3 pulsos, repite cada 5 s                */
    ALARM_TONE_HIGH,     /* 10 pulsos (3+2 · 3+2), repite cada 3 s   */
} alarm_tone_prio_t;

/* Enciende (true) o apaga (false) el tono portador. Se llama desde la tarea de
 * esp_timer o desde quien llama alarm_tone_set: debe ser corta y no bloquear. */
typedef void (*alarm_tone_out_t)(bool on, void *user);

esp_err_t alarm_tone_init(alarm_tone_out_t out, void *user);

/* Fija la prioridad que suena. Si es la misma que ya suena no hace nada (el
 * patron sigue su curso); si cambia, corta y arranca la nueva desde el 1er pulso. */
void alarm_tone_set(alarm_tone_prio_t prio);

alarm_tone_prio_t alarm_tone_get(void);

#ifdef __cplusplus
}
#endif
