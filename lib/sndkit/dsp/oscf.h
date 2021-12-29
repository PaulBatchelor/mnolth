#line 21 "oscf.org"
#ifndef SK_OSCF_H
#define SK_OSCF_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 46 "oscf.org"
typedef struct sk_oscf sk_oscf;
#line 21 "oscf.org"
#line 119 "oscf.org"
void sk_oscf_init(sk_oscf *oscf,
                  unsigned int sr,
                  SKFLT *tab,
                  unsigned long sz,
                  SKFLT iphs);
#line 142 "oscf.org"
void sk_oscf_freq(sk_oscf *oscf, SKFLT freq);
#line 172 "oscf.org"
SKFLT sk_oscf_tick(sk_oscf *oscf);
#line 305 "oscf.org"
SKFLT sk_oscf_tick_extphs(sk_oscf *oscf, SKFLT phs);
#line 28 "oscf.org"
#ifdef SK_OSCF_PRIV
#line 51 "oscf.org"
struct sk_oscf {
#line 64 "oscf.org"
SKFLT phs;
#line 77 "oscf.org"
SKFLT inc;
#line 90 "oscf.org"
SKFLT *tab;
unsigned long sz;
#line 105 "oscf.org"
unsigned int sr;
#line 158 "oscf.org"
SKFLT freq;
SKFLT lfreq;
#line 53 "oscf.org"
};
#line 30 "oscf.org"
#endif
#endif
