#define FPM_I2C_IMPLEMENTATION
#include "fpm_i2c_guard.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
static StaticSemaphore_t storage;
static SemaphoreHandle_t gate;
void fpm_i2c_init(void) { if (!gate) gate = xSemaphoreCreateMutexStatic(&storage); }

// Diagnostics (1.5.22): bus 1 stalls ~0.7 s during the AWS TLS handshake. Log any
// guarded operation whose gate wait or transfer exceeds FPM_I2C_SLOW_MS, with the
// calling task, so the next log shows who blocks the bus and why.
#define FPM_I2C_SLOW_MS 80
static const char *TAG = "i2c_guard";

static bool gate_take(int64_t *t0, int64_t *t1)
{
    *t0 = esp_timer_get_time();
    bool ok = gate && xSemaphoreTake(gate, pdMS_TO_TICKS(100)) == pdTRUE;
    *t1 = esp_timer_get_time();
    if (!ok)
        ESP_LOGW(TAG, "gate timeout after %lld ms (task %s)", (long long)((*t1 - *t0) / 1000),
                 pcTaskGetName(NULL));
    return ok;
}

static void gate_give(const char *op, int64_t t0, int64_t t1, esp_err_t err)
{
    int64_t t2 = esp_timer_get_time();
    xSemaphoreGive(gate);
    int64_t wait_ms = (t1 - t0) / 1000, xfer_ms = (t2 - t1) / 1000;
    if (wait_ms > FPM_I2C_SLOW_MS || xfer_ms > FPM_I2C_SLOW_MS)
        ESP_LOGW(TAG, "slow %s: wait=%lld ms xfer=%lld ms err=%s task=%s", op,
                 (long long)wait_ms, (long long)xfer_ms, esp_err_to_name(err), pcTaskGetName(NULL));
}

esp_err_t fpm_i2c_transmit(i2c_master_dev_handle_t dev, const uint8_t *buf, size_t len, int timeout) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit(dev, buf, len, timeout);
    gate_give("tx", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_receive(i2c_master_dev_handle_t dev, uint8_t *buf, size_t len, int timeout) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_receive(dev, buf, len, timeout);
    gate_give("rx", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_transmit_receive(i2c_master_dev_handle_t dev, const uint8_t *tx, size_t txlen, uint8_t *rx, size_t rxlen, int timeout) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit_receive(dev, tx, txlen, rx, rxlen, timeout);
    gate_give("txrx", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_multi_buffer_transmit(i2c_master_dev_handle_t dev, i2c_master_transmit_multi_buffer_info_t *buffers, size_t count, int timeout) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_multi_buffer_transmit(dev, buffers, count, timeout);
    gate_give("multi_tx", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_bus_reset(i2c_master_bus_handle_t bus) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_reset(bus);
    gate_give("bus_reset", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_bus_rm_device(i2c_master_dev_handle_t dev) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_rm_device(dev);
    gate_give("rm_device", t0, t1, err);
    return err;
}
esp_err_t fpm_i2c_bus_add_device(i2c_master_bus_handle_t bus, const i2c_device_config_t *cfg, i2c_master_dev_handle_t *dev) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_bus_add_device(bus, cfg, dev);
    gate_give("add_device", t0, t1, err);
    return err;
}

esp_err_t fpm_i2c_stop_read(i2c_master_dev_handle_t dev, const uint8_t *addr, size_t len, uint8_t *value, int timeout) {
    int64_t t0, t1;
    if (!gate_take(&t0, &t1)) return ESP_ERR_TIMEOUT;
    esp_err_t err = i2c_master_transmit(dev, addr, len, timeout);
    if (err == ESP_OK) err = i2c_master_receive(dev, value, 1, timeout);
    gate_give("stop_read", t0, t1, err);
    return err;
}
