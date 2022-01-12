#line 31 "trand.org"
#define SK_TRAND_PRIV
#include "trand.h"

#line 47 "trand.org"
void sk_trand_init(sk_trand *tr, unsigned long seed)
{
#line 91 "trand.org"
tr->rng = seed;
tr->val = 0;
tr->changed = 0;
tr->min = 0;
tr->max = 1;
tr->scale = 1.0 / 2147483648L;
#line 50 "trand.org"
}
#line 111 "trand.org"
void sk_trand_min(sk_trand *tr, SKFLT min)
{
    if (tr->min != min) {
        tr->min = min;
        tr->changed = 1;
    }
}

void sk_trand_max(sk_trand *tr, SKFLT max)
{
    if (tr->max != max) {
        tr->max = max;
        tr->changed = 1;
    }
}
#line 140 "trand.org"
SKFLT sk_trand_tick(sk_trand *tr, SKFLT trig)
{
    if (trig != 0) {
        if (tr->changed) {
            tr->changed = 0;
            tr->scale = 1.0 / 2147483648L;
            tr->scale *= (tr->max - tr->min);
        }
        tr->val = tr->min + (tr->rng * tr->scale);
        tr->rng = (1103515245 * tr->rng + 12345) % 2147483648;
    }
    return tr->val;
}
#line 31 "trand.org"
