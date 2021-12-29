#line 45 "chorus.org"
#ifndef SK_CHORUS_H
#define SK_CHORUS_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 83 "chorus.org"
typedef struct sk_chorus sk_chorus;
#line 52 "chorus.org"

#ifdef SK_CHORUS_PRIV
#line 88 "chorus.org"
struct sk_chorus {
#line 174 "chorus.org"
SKFLT rate, prate;
#line 209 "chorus.org"
SKFLT depth;
#line 237 "chorus.org"
SKFLT mix;
#line 286 "chorus.org"
int sr;
#line 300 "chorus.org"
SKFLT *buf;
long sz;
long wpos;
#line 322 "chorus.org"
SKFLT z1;
#line 336 "chorus.org"
SKFLT ym1;
SKFLT a;
#line 370 "chorus.org"
SKFLT mc_x[2];
SKFLT mc_eps;
#line 90 "chorus.org"
};
#line 55 "chorus.org"
#endif

#line 100 "chorus.org"
sk_chorus * sk_chorus_new(int sr, SKFLT delay);
void sk_chorus_del(sk_chorus *c);
#line 134 "chorus.org"
void sk_chorus_init(sk_chorus *c,
                    int sr,
                    SKFLT *buf,
                    long sz);
#line 157 "chorus.org"
void sk_chorus_rate(sk_chorus *c, SKFLT rate);
#line 190 "chorus.org"
void sk_chorus_depth(sk_chorus *c, SKFLT depth);
#line 224 "chorus.org"
void sk_chorus_mix(sk_chorus *c, SKFLT mix);
#line 251 "chorus.org"
SKFLT sk_chorus_tick(sk_chorus *c, SKFLT in);
#line 58 "chorus.org"

#ifdef __plan9__
#pragma incomplete sk_chorus
#endif
#endif
