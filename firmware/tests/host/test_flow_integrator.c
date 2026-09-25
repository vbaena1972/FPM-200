#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "flow_integrator.h"
static void close_to(double a, double b) { assert(fabs(a-b)<1e-12); }
int main(void) {
    flow_integrator_t m = {0};
    for (int i=0;i<=60;++i) flow_integrator_push(&m,i*100000LL,10,true);
    close_to(m.volume_m3,0.001); // 10 L/min x 6 s = 1 litre
    assert(m.missing_us==0);
    // Irregular sampling still integrates a linear ramp correctly.
    memset(&m,0,sizeof(m));
    flow_integrator_push(&m,0,0,true);
    flow_integrator_push(&m,100000,1,true);
    flow_integrator_push(&m,370000,3.7f,true);
    flow_integrator_push(&m,1000000,10,true);
    assert(fabs(m.volume_m3-5.0/60000.0)<1e-11);
    double before=m.volume_m3;
    flow_integrator_push(&m,1000000,100,true); // duplicate ignored
    flow_integrator_push(&m,900000,100,true); // backward stamp ignored
    close_to(m.volume_m3,before);
    flow_integrator_push(&m,1100000,100,false);
    flow_integrator_push(&m,1200000,10,true);
    close_to(m.volume_m3,before); // do not bridge either missing endpoint
    assert(m.missing_us==200000);
    flow_integrator_push(&m,1300000,10,true);
    close_to(m.volume_m3,before+1.0/60000.0);
    before=m.volume_m3;
    flow_integrator_push(&m,3300000,10,true); // long gap excluded
    close_to(m.volume_m3,before);
    flow_integrator_push(&m,3400000,NAN,true);
    flow_integrator_push(&m,3500000,10,true);
    close_to(m.volume_m3,before);
    assert(m.missing_us==2400000);
    puts("PASS: volume units, real timestamps, ramp, duplicates, gaps and invalid samples");
}
