#line 23 "chaosnoise.org"
#include <math.h>
#define SK_CHAOSNOISE_PRIV
#include "chaosnoise.h"
#line 103 "chaosnoise.org"
#define SK_CHAOSNOISE_PHSMAX 0x1000000L
#line 111 "chaosnoise.org"
#define SK_CHAOSNOISE_PHSMSK 0x0FFFFFFL
#line 23 "chaosnoise.org"
#line 144 "chaosnoise.org"
void sk_chaosnoise_chaos(sk_chaosnoise *cn, SKFLT chaos)
{
    cn->chaos = chaos;
}
#line 172 "chaosnoise.org"
void sk_chaosnoise_rate(sk_chaosnoise *cn, SKFLT rate)
{
    cn->rate = rate;
}
#line 200 "chaosnoise.org"
void sk_chaosnoise_init(sk_chaosnoise *cn, int sr, SKFLT init)
{
#line 74 "chaosnoise.org"
cn->y[0] = init;
cn->y[1] = 0;
#line 94 "chaosnoise.org"
cn->phs = 0;
#line 128 "chaosnoise.org"
cn->maxlens = SK_CHAOSNOISE_PHSMAX / (SKFLT) sr;
#line 157 "chaosnoise.org"
sk_chaosnoise_chaos(cn, 1.5);
#line 185 "chaosnoise.org"
sk_chaosnoise_rate(cn, 8000);
#line 203 "chaosnoise.org"
}
#line 216 "chaosnoise.org"
SKFLT sk_chaosnoise_tick(sk_chaosnoise *cn)
{
    SKFLT out;
    out = 0;

#line 234 "chaosnoise.org"
cn->phs += floor(cn->rate * cn->maxlens);
#line 216 "chaosnoise.org"
#line 256 "chaosnoise.org"
if (cn->phs >= SK_CHAOSNOISE_PHSMAX) {
    SKFLT y;

    cn->phs &= SK_CHAOSNOISE_PHSMSK;
    y = fabs(cn->chaos * cn->y[0] - cn->y[1] - 0.05);
    cn->y[1] = cn->y[0];
    cn->y[0] = y;
}
#line 216 "chaosnoise.org"
#line 271 "chaosnoise.org"
out = cn->y[0];
#line 224 "chaosnoise.org"

    return out;
}
#line 23 "chaosnoise.org"
