#pragma once
#include <stdbool.h>
#include "esp_err.h"
typedef struct {
 struct {struct {int volume,max_silence_minutes,reannounce_minutes; bool tone_alert,tone_warn;} alarm;} general;
 struct {struct {bool pressure_min_enabled,pressure_max_enabled,flow_high_enabled,flow_delta_enabled; float pressure_min,pressure_max,flow_delta_threshold,flow_high_limit; int flow_delta_window_ms;} alarm_limits;} sensors;
} AppConfig;
int appcfg_cache_get(AppConfig *out);
AppConfig *appcfg_cache_peek(void);
