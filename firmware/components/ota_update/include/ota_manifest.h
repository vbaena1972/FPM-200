/*
 * ota_manifest.h — manifest de OTA por red (HTTPS). MODULO IDENTICO en MedGuard
 * (firmware/storage/) y FPM-200 (components/ota_update/): mantenerlos iguales.
 *
 * El servidor de firmware (S3 + CloudFront, ver scripts/publish_firmware.sh)
 * publica por producto y canal:
 *     <base>/<producto>/<canal>/manifest.json      canal = stable | beta
 *     {"product":"medguard","channel":"stable","version":"1.7.18",
 *      "project":"medguard_hmi","url":"medguard_1.7.18.bin",
 *      "size":1654784,"sha256":"…"}
 * "url" puede ser absoluta (https://…) o relativa a la carpeta del manifest.
 * El .bin va SIEMPRE firmado: la firma la verifica esp_ota_end, no el manifest.
 */
#ifndef OTA_MANIFEST_H
#define OTA_MANIFEST_H

#include <stdbool.h>
#include <stddef.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char version[32];
    char project[32];
    char url[256];      /* absoluta, https:// */
    size_t size;
    char sha256[65];
} ota_manifest_t;

/* Descarga y valida el manifest de `product` en el canal 0=stable / 1=beta.
 * base: "https://servidor" (sin barra final, o con ella). Bloqueante: usar
 * desde una tarea de trabajo. Abre UNA conexion TLS: pausar antes la sesion
 * MQTT/AWS si la RAM interna es justa. */
esp_err_t ota_manifest_fetch(const char *base, const char *product, int channel,
                             ota_manifest_t *out);

/* Compara versiones "a.b.c[-sufijo]": <0 si a<b, 0 si iguales, >0 si a>b. A
 * igual numero, la version SIN sufijo (release) es mayor que una "-dev". */
int ota_version_compare(const char *a, const char *b);

/* true si `url` apunta directo a un .bin (modo heredado: sin manifest). */
bool ota_url_is_image(const char *url);

#ifdef __cplusplus
}
#endif

#endif
