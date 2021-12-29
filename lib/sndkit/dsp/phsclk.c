#line 25 "phsclk.org"
#include <math.h>
#define SK_PHSCLK_PRIV
#include "phsclk.h"
#line 84 "phsclk.org"
void sk_phsclk_init(sk_phsclk *pc)
{
#line 72 "phsclk.org"
pc->prev = -1;
#line 116 "phsclk.org"
sk_phsclk_nticks(pc, 4);
#line 87 "phsclk.org"
}
#line 100 "phsclk.org"
void sk_phsclk_nticks(sk_phsclk *pc, SKFLT nticks)
{
    pc->nticks = nticks;
}
#line 167 "phsclk.org"
SKFLT sk_phsclk_tick(sk_phsclk *pc, SKFLT in)
{
    SKFLT out;
    out = 0;

    if (in < 1) {
        if (pc->nticks == 1) {
#line 162 "phsclk.org"
if (pc->prev > in || pc->prev == -1) out = 1;
#line 175 "phsclk.org"
        } else {
#line 133 "phsclk.org"
int i, pi;
SKFLT s, ps;
s = in * pc->nticks;
ps = pc->prev * pc->nticks;

i = floor(s);
pi = floor(ps);

if (i != pi) out = 1;
#line 177 "phsclk.org"
        }
    }

    pc->prev = in;

    return out;
}
#line 25 "phsclk.org"
