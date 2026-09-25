#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Round up to whole ticks and add one to guarantee the requested minimum wall
// time even when called just before a tick boundary (1 ms extra at 1000 Hz).
static inline void driver_delay_ms(unsigned ms) {
    vTaskDelay((TickType_t)(((uint64_t)ms * configTICK_RATE_HZ + 999) / 1000) + 1);
}
