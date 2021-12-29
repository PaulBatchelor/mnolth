#line 14 "expon.org"
#ifndef SK_EXPON_H
#define SK_EXPON_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 42 "expon.org"
typedef struct sk_expon sk_expon;
#line 14 "expon.org"
#line 58 "expon.org"
void sk_expon_init(sk_expon *e, int sr);
#line 80 "expon.org"
void sk_expon_a(sk_expon *e, SKFLT val);
void sk_expon_b(sk_expon *e, SKFLT val);
#line 103 "expon.org"
void sk_expon_dur(sk_expon *e, SKFLT dur);
#line 144 "expon.org"
SKFLT sk_expon_tick(sk_expon *e, SKFLT trig);
#line 22 "expon.org"
#ifdef SK_EXPON_PRIV
#line 47 "expon.org"
struct sk_expon {
    SKFLT a, dur, b;
    SKFLT val, incr;
    unsigned long sdur, stime;
    int init;
    int sr;
};
#line 24 "expon.org"
#endif
#endif
