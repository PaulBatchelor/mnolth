#line 45 "sparse.org"
#define SK_SPARSE_PRIV
#include "sparse.h"
#line 152 "sparse.org"
static SKFLT randval(sk_sparse *sp);
#line 45 "sparse.org"
#line 75 "sparse.org"
void sk_sparse_init(sk_sparse *sp, int sr, unsigned long seed)
{
    sp->sr = sr;
#line 97 "sparse.org"
sk_sparse_freq(sp, 30); /* arbitrary positive value */
sp->pfreq = -1;
#line 126 "sparse.org"
sp->thresh = 0;
#line 140 "sparse.org"
sp->rng = seed;
#line 79 "sparse.org"
}
#line 108 "sparse.org"
void sk_sparse_freq(sk_sparse *sp, SKFLT freq)
{
    sp->freq = freq;
}
#line 157 "sparse.org"
static SKFLT randval(sk_sparse *sp)
{
    sp->rng = (1103515245L * sp->rng + 12345L);
    sp->rng %= 2147483648L;

    return (SKFLT)sp->rng / 2147483648L;
}
#line 175 "sparse.org"
SKFLT sk_sparse_tick(sk_sparse *sp)
{
    SKFLT out;
    SKFLT r;
    out = 0;

#line 200 "sparse.org"
if (sp->freq != sp->pfreq) {
    sp->pfreq = sp->freq;
    sp->thresh = sp->freq / (SKFLT)sp->sr;
}
#line 175 "sparse.org"
#line 212 "sparse.org"
r = randval(sp);
#line 175 "sparse.org"
#line 217 "sparse.org"
if (r < sp->thresh) out = (2 * randval(sp)) - 1;
#line 184 "sparse.org"

    return out;
}
#line 45 "sparse.org"
