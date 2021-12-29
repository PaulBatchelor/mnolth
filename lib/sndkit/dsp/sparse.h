#line 25 "sparse.org"
#ifndef SK_SPARSE_H
#define SK_SPARSE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 53 "sparse.org"
typedef struct sk_sparse sk_sparse;
#line 33 "sparse.org"

#ifdef SK_SPARSE_PRIV
#line 58 "sparse.org"
struct sk_sparse {
    int sr;
#line 91 "sparse.org"
SKFLT freq;
SKFLT pfreq;
#line 121 "sparse.org"
SKFLT thresh;
#line 135 "sparse.org"
unsigned long rng;
#line 61 "sparse.org"
};
#line 36 "sparse.org"
#endif

#line 70 "sparse.org"
void sk_sparse_init(sk_sparse *sp, int sr, unsigned long seed);
#line 103 "sparse.org"
void sk_sparse_freq(sk_sparse *sp, SKFLT freq);
#line 170 "sparse.org"
SKFLT sk_sparse_tick(sk_sparse *sp);
#line 39 "sparse.org"

#endif
