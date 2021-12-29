#line 12 "vowshape.org"
#include <math.h>
#include "vowshape.h"
#line 41 "vowshape.org"
void sk_vowshape_constant(SKFLT *a,
                          int start, int len, SKFLT val)
{
    int i;

    for (i = 0; i < len; i++) {
        a[start + i] = val;
    }
}
#line 65 "vowshape.org"
void sk_vowshape_pyramid(SKFLT *a,
                         int center, int nsteps,
                         SKFLT beg, SKFLT end)
{
    int i;

    for (i = 0; i < nsteps; i++) {
        SKFLT y;
        y = (1.0 - ((SKFLT)i / nsteps));
        a[center + i] = (1 - y)*beg + y*end;
    }

    for (i = 0; i < nsteps; i++) {
        SKFLT y;
        y = (1.0 - ((SKFLT)i / nsteps));
        a[center - i] = (1 - y)*beg + y*end;
    }
}
#line 98 "vowshape.org"
void sk_vowshape_parabola(SKFLT *a,
                          int center, int nsteps,
                          SKFLT beg, SKFLT end)
{
    int i;
    double dt;

    dt = 1.0/nsteps;

    for (i = 0; i < nsteps; i++) {
        SKFLT y;
        y = (-((i * dt)*(i * dt)) + 1.0);
        a[center + i] = (1 - y)*beg + y * end;
    }

    for (i = 0; i < nsteps; i++) {
        SKFLT y;
        y = (-((-i * dt)*(-i * dt)) + 1.0);
        a[center - i] = (1 - y)*beg + y * end;
    }
}
#line 135 "vowshape.org"
void sk_vowshape_exponential(SKFLT *a,
                             SKFLT slope,
                             int start,
                             int nsteps,
                             SKFLT beg, SKFLT end)
{
    double dt;
    int i;

    if (nsteps == 1) dt = 0;
    else dt = 1.0/(nsteps - 1);

    for (i = 0; i < nsteps; i++) {
        SKFLT x, y;
        x = i * dt;
        y = (1.0 - exp(x*slope)) / (1 - exp(slope));
        a[start + i] = (1 - y)*beg + y * end;
    }
}
#line 204 "vowshape.org"
void sk_vowshape_drm(SKFLT *a, int len, SKFLT *vals)
{
    int pos;
    int i;
    SKFLT unit;
    int nsmps;

    unit = len / 18.0;

    pos = 0;

    /* R1: ~2 units */

    nsmps = floor(unit * 2);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[0];
        pos++;
    }

    /* R2: ~1 unit */

    nsmps = floor(unit * 1);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[1];
        pos++;
    }

    /* R3: ~2 units */

    nsmps = floor(unit * 2);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[2];
        pos++;
    }

    /* R4: ~4 units */

    nsmps = floor(unit * 4);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[3];
        pos++;
    }

    /* R5: ~4 units */

    nsmps = floor(unit * 4);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[4];
        pos++;
    }

    /* R6: ~2 units */

    nsmps = floor(unit * 2);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[5];
        pos++;
    }

    /* R7: ~1 units */

    nsmps = floor(unit * 1);

    for (i = 0; i < nsmps; i++) {
        a[pos] = vals[6];
        pos++;
    }

    /* R8: ~2 units. Finish it off */

    for (i = pos; i < len; i++) {
        a[i] = vals[7];
    }
}
#line 12 "vowshape.org"
