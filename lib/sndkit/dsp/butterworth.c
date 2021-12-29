#line 58 "butterworth.org"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define SK_BUTTERWORTH_PRIV
#include "butterworth.h"

#define ROOT2 1.4142135623730950488

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static SKFLT filter(SKFLT in, SKFLT *a)
{
    SKFLT t, y;
    /* a5 = t(n - 1); a6 = t(n - 2) */
    t = in - a[3]*a[5] - a[4]*a[6];
    y = t*a[0] + a[1]*a[5] + a[2]*a[6];
    a[6] = a[5];
    a[5] = t;
    return y;
}

void sk_butterworth_init(sk_butterworth *bw, int sr)
{
    int i;
    sk_butterworth_freq(bw, 1000.0);
    bw->lfreq = -1;
    bw->pidsr = M_PI / (SKFLT)sr;
    for (i = 0; i < 7; i++) bw->a[i] = 0.0;
}

void sk_butterworth_freq(sk_butterworth *bw, SKFLT freq)
{
    bw->freq = freq;
}

SKFLT sk_butlp_tick(sk_butterworth *bw, SKFLT in)
{
    if (bw->freq != bw->lfreq) {
        SKFLT *a, c;
        a = bw->a;
        bw->lfreq = bw->freq;
        /* derive C constant used in BLT */
        c = 1.0 / tan((SKFLT)(bw->pidsr * bw->lfreq));

        /* perform BLT, store components */
        a[0] = 1.0 / (1.0 + c*ROOT2 + c*c);
        a[1] = 2*a[0];
        a[2] = a[0];
        a[3] = 2.0 * (1.0 - c*c) * a[0];
        a[4] = (1.0 - c*ROOT2 + c*c) * a[0];
    }

    return filter(in, bw->a);
}

SKFLT sk_buthp_tick(sk_butterworth *bw, SKFLT in)
{
    if (bw->freq != bw->lfreq) {
        SKFLT *a, c;
        a = bw->a;
        bw->lfreq = bw->freq;
        /* derive C constant used in BLT */
        c = tan((SKFLT)(bw->pidsr * bw->freq));

        /* perform BLT, store components */
        a[0] = 1.0 / (1.0 + c*ROOT2 + c*c);
        a[1] = -2*a[0];
        a[2] = a[0];
        a[3] = 2.0 * (c*c - 1.0) * a[0];
        a[4] = (1.0 - c*ROOT2 + c*c) * a[0];
    }

    return filter(in, bw->a);
}
