#pragma once
#include "FreeRTOS.h"
#define pdTRUE 1
typedef void *SemaphoreHandle_t;
static inline SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void) {return (void*)1;}
static inline int xSemaphoreTakeRecursive(SemaphoreHandle_t s, TickType_t t) {return 1;}
static inline void xSemaphoreGiveRecursive(SemaphoreHandle_t s) {}
