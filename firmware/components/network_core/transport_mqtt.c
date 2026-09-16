#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "cJSON.h"
#include <string.h>
#include <time.h>
#include <math.h>

#include "storage.h"
#include "cert_store.h"
#include "sensors_runtime.h"
#include "state_pub.h"

static const char *TAG = "aws_mqtt";

static esp_mqtt_client_handle_t s_client = NULL;
static bool s_is_connected = false;
static TaskHandle_t s_pub_task = NULL;   /* tarea aws_pub: se crea una sola vez */

// === AGREGA ESTAS TRES LÍNEAS AQUÍ ===
static char *s_safe_ca = NULL;
static char *s_safe_cert = NULL;
static char *s_safe_key = NULL;

// === NUEVO: Para guardar el Serial y armar el Topic del Shadow ===
static char s_thing_name[32] = {0};

// ------------------------------------------------------------------
// HELPER CRÍTICO: Inyecta salto de línea y terminador nulo de forma segura
// ------------------------------------------------------------------
static char *prepare_pem(const uint8_t *raw_data, size_t actual_len)
{
    if (!raw_data || actual_len == 0)
        return NULL;

    // Reservamos espacio para los datos exactos + el '\n' + el '\0'
    char *safe_str = calloc(1, actual_len + 2);

    // Copiamos ESTRICTAMENTE los bytes válidos del certificado
    memcpy(safe_str, raw_data, actual_len);

    // Verificamos si el último carácter válido ya es un salto de línea
    if (safe_str[actual_len - 1] != '\n')
    {
        safe_str[actual_len] = '\n';
        safe_str[actual_len + 1] = '\0';
    }
    else
    {
        safe_str[actual_len] = '\0'; // Si ya lo tenía, solo cerramos el string
    }

    return safe_str;
}

// ------------------------------------------------------------------
// Eventos MQTT
// ------------------------------------------------------------------
// ------------------------------------------------------------------
// Eventos MQTT (Con Soporte para AWS Shadow Delta)
// ------------------------------------------------------------------
static void mqtt_event(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Conectado exitosamente a AWS IoT Core");
        s_is_connected = true;

        // SUSCRIPCIÓN AL SHADOW DELTA
        if (strlen(s_thing_name) > 0)
        {
            char delta_topic[128];
            snprintf(delta_topic, sizeof(delta_topic), "$aws/things/%s/shadow/update/delta", s_thing_name);
            int msg_id = esp_mqtt_client_subscribe(event->client, delta_topic, 1);
            ESP_LOGI(TAG, "Suscrito al Shadow Delta (ID: %d): %s", msg_id, delta_topic);
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Desconectado de AWS");
        s_is_connected = false;
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "Suscripción confirmada por AWS. ¡Estamos a la escucha!");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Datos recibidos en el topic: %.*s", event->topic_len, event->topic);

        if (strstr(event->topic, "/shadow/update/delta") != NULL)
        {
            ESP_LOGW(TAG, "¡Cambio de configuración recibido desde la Nube!");

            char *payload = malloc(event->data_len + 1);
            if (payload)
            {
                memcpy(payload, event->data, event->data_len);
                payload[event->data_len] = '\0';

                cJSON *root = cJSON_Parse(payload);
                if (root)
                {
                    cJSON *state = cJSON_GetObjectItem(root, "state");
                    if (state)
                    {
                        AppConfig *tmp_cfg = malloc(sizeof(AppConfig));
                        if (tmp_cfg)
                        {
                            appcfg_load(tmp_cfg);
                            cfg_from_json(tmp_cfg, state);
                            appcfg_save(tmp_cfg);

                            ESP_LOGW(TAG, "=================================================");
                            ESP_LOGW(TAG, "NVS actualizada vía Shadow. Reiniciando equipo...");
                            ESP_LOGW(TAG, "=================================================");

                            // Liberamos la memoria antes de reiniciar
                            free(tmp_cfg);
                            cJSON_Delete(root);
                            free(payload);

                            // Esperamos 1.5 segundos para que los hilos de NVS vacíen los buffers a la Flash
                            vTaskDelay(pdMS_TO_TICKS(1500));

                            // REINICIO DE HARDWARE CONTROLADO POR SOFTWARE
                            esp_restart();
                        }
                    }
                    // Si el flujo continuara normalmente (no delta), se borraría aquí
                    if (root)
                        cJSON_Delete(root);
                }
                if (payload)
                    free(payload);
            }
        }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "Error MQTT (Revisa certificados o política de AWS)");
        break;
    default:
        break;
    }
}

// ------------------------------------------------------------------
// Tarea de Telemetría
// ------------------------------------------------------------------
// Cadencia 30 s (DEC-016) alineada a MedGuard. Modela las señales del FPM-200
// como canales genéricos (ch1=presión, ch2=flujo, ch3=temperatura) para que el
// MISMO ingest/Glue/API/portal de MedGuard los procese SIN cambios. Las stats
// {n,min,max,mean,std} resumen la ventana muestreada a 10 Hz (base para ML);
// 'value' es el último valor filtrado (estado vivo). Unidades de cable = SI
// internas (kPa, L/min, C); la conversión a la unidad de display es del cliente.
#define AWS_TELEMETRY_PERIOD_MS 30000
#define AWS_TELEMETRY_WINDOW_MS 30000

static void add_channel(cJSON *arr, int id, const char *name, const char *unit,
                        float value, const char *state,
                        bool has_lo, float lo, bool has_hi, float hi,
                        const sensor_signal_stats_t *st)
{
    cJSON *ch = cJSON_CreateObject();
    cJSON_AddNumberToObject(ch, "id", id);
    cJSON_AddBoolToObject(ch, "enabled", true);
    cJSON_AddStringToObject(ch, "name", name);
    cJSON_AddStringToObject(ch, "unit", unit);
    if (isfinite(value))
        cJSON_AddNumberToObject(ch, "value", value);
    else
        cJSON_AddNullToObject(ch, "value");
    cJSON_AddStringToObject(ch, "state", state);
    if (has_lo) cJSON_AddNumberToObject(ch, "low_limit", lo);
    if (has_hi) cJSON_AddNumberToObject(ch, "high_limit", hi);
    if (st && st->n > 0) {
        cJSON *s = cJSON_CreateObject();
        cJSON_AddNumberToObject(s, "n",    (double)st->n);
        cJSON_AddNumberToObject(s, "min",  st->min);
        cJSON_AddNumberToObject(s, "max",  st->max);
        cJSON_AddNumberToObject(s, "mean", st->mean);
        cJSON_AddNumberToObject(s, "std",  st->std);
        cJSON_AddItemToObject(ch, "stats", s);
    }
    cJSON_AddItemToArray(arr, ch);
}

static void aws_telemetry_task(void *pvParameters)
{
    char topic[256] = {0};
    char serial[32] = {0};
    char firmware[24] = {0};

    // Límites/flags de alarma capturados de la NVS (para el 'state' por canal).
    float p_lim_min = -10.0f, p_lim_max = 120.0f, f_high_lim = 90.0f;
    bool  p_min_en = true, p_max_en = true, f_high_en = false;

    AppConfig *cfg_app = malloc(sizeof(AppConfig));
    if (cfg_app)
    {
        appcfg_defaults(cfg_app);
        appcfg_load(cfg_app);

        // Topic: base/partner/client/model/serial/telemetry (ruteo AWS, 6 niveles).
        snprintf(topic, sizeof(topic), "%s/%s/%s/%s/%s/telemetry",
                 cfg_app->cloud.topic_base,
                 cfg_app->general.partner,
                 cfg_app->general.client,
                 cfg_app->general.model,
                 cfg_app->general.serial);
        ESP_LOGI(TAG, "Topic MQTT configurado: %s", topic);

        strncpy(serial, cfg_app->general.serial, sizeof(serial) - 1);
        strncpy(firmware, cfg_app->general.fw_version, sizeof(firmware) - 1);

        p_lim_min  = cfg_app->sensors.alarm_limits.pressure_min;
        p_lim_max  = cfg_app->sensors.alarm_limits.pressure_max;
        p_min_en   = cfg_app->sensors.alarm_limits.pressure_min_enabled;
        p_max_en   = cfg_app->sensors.alarm_limits.pressure_max_enabled;
        f_high_lim = cfg_app->sensors.alarm_limits.flow_high_limit;
        f_high_en  = cfg_app->sensors.alarm_limits.flow_high_enabled;

        free(cfg_app);
    }
    else
    {
        strcpy(topic, "vexel/unknown/telemetry");
    }

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(AWS_TELEMETRY_PERIOD_MS));

        if (!s_is_connected)
            continue;

        sensor_sample_t last;
        if (!sensors_runtime_get_last(&last))
        {
            ESP_LOGW(TAG, "Esperando datos válidos del runtime de sensores...");
            continue;
        }

        sensor_window_stats_t ws;
        bool have_stats = sensors_runtime_get_window_stats(AWS_TELEMETRY_WINDOW_MS, &ws);

        // --- Estado por canal (evaluación en unidades SI internas) ---
        // Vocabulario en minúsculas idéntico a MedGuard (state_name en aws_service.c):
        // normal/high/low/fault/disabled -> el mismo portal/app trata a ambos equipos igual.
        const char *p_state = "normal";
        if (!isfinite(last.pressure_kpa))                    p_state = "fault";
        else if (p_min_en && last.pressure_kpa < p_lim_min)  p_state = "low";
        else if (p_max_en && last.pressure_kpa > p_lim_max)  p_state = "high";

        const char *f_state = "normal";
        if (!isfinite(last.flow_lpm))                        f_state = "fault";
        else if (f_high_en && last.flow_lpm > f_high_lim)    f_state = "high";

        const char *t_state = isfinite(last.temp_c) ? "normal" : "fault";

        // device_status: "normal" salvo que algún canal no esté normal -> "alarm"
        // (misma regla y vocabulario que MedGuard, aws_service.c).
        const char *dev_status =
            (strcmp(p_state, "normal") || strcmp(f_state, "normal") || strcmp(t_state, "normal"))
                ? "alarm" : "normal";

        // --- Envelope v2 (mismo contrato que MedGuard: channels[] + stats) ---
        cJSON *root = cJSON_CreateObject();
        cJSON_AddNumberToObject(root, "timestamp", (double)time(NULL)); // epoch s (SNTP listo)
        cJSON_AddStringToObject(root, "device_id", serial);
        cJSON_AddStringToObject(root, "thing_name", serial);
        cJSON_AddStringToObject(root, "firmware", firmware);
        cJSON_AddStringToObject(root, "device_status", dev_status);
        cJSON_AddNumberToObject(root, "window_s", AWS_TELEMETRY_WINDOW_MS / 1000);
        cJSON_AddNumberToObject(root, "configured_channel_count", 3);
        cJSON_AddNumberToObject(root, "enabled_channel_count", 3);
        cJSON_AddNumberToObject(root, "disabled_channel_count", 0);

        cJSON *channels = cJSON_AddArrayToObject(root, "channels");
        add_channel(channels, 1, "Presion", "kPa", last.pressure_kpa, p_state,
                    p_min_en, p_lim_min, p_max_en, p_lim_max,
                    have_stats ? &ws.pressure : NULL);
        add_channel(channels, 2, "Flujo", "L/min", last.flow_lpm, f_state,
                    false, 0.f, f_high_en, f_high_lim,
                    have_stats ? &ws.flow : NULL);
        add_channel(channels, 3, "Temperatura", "C", last.temp_c, t_state,
                    false, 0.f, false, 0.f,
                    have_stats ? &ws.temp : NULL);

        char *pub_payload = cJSON_PrintUnformatted(root);
        cJSON_Delete(root);

        if (pub_payload)
        {
            int msg_id = esp_mqtt_client_publish(s_client, topic, pub_payload, 0, 1, 0);
            if (msg_id >= 0)
                ESP_LOGI(TAG, "Telemetría v2 enviada a AWS. ID: %d", msg_id);
            else
                ESP_LOGE(TAG, "Fallo al publicar telemetría en el broker.");
            free(pub_payload);
        }
    }
}

// ------------------------------------------------------------------
// Inicialización Segura
// ------------------------------------------------------------------
void transport_mqtt_on_time_ready(void)
{
    if (s_client)
    {
        ESP_LOGI(TAG, "MQTT ya estaba inicializado.");
        return;
    }

    // 1. Extraemos los certificados crudos desde el almacén "certs" de la NVS
    uint8_t *root_ca = NULL, *client_cert = NULL, *client_key = NULL;
    size_t r_len = 0, c_len = 0, k_len = 0;

    if (cert_store_load(CERT_SERVER_CA, &root_ca, &r_len) != ESP_OK ||
        cert_store_load(CERT_CLIENT_CERT, &client_cert, &c_len) != ESP_OK ||
        cert_store_load(CERT_CLIENT_KEY, &client_key, &k_len) != ESP_OK)
    {
        ESP_LOGE(TAG, "Error crítico: No se pudieron cargar los certificados desde cert_store");
        if (root_ca)
            free(root_ca);
        if (client_cert)
            free(client_cert);
        if (client_key)
            free(client_key);
        return;
    }

    // 2. Cargamos tu configuración general (identidad, urls, etc) desde el Heap
    AppConfig *cfg_app = malloc(sizeof(AppConfig));
    if (!cfg_app)
    {
        ESP_LOGE(TAG, "No hay memoria suficiente para AppConfig");
        free(root_ca);
        free(client_cert);
        free(client_key);
        return;
    }
    appcfg_defaults(cfg_app);
    appcfg_load(cfg_app);
    // === NUEVO: Guardamos el serial en RAM para usarlo en los eventos MQTT ===
    strncpy(s_thing_name, cfg_app->general.serial, sizeof(s_thing_name) - 1);

    // 3. Preparamos las cadenas PEM seguras (Inyectando el \n si falta)
    //    Las guardamos en las variables estáticas globales del archivo para que NO se destruyan
    if (!s_safe_ca)
        s_safe_ca = prepare_pem(root_ca, r_len);
    if (!s_safe_cert)
        s_safe_cert = prepare_pem(client_cert, c_len);
    if (!s_safe_key)
        s_safe_key = prepare_pem(client_key, k_len);

    // Liberamos los buffers intermedios que ya duplicamos de forma segura
    free(root_ca);
    free(client_cert);
    free(client_key);

    // =========================================================
    // TU LOG DE DEBUG FORENSE (Mantenido intacto para verificar)
    // =========================================================
    ESP_LOGE(TAG, "========= DEBUG CERTIFICADO CA =========");
    size_t final_ca_len = strlen(s_safe_ca);
    ESP_LOGE(TAG, "Longitud original de cert_store: %d", r_len);
    ESP_LOGE(TAG, "Longitud final calculada (strlen): %d", final_ca_len);
    ESP_LOGE(TAG, "Ultimos 25 caracteres en texto:");
    printf("-->%s<--\n", s_safe_ca + (final_ca_len > 25 ? final_ca_len - 25 : 0));
    ESP_LOGE(TAG, "Ultimos 25 bytes en HEX (Buscando basura o falta de \\n):");
    ESP_LOG_BUFFER_HEX_LEVEL(TAG, s_safe_ca + (final_ca_len > 25 ? final_ca_len - 25 : 0), 25, ESP_LOG_ERROR);
    ESP_LOGE(TAG, "========================================");
    // =========================================================

    // 4. Configuramos el motor MQTT apuntando a las variables estáticas s_safe_...
    esp_mqtt_client_config_t cfg = {
        .broker.address.uri = cfg_app->cloud.broker_url,
        .credentials.client_id = cfg_app->general.serial,
        .credentials.authentication.certificate = s_safe_cert,
        .credentials.authentication.key = s_safe_key,
        .broker.verification.certificate = s_safe_ca};

    s_client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_event, NULL);

    // Lanzamos el cliente
    esp_mqtt_client_start(s_client);

    // Devolvemos los 6.5 KB de la estructura de configuración inmediatamente al Heap
    free(cfg_app);

    // Bajamos el stack a 4096 bytes (1024 palabras), suficiente para cJSON.
    // Se crea UNA sola vez: si el cliente se detiene y reinicia (modo config),
    // la tarea sigue viva e inactiva (s_is_connected=false) para no duplicarla.
    if (!s_pub_task)
        xTaskCreatePinnedToCore(aws_telemetry_task, "aws_pub", 4096, NULL, 5, &s_pub_task, 1);
}

void transport_mqtt_on_net_down(void) {}

// Detiene y libera el cliente MQTT (para el "modo configuración": libera RAM
// de AWS/TLS). La tarea aws_pub queda viva pero inactiva (s_is_connected=false),
// así no toca s_client. transport_mqtt_on_time_ready() lo re-inicializa luego.
void transport_mqtt_stop(void)
{
    s_is_connected = false;                 // la tarea deja de publicar (no usa s_client)
    if (s_client)
    {
        esp_mqtt_client_stop(s_client);
        vTaskDelay(pdMS_TO_TICKS(50));       // deja salir cualquier publish en vuelo
        esp_mqtt_client_destroy(s_client);
        s_client = NULL;                    // resetea el guard de on_time_ready()
        ESP_LOGW(TAG, "Cliente MQTT detenido y liberado (modo configuración)");
    }
}
bool cloud_mgr_connected(void) { return s_is_connected; }