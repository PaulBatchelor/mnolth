#line 33 "clkphs.org"
#ifndef SK_CLKPHS_H
#define SK_CLKPHS_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 74 "clkphs.org"
typedef struct sk_clkphs sk_clkphs;
#line 41 "clkphs.org"

#ifdef SK_CLKPHS_PRIV
#line 79 "clkphs.org"
struct sk_clkphs {
#line 89 "clkphs.org"
unsigned long counter;
#line 103 "clkphs.org"
SKFLT inc;
#line 116 "clkphs.org"
SKFLT phs;
#line 133 "clkphs.org"
int start;
#line 151 "clkphs.org"
int wait;
#line 166 "clkphs.org"
int spillover;
#line 182 "clkphs.org"
SKFLT correction;
#line 81 "clkphs.org"
};
#line 44 "clkphs.org"
#endif

#line 57 "clkphs.org"
void sk_clkphs_init(sk_clkphs *c);
#line 199 "clkphs.org"
SKFLT sk_clkphs_tick(sk_clkphs *c, SKFLT clk);
#line 47 "clkphs.org"

#endif
