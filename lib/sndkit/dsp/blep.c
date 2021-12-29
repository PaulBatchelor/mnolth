#line 46 "blep.org"
#include <math.h>

#define SK_BLEP_PRIV
#include "blep.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 89 "blep.org"
static SKFLT polyblep(SKFLT dt, SKFLT t);
#line 248 "blep.org"
static SKFLT tick(sk_blep *blep,
                  SKFLT (*wave)(sk_blep *, SKFLT));
#line 322 "blep.org"
static SKFLT blep_saw(sk_blep *blep, SKFLT t);
#line 363 "blep.org"
static SKFLT blep_square(sk_blep *blep, SKFLT t);
#line 417 "blep.org"
static SKFLT blep_triangle(sk_blep *blep, SKFLT t);
#line 46 "blep.org"
#line 94 "blep.org"
static SKFLT polyblep(SKFLT dt, SKFLT t)
{
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0;
    } else if(t > 1.0 - dt) {
        t = (t - 1.0) / dt;
        return t * t + t + t + 1.0;
    }

    return 0.0;
}
#line 131 "blep.org"
void sk_blep_init(sk_blep *blep, int sr)
{
#line 149 "blep.org"
sk_blep_freq(blep, 1000);
blep->pfreq = -1;
#line 162 "blep.org"
blep->onedsr = 1.0 / sr;
#line 177 "blep.org"
blep->inc = 0;
blep->phs = 0;
#line 198 "blep.org"
blep->A = exp(-1.0/(0.1 * sr));
blep->prev = 0;
#line 222 "blep.org"
blep->R = exp(-1.0/(0.0025 * sr));
blep->x = 0;
blep->y = 0;
#line 134 "blep.org"
}
#line 236 "blep.org"
void sk_blep_freq(sk_blep *blep, SKFLT freq)
{
    blep->freq = freq;
}
#line 254 "blep.org"
static SKFLT tick(sk_blep *blep,
                  SKFLT (*wave)(sk_blep *, SKFLT))
{
    SKFLT out;

    out = 0.0;

#line 274 "blep.org"
if (blep->freq != blep->pfreq) {
    blep->pfreq = blep->freq;
    blep->inc = blep->freq * blep->onedsr;
}
#line 254 "blep.org"
#line 285 "blep.org"
out = wave(blep, blep->phs);
#line 254 "blep.org"
#line 292 "blep.org"
blep->phs += blep->inc;

if (blep->phs > 1.0) {
    blep->phs -= 1.0;
}
#line 264 "blep.org"

    return out;
}
#line 308 "blep.org"
SKFLT sk_blep_saw(sk_blep *blep)
{
    return tick(blep, blep_saw);
}
#line 327 "blep.org"
static SKFLT blep_saw(sk_blep *blep, SKFLT t)
{
    SKFLT value;

    value = (2.0 * t) - 1.0;
    value -= polyblep(blep->inc, t);

    return value;
}
#line 347 "blep.org"
SKFLT sk_blep_square(sk_blep *blep)
{
    return tick(blep, blep_square);
}
#line 368 "blep.org"
static SKFLT blep_square(sk_blep *blep, SKFLT t)
{
    SKFLT value;

    value = t < 0.5 ? 1.0 : -1.0;
    value += polyblep(blep->inc, t);
    value -= polyblep(blep->inc, fmod(t + 0.5, 1.0));

    return value;
}
#line 389 "blep.org"
SKFLT sk_blep_triangle(sk_blep *blep)
{
    return tick(blep, blep_triangle);
}
#line 422 "blep.org"
static SKFLT blep_triangle(sk_blep *blep, SKFLT t)
{
    SKFLT value;

    /* compute square */
    value = t < 0.5 ? 1.0 : -1.0;
    value += polyblep(blep->inc, t);
    value -= polyblep(blep->inc, fmod(t + 0.5, 1.0));

    /* scale and integrate */
    value *= (4.0 / blep->freq);
    value += blep->prev;
    blep->prev = value;

    /* dc blocker */
    blep->y = value - blep->x + blep->R*blep->y;
    blep->x = value;

    return blep->y * 0.8;
}
#line 46 "blep.org"
