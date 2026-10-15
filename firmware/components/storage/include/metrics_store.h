#pragma once
#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

/* Métricas operativas persistentes (NVS, namespace "metrics"), separadas de
 * AppConfig para no reescribir el JSON completo en cada guardado periódico:
 *  - consumo de gas del día (m³) + fecha a la que pertenece
 *  - minutos de servicio acumulados (vida del equipo)
 * Las escribe flow_meter_service (time_sync worker) cada 2 min (1.6.8; antes 10) y al
 * cambiar de dia. Wear: ~720 escrituras/dia en una NVS de 84 KB = decenas de anios.
 * 1.6.8: + TOTALIZADOR (total_m3, nunca lo borra un restablecer: el namespace
 * "metrics" no esta en ningun nivel de "Restablecer"; solo erase-flash) y la hora
 * del ultimo guardado (saved_epoch) para reportar cortes de energia a AWS. */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char     date[12];     /* "YYYY-MM-DD" del acumulado de consumo */
    float    consumo_m3;   /* consumo acumulado de ese día */
    uint32_t service_min;  /* minutos de servicio totales del equipo */
    uint32_t partial;     /* incomplete coverage (gaps/restart/clock adjustment) */
    uint64_t missing_ms;  /* known unmeasured intervals, not estimated volume */
    /* --- 1.6.8 (un blob anterior carga con estos campos en cero) --- */
    double   total_m3;    /* totalizador de vida del equipo */
    int64_t  saved_epoch; /* hora de pared del guardado (0 = sin hora valida) */
    uint32_t since_min;   /* minuto del dia (0-1439) desde el que cuenta "hoy" */
    uint32_t reserved;
} app_metrics_t;

esp_err_t appmetrics_load(app_metrics_t *out);       /* ceros si no existe aún */
esp_err_t appmetrics_store(const app_metrics_t *in); /* persiste + actualiza caché */

/* 1.6.9: blob auxiliar en el mismo namespace "metrics" (min/max 24 h, key "mm24").
 * load devuelve ESP_ERR_NOT_FOUND / ESP_ERR_INVALID_SIZE si no existe o es de otro tamaño. */
esp_err_t appmetrics_load_blob(const char *key, void *buf, size_t len);
esp_err_t appmetrics_store_blob(const char *key, const void *buf, size_t len);

/* Lecturas rápidas desde la caché en RAM (para la UI). */
uint32_t appmetrics_service_min(void);

#ifdef __cplusplus
}
#endif
