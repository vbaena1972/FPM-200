#include "flow_meter.h"
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_app_desc.h" // esp_app_get_description()
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "cJSON.h"
#include "http_api.h"
#include "state_pub.h"
#include "storage.h"           // AppConfig + appcfg_cache_peek (unidades/límites)
#include "sensors_runtime.h"   // lecturas VIVAS (state_pub no se actualiza)
#include "mem_diag.h"
#include "fpm_ble_config.h" // reutiliza el aplicador set_config (mismo contrato que BLE)

static esp_err_t state_get_handler(httpd_req_t *req);
static esp_err_t alarms_get_handler(httpd_req_t *req);
static esp_err_t logs_get_handler(httpd_req_t *req);

static const char *TAG = "http_api";
static httpd_handle_t s_httpd = NULL;
static SemaphoreHandle_t s_lock;

// ----------- Config por defecto (puedes mover a Kconfig si quieres) -----------
#ifndef HTTPD_413_PAYLOAD_TOO_LARGE
#define HTTPD_413_PAYLOAD_TOO_LARGE HTTPD_413_CONTENT_TOO_LARGE
#endif

#ifndef HTTP_API_PORT
#define HTTP_API_PORT (80)
#endif

#ifndef HTTP_API_CTRL_PORT
#define HTTP_API_CTRL_PORT (32768)
#endif

#ifndef HTTP_API_STACK_SIZE
#define HTTP_API_STACK_SIZE (8192) // sube a 12288 si serializas JSON grandes
#endif

#ifndef HTTP_API_MAX_SOCKETS
#define HTTP_API_MAX_SOCKETS (4)
#endif

#ifndef HTTP_API_RECV_BUF
#define HTTP_API_RECV_BUF (2048) // leer cuerpo POST en trozos
#endif
// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
// Utilidades
// ------------------------------------------------------------------------------------
// static void httpd_log_cfg(const httpd_config_t *cfg, esp_err_t err, const char *prefix)
// {
//     if (err == ESP_OK)
//     {
//         ESP_LOGI(TAG, "%s on :%d (ctrl:%d, stack=%u, sockets=%d)",
//                  prefix, (int)cfg->server_port, (int)cfg->ctrl_port,
//                  (unsigned)cfg->stack_size, (int)cfg->max_open_sockets);
//     }
//     else
//     {
//         ESP_LOGE(TAG, "%s failed: %s (port:%d ctrl:%d stack:%u sockets:%d)",
//                  prefix, esp_err_to_name(err), (int)cfg->server_port,
//                  (int)cfg->ctrl_port, (unsigned)cfg->stack_size,
//                  (int)cfg->max_open_sockets);
//     }
// }

static esp_err_t send_json_chunk_begin(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    return httpd_resp_sendstr_chunk(req, "{");
}

static esp_err_t send_json_chunk_kv(httpd_req_t *req, const char *k, const char *v, bool last, bool quote_val)
{
    // envia ,"k":"v"  o  ,"k":v
    esp_err_t err;
    err = httpd_resp_sendstr_chunk(req, "\"");
    if (err != ESP_OK)
        return err;
    err = httpd_resp_sendstr_chunk(req, k);
    if (err != ESP_OK)
        return err;
    err = httpd_resp_sendstr_chunk(req, "\":");
    if (err != ESP_OK)
        return err;

    if (quote_val)
    {
        err = httpd_resp_sendstr_chunk(req, "\"");
        if (err != ESP_OK)
            return err;
        err = httpd_resp_sendstr_chunk(req, v ? v : "");
        if (err != ESP_OK)
            return err;
        err = httpd_resp_sendstr_chunk(req, "\"");
        if (err != ESP_OK)
            return err;
    }
    else
    {
        err = httpd_resp_sendstr_chunk(req, v ? v : "null");
        if (err != ESP_OK)
            return err;
    }

    if (!last)
    {
        err = httpd_resp_sendstr_chunk(req, ",");
    }
    return err;
}

static esp_err_t send_json_chunk_end(httpd_req_t *req)
{
    esp_err_t err = httpd_resp_sendstr_chunk(req, "}");
    if (err != ESP_OK)
        return err;
    return httpd_resp_sendstr_chunk(req, NULL); // fin de chunked
}

// Lee el cuerpo de una petición (POST/PUT) en trozos al buffer dado,
// devolviendo el total leído. No añade '\0' automáticamente si body binario.
// static int recv_body_into_buf(httpd_req_t *req, char *buf, size_t buf_len)
// {
//     int remaining = req->content_len;
//     int total = 0;

//     while (remaining > 0)
//     {
//         size_t to_read = remaining > (int)buf_len ? buf_len : remaining;
//         int r = httpd_req_recv(req, buf, to_read);
//         if (r <= 0)
//         {
//             if (r == HTTPD_SOCK_ERR_TIMEOUT)
//             {
//                 continue;
//             }
//             return r; // error
//         }
//         remaining -= r;
//         total += r;
//     }
//     return total;
// }

// ------------------------------------------------------------------------------------
// /api/v1/info  (GET)
// ------------------------------------------------------------------------------------
static esp_err_t info_get_handler(httpd_req_t *req)
{
    const esp_app_desc_t *ad = esp_app_get_description();

    esp_err_t err = send_json_chunk_begin(req);
    if (err != ESP_OK)
        return err;

    // {"ok":true,
    err = httpd_resp_sendstr_chunk(req, "\"ok\":true,");
    if (err != ESP_OK)
        return err;

    // "project": "...", "fw": "...", "idf": "..."
    err = send_json_chunk_kv(req, "project", ad ? ad->project_name : "unknown", false, true);
    if (err)
        return err;
    err = send_json_chunk_kv(req, "fw", ad ? ad->version : "unknown", false, true);
    if (err)
        return err;
    err = send_json_chunk_kv(req, "idf", ad ? ad->idf_ver : "unknown", true, true);
    if (err)
        return err;

    return send_json_chunk_end(req);
}

// ------------------------------------------------------------------------------------
// /api/v1/config (GET)  -> devuelve la config actual como JSON
// Nota: si ya tienes una función que vuelca el JSON (ej. appcfg_dump_json),
// úsala. Aquí dejo un stub seguro.
// ------------------------------------------------------------------------------------
// Valida el token LAN (Authorization: Bearer <token>) igual que MedGuard. Si el
// equipo aún no tiene token, no bloquea (defensivo durante bring-up).
static bool http_bearer_ok(httpd_req_t *req)
{
    char tok[33];
    fpm_lan_token(tok, sizeof tok);
    if (tok[0] == '\0')
        return true;
    char hdr[80] = {0};
    if (httpd_req_get_hdr_value_str(req, "Authorization", hdr, sizeof hdr) != ESP_OK)
        return false;
    const char *p = hdr;
    if (strncmp(p, "Bearer ", 7) == 0)
        p += 7;
    return strcmp(p, tok) == 0;
}

static esp_err_t cfg_get_handler(httpd_req_t *req)
{
    if (!http_bearer_ok(req))
    {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "unauthorized");
        return ESP_OK;
    }
    // Config completa en el esquema de la app (redactada). Reutiliza el mismo
    // builder que BLE, así LAN y BLE devuelven idéntica estructura.
    char *json = fpm_ble_config_read_json(true);
    if (!json)
    {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "oom");
        return ESP_OK;
    }
    httpd_resp_set_type(req, "application/json");
    esp_err_t err = httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);
    free(json);
    return err;
}

// ------------------------------------------------------------------------------------
// /api/v1/config (PUT)  -> aplica un patch JSON
// Espera algo como: {"base_rev":1,"patch":{...}}
// Llama a tu appcfg_patch() si la tienes. Aquí parsea y valida tamaño.
// ------------------------------------------------------------------------------------
static esp_err_t cfg_put_handler(httpd_req_t *req)
{
    if (!http_bearer_ok(req))
    {
        httpd_resp_send_err(req, HTTPD_401_UNAUTHORIZED, "unauthorized");
        return ESP_OK;
    }
    if (req->content_len <= 0 || req->content_len > (128 * 1024))
    {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "invalid content length");
        return ESP_OK;
    }

    // Reservamos un buffer moderado y parseamos en streaming a cJSON
    char *buf = (char *)malloc(HTTP_API_RECV_BUF + 1);
    if (!buf)
    {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "oom");
        return ESP_OK;
    }

    int remaining = req->content_len;

    // Acumulamos en un cJSON Parser con string temporal (para no copiarlo entero en RAM)
    // Estrategia simple: si el contenido cabe en <= 32 KB lo leemos todo; si no, rechaza.
    const int max_body = (32 * 1024);
    char *full = NULL;
    if (req->content_len > max_body)
    {
        free(buf);
        //        httpd_resp_send_err(req, HTTPD_413_PAYLOAD_TOO_LARGE, "body too large");
        httpd_resp_send_err(req, HTTPD_413_CONTENT_TOO_LARGE, "body too large");

        return ESP_OK;
    }

    full = (char *)malloc(req->content_len + 1);
    if (!full)
    {
        free(buf);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "oom");
        return ESP_OK;
    }

    int copied = 0;
    int timeouts = 0;
    while (remaining > 0)
    {
        int to_read = remaining > HTTP_API_RECV_BUF ? HTTP_API_RECV_BUF : remaining;
        int r = httpd_req_recv(req, buf, to_read);
        if (r <= 0)
        {
            // Bounded: a silent client must not pin the single httpd task forever.
            if (r == HTTPD_SOCK_ERR_TIMEOUT && ++timeouts < 5)
                continue;
            free(buf);
            free(full);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "recv failed");
            return ESP_OK;
        }
        memcpy(full + copied, buf, r);
        copied += r;
        remaining -= r;
    }
    full[copied] = '\0';
    free(buf);

    // Validación mínima: debe ser JSON de objeto.
    cJSON *root = cJSON_ParseWithLength(full, copied);
    if (!root || !cJSON_IsObject(root))
    {
        if (root) cJSON_Delete(root);
        free(full);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "invalid json");
        return ESP_OK;
    }
    cJSON_Delete(root);

    // Aplica el MISMO contrato `set_config` que BLE (secciones display/time/
    // alarms/network/cloud/bluetooth/ota/channels). Reutiliza el aplicador
    // probado: escritura de config por LAN idéntica a MedGuard. El `ver` que
    // envía la app se ignora aquí (campos desconocidos no estorban).
    bool applied = fpm_ble_config_apply_json(full);
    free(full);

    esp_err_t err = send_json_chunk_begin(req);
    if (err == ESP_OK)
    {
        httpd_resp_sendstr_chunk(
            req,
            applied ? "\"ok\":true"
                    : "\"ok\":false,\"message\":\"el equipo rechazo el cambio\"");
        send_json_chunk_end(req);
    }
    return ESP_OK;
}

// ------------------------------------------------------------------------------------
// Registro de URIs
// ------------------------------------------------------------------------------------
static void register_uris(httpd_handle_t server)
{
    const httpd_uri_t uri_info_get = {
        .uri = "/api/v1/info",
        .method = HTTP_GET,
        .handler = info_get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_info_get);

    httpd_uri_t uri_state_get = {
        .uri = "/api/v1/state",
        .method = HTTP_GET,
        .handler = state_get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_state_get);

    const httpd_uri_t uri_cfg_get = {
        .uri = "/api/v1/config",
        .method = HTTP_GET,
        .handler = cfg_get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_cfg_get);

    const httpd_uri_t uri_cfg_put = {
        .uri = "/api/v1/config",
        .method = HTTP_PUT,
        .handler = cfg_put_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_cfg_put);

    // La app Sensvax escribe config por POST (mismo cuerpo set_config). Se
    // registra POST con el mismo handler que PUT para paridad con MedGuard.
    const httpd_uri_t uri_cfg_post = {
        .uri = "/api/v1/config",
        .method = HTTP_POST,
        .handler = cfg_put_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_cfg_post);

    const httpd_uri_t uri_alarms_get = {
        .uri = "/api/v1/alarms",
        .method = HTTP_GET,
        .handler = alarms_get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_alarms_get);

    const httpd_uri_t uri_logs_get = {
        .uri = "/api/v1/logs",
        .method = HTTP_GET,
        .handler = logs_get_handler,
        .user_ctx = NULL};
    httpd_register_uri_handler(server, &uri_logs_get);
}

// Estado en vivo en el ESQUEMA DE CANALES que consume la app (channels[]), no el
// {flow,pressure} plano de state_build_json (que sigue para otros consumidores).
// FPM = 2 canales: Presión (id 1) y Flujo (id 2). Valores en unidades INTERNAS
// (kPa / L/min) con la etiqueta correspondiente, así valor y unidad son
// coherentes; el estado se calcula contra los límites configurados (mismas
// unidades internas). TODO: convertir a la unidad de display del usuario.
static void add_channel(cJSON *arr, int id, const char *name, const char *unit,
                        double value, const char *state, bool hi_en, double hi,
                        bool lo_en, double lo, int decimals)
{
    cJSON *c = cJSON_CreateObject();
    cJSON_AddNumberToObject(c, "id", id);
    cJSON_AddStringToObject(c, "name", name);
    cJSON_AddStringToObject(c, "location", "");
    cJSON_AddNumberToObject(c, "value", value);
    cJSON_AddStringToObject(c, "unit", unit);
    cJSON_AddBoolToObject(c, "enabled", true);
    cJSON_AddStringToObject(c, "state", state);
    cJSON_AddNumberToObject(c, "decimals", decimals);
    if (hi_en) cJSON_AddNumberToObject(c, "hi_limit", hi);
    if (lo_en) cJSON_AddNumberToObject(c, "lo_limit", lo);
    cJSON_AddItemToArray(arr, c);
}

static esp_err_t state_get_handler(httpd_req_t *req)
{
    // Lecturas VIVAS desde sensors_runtime (state_pub/g_state nunca se actualiza:
    // state_update_sensors no se llama en ningún lado -> daba 0s en la app).
    sensor_sample_t last = {0};
    bool have = sensors_runtime_get_last(&last);
    const AppConfig *c = appcfg_cache_peek();

    double p = have ? last.pressure_kpa : 0.0; // kPa (interno)
    double f = have ? last.flow_lpm : 0.0;     // L/min (interno)

    bool p_hi_en = false, p_lo_en = false, f_hi_en = false;
    double p_hi = 0, p_lo = 0, f_hi = 0;
    const char *p_unit = "kPa", *f_unit = "L/min";
    if (c) {
        p_hi_en = c->sensors.alarm_limits.pressure_max_enabled;
        p_lo_en = c->sensors.alarm_limits.pressure_min_enabled;
        p_hi = c->sensors.alarm_limits.pressure_max;
        p_lo = c->sensors.alarm_limits.pressure_min;
        f_hi_en = c->sensors.alarm_limits.flow_high_enabled;
        f_hi = c->sensors.alarm_limits.flow_high_limit;
    }
    const char *p_state = (!have || (last.invalid_mask & SENSOR_INVALID_PRESSURE)) ? "fault" : (p_hi_en && p > p_hi) ? "high"
                        : (p_lo_en && p < p_lo) ? "low" : "normal";
    const char *f_state = (!have || (last.invalid_mask & SENSOR_INVALID_FLOW)) ? "fault" : (f_hi_en && f > f_hi) ? "high" : "normal";

    cJSON *root = cJSON_CreateObject();
    flow_meter_snapshot_t meter = flow_meter_get();
    cJSON_AddNumberToObject(root, "consumption_m3", meter.volume_m3);
    cJSON_AddNumberToObject(root, "consumption_missing_ms", (double)meter.missing_ms);
    cJSON_AddBoolToObject(root, "consumption_partial", meter.partial || !meter.dated);
    cJSON *chs = cJSON_AddArrayToObject(root, "channels");
    add_channel(chs, 1, "Presion", p_unit, p, p_state, p_hi_en, p_hi, p_lo_en, p_lo, 1);
    add_channel(chs, 2, "Flujo", f_unit, f, f_state, f_hi_en, f_hi, false, 0, 2);

    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (!json) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "oom");
        return ESP_OK;
    }
    httpd_resp_set_type(req, "application/json");
    esp_err_t err = httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);
    free(json);
    return err;
}

// La app consume /alarms y /logs al abrir el equipo (LanMonitoringSource). Sin
// estas rutas devolvían 404 y la carga fallaba entera ("no conecta"). FPM aún no
// expone alarmas/auditoría por LAN → se devuelve la estructura vacía válida que
// la app espera ({active:[]} / {events:[]}); el estado de alarma se ve por /state.
static esp_err_t alarms_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, "{\"active\":[]}");
}

static esp_err_t logs_get_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/json");
    return httpd_resp_sendstr(req, "{\"events\":[]}");
}

// ------------------------------------------------------------------------------------
// Hook opcional a eventos de red: arranca al tener IP (solo una vez)
// ------------------------------------------------------------------------------------
// static void on_got_ip(void *arg, esp_event_base_t base, int32_t id, void *data)
// {
//     (void)arg;
//     (void)base;
//     (void)id;
//     (void)data;
//     // Arranque idempotente
//     esp_err_t err = http_api_start();
//     if (err != ESP_OK)
//     {
//         // Si falla por recursos, no lo martilles: deja log y ya
//         ESP_LOGE(TAG, "start after GOT_IP failed: %s", esp_err_to_name(err));
//     }
// }

esp_err_t http_api_init(void)
{
    if (!s_lock)
        s_lock = xSemaphoreCreateMutex();
    return s_lock ? ESP_OK : ESP_ERR_NO_MEM;
}

static esp_err_t http_api__start_locked(void)
{
    if (s_httpd)
    {
        ESP_LOGI(TAG, "httpd already running");
        return ESP_OK;
    }
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.lru_purge_enable = true; // purga handlers viejos si hace falta
    cfg.stack_size = 6144;       // ajusta si tu build lo requiere

    esp_err_t err = httpd_start(&s_httpd, &cfg);
    if (err != ESP_OK)
    {
        s_httpd = NULL;
        ESP_LOGE(TAG, "httpd_start failed: %s", esp_err_to_name(err));
        return err;
    }
    register_uris(s_httpd);
    ESP_LOGI(TAG, "httpd started");
    return ESP_OK;
}

// ------------------------------------------------------------------------------------
// Arranque/parada idempotente del servidor
// ------------------------------------------------------------------------------------
esp_err_t http_api_start(void)
{
    if (!s_lock)
        return ESP_ERR_INVALID_STATE;
    xSemaphoreTake(s_lock, portMAX_DELAY);

    mem_diag_report("HTTP:before_start");
    esp_err_t r = http_api__start_locked();
    mem_diag_report("HTTP:after_start");

    xSemaphoreGive(s_lock);
    return r;
}

void http_api_stop(void)
{
    if (!s_lock)
        return;
    xSemaphoreTake(s_lock, portMAX_DELAY);

    mem_diag_report("HTTP:before_stop");
    if (s_httpd)
    {
        httpd_stop(s_httpd);
        s_httpd = NULL;
        ESP_LOGI(TAG, "httpd stopped");
    }
    mem_diag_report("HTTP:after_stop");

    xSemaphoreGive(s_lock);
}
