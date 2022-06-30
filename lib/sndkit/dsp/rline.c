#line 35 "rline.org"
#include <math.h>
#define SK_RLINE_PRIV
#include "rline.h"
#line 122 "rline.org"
#define SK_RLINE_PHSLEN 0x1000000L
#define SK_RLINE_PHSMSK 0xFFFFFFL
#line 213 "rline.org"
#define LCG(y) (y * 0x343fd + 0x269ec3)
#line 226 "rline.org"
#define RNG(y) ((y >> 1) & 0x7fffffff)
#line 35 "rline.org"
#line 242 "rline.org"
void sk_rline_init(sk_rline *rl, int sr, int seed)
{
#line 109 "rline.org"
rl->rng = seed;
#line 96 "rline.org"
rl->rngscale = 1.0 / ((1L<<31) - 1);
#line 109 "rline.org"
#line 180 "rline.org"
rl->rng = LCG(rl->rng);
rl->start = RNG(rl->rng) * rl->rngscale;
rl->rng = LCG(rl->rng);
rl->end = RNG(rl->rng) * rl->rngscale;
#line 164 "rline.org"
rl->scale = (rl->end - rl->start) / SK_RLINE_PHSLEN;
#line 180 "rline.org"
#line 109 "rline.org"
#line 139 "rline.org"
rl->maxlens = (SKFLT)SK_RLINE_PHSLEN / sr;
#line 154 "rline.org"
rl->phasepos = 0;
#line 273 "rline.org"
sk_rline_min(rl, 0);
#line 300 "rline.org"
sk_rline_max(rl, 1);
#line 327 "rline.org"
sk_rline_rate(rl, 1);
#line 245 "rline.org"
}
#line 258 "rline.org"
void sk_rline_min(sk_rline *rl, SKFLT min)
{
    rl->min = min;
}
#line 285 "rline.org"
void sk_rline_max(sk_rline *rl, SKFLT max)
{
    rl->max= max;
}
#line 312 "rline.org"
void sk_rline_rate(sk_rline *rl, SKFLT rate)
{
    rl->rate= rate;
}
#line 339 "rline.org"
SKFLT sk_rline_tick(sk_rline *rl)
{
    SKFLT out;

    out = 0;

#line 377 "rline.org"
out = rl->start + rl->phasepos*rl->scale;
out = out * (rl->max - rl->min) + rl->min;
#line 339 "rline.org"
#line 390 "rline.org"
rl->phasepos += floor(rl->rate * rl->maxlens);
#line 339 "rline.org"
#line 412 "rline.org"
if (rl->phasepos >= SK_RLINE_PHSLEN) {
    rl->phasepos &= SK_RLINE_PHSMSK;
    rl->start = rl->end;
    rl->rng = LCG(rl->rng);
    rl->end = RNG(rl->rng) * rl->rngscale;
    rl->scale = (rl->end - rl->start) / SK_RLINE_PHSLEN;
}
#line 348 "rline.org"

    return out;
}
#line 463 "rline.org"
void sk_jitseg_init(sk_jitseg *js, int sr, int s1, int s2)
{
    sk_rline_init(&js->main, sr, s1);
    sk_rline_init(&js->rate, sr, s2);
}
#line 487 "rline.org"
void sk_jitseg_min(sk_jitseg *js, SKFLT min)
{
    sk_rline_min(&js->main, min);
}

void sk_jitseg_max(sk_jitseg *js, SKFLT max)
{
    sk_rline_max(&js->main, max);
}
#line 500 "rline.org"
void sk_jitseg_rate_min(sk_jitseg *js, SKFLT min)
{
    sk_rline_min(&js->rate, min);
}

void sk_jitseg_rate_max(sk_jitseg *js, SKFLT max)
{
    sk_rline_max(&js->rate, max);
}
#line 513 "rline.org"
void sk_jitseg_rate_rate(sk_jitseg *js, SKFLT rate)
{
    sk_rline_rate(&js->rate, rate);
}
#line 521 "rline.org"
SKFLT sk_jitseg_tick(sk_jitseg *js)
{
    SKFLT out;

    out = 0;

    sk_rline_rate(&js->main, sk_rline_tick(&js->rate));

    out = sk_rline_tick(&js->main);
    return out;
}
#line 35 "rline.org"
