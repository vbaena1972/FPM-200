#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
typedef struct {
    double volume_m3;     // consumo de HOY (se reinicia a las 00:00 locales)
    double total_m3;      // 1.6.8: totalizador de vida (persistente, ningun restablecer lo borra)
    uint64_t missing_ms;  // tiempo de hoy sin medicion (arranque, calentamiento del FS7, cortes)
    int since_min;        // 1.6.8: minuto del dia desde el que cuenta "hoy" (-1 = sin hora valida)
    bool partial;
    bool dated;
} flow_meter_snapshot_t;
// 1.6.8: ultimo guardado ANTERIOR a este arranque = valores al momento del corte
// (con un retraso maximo del periodo de guardado, 2 min). Lo reporta MQTT a AWS.
typedef struct {
    bool valid;
    char date[12];        // dia al que pertenece day_m3
    double day_m3;
    double total_m3;
    int64_t saved_epoch;  // 0 = el guardado no tenia hora valida
} flow_meter_checkpoint_t;
esp_err_t flow_meter_init(void); // after NVS, before acquisition/tasks
void flow_meter_record(int64_t sample_us, float lpm, bool valid);
flow_meter_snapshot_t flow_meter_get(void);
flow_meter_checkpoint_t flow_meter_boot_checkpoint(void);
void flow_meter_service(void); // slow storage work, never called by acquisition/UI
