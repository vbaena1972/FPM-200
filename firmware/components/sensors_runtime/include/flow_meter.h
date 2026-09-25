#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
typedef struct {
    double volume_m3;
    uint64_t missing_ms;
    bool partial;
    bool dated;
} flow_meter_snapshot_t;
esp_err_t flow_meter_init(void); // after NVS, before acquisition/tasks
void flow_meter_record(int64_t sample_us, float lpm, bool valid);
flow_meter_snapshot_t flow_meter_get(void);
void flow_meter_service(void); // slow storage work, never called by acquisition/UI
