#line 26 "tsmp.org"
#include <math.h>
#define SK_TSMP_PRIV
#include "tsmp.h"

#line 55 "tsmp.org"
void sk_tsmp_init(sk_tsmp *tsmp, SKFLT *tab, unsigned long sz)
{
#line 74 "tsmp.org"
tsmp->pos = -1;
#line 100 "tsmp.org"
tsmp->play = 1.0;
#line 127 "tsmp.org"
tsmp->tab = tab;
tsmp->tabsz = sz;
#line 58 "tsmp.org"
}
#line 110 "tsmp.org"
void sk_tsmp_rate(sk_tsmp *ts, SKFLT play)
{
    ts->play = play;
}
#line 138 "tsmp.org"
SKFLT sk_tsmp_tick(sk_tsmp *tsmp, SKFLT trig)
{
    SKFLT smp;

    smp = 0;

    if (trig != 0) {
#line 81 "tsmp.org"
tsmp->pos = 0;
#line 146 "tsmp.org"
    }

#line 159 "tsmp.org"
if (tsmp->pos >= 0 && tsmp->pos < (tsmp->tabsz - 1)) {
    SKFLT play;
#line 173 "tsmp.org"
unsigned int ipos;
double fract;
SKFLT v1, v2;
SKFLT *tab;
#line 162 "tsmp.org"
    play = tsmp->play;
#line 181 "tsmp.org"
tab = tsmp->tab;
ipos = floor(tsmp->pos);
fract = tsmp->pos - ipos;
v1 = tab[ipos];
v2 = tab[ipos + 1];
smp = (v1 + (v2 - v1) * fract);
#line 164 "tsmp.org"
    tsmp->pos += play;
}
#line 149 "tsmp.org"

    return smp;
}
#line 26 "tsmp.org"
