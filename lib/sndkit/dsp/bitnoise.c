#line 14 "bitnoise.org"
/* tangled from sndkit. do not edit by hand */
#include <stdint.h>
#include <math.h>
#define SK_BITNOISE_PRIV
#include "bitnoise.h"
#line 88 "bitnoise.org"
#define SK_BITNOISE_PHSMAX 0x1000000L
#define SK_BITNOISE_PHSMSK 0x0FFFFFFL
#line 14 "bitnoise.org"
#line 72 "bitnoise.org"
void sk_bitnoise_init(sk_bitnoise *bn, int sr)
{
#line 105 "bitnoise.org"
bn->maxlens = SK_BITNOISE_PHSMAX / (SKFLT) sr;
#line 118 "bitnoise.org"
bn->phs = 0;
#line 132 "bitnoise.org"
bn->lfsr = 1;
#line 145 "bitnoise.org"
bn->pos = 0;
#line 158 "bitnoise.org"
bn->saved = 0;
#line 188 "bitnoise.org"
sk_bitnoise_rate(bn, 1000);
#line 219 "bitnoise.org"
sk_bitnoise_mode(bn, 0);
#line 75 "bitnoise.org"
}
#line 175 "bitnoise.org"
void sk_bitnoise_rate(sk_bitnoise *bn, SKFLT rate)
{
    bn->rate = rate;
}
#line 206 "bitnoise.org"
void sk_bitnoise_mode(sk_bitnoise *bn, int mode)
{
    bn->m = mode;
}
#line 231 "bitnoise.org"
SKFLT sk_bitnoise_tick(sk_bitnoise *bn)
{
    SKFLT out;

    out = 0;
#line 249 "bitnoise.org"
bn->phs += floor(bn->rate * bn->maxlens);
#line 231 "bitnoise.org"
#line 315 "bitnoise.org"
if (bn->phs >= SK_BITNOISE_PHSMAX) {
    SKFLT y;
    bn->phs &= SK_BITNOISE_PHSMSK;
    if (bn->pos > 14) {
        uint16_t f;
        uint16_t x;
        bn->pos = 0;
#line 276 "bitnoise.org"
x = bn->lfsr;
f = (x & 1) ^ ((x >> (bn->m ? 6:1)) & 1);
x >>= 1;
x |= f << 14;
bn->lfsr = x;
#line 323 "bitnoise.org"
    }

#line 294 "bitnoise.org"
y = (bn->lfsr >> bn->pos) & 1;
#line 323 "bitnoise.org"
#line 310 "bitnoise.org"
bn->saved = y * 2 - 1;
#line 327 "bitnoise.org"
    bn->pos++;
}
#line 231 "bitnoise.org"
#line 335 "bitnoise.org"
out = bn->saved;
#line 239 "bitnoise.org"
    return out;
}
#line 14 "bitnoise.org"
