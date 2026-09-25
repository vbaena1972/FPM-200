#pragma once
#include "driver/i2c_master.h"
void fpm_i2c_init(void);
// Two transfers separated by STOP, protected together against bus reset/interleaving.
esp_err_t fpm_i2c_stop_read(i2c_master_dev_handle_t dev, const uint8_t *addr, size_t len, uint8_t *value, int timeout);
esp_err_t fpm_i2c_transmit(i2c_master_dev_handle_t dev, const uint8_t *buf, size_t len, int timeout);
esp_err_t fpm_i2c_receive(i2c_master_dev_handle_t dev, uint8_t *buf, size_t len, int timeout);
esp_err_t fpm_i2c_transmit_receive(i2c_master_dev_handle_t dev, const uint8_t *tx, size_t txlen, uint8_t *rx, size_t rxlen, int timeout);
esp_err_t fpm_i2c_multi_buffer_transmit(i2c_master_dev_handle_t dev, i2c_master_transmit_multi_buffer_info_t *buffers, size_t count, int timeout);
esp_err_t fpm_i2c_bus_reset(i2c_master_bus_handle_t bus);
esp_err_t fpm_i2c_bus_rm_device(i2c_master_dev_handle_t dev);
esp_err_t fpm_i2c_bus_add_device(i2c_master_bus_handle_t bus, const i2c_device_config_t *cfg, i2c_master_dev_handle_t *dev);

#ifndef FPM_I2C_IMPLEMENTATION
#define i2c_master_transmit fpm_i2c_transmit
#define i2c_master_receive fpm_i2c_receive
#define i2c_master_transmit_receive fpm_i2c_transmit_receive
#define i2c_master_multi_buffer_transmit fpm_i2c_multi_buffer_transmit
#define i2c_master_bus_reset fpm_i2c_bus_reset
#define i2c_master_bus_rm_device fpm_i2c_bus_rm_device
#define i2c_master_bus_add_device fpm_i2c_bus_add_device
#endif
