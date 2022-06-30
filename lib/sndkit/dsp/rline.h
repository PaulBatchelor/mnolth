#line 48 "rline.org"
#ifndef SK_RLINE_H
#define SK_RLINE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 68 "rline.org"
typedef struct sk_rline sk_rline;
#line 442 "rline.org"
typedef struct sk_jitseg sk_jitseg;
#line 48 "rline.org"
#line 237 "rline.org"
void sk_rline_init(sk_rline *rl, int sr, int seed);
#line 253 "rline.org"
void sk_rline_min(sk_rline *rl, SKFLT min);
#line 280 "rline.org"
void sk_rline_max(sk_rline *rl, SKFLT max);
#line 307 "rline.org"
void sk_rline_rate(sk_rline *rl, SKFLT rate);
#line 334 "rline.org"
SKFLT sk_rline_tick(sk_rline *rl);
#line 458 "rline.org"
void sk_jitseg_init(sk_jitseg *js, int sr, int s1, int s2);
#line 477 "rline.org"
void sk_jitseg_min(sk_jitseg *js, SKFLT min);
void sk_jitseg_max(sk_jitseg *js, SKFLT max);
void sk_jitseg_rate_min(sk_jitseg *js, SKFLT min);
void sk_jitseg_rate_max(sk_jitseg *js, SKFLT min);
void sk_jitseg_rate_rate(sk_jitseg *js, SKFLT rate);
SKFLT sk_jitseg_tick(sk_jitseg *js);
#line 57 "rline.org"

#ifdef SK_RLINE_PRIV
#line 77 "rline.org"
struct sk_rline {
#line 91 "rline.org"
SKFLT rngscale;
#line 104 "rline.org"
int rng;
#line 134 "rline.org"
SKFLT maxlens;
#line 148 "rline.org"
unsigned long phasepos;
SKFLT scale;
#line 174 "rline.org"
SKFLT start;
SKFLT end;
#line 266 "rline.org"
SKFLT min;
#line 293 "rline.org"
SKFLT max;
#line 320 "rline.org"
SKFLT rate;
#line 79 "rline.org"
};
#line 447 "rline.org"
struct sk_jitseg {
    sk_rline main;
    sk_rline rate;
};
#line 79 "rline.org"
#line 60 "rline.org"
#endif
#endif
