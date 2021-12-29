#line 23 "rephasor.org"
#ifndef SK_REPHASOR_H
#define SK_REPHASOR_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 48 "rephasor.org"
typedef struct sk_rephasor sk_rephasor;
#line 201 "rephasor.org"
typedef struct sk_rephasorx sk_rephasorx;
#line 29 "rephasor.org"
#ifdef SK_REPHASOR_PRIV
#line 53 "rephasor.org"
struct sk_rephasor {
#line 80 "rephasor.org"
SKFLT extphs;
SKFLT phs;
#line 100 "rephasor.org"
SKFLT scale;
#line 55 "rephasor.org"
};
#line 221 "rephasor.org"
struct sk_rephasorx {
   sk_rephasor rp;
   int counter;
   int limit;
   SKFLT N;
};
#line 31 "rephasor.org"
#endif
#line 62 "rephasor.org"
void sk_rephasor_init(sk_rephasor *rp);
#line 112 "rephasor.org"
void sk_rephasor_scale(sk_rephasor *rp, SKFLT scale);
#line 128 "rephasor.org"
SKFLT sk_rephasor_tick(sk_rephasor *rp, SKFLT in);
#line 234 "rephasor.org"
void sk_rephasorx_init(sk_rephasorx *rpx);
#line 252 "rephasor.org"
void sk_rephasorx_factor(sk_rephasorx *rpx, SKFLT N);
#line 269 "rephasor.org"
SKFLT sk_rephasorx_tick_div(sk_rephasorx *rpx, SKFLT in);
SKFLT sk_rephasorx_tick_mul(sk_rephasorx *rpx, SKFLT in);
#line 33 "rephasor.org"
#endif
