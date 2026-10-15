/* Prueba de host del min/max de 24 h (minmax24.h, 1.6.8).
 * gcc -std=c11 -Icomponents/sensors_runtime/include tests/host/test_minmax24.c -o build/t_mm24 && build/t_mm24 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "minmax24.h"

#define H (3600000LL)

int main(void)
{
    static mm24_t m;
    float pmin, pmax, fmin, fmax;

    /* Vacio: todo NAN. */
    mm24_get(&m, 0, &pmin, &pmax, &fmin, &fmax);
    assert(isnan(pmin) && isnan(pmax) && isnan(fmin) && isnan(fmax));

    /* Presion y flujo con validez independiente: un flujo invalido NO descarta la presion
     * (antes una muestra con cualquier bit invalido se descartaba entera). */
    mm24_push(&m, 1000, 300.f, true, 0.f, false);
    mm24_push(&m, 2000, 320.f, true, 5.f, true);
    mm24_get(&m, 3000, &pmin, &pmax, &fmin, &fmax);
    assert(pmin == 300.f && pmax == 320.f && fmin == 5.f && fmax == 5.f);

    /* Un minimo de hace 10 h sigue dentro de las 24 h. */
    mm24_push(&m, 10 * H, 310.f, true, 8.f, true);
    mm24_get(&m, 10 * H, &pmin, &pmax, &fmin, &fmax);
    assert(pmin == 300.f && pmax == 320.f && fmax == 8.f);

    /* A las 24 h la cubeta de la hora 0 sale de la ventana (y se recicla al escribir). */
    mm24_push(&m, 24 * H + 5, 315.f, true, 7.f, true);
    mm24_get(&m, 24 * H + 5, &pmin, &pmax, &fmin, &fmax);
    assert(pmin == 310.f && pmax == 315.f && fmin == 7.f && fmax == 8.f);

    /* Sin muestras nuevas por mas de 24 h: vuelve a NAN. */
    mm24_get(&m, 60 * H, &pmin, &pmax, &fmin, &fmax);
    assert(isnan(pmin) && isnan(fmax));

    /* NAN de entrada se ignora. */
    mm24_push(&m, 61 * H, NAN, true, NAN, true);
    mm24_get(&m, 61 * H, &pmin, &pmax, &fmin, &fmax);
    assert(isnan(pmin) && isnan(fmin));

    /* 1.6.9: guardado en NVS + reinicio = fusion de lo guardado con lo medido desde el
     * arranque (misma hora: combina; hora mas nueva: manda la mas nueva). */
    {
        static mm24_t saved, live;
        const int64_t t0 = 480000LL * H;                /* hora de reloj (epoch) */
        mm24_push(&saved, t0, 300.f, true, 2.f, true);   /* antes del corte, hora t0 */
        mm24_push(&saved, t0 + H, 310.f, true, 3.f, true);
        mm24_push(&live, t0 + H + 60000, 305.f, true, 9.f, true);   /* tras reiniciar, misma hora */
        mm24_merge(&live, &saved);
        mm24_get(&live, t0 + H + 120000, &pmin, &pmax, &fmin, &fmax);
        assert(pmin == 300.f && pmax == 310.f && fmin == 2.f && fmax == 9.f);
        /* Lo guardado de hace mas de 24 h no revive. */
        static mm24_t stale, fresh;
        mm24_push(&stale, t0, 100.f, true, 0.f, false);
        mm24_push(&fresh, t0 + 30 * H, 200.f, true, 0.f, false);
        mm24_merge(&fresh, &stale);
        mm24_get(&fresh, t0 + 30 * H, &pmin, &pmax, &fmin, &fmax);
        assert(pmin == 200.f && pmax == 200.f);
    }

    puts("PASS: min/max 24 h (vacio, validez independiente, ventana 24 h, NAN, fusion tras reinicio)");
    return 0;
}
