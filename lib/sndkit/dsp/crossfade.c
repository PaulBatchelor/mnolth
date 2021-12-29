#line 24 "crossfade.org"
#include <math.h>
#include "crossfade.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SQRT2 1.41421356237309504880

#line 47 "crossfade.org"
SKFLT sk_crossfade_linear(SKFLT a, SKFLT b, SKFLT pos)
{
    return (1 - pos)*a + pos*b;
}
#line 62 "crossfade.org"
SKFLT sk_crossfade_sqrt(SKFLT a, SKFLT b, SKFLT pos)
{
    return a * sqrt(pos) + b * sqrt(1 - pos);
}
#line 77 "crossfade.org"
SKFLT sk_crossfade_equal(SKFLT a, SKFLT b, SKFLT pos)
{
    SKFLT c;
    SKFLT s;
    c = cos(M_PI * pos * 0.5);
    s = sin(M_PI * pos * 0.5);

    a *= SQRT2 * (c + s) * 0.5;
    b *= SQRT2 * (c - s) * 0.5;

    return a + b;
}
#line 24 "crossfade.org"
