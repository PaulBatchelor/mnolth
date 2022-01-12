#line 12 "trand.org"
#ifndef SK_TRAND_H
#define SK_TRAND_H

#ifndef SKFLT
#define SKFLT float
#endif

#ifdef SK_TRAND_PRIV
#line 72 "trand.org"
struct sk_trand {
    unsigned long rng;
    SKFLT min;
    SKFLT max;
    SKFLT val;
    SKFLT scale;
    int changed;
};
#line 21 "trand.org"
#endif

#line 57 "trand.org"
typedef struct sk_trand sk_trand;
#line 21 "trand.org"
#line 42 "trand.org"
void sk_trand_init(sk_trand *tr, unsigned long seed);
#line 105 "trand.org"
void sk_trand_min(sk_trand *tr, SKFLT min);
void sk_trand_max(sk_trand *tr, SKFLT max);
#line 132 "trand.org"
SKFLT sk_trand_tick(sk_trand *tr, SKFLT trig);
#line 25 "trand.org"

#endif
