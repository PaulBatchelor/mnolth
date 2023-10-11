#line 22 "metro.org"
#ifndef SK_METRO_H
#define SK_METRO_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 51 "metro.org"
typedef struct sk_metro sk_metro;
#line 22 "metro.org"
#line 56 "metro.org"
void sk_metro_init(sk_metro *m, int sr);
#line 114 "metro.org"
void sk_metro_freq(sk_metro *m, SKFLT freq);
#line 136 "metro.org"
void sk_metro_reset(sk_metro *m);
#line 152 "metro.org"
SKFLT sk_metro_tick(sk_metro *m);
#line 31 "metro.org"

#ifdef SK_METRO_PRIV
#line 69 "metro.org"
struct sk_metro {
    SKFLT freq;
    SKFLT phs;
    int init;
    SKFLT onedsr;
};
#line 34 "metro.org"
#endif
#endif
