#line 70 "shelf.org"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SK_SHELF_PRIV
#include "shelf.h"
#line 200 "shelf.org"
static SKFLT compute_filter(sk_shelf *shf, SKFLT in);
#line 70 "shelf.org"
#line 121 "shelf.org"
void sk_shelf_init(sk_shelf *shf, int sr)
{
    int i;

    shf->T = 1.0 / sr;

    for (i = 0; i < 2; i++) {
        shf->x[i] = 0;
        shf->y[i] = 0;
        shf->a[i] = 0;
        shf->b[i] = 0;
    }

    shf->b[2] = 0;

    sk_shelf_frequency(shf, 1000);
    sk_shelf_gain(shf, 6);
    sk_shelf_slope(shf, 1.0);
}
#line 154 "shelf.org"
void sk_shelf_frequency(sk_shelf *shf, SKFLT freq)
{

    if (freq != shf->freq) {
        shf->freq = freq;
        shf->changed = 1;
    }
}

void sk_shelf_gain(sk_shelf *shf, SKFLT gain)
{
    if (gain != shf->gain) {
        shf->gain = gain;
        shf->changed = 1;
    }
}

void sk_shelf_slope(sk_shelf *shf, SKFLT slope)
{
    if (slope != shf->slope && slope > 0) {
        shf->slope = slope;
        shf->changed = 1;
    }
}
#line 205 "shelf.org"
static SKFLT compute_filter(sk_shelf *shf, SKFLT in)
{
    SKFLT out;
    SKFLT *b, *a, *x, *y;

    out = 0;

    b = shf->b;
    a = shf->a;
    x = shf->x;
    y = shf->y;

    out =
        b[0]*in + b[1]*x[0] + b[2]*x[1]
        - a[0]*y[0] - a[1]*y[1];

    y[1] = y[0];
    y[0] = out;

    x[1] = x[0];
    x[0] = in;

    return out;
}
#line 302 "shelf.org"
SKFLT sk_shelf_high_tick(sk_shelf *shf, SKFLT in)
{
    SKFLT out;

    out = 0;

    if (shf->changed) {
        SKFLT ia0;
        SKFLT alpha;
        SKFLT A;
        SKFLT k; /* sqrt(A)*alpha*2.0 */
        SKFLT omegaT;
        SKFLT *a, *b;
        SKFLT co; /* cos(omegaT) */

        A = pow(10.0, shf->gain / 40.0);
        omegaT = 2.0 * M_PI * shf->freq * shf->T;
        alpha = sin(omegaT) * 0.5 *
        sqrt((A + (1.0/A))*((1.0/shf->slope) - 1.0) + 2.0);
        co = cos(omegaT);
        k = sqrt(A)*alpha*2.0;

        a = shf->a;
        b = shf->b;

        ia0 = (A+1.0) - (A-1.0)*co + k;

        if (ia0 != 0) ia0 = 1.0 / ia0;
        else ia0 = 0;


        b[0] = A * ((A+1.0) + (A-1.0)*co + k);
        b[0] *= ia0;
        b[1] = -2.0*A*((A-1.0) + (A+1.0)*co);
        b[1] *= ia0;
        b[2] = A*((A+1.0) + (A-1.0)*co - k);
        b[2] *= ia0;

        a[0] = 2.0*((A-1.0) - (A+1.0)*co);
        a[0] *= ia0;
        a[1] = (A+1.0) - (A-1.0)*co - k;
        a[1] *= ia0;

        shf->changed = 0;
    }


    out = compute_filter(shf, in);
    return out;
}
#line 381 "shelf.org"
SKFLT sk_shelf_low_tick(sk_shelf *shf, SKFLT in)
{
    SKFLT out;

    out = 0;

    if (shf->changed) {
        SKFLT ia0;
        SKFLT alpha;
        SKFLT A;
        SKFLT k; /* sqrt(A)*alpha*2.0 */
        SKFLT omegaT;
        SKFLT *a, *b;
        SKFLT co; /* cos(omegaT) */

        A = pow(10.0, shf->gain / 40.0);
        omegaT = 2.0 * M_PI * shf->freq * shf->T;
        alpha = sin(omegaT) * 0.5 *
        sqrt((A + (1.0/A))*((1.0/shf->slope) - 1.0) + 2.0);
        co = cos(omegaT);
        k = sqrt(A)*alpha*2.0;


        a = shf->a;
        b = shf->b;

        ia0 = (A+1.0) + (A-1.0)*co + k;

        if (ia0 != 0) ia0 = 1.0 / ia0;
        else ia0 = 0;

        b[0] = A * ((A+1.0) - (A-1.0)*co + k);
        b[0] *= ia0;
        b[1] = 2.0*A*((A-1.0) - (A+1.0)*co);
        b[1] *= ia0;
        b[2] = A*((A+1.0) - (A-1.0)*co - k);
        b[2] *= ia0;

        a[0] = -2.0*((A-1.0) + (A+1.0)*co);
        a[0] *= ia0;
        a[1] = (A+1.0) + (A-1.0)*co - k;
        a[1] *= ia0;

        shf->changed = 0;
    }

    out = compute_filter(shf, in);
    return out;
}
#line 70 "shelf.org"
