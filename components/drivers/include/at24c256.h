#ifndef AT24C256_H
#define AT24C256_H

#include "esp_err.h"
#include "driver/i2c_master.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Driver para la EEPROM I2C Atmel/Microchip AT24C256C.
 *   - Capacidad: 256 Kbit = 32768 bytes (32 KB)
 *   - Direccionamiento interno de 16 bits
 *   - Tamano de pagina: 64 bytes
 *   - Direccion I2C: 0x50..0x57 segun pines A0/A1/A2 (0x50 con todos a GND)
 *   - WP a GND -> escritura habilitada
 */

#define AT24C256_SIZE_BYTES 32768
#define AT24C256_PAGE_SIZE  64

/**
 * Inicializa la EEPROM en la direccion indicada (usa 0x50 si addr==0).
 */
esp_err_t at24c256_init(i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr);

bool at24c256_is_ready(void);

/**
 * Lee 'len' bytes a partir de la direccion interna 'mem_addr'.
 */
esp_err_t at24c256_read(uint16_t mem_addr, uint8_t *buf, size_t len);

/**
 * Escribe 'len' bytes a partir de 'mem_addr'. Maneja automaticamente los
 * limites de pagina (64 bytes) y el tiempo de ciclo de escritura (~5 ms).
 */
esp_err_t at24c256_write(uint16_t mem_addr, const uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif // AT24C256_H
