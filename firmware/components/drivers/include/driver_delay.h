#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// At 100 Hz a tick is 10 ms. Add one tick to guarantee the requested
// minimum wall time even when called immediately before a tick boundary.
static inline void driver_delay_ms(unsigned ms) {
    vTaskDelay((TickType_t)(((uint64_t)ms * configTICK_RATE_HZ + 999) / 1000) + 1);
}
