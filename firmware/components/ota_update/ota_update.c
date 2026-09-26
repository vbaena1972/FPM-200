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

#include "esp_app_desc.h"
#include "esp_app_format.h"
#include "esp_log.h"
#include "esp_ota_ops.h"

#define OTA_IO_BUFFER_SIZE 4096

static const char *TAG = "ota_update";

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
