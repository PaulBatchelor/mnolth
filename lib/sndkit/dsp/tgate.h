#line 8 "tgate.org"
#ifndef SK_TGATE_H
#define SK_TGATE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 34 "tgate.org"
typedef struct sk_tgate sk_tgate;
#line 16 "tgate.org"

#ifdef SK_TGATE_PRIV
#line 39 "tgate.org"
struct sk_tgate {
    SKFLT dur;
    unsigned long timer;
    int sr;
};
#line 19 "tgate.org"
#endif

#line 48 "tgate.org"
void sk_tgate_init(sk_tgate *tg, int sr);
#line 63 "tgate.org"
void sk_tgate_dur(sk_tgate *tg, SKFLT dur);
#line 78 "tgate.org"
SKFLT sk_tgate_tick(sk_tgate *tg, SKFLT trig);
#line 22 "tgate.org"
#endif
