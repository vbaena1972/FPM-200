#ifndef BMP280_H
#define BMP280_H

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Driver para el barometro Bosch BMP280 / BME280 (presion + temperatura).
 * Se usa como REFERENCIA de presion atmosferica para el "cero" (gauge) del
 * sensor de presion de linea (MS5803): gauge = abs_linea - atm_baro.
 *
 * Direccion I2C: 0x76 (SDO a GND) o 0x77 (SDO a VDD). En esta placa va en 0x77
 * (el MS5803 ocupa 0x76). Del BME280 solo se leen P y T (se ignora humedad).
 *
 * Chip ID (reg 0xD0): 0x58 = BMP280, 0x60 = BME280 (ambos soportados).
 */

/**
 * Inicializa el BMP280/BME280 en la direccion dada: verifica el Chip ID, hace
 * soft-reset, lee la calibracion de fabrica y configura muestreo continuo
 * (normal mode, oversampling x16 presion / x2 temp, filtro IIR). Una sola vez
 * tras crear el bus. Devuelve ESP_OK si respondio y el Chip ID es valido.
 */
esp_err_t bmp280_init(i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr);

/** true si el sensor se inicializo correctamente. */
bool bmp280_is_ready(void);

/**
 * Lee presion y temperatura compensadas.
 *  - pressure_kpa: presion absoluta en kPa (puede ser NULL)
 *  - temp_c:       temperatura en grados Celsius (puede ser NULL)
 * Devuelve ESP_OK si la lectura I2C fue valida.
 */
esp_err_t bmp280_read(float *pressure_kpa, float *temp_c);

#ifdef __cplusplus
}
#endif

#endif // BMP280_H
