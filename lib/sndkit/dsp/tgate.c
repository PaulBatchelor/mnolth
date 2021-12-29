#line 27 "tgate.org"
#define SK_TGATE_PRIV
#include "tgate.h"
#line 53 "tgate.org"
void sk_tgate_init(sk_tgate *tg, int sr)
{
    tg->sr = sr;
    tg->dur = 0;
    tg->timer = 0;
}
#line 68 "tgate.org"
void sk_tgate_dur(sk_tgate *tg, SKFLT dur)
{
    tg->dur = dur;
}
#line 83 "tgate.org"
SKFLT sk_tgate_tick(sk_tgate *tg, SKFLT trig)
{
    SKFLT out;

    out = 0;

    if (trig != 0) {
        tg->timer = tg->dur * tg->sr;
    }

    if (tg->timer != 0) {
        out = 1.0;
        tg->timer--;
    }

    return out;
}
#line 27 "tgate.org"
