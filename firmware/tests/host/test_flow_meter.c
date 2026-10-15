/* Prueba de host de flow_meter (1.6.8): totalizador, guardado periodico, restauracion
 * del consumo de HOY tras un corte de energia (con el apagado en "sin medir"), cambio
 * de dia y el guardado previo al arranque que se reporta a AWS.
 * gcc -std=gnu11 -Itests/host/stubs_fm -Itests/host/stubs -Icomponents/sensors_runtime/include
 *     -Icomponents/storage/include tests/host/test_flow_meter.c -o build/host/t_fm && build/host/t_fm */
#define _POSIX_THREAD_SAFE_FUNCTIONS 1
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Reloj de pared y de arranque controlados por la prueba. */
static time_t g_wall;
static int64_t g_up_us;
#define time(p) (g_wall)
int64_t esp_timer_get_time(void) { return g_up_us; }

/* NVS "metrics" en memoria. */
#include "metrics_store.h"
static app_metrics_t g_nvs;
static int g_stores;
esp_err_t appmetrics_load(app_metrics_t *out) { *out = g_nvs; return ESP_OK; }
esp_err_t appmetrics_store(const app_metrics_t *in) { g_nvs = *in; g_stores++; return ESP_OK; }
/* "24H" (1.6.9): aqui solo se comprueba que el servicio la restaura y la guarda. */
#include "minmax24.h"
static int g_mm24_restores, g_mm24_stores;
esp_err_t appmetrics_load_blob(const char *k, void *b, size_t n) { (void)k; (void)b; (void)n; return ESP_OK; }
esp_err_t appmetrics_store_blob(const char *k, const void *b, size_t n) { (void)k; (void)b; (void)n; g_mm24_stores++; return ESP_OK; }
void sensors_runtime_mm24_copy(mm24_t *o) { (void)o; }
void sensors_runtime_mm24_restore(const mm24_t *i) { (void)i; g_mm24_restores++; }
#define SENSORS_RUNTIME_H 1   /* evita el header real (AppConfig, I2C); arriba van los stubs */

#include "../../components/sensors_runtime/flow_meter.c"

/* "Reinicio": borra el estado en RAM del modulo (la NVS g_nvs sobrevive). */
static void reboot(int64_t up_us)
{
    memset(&s_meter, 0, sizeof s_meter);
    memset(&s_saved, 0, sizeof s_saved);
    memset(&s_boot_cp, 0, sizeof s_boot_cp);
    memset(s_date, 0, sizeof s_date);
    s_partial = s_seeded = s_force_save = false;
    s_total_m3 = 0; s_since_min = -1;
    s_gate = NULL;
    g_up_us = up_us;
    assert(flow_meter_init() == ESP_OK);
}
/* 60 L/min durante n s (muestras cada 100 ms) = n L. */
static void flow_for(int seconds)
{
    for (int i = 0; i < seconds * 10; i++) {
        g_up_us += 100000; if (i % 10 == 9) g_wall += 1;
        flow_meter_record(g_up_us, 60.0f, true);
    }
}
static void near(double a, double b) { if (fabs(a - b) > 1e-6) { printf("%.9f != %.9f\n", a, b); assert(0); } }

int main(void)
{
    _putenv("TZ=UTC0"); tzset();
    g_wall = 1760486400;                 /* 2025-10-15 00:00:00 UTC */
    g_wall += 8 * 3600;                  /* 08:00 */

    /* 1) Primer arranque (NVS vacia): hoy cuenta desde el arranque (08:00). */
    reboot(0);
    assert(!flow_meter_boot_checkpoint().valid);
    flow_for(60);                        /* 60 L */
    flow_meter_snapshot_t s = flow_meter_get();
    near(s.volume_m3, 0.0599); near(s.total_m3, 0.0599);     /* 1a muestra sin previa */
    assert(s.since_min == 8 * 60);

    /* 2) Guardado: el de la siembra (forzado) y luego cada 2 min, no antes.
     *    1.6.9: el primer servicio recupera la "24H" de NVS. */
    flow_meter_service(); assert(g_stores == 1 && g_mm24_restores == 1 && g_mm24_stores == 0);
    flow_for(30); flow_meter_service(); assert(g_stores == 1);
    flow_for(100); flow_meter_service(); assert(g_stores == 2);
    assert(!strcmp(g_nvs.date, "2025-10-15") && g_nvs.saved_epoch == g_wall);
    near(g_nvs.total_m3, flow_meter_get().total_m3);
    const double day_at_cut = g_nvs.consumo_m3, total_at_cut = g_nvs.total_m3;
    const time_t cut = g_wall;

    /* 3) Corte de energia de 30 min el mismo dia: se restaura HOY y TOTAL, el apagado
     *    queda como "sin medir" y el guardado previo queda disponible para AWS. */
    g_wall = cut + 1800;
    reboot(0);
    flow_meter_checkpoint_t cp = flow_meter_boot_checkpoint();
    assert(cp.valid && cp.saved_epoch == cut && !strcmp(cp.date, "2025-10-15"));
    near(cp.day_m3, (float)day_at_cut); near(cp.total_m3, total_at_cut);
    flow_for(60);
    s = flow_meter_get();
    near(s.total_m3, total_at_cut + 0.0599);
    near(s.volume_m3, (float)day_at_cut + 0.0599);
    assert(s.since_min == 8 * 60);       /* sigue contando desde las 08:00 */
    assert(s.missing_ms >= 1800 * 1000ULL && s.missing_ms < 1900 * 1000ULL);

    /* 3b) 1.6.9: un TOTAL guardado menor que HOY (equipo que paso por 1.6.8) se corrige. */
    {
        s_force_save = true; flow_meter_service();      /* NVS al dia antes de copiarla */
        app_metrics_t keep = g_nvs;
        g_nvs.total_m3 = 0.001;
        reboot(0);
        flow_for(1);
        s = flow_meter_get();
        assert(s.total_m3 >= s.volume_m3);
        g_nvs = keep;
        reboot(0);
        flow_for(1);
    }

    /* 4) Medianoche: HOY se reinicia (desde 00:00), TOTAL sigue, y se guarda enseguida. */
    g_wall = 1760486400 + 86400 - 5;     /* 23:59:55 */
    flow_for(10);                        /* cruza a 2025-10-16 */
    s = flow_meter_get();
    assert(s.since_min == 0 && s.volume_m3 < 0.006);
    assert(s.total_m3 > total_at_cut + 0.0599 + 0.009);
    int before = g_stores; flow_meter_service(); assert(g_stores == before + 1);
    assert(!strcmp(g_nvs.date, "2025-10-16"));

    /* 5) Sin hora valida tras el arranque: guarda el TOTAL sin pisar el dia guardado. */
    const double total_now = flow_meter_get().total_m3;
    g_wall = 0;
    reboot(0);
    flow_for(10);
    s = flow_meter_get();
    assert(!s.dated && s.since_min == -1);
    near(s.total_m3, total_now + 0.0099);
    g_up_us += 130000000; flow_meter_service();
    assert(!strcmp(g_nvs.date, "2025-10-16") && g_nvs.saved_epoch == 0);
    near(g_nvs.total_m3, total_now + 0.0099);

    /* 6) 1.6.9: la "24H" se guarda cada 10 min (no en cada checkpoint de 2 min). */
    {
        int st = g_mm24_stores;
        g_up_us += 300000000; flow_meter_service(); assert(g_mm24_stores == st);
        g_up_us += 400000000; flow_meter_service(); assert(g_mm24_stores == st + 1);
    }

    puts("PASS: flow_meter (totalizador, guardado 2 min, corte de energia, medianoche, sin hora, 24H en NVS)");
    return 0;
}
