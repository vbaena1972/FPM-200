#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
typedef struct {
    double volume_m3;
    uint64_t missing_us;
    int64_t previous_us;
    float previous_lpm;
    bool have_previous, previous_valid;
} flow_integrator_t;
// Integrate between actual read-completion timestamps, not RTOS ticks/UI frames.
// A missing endpoint or a gap >1 s makes the entire interval unmeasured.
static inline void flow_integrator_push(flow_integrator_t *m, int64_t us,
                                        float lpm, bool valid) {
    valid = valid && isfinite(lpm) && lpm >= 0;
    if (m->have_previous && us <= m->previous_us) return;
    if (m->have_previous) {
        int64_t dt = us - m->previous_us;
        if (valid && m->previous_valid && dt <= 1000000)
            m->volume_m3 += ((double)m->previous_lpm + lpm) * 0.5 * (double)dt / 60000000000.0;
        else m->missing_us += (uint64_t)dt;
    }
    m->previous_us = us;
    m->previous_lpm = lpm;
    m->previous_valid = valid;
    m->have_previous = true;
}
