#line 22 "vardelay.org"
#include <math.h>
#include <stdlib.h>
#define SK_VARDELAY_PRIV
#include "vardelay.h"
#line 61 "vardelay.org"
void sk_vardelay_init(sk_vardelay *vd, int sr,
                      SKFLT *buf, unsigned long sz)
{
    vd->sr = sr;
#line 126 "vardelay.org"
if (sz < 4) {
    vd->buf = NULL;
    vd->buf = 0;
} else {
    vd->buf = buf;
    vd->sz = sz;
}
#line 149 "vardelay.org"
vd->prev = 0;
#line 170 "vardelay.org"
vd->writepos = 0;
#line 208 "vardelay.org"
sk_vardelay_delay(vd, ((SKFLT)sz / sr) * 0.5);
#line 236 "vardelay.org"
sk_vardelay_feedback(vd, 0);
#line 66 "vardelay.org"
}
#line 187 "vardelay.org"
void sk_vardelay_delay(sk_vardelay *vd, SKFLT delay)
{
    vd->dels = delay * vd->sr;
}

void sk_vardelay_delays(sk_vardelay *vd, SKFLT delay)
{
    vd->dels = delay;
}
#line 221 "vardelay.org"
void sk_vardelay_feedback(sk_vardelay *vd, SKFLT feedback)
{
    vd->feedback = feedback;
}
#line 248 "vardelay.org"
SKFLT sk_vardelay_tick(sk_vardelay *vd, SKFLT in)
{
    SKFLT out;
    SKFLT dels;
    SKFLT f;
    long i;
    SKFLT s[4];
    unsigned long n[4];
    SKFLT a, b, c, d;

    out = 0;
#line 277 "vardelay.org"
if (vd->buf == NULL || vd->sz == 0) return 0;
#line 248 "vardelay.org"
#line 284 "vardelay.org"
vd->buf[vd->writepos] = in + vd->prev * vd->feedback;
#line 248 "vardelay.org"
#line 297 "vardelay.org"
dels = vd->dels;
i = floor(dels);
f = i - dels;
i = vd->writepos - i;
#line 248 "vardelay.org"
#line 322 "vardelay.org"
if ((f < 0.0) || (i < 0)) {
    /* flip fractional component */
    f = f + 1.0;
    /* go backwards one sample */
    i = i - 1;
    while (i < 0) i += vd->sz;
} else while(i >= vd->sz) i -= vd->sz;
#line 248 "vardelay.org"
#line 336 "vardelay.org"
/* x(n) */
n[1] = i;

/* x(n + 1) */
if (i == (vd->sz - 1)) n[2] = 0;
else n[2] = n[1] + 1;

/* x(n - 1) */
if (i == 0) n[0] = vd->sz - 1;
else n[0] = i - 1;

if (n[2] == vd->sz - 1) n[3] = 0;
else n[3] = n[2] + 1;

{
    int j;
    for (j = 0; j < 4; j++) s[j] = vd->buf[n[j]];
}
#line 248 "vardelay.org"
#line 361 "vardelay.org"
{
    SKFLT tmp[2];

    d = ((f * f) - 1) * 0.1666666667;
    tmp[0] = (f + 1.0) * 0.5;
    tmp[1] = 3.0 * d;
    a = tmp[0] - 1.0 - d;
    c = tmp[0] - tmp[1];
    b = tmp[1] - f;
}
#line 248 "vardelay.org"
#line 381 "vardelay.org"
out = (a*s[0] + b*s[1] + c*s[2] + d*s[3]) * f + s[1];
#line 248 "vardelay.org"
#line 389 "vardelay.org"
vd->writepos++;
if (vd->writepos == vd->sz) vd->writepos = 0;
#line 248 "vardelay.org"
#line 398 "vardelay.org"
vd->prev = out;
#line 268 "vardelay.org"

    return out;
}
#line 451 "vardelay.org"
void sk_clkdel_init(sk_clkdel *cd, int sr,
                    SKFLT *buf,
                    unsigned long sz)
{
    sk_vardelay_init(&cd->vd, sr, buf, sz);
    cd->phs = -1;
    cd->timer = 0;
    cd->isr = 1.0 / (SKFLT) sr;
    sk_vardelay_delays(&cd->vd, sz - 1);
}
#line 478 "vardelay.org"
SKFLT sk_clkdel_tick(sk_clkdel *cd, SKFLT in, SKFLT phs)
{
    if (phs < 0) {
        cd->phs = 0;
        cd->timer = 0;
        return sk_vardelay_tick(&cd->vd, in);
    }

    if (phs < cd->phs) {
        sk_vardelay_delay(&cd->vd, cd->timer * cd->isr);
        cd->timer = 0;
    }

    cd->phs = phs;
    cd->timer++;

    return sk_vardelay_tick(&cd->vd, in);
}
#line 22 "vardelay.org"
