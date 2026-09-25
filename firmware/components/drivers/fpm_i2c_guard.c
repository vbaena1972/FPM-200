#define FPM_I2C_IMPLEMENTATION
#include "fpm_i2c_guard.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
static StaticSemaphore_t storage;
static SemaphoreHandle_t gate;
void fpm_i2c_init(void) { if (!gate) gate = xSemaphoreCreateMutexStatic(&storage); }
esp_err_t fpm_i2c_transmit(i2c_master_dev_handle_t dev, const uint8_t *buf, size_t len, int timeout) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit(dev, buf, len, timeout);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_receive(i2c_master_dev_handle_t dev, uint8_t *buf, size_t len, int timeout) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_receive(dev, buf, len, timeout);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_transmit_receive(i2c_master_dev_handle_t dev, const uint8_t *tx, size_t txlen, uint8_t *rx, size_t rxlen, int timeout) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit_receive(dev, tx, txlen, rx, rxlen, timeout);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_multi_buffer_transmit(i2c_master_dev_handle_t dev, i2c_master_transmit_multi_buffer_info_t *buffers, size_t count, int timeout) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_multi_buffer_transmit(dev, buffers, count, timeout);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_bus_reset(i2c_master_bus_handle_t bus) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_reset(bus);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_bus_rm_device(i2c_master_dev_handle_t dev) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_rm_device(dev);
    xSemaphoreGive(gate);
    return err;
}
esp_err_t fpm_i2c_bus_add_device(i2c_master_bus_handle_t bus, const i2c_device_config_t *cfg, i2c_master_dev_handle_t *dev) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_add_device(bus, cfg, dev);
    xSemaphoreGive(gate);
    return err;
}

esp_err_t fpm_i2c_stop_read(i2c_master_dev_handle_t dev, const uint8_t *addr, size_t len, uint8_t *value, int timeout) {
    if (!gate || xSemaphoreTake(gate, pdMS_TO_TICKS(100)) != pdTRUE) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit(dev, addr, len, timeout);
    if (err == ESP_OK) err = i2c_master_receive(dev, value, 1, timeout);
    xSemaphoreGive(gate);
    return err;
}
