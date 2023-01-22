#line 42 "gtick.org"
#define SK_GTICK_PRIV
#include "gtick.h"
#line 70 "gtick.org"
void sk_gtick_init(sk_gtick *gt)
{
    gt->prev = 0;
}
#line 94 "gtick.org"
SKFLT sk_gtick_tick(sk_gtick *gt, SKFLT gate)
{
    SKFLT out;
    out = 0;

    if (gate > 0 && gt->prev <= 0) {
        out = 1;
    }

    gt->prev = gate;

    return out;
}
#line 42 "gtick.org"
