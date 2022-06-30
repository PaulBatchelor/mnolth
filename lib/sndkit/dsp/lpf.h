#line 10 "lpf.org"
#ifndef SK_LPF_H
#define SK_LPF_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 44 "lpf.org"
typedef struct sk_lpf sk_lpf;
#line 10 "lpf.org"
#line 58 "lpf.org"
void sk_lpf_init(sk_lpf *lpf, int sr);
#line 126 "lpf.org"
void sk_lpf_cutoff(sk_lpf *lpf, SKFLT cutoff);
#line 164 "lpf.org"
void sk_lpf_q(sk_lpf *lpf, SKFLT q);
#line 202 "lpf.org"
SKFLT sk_lpf_tick(sk_lpf *lpf, SKFLT in);
#line 19 "lpf.org"

#ifdef SK_LPF_PRIV
#line 49 "lpf.org"
struct sk_lpf {
#line 76 "lpf.org"
SKFLT y[2];
SKFLT a0, b1, b2;
#line 96 "lpf.org"
int update;
#line 111 "lpf.org"
SKFLT tpidsr;
#line 147 "lpf.org"
SKFLT cutoff;
#line 189 "lpf.org"
SKFLT q;
#line 51 "lpf.org"
};
#line 22 "lpf.org"
#endif

#endif
