#pragma once
#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"
typedef void *i2c_master_dev_handle_t;
typedef void *i2c_master_bus_handle_t;
#define I2C_ADDR_BIT_LEN_7 0
typedef struct { int dev_addr_length; unsigned device_address, scl_speed_hz; } i2c_device_config_t;
typedef struct { const uint8_t *buffer; size_t buffer_size; } i2c_master_transmit_multi_buffer_info_t;
