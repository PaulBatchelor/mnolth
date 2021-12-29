#line 30 "expon.org"
#include <math.h>
#define SK_EXPON_PRIV
#include "expon.h"
#line 119 "expon.org"
static void expon_reinit(sk_expon *e);
#line 30 "expon.org"
#line 64 "expon.org"
void sk_expon_init(sk_expon *e, int sr)
{
    sk_expon_a(e, 0.000001);
    sk_expon_b(e, 1);
    sk_expon_dur(e, 1);
    e->sr = sr;
    e->init = 1;

    expon_reinit(e);
}
#line 86 "expon.org"
void sk_expon_a(sk_expon *e, SKFLT val)
{
    if (val != 0) e->a = val;
}

void sk_expon_b(sk_expon *e, SKFLT val)
{
    if (val != 0) e->b = val;
}
#line 108 "expon.org"
void sk_expon_dur(sk_expon *e, SKFLT dur)
{
    e->dur = dur;
}
#line 124 "expon.org"
static void expon_reinit(sk_expon *e)
{
    SKFLT onedsr;
    e->stime = 0;
    e->sdur = e->dur * e->sr;
    onedsr = 1.0 / e->sr;

    if ((e->a * e->b) > 0.0) {
        e->incr = pow((SKFLT)(e->b / e->a), onedsr / e->dur);
    } else {
        e->incr = 1;
        e->val = e->a;
    }

    e->val = e->a;
}
#line 149 "expon.org"
SKFLT sk_expon_tick(sk_expon *e, SKFLT trig)
{
    SKFLT out;

    out = 0;

    if (trig != 0) {
        expon_reinit(e);
        e->init = 0;
    }

    if (e->init) {
        out = 0;
    }

    if (e->stime < e->sdur) {
        SKFLT val = e->val;
        e->val *= e->incr;
        e->stime++;
        out = val;
    } else {
        out = e->b;
    }

    return out;
}
#line 30 "expon.org"
