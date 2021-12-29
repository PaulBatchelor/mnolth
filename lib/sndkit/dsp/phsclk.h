#line 33 "phsclk.org"
#ifndef PHSCLK_H
#define PHSCLK_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 55 "phsclk.org"
typedef struct sk_phsclk sk_phsclk;
#line 33 "phsclk.org"
#line 79 "phsclk.org"
void sk_phsclk_init(sk_phsclk *pc);
#line 95 "phsclk.org"
void sk_phsclk_nticks(sk_phsclk *pc, SKFLT nticks);
#line 124 "phsclk.org"
SKFLT sk_phsclk_tick(sk_phsclk *pc, SKFLT in);
#line 42 "phsclk.org"

#ifdef SK_PHSCLK_PRIV
#line 60 "phsclk.org"
struct sk_phsclk {
#line 67 "phsclk.org"
SKFLT prev;
#line 108 "phsclk.org"
SKFLT nticks;
#line 62 "phsclk.org"
};
#line 45 "phsclk.org"
#endif

#endif
