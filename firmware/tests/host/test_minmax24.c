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

    puts("PASS: min/max 24 h (vacio, validez independiente, ventana 24 h, NAN)");
    return 0;
}
