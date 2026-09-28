/*
 * ota_update.h — actualizacion de firmware desde un archivo (microSD).
 *
 * Equivalente al ota_service de MedGuard (fuente microSD). Seguridad, igual en
 * ambos equipos y SIN eFuses:
 *   - la imagen debe ser de ESTE proyecto (project_name del app descriptor);
 *   - esp_ota_end verifica checksum y la FIRMA RSA-3072 (OTA firmada,
 *     CONFIG_SECURE_SIGNED_ON_UPDATE_NO_SECURE_BOOT): un .bin sin firmar o
 *     firmado con otra clave se rechaza y el equipo sigue con su firmware;
 *   - la imagen nueva arranca "pendiente de verificar": si no completa el
 *     arranque (panic/WDT antes de esp_ota_mark_app_valid_cancel_rollback en
 *     main.c) el bootloader vuelve sola a la version anterior.
 * La particion en uso nunca se toca: un corte de energia durante la escritura
 * deja el equipo con el firmware actual.
 */
#pragma once

#include <stddef.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Progreso 0..100 y etapa legible. Se llama desde la tarea que instala. */
typedef void (*ota_update_progress_cb_t)(int pct, const char *stage, void *user);

/* Valida e instala `path`. Si todo sale bien deja la imagen nueva como
 * particion de arranque (se aplica al reiniciar) y copia su version en
 * `version_out`. No reinicia. */
esp_err_t ota_update_from_file(const char *path, ota_update_progress_cb_t cb,
                               void *user, char *version_out, size_t version_size);

/* Busca en `dir` el firmware a instalar. Nombres aceptados (sin distinguir
 * mayusculas): versionados fpm200_<version>.bin (p.ej. fpm200_1.5.27.bin, el
 * build los deja en build/) o el heredado firmware.bin. Ignora imagenes de otro
 * proyecto y, si hay varias, elige la de VERSION MAS ALTA segun el descriptor
 * interno (el nombre es para las personas). Devuelve ESP_ERR_NOT_FOUND si no
 * hay ninguna. Los ya procesados (.instalado/.rechazado) no terminan en .bin. */
esp_err_t ota_update_find_file(const char *dir, char *path, size_t path_size,
                               char *version, size_t version_size);

#ifdef __cplusplus
}
#endif
