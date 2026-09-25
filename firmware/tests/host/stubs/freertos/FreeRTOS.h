#pragma once
#include <stdint.h>
typedef uint32_t TickType_t;
#define pdMS_TO_TICKS(ms) ((ms)*100/1000)

#define configTICK_RATE_HZ 100
