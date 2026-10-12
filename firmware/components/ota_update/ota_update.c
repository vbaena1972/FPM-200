/*
 * ota_update.c — ver ota_update.h. Portado del ota_service de MedGuard
 * (install_sd), con escritura secuencial (borra la flash por bloques mientras
 * escribe: no hay un borrado largo de 3 MB que frene las demas tareas).
 */
#include "ota_update.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <strings.h>

#include "esp_app_desc.h"
#include "esp_app_format.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_crt_bundle.h"
#include "esp_https_ota.h"
#include "ota_manifest.h"

#define OTA_IO_BUFFER_SIZE 4096

static const char *TAG = "ota_update";

/* Con escritura secuencial (o borrado de solo el tamano de la imagen) el resto
 * de la particion conserva lo que tenia. El bloque de firma se busca justo
 * despues del final de la imagen (alineado a 4 KB): si la particion guardaba la
 * version FIRMADA de este mismo binario, un .bin SIN firmar heredaria esa firma
 * vieja y se aceptaria (visto en HW 2026-09-28). Se borra esa zona antes de
 * esp_ota_end para que la verificacion solo vea lo que trae el archivo. */
#define OTA_STALE_TAIL_ERASE (64U * 1024U)
static esp_err_t erase_stale_tail(const esp_partition_t *partition, size_t written)
{
    const size_t sector = partition->erase_size ? partition->erase_size : 4096U;
    size_t start = (written + sector - 1U) / sector * sector;
    if (start >= partition->size) return ESP_OK;
    size_t len = OTA_STALE_TAIL_ERASE;
    if (len > partition->size - start) len = partition->size - start;
    return esp_partition_erase_range(partition, start, len);
}

static void notify(ota_update_progress_cb_t cb, void *user, int pct, const char *stage)
{
    if (cb) cb(pct, stage, user);
}

/* Lee el app descriptor del .bin y comprueba que sea de este proyecto. */
static esp_err_t read_description(FILE *file, esp_app_desc_t *desc)
{
    esp_image_header_t image_header;
    esp_image_segment_header_t segment_header;
    if (fread(&image_header, 1, sizeof image_header, file) != sizeof image_header ||
        fread(&segment_header, 1, sizeof segment_header, file) != sizeof segment_header ||
        fread(desc, 1, sizeof *desc, file) != sizeof *desc)
        return ESP_ERR_INVALID_SIZE;
    if (image_header.magic != ESP_IMAGE_HEADER_MAGIC ||
        desc->magic_word != ESP_APP_DESC_MAGIC_WORD)
        return ESP_ERR_OTA_VALIDATE_FAILED;

    const esp_app_desc_t *running = esp_app_get_description();
    if (strncmp(desc->project_name, running->project_name,
                sizeof desc->project_name) != 0) {
        ESP_LOGE(TAG, "Imagen de otro proyecto ('%s'), se esperaba '%s'",
                 desc->project_name, running->project_name);
        return ESP_ERR_INVALID_VERSION;
    }
    return ESP_OK;
}

esp_err_t ota_update_from_file(const char *path, ota_update_progress_cb_t cb,
                               void *user, char *version_out, size_t version_size)
{
    if (!path) return ESP_ERR_INVALID_ARG;
    struct stat st;
    if (stat(path, &st) != 0 || st.st_size <= 0) return ESP_ERR_NOT_FOUND;
    const size_t image_size = (size_t)st.st_size;

    FILE *file = fopen(path, "rb");
    if (!file) return ESP_ERR_NOT_FOUND;

    notify(cb, user, 0, "Validando firmware");
    esp_app_desc_t desc;
    esp_err_t err = read_description(file, &desc);
    if (err != ESP_OK) {
        fclose(file);
        return err;
    }
    const esp_app_desc_t *running = esp_app_get_description();
    ESP_LOGW(TAG, "Firmware en microSD: %s (en uso: %s), %u bytes",
             desc.version, running->version, (unsigned)image_size);

    const esp_partition_t *partition = esp_ota_get_next_update_partition(NULL);
    if (!partition) {
        fclose(file);
        return ESP_ERR_NOT_FOUND;
    }
    if (image_size > partition->size) {
        ESP_LOGE(TAG, "Imagen de %u bytes no cabe en %s (%u bytes)",
                 (unsigned)image_size, partition->label, (unsigned)partition->size);
        fclose(file);
        return ESP_ERR_INVALID_SIZE;
    }

    uint8_t *buffer = malloc(OTA_IO_BUFFER_SIZE);
    if (!buffer) {
        fclose(file);
        return ESP_ERR_NO_MEM;
    }

    esp_ota_handle_t handle = 0;
    err = esp_ota_begin(partition, OTA_WITH_SEQUENTIAL_WRITES, &handle);
    if (err != ESP_OK) {
        free(buffer);
        fclose(file);
        return err;
    }

    rewind(file);
    size_t written = 0;
    int last_pct = -1;
    while (err == ESP_OK) {
        size_t count = fread(buffer, 1, OTA_IO_BUFFER_SIZE, file);
        if (count == 0) {
            if (ferror(file)) err = ESP_FAIL;
            break;
        }
        err = esp_ota_write(handle, buffer, count);
        written += count;
        int pct = (int)(written * 95U / image_size);
        if (pct != last_pct) {
            notify(cb, user, pct, "Escribiendo firmware");
            last_pct = pct;
        }
    }
    free(buffer);
    fclose(file);

    if (err != ESP_OK || written != image_size) {
        esp_ota_abort(handle);
        ESP_LOGE(TAG, "Escritura OTA fallida: %s (%u/%u bytes)", esp_err_to_name(err),
                 (unsigned)written, (unsigned)image_size);
        return err != ESP_OK ? err : ESP_ERR_INVALID_SIZE;
    }

    err = erase_stale_tail(partition, written);
    if (err != ESP_OK) {
        esp_ota_abort(handle);
        return err;
    }

    /* Verifica checksum + FIRMA (OTA firmada). Imagen ajena => se rechaza aqui. */
    notify(cb, user, 97, "Verificando firma");
    err = esp_ota_end(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Imagen rechazada (%s): sin firma valida o corrupta",
                 esp_err_to_name(err));
        return err;
    }
    err = esp_ota_set_boot_partition(partition);
    if (err != ESP_OK) return err;

    if (version_out && version_size) snprintf(version_out, version_size, "%s", desc.version);
    notify(cb, user, 100, "Firmware listo");
    ESP_LOGW(TAG, "Firmware %s instalado en %s; se aplica al reiniciar",
             desc.version, partition->label);
    return ESP_OK;
}

/* ---- Seleccion del archivo (misma regla que MedGuard storage_service) ---- */

static bool ends_with_ci(const char *s, const char *suffix)
{
    size_t n = strlen(s), m = strlen(suffix);
    return n >= m && strcasecmp(s + n - m, suffix) == 0;
}

/* <0, 0, >0 como strcmp, comparando los numeros de "1.10.2-dev" por valor;
 * a igualdad numerica, la version sin sufijo ("-dev", "-rc1") es mayor. */
static int version_cmp(const char *a, const char *b)
{
    while (*a || *b) {
        bool da = *a >= '0' && *a <= '9', db = *b >= '0' && *b <= '9';
        if (da && db) {
            unsigned long va = strtoul(a, (char **)&a, 10);
            unsigned long vb = strtoul(b, (char **)&b, 10);
            if (va != vb) return va < vb ? -1 : 1;
            continue;
        }
        if (*a == '.' && *b == '.') { ++a; ++b; continue; }
        if (!*a) return 1;           /* a termino: "1.5.27" > "1.5.27-dev" */
        if (!*b) return -1;
        return strcmp(a, b);
    }
    return 0;
}

esp_err_t ota_update_find_file(const char *dir, char *path, size_t path_size,
                               char *version, size_t version_size)
{
    if (!dir || !path || path_size == 0) return ESP_ERR_INVALID_ARG;
    path[0] = '\0';
    char best_version[32] = "";
    DIR *d = opendir(dir);
    if (!d) return ESP_ERR_NOT_FOUND;
    struct dirent *entry;
    char candidate[160];
    while ((entry = readdir(d)) != NULL) {
        const char *name = entry->d_name;
        if (!ends_with_ci(name, ".bin")) continue;
        if (strncasecmp(name, "fpm", 3) != 0 && strcasecmp(name, "firmware.bin") != 0)
            continue;
        if (snprintf(candidate, sizeof candidate, "%s/%s", dir, name) >=
            (int)sizeof candidate)
            continue;
        FILE *file = fopen(candidate, "rb");
        if (!file) continue;
        esp_app_desc_t desc;
        esp_err_t err = read_description(file, &desc);
        fclose(file);
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "Ignorado %s: no es una imagen de este equipo", candidate);
            continue;
        }
        if (!path[0] || version_cmp(desc.version, best_version) > 0) {
            strlcpy(path, candidate, path_size);
            strlcpy(best_version, desc.version, sizeof best_version);
        }
    }
    closedir(d);
    if (!path[0]) return ESP_ERR_NOT_FOUND;
    if (version && version_size) strlcpy(version, best_version, version_size);
    ESP_LOGI(TAG, "Firmware en microSD: %s (version %s)", path, best_version);
    return ESP_OK;
}
/* ---- OTA por red (HTTPS), 1.5.37 — mismo esquema que MedGuard 1.7.18 ----
 * <base>/fpm200/<stable|beta>/manifest.json (ota_manifest.h). Solo se instala
 * una version MAS NUEVA que la que corre (si no: ESP_ERR_NOT_SUPPORTED). El
 * llamador debe pausar antes la sesion MQTT (transport_mqtt_stop): la RAM
 * interna no alcanza para dos sesiones TLS. */
#define OTA_PRODUCT "fpm200"

static esp_err_t fetch_manifest(const char *base, int channel, ota_manifest_t *manifest)
{
    esp_err_t err = ota_manifest_fetch(base, OTA_PRODUCT, channel, manifest);
    if (err != ESP_OK) return err;
    const esp_app_desc_t *running = esp_app_get_description();
    if (manifest->project[0] &&
        strncmp(manifest->project, running->project_name, sizeof running->project_name) != 0) {
        ESP_LOGE(TAG, "Manifest de %s, no de %s", manifest->project, running->project_name);
        return ESP_ERR_INVALID_VERSION;
    }
    return ESP_OK;
}

esp_err_t ota_update_check_url(const char *base, int channel,
                               char *version_out, size_t version_size, bool *newer)
{
    ota_manifest_t manifest;
    esp_err_t err = fetch_manifest(base, channel, &manifest);
    if (err != ESP_OK) return err;
    if (version_out && version_size) snprintf(version_out, version_size, "%s", manifest.version);
    if (newer) *newer = ota_version_compare(manifest.version, esp_app_get_description()->version) > 0;
    return ESP_OK;
}

esp_err_t ota_update_from_url(const char *base, int channel, ota_update_progress_cb_t cb,
                              void *user, char *version_out, size_t version_size)
{
    ota_manifest_t manifest;
    esp_err_t err = fetch_manifest(base, channel, &manifest);
    if (err != ESP_OK) return err;
    if (ota_version_compare(manifest.version, esp_app_get_description()->version) <= 0)
        return ESP_ERR_NOT_SUPPORTED;

    esp_http_client_config_t http = {
        .url = manifest.url,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .timeout_ms = 20000,
        .keep_alive_enable = true,
        /* 1.7.20: el buffer por defecto (512 B) hacia la descarga muy lenta
         * (~10 KB/s, 2,5 MB en ~4 min en HW 2026-10-12). */
        .buffer_size = 8192,
    };
    esp_https_ota_config_t ota = { .http_config = &http };
    esp_https_ota_handle_t handle = NULL;
    notify(cb, user, 1, "Conectando al servidor");
    err = esp_https_ota_begin(&ota, &handle);
    if (err != ESP_OK) return err;

    esp_app_desc_t desc;
    err = esp_https_ota_get_img_desc(handle, &desc);
    if (err == ESP_OK) {
        const esp_app_desc_t *running = esp_app_get_description();
        if (strncmp(desc.project_name, running->project_name, sizeof desc.project_name) != 0) {
            ESP_LOGE(TAG, "Imagen de otro proyecto (%s)", desc.project_name);
            err = ESP_ERR_INVALID_VERSION;
        }
    }
    if (err != ESP_OK) {
        esp_https_ota_abort(handle);
        return err;
    }

    int total = esp_https_ota_get_image_size(handle);
    do {
        err = esp_https_ota_perform(handle);
        int read = esp_https_ota_get_image_len_read(handle);
        notify(cb, user, total > 0 && read > 0 ? (int)((long long)read * 95 / total) : 1,
               "Descargando firmware");
    } while (err == ESP_ERR_HTTPS_OTA_IN_PROGRESS);
    if (err != ESP_OK || !esp_https_ota_is_complete_data_received(handle)) {
        ESP_LOGE(TAG, "Descarga OTA incompleta: %s", esp_err_to_name(err));
        esp_https_ota_abort(handle);
        return err == ESP_OK ? ESP_ERR_INVALID_SIZE : err;
    }
    /* Misma proteccion que la microSD: sin restos de una firma vieja. */
    const esp_partition_t *partition = esp_ota_get_next_update_partition(NULL);
    int read = esp_https_ota_get_image_len_read(handle);
    err = partition && read > 0 ? erase_stale_tail(partition, (size_t)read) : ESP_FAIL;
    if (err != ESP_OK) {
        esp_https_ota_abort(handle);
        return err;
    }
    notify(cb, user, 97, "Verificando firma");
    err = esp_https_ota_finish(handle);   /* esp_ota_end (firma) + particion de arranque */
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Imagen rechazada (%s): sin firma valida o corrupta", esp_err_to_name(err));
        return err;
    }
    if (version_out && version_size) snprintf(version_out, version_size, "%s", desc.version);
    notify(cb, user, 100, "Firmware listo");
    ESP_LOGW(TAG, "Firmware %s descargado e instalado; se aplica al reiniciar", desc.version);
    return ESP_OK;
}
