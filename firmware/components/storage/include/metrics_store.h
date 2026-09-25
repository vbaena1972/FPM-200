#pragma once
#include "esp_err.h"
#include <stdint.h>

/* Métricas operativas persistentes (NVS, namespace "metrics"), separadas de
 * AppConfig para no reescribir el JSON completo en cada guardado periódico:
 *  - consumo de gas del día (m³) + fecha a la que pertenece
 *  - minutos de servicio acumulados (vida del equipo)
 * Las escribe flow_meter_service (time_sync worker) cada ~10 min; wear de NVS aceptable. */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char     date[12];     /* "YYYY-MM-DD" del acumulado de consumo */
    float    consumo_m3;   /* consumo acumulado de ese día */
    uint32_t service_min;  /* minutos de servicio totales del equipo */
    uint32_t partial;     /* incomplete coverage (gaps/restart/clock adjustment) */
    uint64_t missing_ms;  /* known unmeasured intervals, not estimated volume */
} app_metrics_t;

esp_err_t appmetrics_load(app_metrics_t *out);       /* ceros si no existe aún */
esp_err_t appmetrics_store(const app_metrics_t *in); /* persiste + actualiza caché */

/* Lecturas rápidas desde la caché en RAM (para la UI). */
uint32_t appmetrics_service_min(void);

#ifdef __cplusplus
}
#endif
