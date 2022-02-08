#line 24 "qgliss.org"
#ifndef SK_QGLISS_H
#define SK_QGLISS_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 72 "qgliss.org"
typedef struct sk_qgliss sk_qgliss;
#line 31 "qgliss.org"

#ifdef SK_QGLISS_PRIV
#line 90 "qgliss.org"
struct sk_qgliss {
#line 103 "qgliss.org"
int init;
#line 116 "qgliss.org"
SKFLT gliss;
#line 132 "qgliss.org"
SKFLT gl, igl;
#line 145 "qgliss.org"
SKFLT phs;
#line 159 "qgliss.org"
SKFLT prv, nxt;
#line 173 "qgliss.org"
SKFLT *tab;
int sz;
#line 92 "qgliss.org"
};
#line 34 "qgliss.org"
#endif

#line 55 "qgliss.org"
void sk_qgliss_gliss(sk_qgliss *qg, SKFLT gliss);
#line 77 "qgliss.org"
void sk_qgliss_init(sk_qgliss *qg, SKFLT *tab, int sz);
#line 190 "qgliss.org"
SKFLT sk_qgliss_tick(sk_qgliss *qg, SKFLT in, SKFLT phs);
#line 37 "qgliss.org"
#endif
