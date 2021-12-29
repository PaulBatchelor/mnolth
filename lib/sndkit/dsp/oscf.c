#line 36 "oscf.org"
#include <math.h>
#define SK_OSCF_PRIV
#include "oscf.h"
#line 128 "oscf.org"
void sk_oscf_init(sk_oscf *oscf,
                  unsigned int sr,
                  SKFLT *tab,
                  unsigned long sz,
                  SKFLT iphs)
{
#line 69 "oscf.org"
oscf->phs = iphs;
#line 82 "oscf.org"
oscf->inc = 0;
#line 96 "oscf.org"
oscf->tab = tab;
oscf->sz = sz;
#line 110 "oscf.org"
oscf->sr = sr;
#line 164 "oscf.org"
sk_oscf_freq(oscf, 440);
oscf->lfreq = -1;
#line 135 "oscf.org"
}
#line 147 "oscf.org"
void sk_oscf_freq(sk_oscf *oscf, SKFLT freq)
{
    oscf->freq = freq;
}
#line 177 "oscf.org"
SKFLT sk_oscf_tick(sk_oscf *oscf)
{
    SKFLT out;
    unsigned long ipos;
    SKFLT fpos;
    SKFLT x[2];
    SKFLT phs;

    phs = oscf->phs;

#line 224 "oscf.org"
if (oscf->lfreq != oscf->freq) {
    oscf->lfreq = oscf->freq;

    oscf->inc = oscf->freq / (SKFLT)oscf->sr;
}
#line 177 "oscf.org"
#line 241 "oscf.org"
fpos = phs * oscf->sz;
ipos = floor(fpos);
fpos = fpos - ipos;
#line 177 "oscf.org"
#line 256 "oscf.org"
x[0] = oscf->tab[ipos];

if (ipos >= (oscf->sz - 1)) {
    x[1] = oscf->tab[0];
} else {
    x[1] = oscf->tab[ipos + 1];
}
#line 177 "oscf.org"
#line 270 "oscf.org"
out = fpos * x[1] + (1 - fpos) * x[0];
#line 177 "oscf.org"
#line 277 "oscf.org"
phs += oscf->inc;
#line 177 "oscf.org"
#line 291 "oscf.org"
while (phs >= 1) phs--;
while (phs < 0) phs++;
#line 193 "oscf.org"

    oscf->phs = phs;

    return out;
}
#line 316 "oscf.org"
SKFLT sk_oscf_tick_extphs(sk_oscf *oscf, SKFLT phs)
{
    SKFLT out;
    unsigned long ipos;
    SKFLT fpos;
    SKFLT x[2];

    out = 0;

#line 291 "oscf.org"
while (phs >= 1) phs--;
while (phs < 0) phs++;
#line 316 "oscf.org"
#line 224 "oscf.org"
if (oscf->lfreq != oscf->freq) {
    oscf->lfreq = oscf->freq;

    oscf->inc = oscf->freq / (SKFLT)oscf->sr;
}
#line 316 "oscf.org"
#line 241 "oscf.org"
fpos = phs * oscf->sz;
ipos = floor(fpos);
fpos = fpos - ipos;
#line 316 "oscf.org"
#line 256 "oscf.org"
x[0] = oscf->tab[ipos];

if (ipos >= (oscf->sz - 1)) {
    x[1] = oscf->tab[0];
} else {
    x[1] = oscf->tab[ipos + 1];
}
#line 316 "oscf.org"
#line 270 "oscf.org"
out = fpos * x[1] + (1 - fpos) * x[0];
#line 330 "oscf.org"

    return out;
}
#line 36 "oscf.org"
