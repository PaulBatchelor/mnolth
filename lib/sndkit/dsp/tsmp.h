#line 8 "tsmp.org"
#ifndef SK_TSMP_H
#define SK_TSMP_H

#ifndef SKFLT
#define SKFLT float
#endif

#ifdef SK_TSMP_PRIV
#line 43 "tsmp.org"
struct sk_tsmp {
#line 66 "tsmp.org"
double pos;
#line 95 "tsmp.org"
SKFLT play;
#line 121 "tsmp.org"
SKFLT *tab;
unsigned long tabsz;
#line 45 "tsmp.org"
};
#line 17 "tsmp.org"
#endif
#line 38 "tsmp.org"
typedef struct sk_tsmp sk_tsmp;
#line 17 "tsmp.org"
#line 50 "tsmp.org"
void sk_tsmp_init(sk_tsmp *tsmp, SKFLT *tab, unsigned long sz);
#line 105 "tsmp.org"
void sk_tsmp_rate(sk_tsmp *ts, SKFLT play);
#line 133 "tsmp.org"
SKFLT sk_tsmp_tick(sk_tsmp *tsmp, SKFLT trig);
#line 20 "tsmp.org"

#endif
