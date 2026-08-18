#ifndef SFM3300_H
#define SFM3300_H

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Driver para el caudalimetro digital Sensirion SFM3300-D (aplicaciones
 * medicas, aire/O2, rango +-250 slm). Interfaz I2C, direccion 0x40.
 *
 * NOTA de hardware: el SFM3300 se alimenta a 5 V. Asegurar que las lineas
 * I2C (SDA/SCL) usen pull-ups a 3.3 V (no a 5 V) para no exceder el maximo
 * de los GPIO del ESP32-S3, o intercalar un level-shifter.
 *
 * En esta placa va en un bus I2C aparte: SDA=GPIO43, SCL=GPIO44.
 * Se usa como referencia para calibrar/observar el FS7 y el bypass.
 */

/**
 * Inicializa el SFM3300: lo agrega al bus, hace soft-reset y arranca la
 * medicion continua (comando 0x1000). Tras esto, cada lectura devuelve el
 * ultimo flujo medido.
 */
esp_err_t sfm3300_init(i2c_master_bus_handle_t bus_handle);

bool sfm3300_is_ready(void);

/**
 * Lee el flujo actual en slm (standard liters per minute). Bidireccional
 * (signo = sentido del flujo). Devuelve:
 *  - ESP_OK con *slm valido
 *  - ESP_ERR_INVALID_RESPONSE si el dato aun no es valido (el sensor NACKea)
 *  - ESP_ERR_INVALID_CRC si el CRC no coincide
 */
esp_err_t sfm3300_read(float *slm);

#ifdef __cplusplus
}
#endif

#endif // SFM3300_H
