#pragma once
/* Minimo/maximo de las ultimas 24 h (1.6.8) para la pildora "24H" del dashboard.
 * Antes la pildora usaba sensors_runtime_get_min_max(60000): el buffer de muestras
 * solo cubre ~1 min y una muestra con CUALQUIER bit invalido (p. ej. flujo
 * calentando) se descartaba entera -> la "24H" era en realidad el ultimo minuto.
 * Aqui: 24 cubetas de 1 hora, presion y flujo con validez independiente. Ventana
 * efectiva 23-24 h. ~0,8 KB. 1.6.9: las horas son de RELOJ (epoch, no tiempo de
 * arranque) y flow_meter guarda las cubetas en NVS cada 10 min, asi la "24H"
 * sobrevive a reinicios y cortes (antes se perdia al reiniciar).
 * Sin dependencias de ESP-IDF (prueba de host: tests/host/test_minmax24.c). */
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define MM24_SLOTS   24
#define MM24_SLOT_MS 3600000LL

typedef struct {
    int64_t hour_plus1;            /* hora (ts/1h) + 1 de la cubeta; 0 = vacia */
    float pmin, pmax, fmin, fmax;
    bool p_ok, f_ok;
} mm24_slot_t;

typedef struct { mm24_slot_t s[MM24_SLOTS]; } mm24_t;

static inline void mm24_push(mm24_t *m, int64_t ts_ms, float p, bool p_ok, float f, bool f_ok)
{
    if (ts_ms < 0) return;
    int64_t h = ts_ms / MM24_SLOT_MS;
    mm24_slot_t *s = &m->s[h % MM24_SLOTS];
    if (s->hour_plus1 != h + 1) {        /* cubeta de hace 24 h (o vacia): reiniciar */
        s->hour_plus1 = h + 1;
        s->p_ok = s->f_ok = false;
    }
    if (p_ok && isfinite(p)) {
        if (!s->p_ok) { s->pmin = s->pmax = p; s->p_ok = true; }
        else { if (p < s->pmin) s->pmin = p; if (p > s->pmax) s->pmax = p; }
    }
    if (f_ok && isfinite(f)) {
        if (!s->f_ok) { s->fmin = s->fmax = f; s->f_ok = true; }
        else { if (f < s->fmin) s->fmin = f; if (f > s->fmax) s->fmax = f; }
    }
}

/* 1.6.9: une lo guardado en NVS (src) con lo medido desde el arranque (dst). Misma
 * hora: se combinan min/max; cubeta vacia o de una hora anterior: se toma la de src. */
static inline void mm24_merge(mm24_t *dst, const mm24_t *src)
{
    for (int i = 0; i < MM24_SLOTS; i++) {
        const mm24_slot_t *s = &src->s[i];
        mm24_slot_t *d = &dst->s[i];
        if (!s->hour_plus1) continue;
        if (d->hour_plus1 < s->hour_plus1) { *d = *s; continue; }
        if (d->hour_plus1 != s->hour_plus1) continue;      /* dst mas nueva: manda dst */
        if (s->p_ok) {
            if (!d->p_ok) { d->pmin = s->pmin; d->pmax = s->pmax; d->p_ok = true; }
            else { if (s->pmin < d->pmin) d->pmin = s->pmin; if (s->pmax > d->pmax) d->pmax = s->pmax; }
        }
        if (s->f_ok) {
            if (!d->f_ok) { d->fmin = s->fmin; d->fmax = s->fmax; d->f_ok = true; }
            else { if (s->fmin < d->fmin) d->fmin = s->fmin; if (s->fmax > d->fmax) d->fmax = s->fmax; }
        }
    }
}

/* Min/max de las cubetas de las ultimas 24 h; NAN donde no hubo dato valido. */
static inline void mm24_get(const mm24_t *m, int64_t now_ms,
                            float *pmin, float *pmax, float *fmin, float *fmax)
{
    float a = NAN, b = NAN, c = NAN, d = NAN;
    int64_t h = now_ms / MM24_SLOT_MS;
    for (int i = 0; i < MM24_SLOTS; i++) {
        const mm24_slot_t *s = &m->s[i];
        int64_t sh = s->hour_plus1 - 1;
        if (!s->hour_plus1 || sh > h || sh <= h - MM24_SLOTS) continue;
        if (s->p_ok) {
            if (!(a <= s->pmin)) a = s->pmin;    /* !(a<=x) tambien vale con a=NAN */
            if (!(b >= s->pmax)) b = s->pmax;
        }
        if (s->f_ok) {
            if (!(c <= s->fmin)) c = s->fmin;
            if (!(d >= s->fmax)) d = s->fmax;
        }
    }
    *pmin = a; *pmax = b; *fmin = c; *fmax = d;
}
