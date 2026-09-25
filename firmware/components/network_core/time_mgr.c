#include "flow_meter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "time_mgr.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_netif_sntp.h"
#include "rtc_rv3028.h"
#include <sys/time.h>
#include <time.h>
#include "storage.h"
#include "transport_mqtt.h" // Para avisar a MQTT

static const char *TAG = "time_mgr";
static bool s_time_valid = false;

// A single worker owns the slow RTC/MQTT work. The TCP/IP callback only
// overwrites a one-element mailbox; repeated syncs cannot accumulate tasks.
static QueueHandle_t s_sync_queue;
static void time_sync_worker(void *arg)
{
    time_t stamp;
    while (true) {
        flow_meter_service();
        if (xQueueReceive(s_sync_queue, &stamp, pdMS_TO_TICKS(1000)) != pdTRUE) continue;
        struct tm local = {0};
        localtime_r(&stamp, &local);
        esp_err_t err = rtc_rv3028_set_time(&local);
        if (err != ESP_OK)
            ESP_LOGW(TAG, "RTC sync failed (%s); system time remains valid", esp_err_to_name(err));
        else
            ESP_LOGI(TAG, "RTC synchronized");
        transport_mqtt_on_time_ready();
    }
}
static void time_sync_notification_cb(struct timeval *tv)
{
    s_time_valid = true;
    time_t stamp = tv->tv_sec;
    if (s_sync_queue) xQueueOverwrite(s_sync_queue, &stamp);
}

void time_mgr_init(void)
{
    if (!s_sync_queue) {
        s_sync_queue = xQueueCreate(1, sizeof(time_t));
        if (!s_sync_queue || xTaskCreate(time_sync_worker, "time_sync", 8192,
                                        NULL, 3, NULL) != pdPASS) {
            if (s_sync_queue) vQueueDelete(s_sync_queue);
            s_sync_queue = NULL;
            ESP_LOGE(TAG, "Cannot create time sync worker");
        }
    }
    // Configuramos la zona horaria de Colombia por defecto
    setenv("TZ", "COT5", 1);
    tzset();

    // Intentamos cargar la hora desde el RTC RV-3028
    struct tm rtc_time = {0};
    if (rtc_rv3028_get_time(&rtc_time) == ESP_OK) {
        // Validamos si el RTC tiene un año coherente (ej. > 2023)
        if ((rtc_time.tm_year + 1900) >= 2024) {
            struct timeval tv;
            tv.tv_sec = mktime(&rtc_time);
            tv.tv_usec = 0;
            settimeofday(&tv, NULL);
            ESP_LOGI(TAG, "Hora del sistema restaurada desde RTC: %04d-%02d-%02d %02d:%02d",
                     rtc_time.tm_year + 1900, rtc_time.tm_mon + 1, rtc_time.tm_mday,
                     rtc_time.tm_hour, rtc_time.tm_min);
            s_time_valid = true;
        } else {
            ESP_LOGW(TAG, "El RTC tiene una fecha antigua o descalibrada. Esperando a SNTP.");
        }
    }
}

void time_mgr_start_sntp(void)
{
    ESP_LOGI(TAG, "Iniciando servicio SNTP con redundancia...");

    // 1. Limpiamos cualquier instancia previa por si el Wi-Fi se reconectó
    esp_netif_sntp_deinit();

    // 2. Configuramos el servidor principal
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    config.sync_cb = time_sync_notification_cb;
    
    // 3. Inicializamos el motor SNTP
    esp_netif_sntp_init(&config);

    // 4. INYECCIÓN CRÍTICA: Añadimos servidores de respaldo de alta disponibilidad
    esp_sntp_setservername(1, "time.google.com");
    esp_sntp_setservername(2, "time.nist.gov");
    
    ESP_LOGI(TAG, "Servidores NTP configurados. Esperando sincronización...");
}

bool time_mgr_is_time_valid(void)
{
    return s_time_valid;
}

bool time_mgr_set_datetime(int year, int month, int day,
                           int hour, int minute, int second)
{
    struct tm t = {0};
    t.tm_year  = year - 1900;
    t.tm_mon   = month - 1;
    t.tm_mday  = day;
    t.tm_hour  = hour;
    t.tm_min   = minute;
    t.tm_sec   = second;
    t.tm_isdst = -1;                 // deja que la TZ resuelva el horario de verano

    time_t epoch = mktime(&t);       // interpreta los componentes como hora LOCAL
    if (epoch <= 0) {
        ESP_LOGW(TAG, "set_datetime: fecha/hora inválida");
        return false;
    }

    // 1) Hora del sistema (UTC internamente)
    struct timeval tv = { .tv_sec = epoch, .tv_usec = 0 };
    settimeofday(&tv, NULL);

    // 2) RTC de hardware en hora LOCAL (igual que la sincronización SNTP)
    struct tm local;
    localtime_r(&epoch, &local);
    if (rtc_rv3028_set_time(&local) != ESP_OK)
        ESP_LOGW(TAG, "set_datetime: no se pudo actualizar el RTC");

    s_time_valid = true;
    ESP_LOGI(TAG, "Reloj ajustado manualmente: %04d-%02d-%02d %02d:%02d:%02d",
             year, month, day, hour, minute, second);

    // Avisa a MQTT que ya hay hora válida (certificados TLS necesitan hora)
    transport_mqtt_on_time_ready();
    return true;
}