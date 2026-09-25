#include "fpm_i2c_guard.h"
#include "driver_delay.h"
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

    // Same 100 kHz byte protocol validated on this board in the standalone test.
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = 100000,
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
    err = fpm_i2c_stop_read(s_dev, addr_bytes, 2, &probe, 100);
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
    if (mem_addr >= AT24C256_SIZE_BYTES || len > AT24C256_SIZE_BYTES - mem_addr)
        return ESP_ERR_INVALID_SIZE;

    for (size_t i = 0; i < len; ++i) {
        uint16_t addr = mem_addr + i;
        uint8_t address[2] = {(uint8_t)(addr >> 8), (uint8_t)addr};
        esp_err_t err = ESP_FAIL;
        for (unsigned attempt = 0; attempt < 3; ++attempt) {
            err = fpm_i2c_stop_read(s_dev, address, 2, buf + i, 100);
            if (err == ESP_OK) break;
            driver_delay_ms(AT24C256_WRITE_CYCLE_MS);
        }
        if (err != ESP_OK) return err;
        // Hardware-tested pacing, including the last byte for back-to-back calls.
        // stop_read has already released the bus guard before this sleep.
        // Validated as vTaskDelay(1) at 100 Hz (~10 ms); expressed in ms so it
        // stays ~10 ms at any tick rate (1000 Hz since 1.5.19).
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return ESP_OK;
}

// Verify even after NACK: the device may have accepted a partial write.
// Keep retries bounded, and distinguish transport failure from readback mismatch.
static esp_err_t write_verified(uint16_t addr, const uint8_t *data, size_t len)
{
    uint8_t tx[18], verify[16];
    tx[0] = addr >> 8;
    tx[1] = addr & 0xff;
    memcpy(tx + 2, data, len);
    esp_err_t err = ESP_FAIL;
    for (unsigned attempt = 0; attempt < 3; ++attempt) {
        esp_err_t sent = i2c_master_transmit(s_dev, tx, len + 2, 100);
        driver_delay_ms(AT24C256_WRITE_CYCLE_MS);
        esp_err_t read = at24c256_read(addr, verify, len);
        if (read == ESP_OK && memcmp(verify, data, len) == 0) return ESP_OK;
        err = sent != ESP_OK ? sent : (read != ESP_OK ? read : ESP_ERR_INVALID_RESPONSE);
        ESP_LOGW(TAG, "EEPROM addr=0x%04X bytes=%u attempt=%u tx=%s read=%s match=0",
                 addr, (unsigned)len, attempt + 1, esp_err_to_name(sent), esp_err_to_name(read));
    }
    return err;
}

esp_err_t at24c256_write(uint16_t mem_addr, const uint8_t *buf, size_t len)
{
    if (!s_dev || !s_ready) return ESP_ERR_INVALID_STATE;
    if (!buf || !len) return ESP_ERR_INVALID_ARG;
    if (mem_addr >= AT24C256_SIZE_BYTES || len > AT24C256_SIZE_BYTES - mem_addr)
        return ESP_ERR_INVALID_SIZE;

    for (size_t i = 0; i < len; ++i) {
        esp_err_t err = write_verified(mem_addr + i, buf + i, 1);
        if (err != ESP_OK) return err;
    }
    // Re-read the entire requested range twice, after all writes have finished.
    // This catches corruption of earlier bytes by subsequent writes.
    uint8_t verify[64];
    for (unsigned pass = 0; pass < 2; ++pass) {
        for (size_t off = 0; off < len; off += sizeof(verify)) {
            size_t count = len - off;
            if (count > sizeof(verify)) count = sizeof(verify);
            esp_err_t err = at24c256_read(mem_addr + off, verify, count);
            if (err != ESP_OK) return err;
            if (memcmp(verify, buf + off, count)) {
                ESP_LOGE(TAG, "EEPROM final verification failed at 0x%04X", (unsigned)(mem_addr + off));
                return ESP_ERR_INVALID_RESPONSE;
            }
        }
    }
    return ESP_OK;
}

// Reserved scratch area outside calibration. Run before acquisition starts.
// Power loss can interrupt restoration, so never use calibration as scratch.
esp_err_t at24c256_self_test(void)
{
    enum { BASE = 0x0130, SIZE = 64 };
    uint8_t saved[SIZE], check[SIZE], pattern[SIZE];
    esp_err_t result = at24c256_read(BASE, saved, SIZE);
    if (result != ESP_OK) return result;
    // Confirm the backup using independent byte reads before any mutation.
    for (unsigned i = 0; i < SIZE; ++i) {
        result = at24c256_read(BASE + i, check + i, 1);
        if (result != ESP_OK) return result;
    }
    if (memcmp(saved, check, SIZE)) {
        ESP_LOGE(TAG, "EEPROM_TEST backup unstable: block/byte disagree; no writes performed");
        return ESP_ERR_INVALID_RESPONSE;
    }
    ESP_LOGI(TAG, "EEPROM_TEST begin scratch=0x0130..0x016f (crosses page boundary)");
    for (unsigned pass = 0; pass < 4; ++pass) {
        for (unsigned i = 0; i < SIZE; ++i)
            pattern[i] = pass == 0 ? 0x00 : pass == 1 ? 0xff :
                         pass == 2 ? (i & 1 ? 0x55 : 0xaa) : (uint8_t)(i ^ 0x69);
        result = at24c256_write(BASE, pattern, SIZE);
        if (result != ESP_OK) break;
        // Immediate and delayed full-block reads expose corruption hidden by
        // the short readbacks performed in the write driver.
        for (unsigned repeat = 0; repeat < 3; ++repeat) {
            driver_delay_ms(20);
            result = at24c256_read(BASE, check, SIZE);
            if (result != ESP_OK) break;
            for (unsigned i = 0; i < SIZE; ++i) {
                if (check[i] != pattern[i]) {
                    ESP_LOGE(TAG, "EEPROM_TEST mismatch pass=%u read=%u addr=0x%04X expected=%02X got=%02X",
                             pass, repeat, BASE + i, pattern[i], check[i]);
                    result = ESP_ERR_INVALID_RESPONSE;
                    break;
                }
            }
            if (result != ESP_OK) break;
        }
        ESP_LOGI(TAG, "EEPROM_TEST pattern=%u result=%s", pass, esp_err_to_name(result));
        if (result != ESP_OK) break;
    }
    // Always attempt restoration after the first write, including failures.
    esp_err_t restored = at24c256_write(BASE, saved, SIZE);
    if (restored == ESP_OK) {
        for (unsigned i = 0; i < SIZE; ++i) {
            restored = at24c256_read(BASE + i, check + i, 1);
            if (restored != ESP_OK) break;
        }
        if (restored == ESP_OK && memcmp(saved, check, SIZE)) restored = ESP_ERR_INVALID_RESPONSE;
    }
    ESP_LOGI(TAG, "EEPROM_TEST %s restore=%s", result == ESP_OK ? "PASS" : "FAIL", esp_err_to_name(restored));
    return restored != ESP_OK ? restored : result;
}
