#line 33 "valp1.org"
#include <math.h>
#define SK_VALP1_PRIV
#include "valp1.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 87 "valp1.org"
void sk_valp1_freq(sk_valp1 *lp, SKFLT freq)
{
    lp->freq = freq;
}
#line 161 "valp1.org"
void sk_valp1_init(sk_valp1 *lp, int sr)
{
#line 104 "valp1.org"
sk_valp1_freq(lp, 1000);
lp->pfreq = -1;
#line 119 "valp1.org"
lp->s = 0;
#line 133 "valp1.org"
lp->G = 0;
#line 148 "valp1.org"
lp->T = 1.0 / (SKFLT)sr;
#line 164 "valp1.org"
}
#line 183 "valp1.org"
SKFLT sk_valp1_tick(sk_valp1 *lp, SKFLT in)
{
    SKFLT out;
    SKFLT v;
    out = 0;
#line 248 "valp1.org"
if (lp->pfreq != lp->freq) {
    SKFLT wc;
    SKFLT wa;
    SKFLT g;

    wc = 2.0 * M_PI * lp->freq;
    wa = (2.0/lp->T) * tan(wc * lp->T * 0.5);
    g = wa * lp->T * 0.5;
    lp->G = g / (1.0 + g);

    lp->pfreq = lp->freq;
}
#line 183 "valp1.org"
#line 275 "valp1.org"
v = (in - lp->s) * lp->G;
out = v + lp->s;
lp->s = out + v;
#line 190 "valp1.org"
    return out;
}
#line 33 "valp1.org"
