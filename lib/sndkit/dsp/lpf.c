#line 29 "lpf.org"
#include <math.h>
#define SK_LPF_PRIV
#include "lpf.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 63 "lpf.org"
void sk_lpf_init(sk_lpf *lpf, int sr)
{
#line 82 "lpf.org"
lpf->y[0] = 0.0;
lpf->y[1] = 0.0;
lpf->a0 = 0.0;
lpf->b1 = 0.0;
lpf->b2 = 0.0;
#line 101 "lpf.org"
lpf->update = 1;
#line 116 "lpf.org"
lpf->tpidsr = 2.0 * M_PI / (SKFLT)sr;
#line 152 "lpf.org"
sk_lpf_cutoff(lpf, 1000); /* arbitrary default */
#line 194 "lpf.org"
sk_lpf_q(lpf, 1.0); /* arbitrary default */
#line 66 "lpf.org"
}
#line 135 "lpf.org"
void sk_lpf_cutoff(sk_lpf *lpf, SKFLT cutoff)
{
    lpf->cutoff = cutoff;
    lpf->update = 1;
}
#line 177 "lpf.org"
void sk_lpf_q(sk_lpf *lpf, SKFLT q)
{
    if (q < 0.001) q = 0.001;
    lpf->q = q;
    lpf->update = 1;
}
#line 207 "lpf.org"
SKFLT sk_lpf_tick(sk_lpf *lpf, SKFLT in)
{
    SKFLT out;
    SKFLT y0;
    out = 0.0;
#line 239 "lpf.org"
if (lpf->update) {
    SKFLT C, D;
    SKFLT freq;
    SKFLT qres;

    qres = (1.0 / lpf->q);
    if (qres < 0.001) qres = 0.001;

    /* convert to radians/sample */
    freq = lpf->cutoff * lpf->tpidsr;

    /* intermediates */
    D = tan(freq * qres * 0.5);
    C = (1.0 - D) / (1.0 + D);

    lpf->b1 = (1.0 + C) * cos(freq);
    lpf->b2 = -C;
    lpf->a0 = (1.0 + C - lpf->b1) * 0.25;

    lpf->update = 0;
}
#line 207 "lpf.org"
#line 271 "lpf.org"
    y0 = lpf->a0*in + lpf->b1*lpf->y[0] + lpf->b2*lpf->y[1];
    out = y0 + 2.0*lpf->y[0] + lpf->y[1];
    lpf->y[1] = lpf->y[0];
    lpf->y[0] = y0;
#line 214 "lpf.org"
    return out;
}
#line 29 "lpf.org"
