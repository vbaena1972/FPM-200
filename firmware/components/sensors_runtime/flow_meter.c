#include "flow_meter.h"
#include "flow_integrator.h"
#include "metrics_store.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <string.h>
#include <time.h>

static StaticSemaphore_t s_gate_storage;
static SemaphoreHandle_t s_gate;
static flow_integrator_t s_meter;
static app_metrics_t s_saved;
static char s_date[12];
static bool s_partial, s_seeded;
static int64_t s_boot_us, s_save_us;
static uint32_t s_base_service_min;

esp_err_t flow_meter_init(void) {
    if (s_gate) return ESP_OK;
    esp_err_t err = appmetrics_load(&s_saved);
    if (err != ESP_OK) return err;
    s_base_service_min = s_saved.service_min;
    s_boot_us = s_save_us = esp_timer_get_time();
    s_gate = xSemaphoreCreateMutexStatic(&s_gate_storage);
    return s_gate ? ESP_OK : ESP_ERR_NO_MEM;
}

void flow_meter_record(int64_t sample_us, float lpm, bool valid) {
    if (!s_gate) return;
    time_t wall = time(NULL);
    struct tm local;
    localtime_r(&wall, &local);
    char date[12] = {0};
    if (local.tm_year >= 124) strftime(date, sizeof(date), "%Y-%m-%d", &local);
    xSemaphoreTake(s_gate, portMAX_DELAY);
    if (date[0] && !s_seeded) {
        if (!strcmp(date, s_saved.date)) {
            if (isfinite(s_saved.consumo_m3) && s_saved.consumo_m3 >= 0)
                s_meter.volume_m3 += s_saved.consumo_m3;
            s_meter.missing_us += s_saved.missing_ms * 1000;
            // A periodic checkpoint cannot reconstruct measurements lost on reboot.
            s_partial = true;
        }
        memcpy(s_date, date, sizeof(s_date));
        s_seeded = true;
    } else if (date[0] && strcmp(date, s_date) > 0) {
        // Do not assign a cross-midnight interval to the wrong day. Account for
        // that interval as missing instead of silently claiming a complete day.
        uint64_t boundary_gap = s_meter.have_previous && sample_us > s_meter.previous_us
            ? (uint64_t)(sample_us - s_meter.previous_us) : 0;
        memset(&s_meter, 0, sizeof(s_meter));
        s_meter.missing_us = boundary_gap;
        s_partial = boundary_gap != 0;
        memcpy(s_date, date, sizeof(s_date));
    } else if (date[0] && strcmp(date, s_date) < 0) {
        // Backward SNTP/date corrections must not erase or reload consumption.
        s_partial = true;
    }
    flow_integrator_push(&s_meter, sample_us, lpm, valid);
    if (!date[0] || s_meter.missing_us) s_partial = true;
    xSemaphoreGive(s_gate);
}

flow_meter_snapshot_t flow_meter_get(void) {
    flow_meter_snapshot_t out = {0};
    if (!s_gate) { out.partial = true; return out; }
    xSemaphoreTake(s_gate, portMAX_DELAY);
    out.volume_m3 = s_meter.volume_m3;
    out.missing_ms = s_meter.missing_us / 1000;
    out.partial = s_partial;
    out.dated = s_seeded;
    // If acquisition stops entirely, show loss even before the next sample.
    if (!s_meter.have_previous) out.partial = true;
    else {
        int64_t gap_us = esp_timer_get_time() - s_meter.previous_us;
        if (gap_us > 1000000) {
            out.partial = true;
            out.missing_ms += (uint64_t)gap_us / 1000;
        }
    }
    xSemaphoreGive(s_gate);
    return out;
}

void flow_meter_service(void) {
    if (!s_gate) return;
    int64_t now = esp_timer_get_time();
    if (now - s_save_us < 600000000) return;
    app_metrics_t snapshot = {0};
    xSemaphoreTake(s_gate, portMAX_DELAY);
    if (!s_seeded) { xSemaphoreGive(s_gate); return; }
    memcpy(snapshot.date, s_date, sizeof(snapshot.date));
    snapshot.consumo_m3 = (float)s_meter.volume_m3;
    snapshot.service_min = s_base_service_min + (uint32_t)((now - s_boot_us) / 60000000);
    snapshot.missing_ms = s_meter.missing_us / 1000;
    snapshot.partial = s_partial;
    xSemaphoreGive(s_gate);
    // Flash operations are outside the acquisition mutex and the LVGL task.
    esp_err_t err = appmetrics_store(&snapshot);
    if (err != ESP_OK) ESP_LOGW("flow_meter", "Checkpoint failed: %s", esp_err_to_name(err));
    s_save_us = err == ESP_OK ? now : now - 590000000; // retry after 10 s
}
