#ifndef MS5803_H
#define MS5803_H

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Driver para el sensor de presion/temperatura TE Connectivity
 * MS5803-14BA (rango 0..14 bar). Interfaz I2C (pin PS a VDD).
 *
 * Direccion I2C: depende del pin CSB.
 *   - CSB a VDD (High) -> 0x76
 *   - CSB a GND (Low)  -> 0x77
 * El init prueba 0x76 y, si no responde, 0x77 (y lo registra en el log).
 */

/**
 * Inicializa el MS5803: lo agrega al bus I2C, hace reset, lee la PROM de
 * calibracion y valida el CRC4. Debe llamarse una sola vez tras crear el bus.
 * Devuelve ESP_OK si el sensor respondio y la PROM es valida.
 */
esp_err_t ms5803_init(i2c_master_bus_handle_t bus_handle);

/**
 * Indica si el sensor se inicializo correctamente (PROM valida).
 */
bool ms5803_is_ready(void);

/**
 * Realiza una conversion completa (D1 presion + D2 temperatura, OSR=4096),
 * aplica compensacion de 1er y 2do orden y devuelve resultados en unidades SI.
 *  - pressure_kpa: presion absoluta en kPa (puede ser NULL)
 *  - temp_c:       temperatura en grados Celsius (puede ser NULL)
 *
 * NOTA: esta funcion bloquea ~20 ms (dos conversiones a OSR 4096).
 */
esp_err_t ms5803_read(float *pressure_kpa, float *temp_c);

#ifdef __cplusplus
}
#endif

#endif // MS5803_H
