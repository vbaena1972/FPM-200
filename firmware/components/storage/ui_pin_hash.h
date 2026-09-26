/*
 * ui_pin_hash.h — PIN/passphrase guardados como hash con sal (PBKDF2-HMAC-SHA256).
 *
 * COPIA IDENTICA de MedGuard-12IoT/firmware/ui/ui_pin_hash.h (proyectos hermanos:
 * mismo formato "p1$" en ambos). Si se cambia aqui, cambiarlo alla.
 *
 * Formato almacenado (en ui_user_t.pin, ver UI_PIN_STORE_LEN):
 *     "p1$<sal 16 hex>$<dk 32 hex>"     p1 = PBKDF2-SHA256, 4096 iteraciones,
 *                                        sal de 8 bytes, clave derivada de 16 bytes
 * Un valor SIN el prefijo "p1$" es un PIN heredado en texto plano: se sigue
 * aceptando al validar y se convierte a hash la primera vez que se aplica la
 * config (migracion automatica, ver ui_app_config_apply).
 *
 * 100 % software y portable (sin mbedtls, sin eFuses): compila igual en el
 * firmware y en los simuladores de PC. Es REVERSIBLE: no toca el chip.
 *
 * Alcance honesto: impide LEER el PIN desde un volcado de NVS / respaldo de
 * microSD / config exportada. Un PIN de 4 digitos sigue siendo adivinable por
 * fuerza bruta offline (10^4 intentos) si alguien extrae el hash; la proteccion
 * completa del almacenamiento es el cifrado de flash de produccion (ver
 * docs/SEGURIDAD_PRODUCCION.md). La passphrase del fabricante (>= 8
 * alfanumericos) si queda bien protegida.
 */
#ifndef UI_PIN_HASH_H
#define UI_PIN_HASH_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UI_PIN_HASH_PREFIX   "p1$"
#define UI_PIN_HASH_ITER     4096
#define UI_PIN_STORE_LEN     72     /* tamano de ui_user_t.pin (hash + NUL con holgura) */

/* true si `stored` ya es un hash "p1$...". Header-only a proposito: app_config.c
 * lo usa para validar sin enlazar el modulo (pruebas en host). */
static inline bool ui_pin_is_hashed(const char *stored)
{
    return stored && strncmp(stored, UI_PIN_HASH_PREFIX, 3) == 0;
}

/* Deriva el hash de `plain` con sal aleatoria nueva y lo escribe en `out`.
 * Devuelve false si `out_size` no alcanza o `plain` es vacio. */
bool ui_pin_hash(const char *plain, char *out, size_t out_size);

/* Compara `plain` contra lo almacenado (hash o texto plano heredado). Tiempo
 * constante en la comparacion del hash. */
bool ui_pin_verify(const char *stored, const char *plain);

/* Si `stored` es texto plano no vacio, lo reemplaza en sitio por su hash.
 * Devuelve true si hubo conversion (hay que persistir). */
bool ui_pin_normalize(char *stored, size_t size);

/* Primitiva expuesta para las pruebas con vectores conocidos (RFC 7914 §11). */
void ui_pbkdf2_sha256(const unsigned char *pass, size_t pass_len,
                      const unsigned char *salt, size_t salt_len,
                      unsigned iterations, unsigned char *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* UI_PIN_HASH_H */
