#include "metrics_store.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>
#include <stddef.h>
#include <math.h>

#define MET_NS  "metrics"
#define MET_KEY "m"

static app_metrics_t s_cache;      /* última copia conocida (para la UI) */
static bool s_cache_ok = false;

esp_err_t appmetrics_load(app_metrics_t *out)
{
    if (!out) return ESP_ERR_INVALID_ARG;
    memset(out, 0, sizeof(*out));

    nvs_handle_t h;
    esp_err_t r = nvs_open(MET_NS, NVS_READONLY, &h);
    if (r == ESP_OK) {
        size_t len = sizeof(*out);
        r = nvs_get_blob(h, MET_KEY, out, &len);
        nvs_close(h);
        /* Versiones aceptadas: actual, 1.5.x/1.6.7 (hasta missing_ms) y la primera
         * (hasta service_min). La parte nueva queda en cero (memset de arriba). */
        if (r != ESP_OK || (len != sizeof(*out) && len != offsetof(app_metrics_t, total_m3) &&
                            len != offsetof(app_metrics_t, partial)))
            memset(out, 0, sizeof(*out));   /* primera vez / blob de otra versión */
        else if (len < offsetof(app_metrics_t, total_m3) + sizeof(out->total_m3))
            /* 1.6.9: al actualizar desde <= 1.6.7 el totalizador arranca con el consumo
             * del dia ya guardado (en 1.6.8 arrancaba en 0 y quedaba TOTAL < HOY). */
            out->total_m3 = (isfinite(out->consumo_m3) && out->consumo_m3 > 0) ? out->consumo_m3 : 0;
        else if (!isfinite(out->total_m3) || out->total_m3 < 0)
            out->total_m3 = 0;
    }
    out->date[sizeof(out->date) - 1] = '\0';
    s_cache = *out;
    s_cache_ok = true;
    return ESP_OK;
}

esp_err_t appmetrics_store(const app_metrics_t *in)
{
    if (!in) return ESP_ERR_INVALID_ARG;

    nvs_handle_t h;
    esp_err_t r = nvs_open(MET_NS, NVS_READWRITE, &h);
    if (r != ESP_OK) return r;
    r = nvs_set_blob(h, MET_KEY, in, sizeof(*in));
    if (r == ESP_OK) r = nvs_commit(h);
    nvs_close(h);

    if (r == ESP_OK) { s_cache = *in; s_cache_ok = true; }
    return r;
}

esp_err_t appmetrics_load_blob(const char *key, void *buf, size_t len)
{
    if (!key || !buf) return ESP_ERR_INVALID_ARG;
    nvs_handle_t h;
    esp_err_t r = nvs_open(MET_NS, NVS_READONLY, &h);
    if (r != ESP_OK) return r;
    size_t got = len;
    r = nvs_get_blob(h, key, buf, &got);
    nvs_close(h);
    if (r == ESP_OK && got != len) r = ESP_ERR_INVALID_SIZE;
    return r;
}

esp_err_t appmetrics_store_blob(const char *key, const void *buf, size_t len)
{
    if (!key || !buf) return ESP_ERR_INVALID_ARG;
    nvs_handle_t h;
    esp_err_t r = nvs_open(MET_NS, NVS_READWRITE, &h);
    if (r != ESP_OK) return r;
    r = nvs_set_blob(h, key, buf, len);
    if (r == ESP_OK) r = nvs_commit(h);
    nvs_close(h);
    return r;
}

uint32_t appmetrics_service_min(void)
{
    return s_cache_ok ? s_cache.service_min : 0;
}
