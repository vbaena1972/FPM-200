#ifndef ADS1115_H
#define ADS1115_H

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Driver para el ADC de 16 bits TI ADS1115 (familia ADS111x).
 * Direccion I2C segun el pin ADDR:
 *   - ADDR a GND -> 0x48 (default en esta placa)
 *   - ADDR a VDD -> 0x49
 *   - ADDR a SDA -> 0x4A
 *   - ADDR a SCL -> 0x4B
 *
 * En esta placa el canal AIN0 (single-ended) recibe la senal analogica
 * del sensor de flujo FS7 (FlowAnalog).
 */

// Canales single-ended (respecto a GND)
typedef enum {
    ADS1115_MUX_AIN0 = 0, // single-ended AIN0
    ADS1115_MUX_AIN1 = 1,
    ADS1115_MUX_AIN2 = 2,
    ADS1115_MUX_AIN3 = 3,
} ads1115_channel_t;

/**
 * Inicializa el ADS1115 en la direccion 0x48 y lo agrega al bus I2C.
 * PGA por defecto: +-4.096 V (adecuado para senales 0..3.3 V).
 */
esp_err_t ads1115_init(i2c_master_bus_handle_t bus_handle);

bool ads1115_is_ready(void);

/**
 * Recuperacion especifica del ADS1115: quita y vuelve a agregar el dispositivo
 * al bus I2C y reverifica comunicacion. Util cuando el ADS hace NACK sostenido
 * (INVALID_RESPONSE) mientras el resto del bus sigue OK, sin necesidad de
 * resetear todo el controlador del bus. Devuelve ESP_OK si el ADS responde.
 */
esp_err_t ads1115_recover(void);

/**
 * Lee un canal single-ended en modo single-shot y devuelve el codigo crudo
 * de 16 bits con signo.
 */
esp_err_t ads1115_read_raw(ads1115_channel_t ch, int16_t *raw);

/**
 * Lee un canal single-ended y devuelve el voltaje en voltios
 * (usando el PGA configurado, +-4.096 V -> LSB = 125 uV).
 */
esp_err_t ads1115_read_voltage(ads1115_channel_t ch, float *volts);

#ifdef __cplusplus
}
#endif

#endif // ADS1115_H
