#pragma once
#include <stdint.h>
#include "esp_err.h"
#define LEDC_LOW_SPEED_MODE 0
#define LEDC_CHANNEL_0 0
#define LEDC_TIMER_0 0
#define LEDC_TIMER_10_BIT 10
#define LEDC_AUTO_CLK 0
#define LEDC_INTR_DISABLE 0
typedef struct {int speed_mode,timer_num,duty_resolution,freq_hz,clk_cfg;} ledc_timer_config_t;
typedef struct {int speed_mode,channel,timer_sel,intr_type,gpio_num,duty,hpoint;} ledc_channel_config_t;
int ledc_timer_config(const ledc_timer_config_t *c);
int ledc_channel_config(const ledc_channel_config_t *c);
int ledc_set_duty(int m,int c,uint32_t duty);
int ledc_update_duty(int m,int c);
