#line 34 "vardelay.org"
#ifndef SK_VARDELAY_H
#define SK_VARDELAY_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 90 "vardelay.org"
typedef struct sk_vardelay sk_vardelay;
#line 424 "vardelay.org"
typedef struct sk_clkdel sk_clkdel;
#line 34 "vardelay.org"
#line 55 "vardelay.org"
void sk_vardelay_init(sk_vardelay *vd, int sr,
                      SKFLT *buf, unsigned long sz);
#line 181 "vardelay.org"
void sk_vardelay_delay(sk_vardelay *vd, SKFLT delay);
void sk_vardelay_delays(sk_vardelay *vd, SKFLT delay);
#line 216 "vardelay.org"
void sk_vardelay_feedback(sk_vardelay *vd, SKFLT feedback);
#line 243 "vardelay.org"
SKFLT sk_vardelay_tick(sk_vardelay *vd, SKFLT in);
#line 444 "vardelay.org"
void sk_clkdel_init(sk_clkdel *cd, int sr,
                    SKFLT *buf,
                    unsigned long sz);
#line 469 "vardelay.org"
SKFLT sk_clkdel_tick(sk_clkdel *cd, SKFLT in, SKFLT phs);
#line 43 "vardelay.org"
#ifdef SK_VARDELAY_PRIV
#line 66 "vardelay.org"
#line 95 "vardelay.org"
struct sk_vardelay {
#line 106 "vardelay.org"
int sr;
#line 120 "vardelay.org"
SKFLT *buf;
unsigned long sz;
#line 144 "vardelay.org"
SKFLT prev;
#line 165 "vardelay.org"
long writepos;
#line 200 "vardelay.org"
SKFLT dels;
#line 229 "vardelay.org"
SKFLT feedback;
#line 97 "vardelay.org"
};
#line 66 "vardelay.org"
#line 429 "vardelay.org"
struct sk_clkdel {
    sk_vardelay vd;
    SKFLT phs;
    unsigned long timer;
    SKFLT isr;
};
#line 66 "vardelay.org"
#line 45 "vardelay.org"
#endif
#endif
