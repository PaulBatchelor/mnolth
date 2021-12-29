#line 30 "tseq.org"
#define SK_TSEQ_PRIV
#include "tseq.h"

#line 62 "tseq.org"
void sk_tseq_init(sk_tseq *ts, SKFLT *seq, int sz)
{
    ts->seq = seq;
    ts->sz = sz;
    ts->pos = -1;
    ts->mode = 0; /* TODO */
}
#line 79 "tseq.org"
SKFLT sk_tseq_tick(sk_tseq *ts, SKFLT trig)
{
    SKFLT out;

    out = 0;

    if (trig != 0) {
        ts->pos++;
        if (ts->pos >= ts->sz) ts->pos = 0;
    }

    if (ts->pos < 0) ts->pos = 0;
    if (ts->pos >= ts->sz) ts->pos = ts->sz - 1;

    out = ts->seq[ts->pos];

    return out;
}
#line 30 "tseq.org"
