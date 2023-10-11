#line 40 "metro.org"
#define SK_METRO_PRIV
#include "metro.h"
#line 61 "metro.org"
void sk_metro_init(sk_metro *m, int sr)
{
#line 83 "metro.org"
sk_metro_freq(m, 2.0);
#line 91 "metro.org"
m->phs = 0;
#line 99 "metro.org"
m->init = 1;
#line 107 "metro.org"
m->onedsr = 1.0 / sr;
#line 64 "metro.org"
}
#line 119 "metro.org"
void sk_metro_freq(sk_metro *m, SKFLT freq)
{
    m->freq = freq;
}
#line 141 "metro.org"
void sk_metro_reset(sk_metro *m)
{
    m->init = 1;
    m->phs = 0;
}
#line 167 "metro.org"
SKFLT sk_metro_tick(sk_metro *m)
{
    SKFLT out;
    SKFLT phs;
    SKFLT freq;

    phs = m->phs;
    freq = m->freq;

    out = 0;

    if (m->init) {
        m->init = 0;
        out = 1.0;
    } else {
        phs += freq * m->onedsr;

        if (phs >= 1) {
            out = 1.0;
            phs -= 1.0;
        }
    }

    m->phs = phs;
    return out;
}
#line 40 "metro.org"
