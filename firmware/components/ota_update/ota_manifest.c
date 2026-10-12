/*
 * ota_manifest.c — ver ota_manifest.h. MODULO IDENTICO en MedGuard y FPM-200.
 */
#include "ota_manifest.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "cJSON.h"
#include "esp_crt_bundle.h"
#include "esp_heap_caps.h"
#include "esp_http_client.h"
#include "esp_log.h"

static const char *TAG = "ota_manifest";

#define MANIFEST_MAX_BYTES 2048

bool ota_url_is_image(const char *url)
{
    if (!url) return false;
    size_t n = strlen(url);
    return n > 4 && strcasecmp(url + n - 4, ".bin") == 0;
}

static void copy_str(char *dst, size_t size, const cJSON *item)
{
    if (cJSON_IsString(item) && item->valuestring)
        snprintf(dst, size, "%s", item->valuestring);
    else if (size)
        dst[0] = '\0';
}

esp_err_t ota_manifest_fetch(const char *base, const char *product, int channel,
                             ota_manifest_t *out)
{
    if (!base || !product || !out || strncmp(base, "https://", 8) != 0)
        return ESP_ERR_INVALID_ARG;
    memset(out, 0, sizeof *out);

    /* <base>/<producto>/<canal>/ (sin dobles barras) */
    char dir[200];
    size_t blen = strlen(base);
    while (blen > 8 && base[blen - 1] == '/') blen--;
    snprintf(dir, sizeof dir, "%.*s/%s/%s/", (int)blen, base, product,
             channel == 1 ? "beta" : "stable");
    char url[232];
    snprintf(url, sizeof url, "%smanifest.json", dir);

    esp_http_client_config_t config = {
        .url = url,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 15000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) return ESP_ERR_NO_MEM;

    char *body = heap_caps_malloc(MANIFEST_MAX_BYTES + 1, MALLOC_CAP_SPIRAM);
    if (!body) body = malloc(MANIFEST_MAX_BYTES + 1);
    if (!body) {
        esp_http_client_cleanup(client);
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = esp_http_client_open(client, 0);
    int len = 0;
    if (err == ESP_OK) {
        esp_http_client_fetch_headers(client);
        int status = esp_http_client_get_status_code(client);
        if (status != 200) {
            ESP_LOGE(TAG, "Manifest %s: HTTP %d", url, status);
            /* S3 detras de CloudFront (OAC, sin ListBucket) responde 403 -no 404-
             * cuando el archivo no existe: ambos = "no hay firmware publicado". */
            err = (status == 404 || status == 403) ? ESP_ERR_NOT_FOUND : ESP_FAIL;
        } else {
            int r;
            while (len < MANIFEST_MAX_BYTES &&
                   (r = esp_http_client_read(client, body + len, MANIFEST_MAX_BYTES - len)) > 0)
                len += r;
            if (len <= 0) err = ESP_ERR_INVALID_RESPONSE;
        }
    } else {
        ESP_LOGE(TAG, "No se pudo abrir %s: %s", url, esp_err_to_name(err));
    }
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    if (err != ESP_OK) {
        free(body);
        return err;
    }
    body[len] = '\0';

    cJSON *root = cJSON_Parse(body);
    free(body);
    if (!root) return ESP_ERR_INVALID_RESPONSE;
    copy_str(out->version, sizeof out->version, cJSON_GetObjectItemCaseSensitive(root, "version"));
    copy_str(out->project, sizeof out->project, cJSON_GetObjectItemCaseSensitive(root, "project"));
    copy_str(out->sha256, sizeof out->sha256, cJSON_GetObjectItemCaseSensitive(root, "sha256"));
    const cJSON *size = cJSON_GetObjectItemCaseSensitive(root, "size");
    if (cJSON_IsNumber(size) && size->valuedouble > 0) out->size = (size_t)size->valuedouble;
    char rel[160];
    copy_str(rel, sizeof rel, cJSON_GetObjectItemCaseSensitive(root, "url"));
    const cJSON *prod = cJSON_GetObjectItemCaseSensitive(root, "product");
    bool product_ok = !cJSON_IsString(prod) || strcmp(prod->valuestring, product) == 0;
    cJSON_Delete(root);

    if (!out->version[0] || !rel[0] || !product_ok) {
        ESP_LOGE(TAG, "Manifest invalido (version/url/producto)");
        return ESP_ERR_INVALID_RESPONSE;
    }
    if (strncmp(rel, "https://", 8) == 0)
        snprintf(out->url, sizeof out->url, "%s", rel);
    else if (strstr(rel, "://"))
        return ESP_ERR_INVALID_RESPONSE;   /* solo https */
    else
        snprintf(out->url, sizeof out->url, "%s%s", dir, rel);
    ESP_LOGI(TAG, "Manifest %s/%s: version %s, %u bytes", product,
             channel == 1 ? "beta" : "stable", out->version, (unsigned)out->size);
    return ESP_OK;
}

/* Lee hasta 3 numeros "a.b.c" y deja *rest en el sufijo (p. ej. "-dev"). */
static void parse_version(const char *v, long num[3], const char **rest)
{
    num[0] = num[1] = num[2] = 0;
    const char *p = v ? v : "";
    if (*p == 'v' || *p == 'V') p++;
    for (int i = 0; i < 3; i++) {
        char *end;
        num[i] = strtol(p, &end, 10);
        p = end;
        if (*p != '.') break;
        p++;
    }
    *rest = p;
}

int ota_version_compare(const char *a, const char *b)
{
    long na[3], nb[3];
    const char *sa, *sb;
    parse_version(a, na, &sa);
    parse_version(b, nb, &sb);
    for (int i = 0; i < 3; i++)
        if (na[i] != nb[i]) return na[i] < nb[i] ? -1 : 1;
    if (!*sa && *sb) return 1;    /* release > -dev */
    if (*sa && !*sb) return -1;
    return strcmp(sa, sb);
}
