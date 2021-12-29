#line 29 "bitosc.org"
#include <math.h>
#include "bitosc.h"
#line 41 "bitosc.org"
SKFLT sk_bitosc_tick(SKFLT phs, unsigned long wt, int sz)
{
    SKFLT out;
    int pos;

    if (phs == 1) pos = sz - 1;
    else pos = floor(phs * sz);

    out = (wt & (1 << pos)) >> pos;
    out = (out * 2) - 1;

    return out;
}
#line 29 "bitosc.org"
