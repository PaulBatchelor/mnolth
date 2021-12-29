#line 67 "chorus.org"
#include <math.h>
#include <stdlib.h>
#define SK_CHORUS_PRIV
#include "chorus.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 106 "chorus.org"
sk_chorus * sk_chorus_new(int sr, SKFLT delay)
{
    sk_chorus *c;
    SKFLT *buf;
    long sz;

    c = malloc(sizeof(sk_chorus));
    sz = floor(delay * sr);
    buf = malloc(sizeof(SKFLT) * sz);
    sk_chorus_init(c, sr, buf, sz);

    return c;
}

void sk_chorus_del(sk_chorus *c)
{
    free(c->buf);
    free(c);
    c = NULL;
}
#line 142 "chorus.org"
void sk_chorus_init(sk_chorus *c,
                    int sr,
                    SKFLT *buf,
                    long sz)
{
#line 181 "chorus.org"
c->prate = -1;
sk_chorus_rate(c, 0.5);
#line 214 "chorus.org"
sk_chorus_depth(c, 1);
#line 242 "chorus.org"
sk_chorus_mix(c, 0.5);
#line 291 "chorus.org"
c->sr = sr;
#line 307 "chorus.org"
c->buf = buf;
c->sz = sz;
c->wpos = sz - 1;
{
    long i;
    for (i = 0; i < sz; i++) c->buf[i] = 0;
}
#line 327 "chorus.org"
c->z1 = 0;
#line 342 "chorus.org"
c->ym1 = 0;
#line 351 "chorus.org"
{
    SKFLT b;
    SKFLT freq;

    freq = 2020;

    b = 2.0 - cos(freq * (2 * M_PI / sr));
    c->a = b - sqrt(b*b - 1);
}
#line 380 "chorus.org"
c->mc_x[0] = 1;
c->mc_x[1] = 0;
c->mc_eps = 0;
#line 148 "chorus.org"
}
#line 162 "chorus.org"
void sk_chorus_rate(sk_chorus *c, SKFLT rate)
{
    c->rate = rate;
}

#line 199 "chorus.org"
void sk_chorus_depth(sk_chorus *c, SKFLT depth)
{
    if (depth < 0) depth = 0;
    if (depth > 1) depth = 1;
    c->depth = depth;
}
#line 229 "chorus.org"
void sk_chorus_mix(sk_chorus *c, SKFLT mix)
{
    c->mix = mix;
}
#line 256 "chorus.org"
SKFLT sk_chorus_tick(sk_chorus *c, SKFLT in)
{
    SKFLT out;
    SKFLT lfo;
    SKFLT t;
    SKFLT frac;
    long p1, p2;
    out = 0;

#line 389 "chorus.org"
if (c->prate != c->rate) {
    c->prate = c->rate;
    c->mc_eps = 2.0 * sin(M_PI * (c->rate / c->sr));
}

c->mc_x[0] = c->mc_x[0] + c->mc_eps * c->mc_x[1];
c->mc_x[1] = -c->mc_eps * c->mc_x[0] + c->mc_x[1];
#line 256 "chorus.org"
#line 402 "chorus.org"
lfo = (c->mc_x[1] + 1) * 0.5;
#line 256 "chorus.org"
#line 409 "chorus.org"
t = (lfo * 0.9 * c->depth + 0.05) * c->sz;
#line 256 "chorus.org"
#line 416 "chorus.org"
p1 = c->wpos - (int)floor(t);
if (p1 < 0) p1 += c->sz;
#line 256 "chorus.org"
#line 425 "chorus.org"
p2 = p1 - 1;
if (p2 < 0) p2 += c->sz;
#line 256 "chorus.org"
#line 433 "chorus.org"
frac = t - (int)floor(t);
#line 256 "chorus.org"
#line 440 "chorus.org"
out = c->buf[p2] + c->buf[p1]*(1 - frac) - (1 - frac)*c->z1;
c->z1 = out;
#line 256 "chorus.org"
#line 448 "chorus.org"
c->ym1 = (1 - c->a) * out + c->a*c->ym1;
out = c->ym1;
#line 256 "chorus.org"
#line 456 "chorus.org"
c->buf[c->wpos] = in;
#line 256 "chorus.org"
#line 463 "chorus.org"
c->wpos++;
if (c->wpos >= c->sz) c->wpos = 0;
#line 256 "chorus.org"
#line 472 "chorus.org"
out = c->mix * out + (1 - c->mix) * in;
#line 276 "chorus.org"

    return out;
}
#line 67 "chorus.org"
