#include <assert.h>
#include <stdio.h>
#include "alarm_mgr.h"
#include "storage.h"
#include "driver/ledc.h"
static int64_t now;
static uint32_t duty;
static unsigned transitions;
static AppConfig cfg;
int64_t esp_timer_get_time(void) {return now;}
int appcfg_cache_get(AppConfig *out) {*out=cfg; return ESP_OK;}
AppConfig *appcfg_cache_peek(void) {return &cfg;}
int ledc_timer_config(const ledc_timer_config_t *c) {return ESP_OK;}
int ledc_channel_config(const ledc_channel_config_t *c) {return ESP_OK;}
int ledc_set_duty(int m,int c,uint32_t d) {duty=d;return ESP_OK;}
int ledc_update_duty(int m,int c) {return ESP_OK;}
static void changed(alarm_clinical_state_t s) {++transitions;}
int main(void) {
 cfg.general.alarm.volume=80; cfg.general.alarm.tone_alert=true; cfg.general.alarm.tone_warn=true;
 cfg.general.alarm.max_silence_minutes=1; cfg.general.alarm.reannounce_minutes=1;
 cfg.sensors.alarm_limits.pressure_max_enabled=true; cfg.sensors.alarm_limits.pressure_max=100;
 cfg.sensors.alarm_limits.flow_delta_window_ms=1000;
 assert(alarm_mgr_init(36)==ESP_OK); alarm_mgr_set_state_change_cb(changed);
 now=500000; assert(alarm_mgr_process(0,0,8)); assert(duty>0);
 assert(alarm_mgr_get_current_state()==ALARM_STATE_WARNING);
 now=1000000; alarm_mgr_process(0,0,8); assert(duty==0);
 now=1500000; alarm_mgr_process(0,0,8); assert(duty>0);
 alarm_mgr_press_mute(); assert(duty==0 && alarm_mgr_is_muted());
 // Escalation must break a technical warning's mute.
 now=1700000; alarm_mgr_process(130,0,0); assert(!alarm_mgr_is_muted());
 assert(alarm_mgr_get_current_state()==ALARM_STATE_ALERT);
 now=1850000; alarm_mgr_process(130,0,0); assert(duty==0);
 now=2000000; alarm_mgr_process(130,0,0); assert(duty>0);
 alarm_mgr_set_audio_inhibit_noncritical(true); assert(duty>0);
 now=2050000; alarm_mgr_process(0,0,8); assert(duty==0);
 alarm_mgr_set_audio_inhibit_noncritical(false);
 now=2600000; alarm_mgr_process(0,0,8); assert(duty>0);
 now=2650000; alarm_mgr_process(0,0,0); assert(duty==0);
 alarm_mgr_test_buzzer(); now=2700000; alarm_mgr_process(0,0,0); assert(duty>0);
 now=4700000; alarm_mgr_process(0,0,0); assert(duty==0);
 assert(transitions>=4);

 // 1.5.23 flow confirmation: a 200 ms Wi-Fi-noise spike must not raise a leak
 // alarm; a sustained step must, within ~0.5 s, and must clear after ~a window.
 cfg.sensors.alarm_limits.flow_delta_enabled=true; cfg.sensors.alarm_limits.flow_delta_threshold=5;
 cfg.sensors.alarm_limits.flow_delta_window_ms=2000;
 now=10000000; alarm_mgr_process(0,0,0);                 // baseline 0 L/min
 for (int i=1;i<=2;i++){ now=10000000+i*100000; alarm_mgr_process(0,12,0);
   assert(alarm_mgr_get_current_state()==ALARM_STATE_NORMAL); }
 now=10300000; alarm_mgr_process(0,0,0); assert(alarm_mgr_get_current_state()==ALARM_STATE_NORMAL);
 for (int i=1;i<=4;i++){ now=10300000+i*100000; alarm_mgr_process(0,12,0);
   assert(alarm_mgr_get_current_state()==ALARM_STATE_NORMAL); }   // < 500 ms: pending
 now=10900000; alarm_mgr_process(0,12,0); assert(alarm_mgr_get_current_state()==ALARM_STATE_WARNING);
 now=15000000; alarm_mgr_process(0,12,0); assert(alarm_mgr_get_current_state()==ALARM_STATE_NORMAL);
 puts("PASS: alarm cadence, mute/escalation, UI inhibit, normal reset, bounded buzzer test, flow confirmation");
}
