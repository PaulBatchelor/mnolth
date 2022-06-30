#line 142 "rephasor.org"
#ifndef SK_REPHASOR_H
#define SK_REPHASOR_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 168 "rephasor.org"
typedef struct sk_rephasor sk_rephasor;
#line 148 "rephasor.org"
#ifdef SK_REPHASOR_PRIV
#line 197 "rephasor.org"
struct sk_rephasor {
    SKFLT pr;
    SKFLT pc[2];
    SKFLT pe[2];
    SKFLT c;
    SKFLT s;
    SKFLT si;

    SKFLT ir;
    SKFLT ic;
};
#line 150 "rephasor.org"
#endif
#line 214 "rephasor.org"
void sk_rephasor_init(sk_rephasor *rp);
#line 243 "rephasor.org"
void sk_rephasor_scale(sk_rephasor *rp, SKFLT scale);
#line 266 "rephasor.org"
SKFLT sk_rephasor_tick(sk_rephasor *rp, SKFLT ext);
#line 353 "rephasor.org"
SKFLT sk_rephasor_tick_nosync(sk_rephasor *rp, SKFLT ext);
#line 152 "rephasor.org"
#endif
