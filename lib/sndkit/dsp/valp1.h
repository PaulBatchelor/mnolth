#line 46 "valp1.org"
#ifndef SK_VALP1_H
#define SK_VALP1_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 67 "valp1.org"
typedef struct sk_valp1 sk_valp1;
#line 46 "valp1.org"
#line 82 "valp1.org"
void sk_valp1_freq(sk_valp1 *lp, SKFLT freq);
#line 156 "valp1.org"
void sk_valp1_init(sk_valp1 *lp, int sr);
#line 178 "valp1.org"
SKFLT sk_valp1_tick(sk_valp1 *lp, SKFLT in);
#line 55 "valp1.org"

#ifdef SK_VALP1_PRIV
#line 72 "valp1.org"
struct sk_valp1 {
#line 98 "valp1.org"
SKFLT freq;
SKFLT pfreq;
#line 114 "valp1.org"
SKFLT s;
#line 128 "valp1.org"
SKFLT G;
#line 143 "valp1.org"
SKFLT T;
#line 74 "valp1.org"
};
#line 58 "valp1.org"
#endif

#endif
