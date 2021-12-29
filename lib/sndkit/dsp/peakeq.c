#line 77 "peakeq.org"
#include <math.h>
#define SK_PEAKEQ_PRIV
#include "peakeq.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 133 "peakeq.org"
void sk_peakeq_init(sk_peakeq *eq, int sr)
{
#line 152 "peakeq.org"
eq->v[0] = 0;
eq->v[1] = 0;
#line 167 "peakeq.org"
eq->a = 0;
eq->b = 0;
#line 181 "peakeq.org"
eq->sr = sr;
#line 215 "peakeq.org"
sk_peakeq_freq(eq, 1000);
eq->pfreq = -1;
#line 249 "peakeq.org"
sk_peakeq_bandwidth(eq, 1000);
eq->pbw = -1;
#line 276 "peakeq.org"
sk_peakeq_gain(eq, 1.0);
#line 136 "peakeq.org"
}
#line 195 "peakeq.org"
void sk_peakeq_freq(sk_peakeq *eq, SKFLT freq)
{
    eq->freq = freq;
}
#line 229 "peakeq.org"
void sk_peakeq_bandwidth(sk_peakeq *eq, SKFLT bw)
{
    eq->bw = bw;
}
#line 263 "peakeq.org"
void sk_peakeq_gain(sk_peakeq *eq, SKFLT gain)
{
    eq->gain = gain;
}
#line 288 "peakeq.org"
SKFLT sk_peakeq_tick(sk_peakeq *eq, SKFLT in)
{
    SKFLT out;
    SKFLT v;
    SKFLT y;
    out = 0;

#line 317 "peakeq.org"
if (eq->bw != eq->pbw || eq->freq != eq->pfreq) {
    SKFLT c;
    eq->b = -cos(2 * M_PI * eq->freq / eq->sr);
    c = tan(M_PI * eq->bw / eq->sr);
    eq->a = (1.0 - c) / (1.0 + c);

    eq->pbw = eq->bw;
    eq->pfreq = eq->freq;
}
#line 288 "peakeq.org"
#line 335 "peakeq.org"
v = in - eq->b*(1.0 + eq->a)*eq->v[0] - eq->a*eq->v[1];
y = eq->a*v + eq->b*(1.0 + eq->a)*eq->v[0] + eq->v[1];
#line 288 "peakeq.org"
#line 349 "peakeq.org"
out = ((in + y) + eq->gain*(in - y)) * 0.5;
#line 288 "peakeq.org"
#line 361 "peakeq.org"
eq->v[1] = eq->v[0];
eq->v[0] = v;
#line 299 "peakeq.org"

    return out;
}
#line 77 "peakeq.org"
