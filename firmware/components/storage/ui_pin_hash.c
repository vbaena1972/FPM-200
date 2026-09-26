/*
 * ui_pin_hash.c — ver ui_pin_hash.h. SHA-256 (FIPS 180-4) + HMAC + PBKDF2
 * compactos y portables, para no depender de mbedtls (el simulador no lo tiene).
 */
#include "ui_pin_hash.h"

#include <stdint.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "esp_random.h"
#else
#include <stdlib.h>
#include <time.h>
#endif

#define SALT_LEN 8
#define DK_LEN   16

/* ------------------------------------------------------------ SHA-256 ---- */

typedef struct {
    uint32_t h[8];
    uint64_t len;          /* bytes procesados */
    unsigned char buf[64];
    size_t used;
} sha256_ctx_t;

static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

#define ROR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

static void sha256_block(sha256_ctx_t *c, const unsigned char *p)
{
    uint32_t w[64];
    for (int i = 0; i < 16; ++i)
        w[i] = ((uint32_t)p[4 * i] << 24) | ((uint32_t)p[4 * i + 1] << 16) |
               ((uint32_t)p[4 * i + 2] << 8) | (uint32_t)p[4 * i + 3];
    for (int i = 16; i < 64; ++i) {
        uint32_t s0 = ROR(w[i - 15], 7) ^ ROR(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = ROR(w[i - 2], 17) ^ ROR(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    uint32_t a = c->h[0], b = c->h[1], cc = c->h[2], d = c->h[3];
    uint32_t e = c->h[4], f = c->h[5], g = c->h[6], h = c->h[7];
    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + (ROR(e, 6) ^ ROR(e, 11) ^ ROR(e, 25)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
        uint32_t t2 = (ROR(a, 2) ^ ROR(a, 13) ^ ROR(a, 22)) + ((a & b) ^ (a & cc) ^ (b & cc));
        h = g; g = f; f = e; e = d + t1; d = cc; cc = b; b = a; a = t1 + t2;
    }
    c->h[0] += a; c->h[1] += b; c->h[2] += cc; c->h[3] += d;
    c->h[4] += e; c->h[5] += f; c->h[6] += g; c->h[7] += h;
}

static void sha256_init(sha256_ctx_t *c)
{
    static const uint32_t iv[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
    };
    memcpy(c->h, iv, sizeof iv);
    c->len = 0;
    c->used = 0;
}

static void sha256_update(sha256_ctx_t *c, const unsigned char *p, size_t n)
{
    c->len += n;
    while (n) {
        size_t take = 64 - c->used;
        if (take > n) take = n;
        memcpy(c->buf + c->used, p, take);
        c->used += take;
        p += take;
        n -= take;
        if (c->used == 64) {
            sha256_block(c, c->buf);
            c->used = 0;
        }
    }
}

static void sha256_final(sha256_ctx_t *c, unsigned char out[32])
{
    uint64_t bits = c->len * 8U;
    unsigned char pad = 0x80;
    sha256_update(c, &pad, 1);
    pad = 0;
    while (c->used != 56) sha256_update(c, &pad, 1);
    unsigned char lenb[8];
    for (int i = 0; i < 8; ++i) lenb[i] = (unsigned char)(bits >> (56 - 8 * i));
    sha256_update(c, lenb, 8);
    for (int i = 0; i < 8; ++i) {
        out[4 * i] = (unsigned char)(c->h[i] >> 24);
        out[4 * i + 1] = (unsigned char)(c->h[i] >> 16);
        out[4 * i + 2] = (unsigned char)(c->h[i] >> 8);
        out[4 * i + 3] = (unsigned char)c->h[i];
    }
}

/* --------------------------------------------------------- HMAC/PBKDF2 ---- */

/* Estados internos/externos precalculados: cada iteracion de PBKDF2 cuesta
 * 2 bloques SHA-256 en lugar de 4. */
typedef struct { sha256_ctx_t inner, outer; } hmac_key_t;

static void hmac_key_init(hmac_key_t *k, const unsigned char *key, size_t key_len)
{
    unsigned char block[64] = {0};
    if (key_len > 64) {
        sha256_ctx_t t;
        sha256_init(&t);
        sha256_update(&t, key, key_len);
        sha256_final(&t, block);
    } else {
        memcpy(block, key, key_len);
    }
    unsigned char ipad[64], opad[64];
    for (int i = 0; i < 64; ++i) {
        ipad[i] = block[i] ^ 0x36;
        opad[i] = block[i] ^ 0x5c;
    }
    sha256_init(&k->inner);
    sha256_update(&k->inner, ipad, 64);
    sha256_init(&k->outer);
    sha256_update(&k->outer, opad, 64);
}

static void hmac(const hmac_key_t *k, const unsigned char *msg, size_t n,
                 unsigned char out[32])
{
    sha256_ctx_t c = k->inner;
    unsigned char ih[32];
    sha256_update(&c, msg, n);
    sha256_final(&c, ih);
    c = k->outer;
    sha256_update(&c, ih, 32);
    sha256_final(&c, out);
}

void ui_pbkdf2_sha256(const unsigned char *pass, size_t pass_len,
                      const unsigned char *salt, size_t salt_len,
                      unsigned iterations, unsigned char *out, size_t out_len)
{
    hmac_key_t key;
    hmac_key_init(&key, pass, pass_len);
    uint32_t block_index = 1;
    while (out_len) {
        /* U1 = HMAC(P, S || INT(i)) */
        sha256_ctx_t c = key.inner;
        unsigned char be[4] = {
            (unsigned char)(block_index >> 24), (unsigned char)(block_index >> 16),
            (unsigned char)(block_index >> 8), (unsigned char)block_index,
        };
        unsigned char u[32], t[32], ih[32];
        sha256_update(&c, salt, salt_len);
        sha256_update(&c, be, 4);
        sha256_final(&c, ih);
        c = key.outer;
        sha256_update(&c, ih, 32);
        sha256_final(&c, u);
        memcpy(t, u, 32);
        for (unsigned it = 1; it < iterations; ++it) {
            hmac(&key, u, 32, u);
            for (int j = 0; j < 32; ++j) t[j] ^= u[j];
        }
        size_t take = out_len < 32 ? out_len : 32;
        memcpy(out, t, take);
        out += take;
        out_len -= take;
        ++block_index;
    }
}

/* ------------------------------------------------------------- formato ---- */

static void to_hex(const unsigned char *in, size_t n, char *out)
{
    static const char hx[] = "0123456789abcdef";
    for (size_t i = 0; i < n; ++i) {
        out[2 * i] = hx[in[i] >> 4];
        out[2 * i + 1] = hx[in[i] & 0x0F];
    }
    out[2 * n] = '\0';
}

static bool from_hex(const char *in, size_t n, unsigned char *out)
{
    for (size_t i = 0; i < n; ++i) {
        unsigned v = 0;
        for (int k = 0; k < 2; ++k) {
            char ch = in[2 * i + k];
            v <<= 4;
            if (ch >= '0' && ch <= '9') v |= (unsigned)(ch - '0');
            else if (ch >= 'a' && ch <= 'f') v |= (unsigned)(ch - 'a' + 10);
            else return false;
        }
        out[i] = (unsigned char)v;
    }
    return true;
}

static void random_salt(unsigned char *salt, size_t n)
{
#ifdef ESP_PLATFORM
    esp_fill_random(salt, n);
#else
    static bool seeded;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = true; }
    for (size_t i = 0; i < n; ++i) salt[i] = (unsigned char)(rand() & 0xFF);
#endif
}

static void derive(const char *plain, const unsigned char *salt, unsigned char *dk)
{
    ui_pbkdf2_sha256((const unsigned char *)plain, strlen(plain), salt, SALT_LEN,
                     UI_PIN_HASH_ITER, dk, DK_LEN);
}

bool ui_pin_hash(const char *plain, char *out, size_t out_size)
{
    /* "p1$" + 16 hex + "$" + 32 hex + NUL = 53 */
    if (!plain || !plain[0] || !out || out_size < 3 + 2 * SALT_LEN + 1 + 2 * DK_LEN + 1)
        return false;
    unsigned char salt[SALT_LEN], dk[DK_LEN];
    random_salt(salt, sizeof salt);
    derive(plain, salt, dk);
    char salt_hex[2 * SALT_LEN + 1], dk_hex[2 * DK_LEN + 1];
    to_hex(salt, SALT_LEN, salt_hex);
    to_hex(dk, DK_LEN, dk_hex);
    snprintf(out, out_size, UI_PIN_HASH_PREFIX "%s$%s", salt_hex, dk_hex);
    return true;
}

bool ui_pin_verify(const char *stored, const char *plain)
{
    if (!stored || !plain || !stored[0] || !plain[0]) return false;
    if (!ui_pin_is_hashed(stored)) return strcmp(stored, plain) == 0;  /* heredado */

    const char *s = stored + 3;
    if (strlen(s) != 2 * SALT_LEN + 1 + 2 * DK_LEN || s[2 * SALT_LEN] != '$')
        return false;
    unsigned char salt[SALT_LEN], want[DK_LEN], got[DK_LEN];
    if (!from_hex(s, SALT_LEN, salt) || !from_hex(s + 2 * SALT_LEN + 1, DK_LEN, want))
        return false;
    derive(plain, salt, got);
    unsigned char diff = 0;
    for (int i = 0; i < DK_LEN; ++i) diff |= (unsigned char)(got[i] ^ want[i]);
    return diff == 0;
}

bool ui_pin_normalize(char *stored, size_t size)
{
    if (!stored || !stored[0] || ui_pin_is_hashed(stored)) return false;
    char hashed[UI_PIN_STORE_LEN];
    if (!ui_pin_hash(stored, hashed, sizeof hashed) || strlen(hashed) >= size)
        return false;
    memset(stored, 0, size);          /* no dejar restos del PIN en claro */
    memcpy(stored, hashed, strlen(hashed) + 1);
    return true;
}
