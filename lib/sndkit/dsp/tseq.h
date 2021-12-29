#line 11 "tseq.org"
#ifndef SK_TSEQ_H
#define SK_TSEQ_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 38 "tseq.org"
typedef struct sk_tseq sk_tseq;
#line 11 "tseq.org"
#line 53 "tseq.org"
void sk_tseq_init(sk_tseq *ts, SKFLT *seq, int sz);
#line 74 "tseq.org"
SKFLT sk_tseq_tick(sk_tseq *ts, SKFLT trig);
#line 20 "tseq.org"

#ifdef SK_TSEQ_PRIV
#line 43 "tseq.org"
struct sk_tseq {
    SKFLT *seq;
    int sz;
    int pos;
    int mode; /* TODO */
};
#line 23 "tseq.org"
#endif

#endif
