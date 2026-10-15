#pragma once
/* Stub de semaforos estaticos para test_flow_meter.c (un solo hilo). */
#include "FreeRTOS.h"
#define portMAX_DELAY 0xFFFFFFFFu
typedef void *SemaphoreHandle_t;
typedef struct { int dummy; } StaticSemaphore_t;
static inline SemaphoreHandle_t xSemaphoreCreateMutexStatic(StaticSemaphore_t *s) { return s; }
static inline int xSemaphoreTake(SemaphoreHandle_t s, TickType_t t) { (void)s; (void)t; return 1; }
static inline int xSemaphoreGive(SemaphoreHandle_t s) { (void)s; return 1; }
