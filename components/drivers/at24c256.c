#include "at24c256.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "at24c256";

#define AT24C256_DEFAULT_ADDR 0x50
#define AT24C256_WRITE_CYCLE_MS 6 // ciclo de escritura ~5 ms (margen 6 ms)

static i2c_master_dev_handle_t s_dev = NULL;
static bool s_ready = false;

esp_err_t at24c256_init(i2c_master_bus_handle_t bus_handle, uint8_t i2c_addr)
{
    if (!bus_handle)
        return ESP_ERR_INVALID_ARG;

    s_ready = false;
    s_dev = NULL;

    uint8_t addr = i2c_addr ? i2c_addr : AT24C256_DEFAULT_ADDR;

    // 50 kHz (no 100): el write del bloque de calibracion es la transaccion mas
    // larga del bus 1 (~54 bytes) y con 6 dispositivos + pull-ups de 4.7k los
    // flancos son lentos; a mitad de reloj hay mas margen y el write deja de
    // NACKear. Las lecturas (cortas) ya iban bien.
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = 50000,
    };
    esp_err_t err = i2c_master_bus_add_device(bus_handle, &dev_cfg, &s_dev);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error agregando EEPROM al bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    // Prueba de presencia: leemos 1 byte de la direccion 0
    uint8_t addr_bytes[2] = {0, 0};
    uint8_t probe = 0;
    err = i2c_master_transmit_receive(s_dev, addr_bytes, 2, &probe, 1, 100);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "EEPROM AT24C256C no responde en 0x%02X: %s", addr, esp_err_to_name(err));
        i2c_master_bus_rm_device(s_dev);
        s_dev = NULL;
        return err;
    }

    s_ready = true;
    ESP_LOGI(TAG, "EEPROM AT24C256C OK en 0x%02X (32 KB, pagina 64 B)", addr);
    return ESP_OK;
}

bool at24c256_is_ready(void)
{
    return s_ready;
}

esp_err_t at24c256_read(uint16_t mem_addr, uint8_t *buf, size_t len)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;
    if (!buf || len == 0)
        return ESP_ERR_INVALID_ARG;
    if ((size_t)mem_addr + len > AT24C256_SIZE_BYTES)
        return ESP_ERR_INVALID_SIZE;

    uint8_t addr_bytes[2] = {(uint8_t)(mem_addr >> 8), (uint8_t)(mem_addr & 0xFF)};
    return i2c_master_transmit_receive(s_dev, addr_bytes, 2, buf, len, 200);
}

esp_err_t at24c256_write(uint16_t mem_addr, const uint8_t *buf, size_t len)
{
    if (!s_dev || !s_ready)
        return ESP_ERR_INVALID_STATE;
    if (!buf || len == 0)
        return ESP_ERR_INVALID_ARG;
    if ((size_t)mem_addr + len > AT24C256_SIZE_BYTES)
        return ESP_ERR_INVALID_SIZE;

    size_t remaining = len;
    uint16_t addr = mem_addr;
    const uint8_t *p = buf;

    // Un pequeno buffer para juntar los 2 bytes de direccion + datos de una pagina
    uint8_t tx[2 + AT24C256_PAGE_SIZE];

    while (remaining > 0)
    {
        // No cruzar el limite de pagina de 64 bytes
        size_t page_off = addr % AT24C256_PAGE_SIZE;
        size_t space = AT24C256_PAGE_SIZE - page_off;
        size_t chunk = (remaining < space) ? remaining : space;
        // Tope de 32 B: transacciones mas cortas fallan menos en el bus marginal.
        if (chunk > 32)
            chunk = 32;

        tx[0] = (uint8_t)(addr >> 8);
        tx[1] = (uint8_t)(addr & 0xFF);
        memcpy(&tx[2], p, chunk);

        // Reintentamos el page write: en el bus 1 marginal un NACK puntual es
        // comun en la transaccion larga. Entre intentos dejamos respirar el bus.
        esp_err_t err = ESP_FAIL;
        for (int attempt = 0; attempt < 4; attempt++)
        {
            err = i2c_master_transmit(s_dev, tx, chunk + 2, 200);
            if (err == ESP_OK)
                break;
            vTaskDelay(pdMS_TO_TICKS(AT24C256_WRITE_CYCLE_MS));
        }
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Error escribiendo EEPROM en 0x%04X: %s", addr, esp_err_to_name(err));
            return err;
        }

        // Esperamos el ciclo de escritura interno antes del siguiente chunk
        vTaskDelay(pdMS_TO_TICKS(AT24C256_WRITE_CYCLE_MS));

        addr += chunk;
        p += chunk;
        remaining -= chunk;
    }

    return ESP_OK;
}
