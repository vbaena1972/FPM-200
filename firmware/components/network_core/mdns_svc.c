#include "mdns_svc.h"

#include "esp_app_desc.h"
#include "esp_log.h"
#include "mdns.h"

static const char *TAG = "mdns_svc";

#define MDNS_SERVICE_TYPE "_fpm"
#define MDNS_PROTO        "_tcp"
#define MDNS_HTTP_PORT    80

static bool s_started;

esp_err_t mdns_svc_start(const char *instance_id)
{
    if (s_started) return ESP_OK;

    const char *id = (instance_id && instance_id[0]) ? instance_id : "fpm";

    esp_err_t err = mdns_init();
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "mdns_init fallo: %s", esp_err_to_name(err));
        return err;
    }
    err = mdns_hostname_set(id);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "mdns_hostname_set fallo: %s", esp_err_to_name(err));
        mdns_free();
        return err;
    }
    mdns_instance_name_set(id);

    const esp_app_desc_t *app = esp_app_get_description();
    mdns_txt_item_t txt[] = {
        {"id", (char *)id},
        {"model", "FPM-200"},
        {"fw", (char *)(app ? app->version : "")},
    };
    err = mdns_service_add(id, MDNS_SERVICE_TYPE, MDNS_PROTO, MDNS_HTTP_PORT,
                           txt, sizeof txt / sizeof txt[0]);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "mdns_service_add fallo: %s", esp_err_to_name(err));
        mdns_free();
        return err;
    }

    s_started = true;
    ESP_LOGI(TAG, "mDNS activo: %s.local + %s.%s:%d (TXT id=%s)",
             id, MDNS_SERVICE_TYPE, MDNS_PROTO, MDNS_HTTP_PORT, id);
    return ESP_OK;
}

void mdns_svc_stop(void)
{
    if (!s_started) return;
    mdns_free();
    s_started = false;
    ESP_LOGI(TAG, "mDNS detenido");
}
