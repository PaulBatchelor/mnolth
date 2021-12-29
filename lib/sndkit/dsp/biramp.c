#line 32 "biramp.org"
#define SK_BIRAMP_PRIV
#include "biramp.h"
#line 59 "biramp.org"
SKFLT sk_biramp_stateless(SKFLT phs, SKFLT pos)
{
    SKFLT out;
    out = 0;

    if (pos == 0 || pos == 1) return pos;

    if (phs < pos) out = phs / pos;
    else out = (1 - phs) / (1 - pos);

    return out;
}
#line 98 "biramp.org"
SKFLT sk_biramp_stateful(SKFLT phs, SKFLT pos,
                         SKFLT *lphs, SKFLT *lpos)
{
    SKFLT out;
    out = 0;

    if (*lphs > phs || *lpos < 0) {
        *lpos = pos;
    }

    out = sk_biramp_stateless(phs, *lpos);

    *lphs = phs;
    return out;
}
#line 136 "biramp.org"
SKFLT sk_biramp_flipper(SKFLT phs, SKFLT *lphs, int *flipit)
{
    SKFLT out;
    out = 0;

    if (*lphs > phs) {
        (*flipit) =  ~(*flipit);
    }

    out = phs;
    *lphs = phs;

    if (*flipit) out = 1 - out;

    return out;
}
#line 32 "biramp.org"
