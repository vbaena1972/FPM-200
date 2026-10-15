#include "flow_meter.h"
#include "flow_integrator.h"
#include "metrics_store.h"
#include "sensors_runtime.h"   // cubetas de la "24H" (1.6.9)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_log.h"
#include <string.h>
#include <time.h>

// 1.6.8: checkpoint cada 2 min (antes 10) para perder poco ante un corte de energia.
#define CHECKPOINT_US 120000000LL

static StaticSemaphore_t s_gate_storage;
static SemaphoreHandle_t s_gate;
static flow_integrator_t s_meter;
static app_metrics_t s_saved;          // lo que habia en NVS al arrancar
static flow_meter_checkpoint_t s_boot_cp;
static char s_date[12];
static bool s_partial, s_seeded, s_force_save;
static double s_total_m3;              // totalizador (1.6.8)
static int s_since_min = -1;           // minuto del dia desde el que cuenta "hoy"
static int64_t s_boot_us, s_save_us;
static uint32_t s_base_service_min;

esp_err_t flow_meter_init(void) {
    if (s_gate) return ESP_OK;
    esp_err_t err = appmetrics_load(&s_saved);
    if (err != ESP_OK) return err;
    s_base_service_min = s_saved.service_min;
    s_total_m3 = s_saved.total_m3;
    s_boot_cp.valid = s_saved.date[0] || s_saved.total_m3 > 0 || s_saved.saved_epoch > 0;
    memcpy(s_boot_cp.date, s_saved.date, sizeof(s_boot_cp.date));
    s_boot_cp.day_m3 = s_saved.consumo_m3;
    s_boot_cp.total_m3 = s_saved.total_m3;
    s_boot_cp.saved_epoch = s_saved.saved_epoch;
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
            // El apagado (del ultimo guardado al arranque) tampoco se midio: se suma
            // a "sin medir" para que un corte de energia quede a la vista.
            int64_t boot_wall = (int64_t)wall - (sample_us - s_boot_us) / 1000000;
            if (s_saved.saved_epoch > 0 && boot_wall > s_saved.saved_epoch)
                s_meter.missing_us += (uint64_t)(boot_wall - s_saved.saved_epoch) * 1000000ULL;
            s_since_min = s_saved.since_min < 1440 ? (int)s_saved.since_min : 0;
            // 1.6.9: el total de vida nunca es menor que el consumo de hoy (equipos que
            // pasaron por 1.6.8, donde el total arrancaba en 0 con HOY ya acumulado).
            if (s_total_m3 < s_meter.volume_m3) s_total_m3 = s_meter.volume_m3;
            // A periodic checkpoint cannot reconstruct measurements lost on reboot.
            s_partial = true;
        } else {
            // Dia nuevo sin guardado previo: "hoy" cuenta desde el arranque.
            int now_min = local.tm_hour * 60 + local.tm_min;
            int up_min = (int)((sample_us - s_boot_us) / 60000000LL);
            s_since_min = now_min > up_min ? now_min - up_min : 0;
        }
        memcpy(s_date, date, sizeof(s_date));
        s_seeded = true;
        s_force_save = true;
    } else if (date[0] && strcmp(date, s_date) > 0) {
        // Do not assign a cross-midnight interval to the wrong day. Account for
        // that interval as missing instead of silently claiming a complete day.
        uint64_t boundary_gap = s_meter.have_previous && sample_us > s_meter.previous_us
            ? (uint64_t)(sample_us - s_meter.previous_us) : 0;
        memset(&s_meter, 0, sizeof(s_meter));
        s_meter.missing_us = boundary_gap;
        s_partial = boundary_gap != 0;
        s_since_min = 0;
        memcpy(s_date, date, sizeof(s_date));
        s_force_save = true;               // el dia nuevo queda en NVS enseguida
    } else if (date[0] && strcmp(date, s_date) < 0) {
        // Backward SNTP/date corrections must not erase or reload consumption.
        s_partial = true;
    }
    double before = s_meter.volume_m3;
    flow_integrator_push(&s_meter, sample_us, lpm, valid);
    if (s_meter.volume_m3 > before) s_total_m3 += s_meter.volume_m3 - before;
    if (!date[0] || s_meter.missing_us) s_partial = true;
    xSemaphoreGive(s_gate);
}

flow_meter_snapshot_t flow_meter_get(void) {
    flow_meter_snapshot_t out = {0};
    out.since_min = -1;
    if (!s_gate) { out.partial = true; return out; }
    xSemaphoreTake(s_gate, portMAX_DELAY);
    out.volume_m3 = s_meter.volume_m3;
    out.total_m3 = s_total_m3;
    out.missing_ms = s_meter.missing_us / 1000;
    out.partial = s_partial;
    out.dated = s_seeded;
    out.since_min = s_seeded ? s_since_min : -1;
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

flow_meter_checkpoint_t flow_meter_boot_checkpoint(void) { return s_boot_cp; }

// 1.6.9: min/max de 24 h ("24H" de las tarjetas) en NVS: se recupera en el primer
// ciclo de servicio (ya corre la adquisicion: se FUSIONA con lo medido desde el
// arranque) y se guarda cada 10 min (~0,8 KB; perder <= 10 min de min/max ante un
// corte es aceptable y el desgaste de NVS queda en decenas de anios).
#define MM24_SAVE_US 600000000LL
static mm24_t s_mm24_buf;              // solo lo usa el worker de time_mgr
static bool s_mm24_restored;
static int64_t s_mm24_save_us;
static void mm24_service(int64_t now)
{
    if (!s_mm24_restored) {
        s_mm24_restored = true;
        s_mm24_save_us = now;
        if (appmetrics_load_blob("mm24", &s_mm24_buf, sizeof s_mm24_buf) == ESP_OK)
            sensors_runtime_mm24_restore(&s_mm24_buf);
        return;
    }
    if (now - s_mm24_save_us < MM24_SAVE_US) return;
    s_mm24_save_us = now;
    sensors_runtime_mm24_copy(&s_mm24_buf);
    esp_err_t err = appmetrics_store_blob("mm24", &s_mm24_buf, sizeof s_mm24_buf);
    if (err != ESP_OK) ESP_LOGW("flow_meter", "24H checkpoint failed: %s", esp_err_to_name(err));
}

void flow_meter_service(void) {
    if (!s_gate) return;
    int64_t now = esp_timer_get_time();
    mm24_service(now);
    if (!s_force_save && now - s_save_us < CHECKPOINT_US) return;
    app_metrics_t snapshot = s_saved;      // sin hora valida: conserva el dia guardado
    xSemaphoreTake(s_gate, portMAX_DELAY);
    if (s_seeded) {
        memcpy(snapshot.date, s_date, sizeof(snapshot.date));
        snapshot.consumo_m3 = (float)s_meter.volume_m3;
        snapshot.missing_ms = s_meter.missing_us / 1000;
        snapshot.partial = s_partial;
        snapshot.since_min = s_since_min >= 0 ? (uint32_t)s_since_min : 0;
    }
    snapshot.total_m3 = s_total_m3;
    snapshot.service_min = s_base_service_min + (uint32_t)((now - s_boot_us) / 60000000);
    s_force_save = false;
    xSemaphoreGive(s_gate);
    time_t wall = time(NULL);
    struct tm local;
    localtime_r(&wall, &local);
    snapshot.saved_epoch = local.tm_year >= 124 ? (int64_t)wall : 0;
    // Flash operations are outside the acquisition mutex and the LVGL task.
    esp_err_t err = appmetrics_store(&snapshot);
    if (err != ESP_OK) ESP_LOGW("flow_meter", "Checkpoint failed: %s", esp_err_to_name(err));
    s_save_us = err == ESP_OK ? now : now - CHECKPOINT_US + 10000000LL; // retry after 10 s
}
