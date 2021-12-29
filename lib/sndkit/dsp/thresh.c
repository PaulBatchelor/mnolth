#line 32 "thresh.org"
#define SK_THRESH_PRIV
#include "thresh.h"
#line 43 "thresh.org"
SKFLT sk_thresh_stateless(SKFLT in, SKFLT prev, SKFLT thresh, int mode)
{
    SKFLT out;

    if (in == prev) return 0;

    out = 0;
    switch (mode) {
        case 0:
            out = in > thresh && prev < thresh;
            break;
        case 1:
            out = in < thresh && prev > thresh;
            break;
        case 2:
            out =
                (in > thresh && prev < thresh) ||
                (in < thresh && prev > thresh);
            break;
        default:
            break;
    }

    return out;
}
#line 95 "thresh.org"
void sk_thresh_init(sk_thresh *th)
{
    sk_thresh_mode(th, 0);
    sk_thresh_value(th, 0);
    th->prev = -1;
    th->init = 0;
}
#line 106 "thresh.org"
void sk_thresh_mode(sk_thresh *th, int mode)
{
    th->mode = mode;
}
#line 114 "thresh.org"
void sk_thresh_value(sk_thresh *th, SKFLT value)
{
    th->value = value;
}
#line 122 "thresh.org"
SKFLT sk_thresh_tick(sk_thresh *th, SKFLT in)
{
    SKFLT out;

    if (th->init) {
        th->prev = in;
        th->init = 0;
        return 0;
    }

    out = sk_thresh_stateless(in, th->prev, th->value, th->mode);

    th->prev = in;

    return out;
}
#line 32 "thresh.org"
