#line 31 "tdiv.org"
#define SK_TDIV_PRIV
#include "tdiv.h"
#line 59 "tdiv.org"
void sk_tdiv_init(sk_tdiv *tdiv)
{
    tdiv->cnt = 0;
    sk_tdiv_offset(tdiv, 0);
    sk_tdiv_divide(tdiv, 4);
}
#line 91 "tdiv.org"
void sk_tdiv_divide(sk_tdiv *tdiv, int divide)
{
    if (divide > 0) tdiv->div = divide;
}
#line 105 "tdiv.org"
void sk_tdiv_offset(sk_tdiv *tdiv, int offset)
{
    tdiv->off = offset;
}
#line 133 "tdiv.org"
SKFLT sk_tdiv_tick(sk_tdiv *td, SKFLT trig)
{
    SKFLT out;

    out = 0.0;

    if (trig != 0) {
        if (td->cnt == td->off) out = 1.0;
        td->cnt++;
        if (td->cnt >= td->div) td->cnt = 0;
    }

    return out;
}
#line 31 "tdiv.org"
