#line 32 "chaosnoise.org"
#ifndef SK_CHAOSNOISE_H
#define SK_CHAOSNOISE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 54 "chaosnoise.org"
typedef struct sk_chaosnoise sk_chaosnoise;
#line 32 "chaosnoise.org"
#line 139 "chaosnoise.org"
void sk_chaosnoise_chaos(sk_chaosnoise *cn, SKFLT chaos);
#line 167 "chaosnoise.org"
void sk_chaosnoise_rate(sk_chaosnoise *cn, SKFLT rate);
#line 195 "chaosnoise.org"
void sk_chaosnoise_init(sk_chaosnoise *cn, int sr, SKFLT init);
#line 211 "chaosnoise.org"
SKFLT sk_chaosnoise_tick(sk_chaosnoise *cn);
#line 41 "chaosnoise.org"

#ifdef SK_CHAOSNOISE_PRIV
#line 59 "chaosnoise.org"
struct sk_chaosnoise {
#line 69 "chaosnoise.org"
SKFLT y[2];
#line 89 "chaosnoise.org"
long phs;
#line 123 "chaosnoise.org"
SKFLT maxlens;
#line 152 "chaosnoise.org"
SKFLT chaos;
#line 180 "chaosnoise.org"
SKFLT rate;
#line 61 "chaosnoise.org"
};
#line 44 "chaosnoise.org"
#endif

#endif
