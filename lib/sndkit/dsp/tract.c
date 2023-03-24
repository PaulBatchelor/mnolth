#line 49 "tract.org"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SK_TRACT_PRIV
#include "tract.h"

#line 563 "tract.org"
static void calculate_nose_reflections(sk_tract *tr);
#line 697 "tract.org"
static void calculate_reflections(sk_tract *tr);
#line 781 "tract.org"
static SKFLT tract_compute(sk_tract *tr, SKFLT in);
#line 49 "tract.org"
#line 102 "tract.org"
void sk_tract_init(sk_tract *tr)
{
#line 121 "tract.org"
tr->glottal_reflection = 0.75;
tr->lip_reflection = -0.85;
#line 135 "tract.org"
tr->n = 44;
#line 148 "tract.org"
memset(tr->diameter, 0, 44 * sizeof(SKFLT));
#line 157 "tract.org"
{
    int i;

    for(i = 0; i < tr->n; i++) {
        SKFLT diameter = 0;
        if(i < 7 - 0.5) {
            diameter = 0.6;
        } else if( i < 12) {
            diameter = 1.1;
        } else {
            diameter = 1.5;
        }

        tr->diameter[i] = diameter;
    }
}
#line 148 "tract.org"
#line 188 "tract.org"
memset(tr->A, 0, 44 * sizeof(SKFLT));
memset(tr->reflection, 0, 44 * sizeof(SKFLT));
#line 209 "tract.org"
memset(tr->junction_outL, 0, 44 * sizeof(SKFLT));
memset(tr->L, 0, 44 * sizeof(SKFLT));
memset(tr->junction_outR, 0, 44 * sizeof(SKFLT));
memset(tr->R, 0, 44 * sizeof(SKFLT));
#line 228 "tract.org"
tr->shape = NULL;
tr->ud = NULL;
#line 243 "tract.org"
sk_tract_use_diameters(tr, 1);
#line 485 "tract.org"
tr->velum = 0;
tr->nose_start = 17;
tr->reflection_left = 0;
tr->reflection_right = 0;
memset(tr->noseL, 0, 28 * sizeof(SKFLT));
memset(tr->noseR, 0, 28 * sizeof(SKFLT));
memset(tr->noseA, 0, 28 * sizeof(SKFLT));
#line 501 "tract.org"
sk_tract_use_velum(tr, 0);
#line 539 "tract.org"
{
    int i;
    for (i = 0; i < 28; i++) {
        SKFLT d;
        d = 2 * ((SKFLT)i / 28);
        if(d < 1) {
            d = 0.4 + 1.6 * d;
        } else {
            d = 0.5 + 1.5*(2-d);
        }
        d = d < 1.9 ? d : 1.9;
        tr->nose_diameter[i] = d;
    }
    calculate_nose_reflections(tr);
}
#line 105 "tract.org"
    calculate_reflections(tr);
}
#line 256 "tract.org"
void sk_tract_use_diameters(sk_tract *tr, int mode)
{
    tr->use_diameters = mode;
}
#line 276 "tract.org"
SKFLT* sk_tract_diameters(sk_tract *tr)
{
    return tr->diameter;
}
#line 292 "tract.org"
int sk_tract_size(sk_tract *tr)
{
    return tr->n;
}
#line 326 "tract.org"
void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud)
{
    tract->shape = shape;
    tract->ud = ud;
}
#line 355 "tract.org"
static void set_diameters(sk_tract *tract,
                          int blade_start,
                          int lip_start,
                          int tip_start,
                          SKFLT tongue_index,
                          SKFLT tongue_diameter,
                          SKFLT *diameters)
{
    int i;
    SKFLT t;
    SKFLT fixed_tongue_diameter;
    SKFLT curve;
    int grid_offset = 0;

    for(i = blade_start; i < lip_start; i++) {
        t = 1.1 * M_PI *
            (SKFLT)(tongue_index - i)/(tip_start - blade_start);
        fixed_tongue_diameter = 2+(tongue_diameter-2)/1.5;
        curve = (1.5 - fixed_tongue_diameter + grid_offset) * cos(t);
        if(i == blade_start - 2 || i == lip_start - 1) curve *= 0.8;
        if(i == blade_start || i == lip_start - 2) curve *= 0.94;
        diameters[i] = 1.5 - curve;
    }
}

void sk_tract_tongue_shape(sk_tract *tract,
                           SKFLT position,
                           SKFLT diameter)
{
    position = 12 + 16.0 * position;
    diameter = 3.5 * diameter;
    set_diameters(tract, 10, 39, 32,
                  position, diameter, tract->diameter);
}
#line 413 "tract.org"
void sk_tract_set_area_shape(sk_tract *tr, SKFLT *areas, int sz)
{
    int n;
    SKFLT last;

    last = 0;

    for (n = 0; n < tr->n; n++) {
        if (n >= sz) {
            tr->A[n] = last;
        } else {
            tr->A[n] = areas[n];
            last = areas[n];
        }
    }
}
#line 440 "tract.org"
SKFLT* sk_tract_areas(sk_tract *tr)
{
    return tr->A;
}
#line 511 "tract.org"
void sk_tract_use_velum(sk_tract *tr, int mode)
{
    tr->use_velum = mode;
}
#line 524 "tract.org"
void sk_tract_velum(sk_tract *tract, SKFLT velum)
{
    tract->velum = velum;
}
#line 568 "tract.org"
static void calculate_nose_reflections(sk_tract *tr)
{
    int i;

    for(i = 0; i < 28; i++) {
        tr->noseA[i] = tr->nose_diameter[i] * tr->nose_diameter[i];
    }

    for(i = 1; i < 28; i++) {
        tr->nose_reflection[i] = (tr->noseA[i - 1] - tr->noseA[i]) /
            (tr->noseA[i-1] + tr->noseA[i]);
    }
}
#line 675 "tract.org"
SKFLT sk_tract_tick(sk_tract *tr, SKFLT in)
{
    SKFLT tmp;
    SKFLT out;

    out = 0;

    calculate_reflections(tr);
    tmp = 0;

    /* compute twice for forwards/backwards propogation */
    tmp += tract_compute(tr, in);
    tmp += tract_compute(tr, in);

    out = tmp * 0.125;

    return out;
}
#line 702 "tract.org"
static void calculate_reflections(sk_tract *tr)
{
    int i;
    SKFLT *diam;

    diam = tr->diameter;

#line 723 "tract.org"
if (tr->shape != NULL) {
    if (tr->use_diameters)
        tr->shape(tr, tr->diameter, tr->ud);
    else
        tr->shape(tr, tr->A, tr->ud);
}
#line 702 "tract.org"
#line 737 "tract.org"
if (tr->use_diameters) {
    for(i = 0; i < tr->n; i++) {
        tr->A[i] = diam[i] * diam[i];
    }
}
#line 702 "tract.org"
#line 766 "tract.org"
for(i = 1; i < tr->n; i++) {
    if(tr->A[i] == 0) {
        tr->reflection[i] = 0.999; /* to prevent bad behavior if 0 */
    } else {
        tr->reflection[i] =
            (tr->A[i - 1] - tr->A[i]) / (tr->A[i - 1] + tr->A[i]);
    }
}
#line 702 "tract.org"
#line 594 "tract.org"
if (tr->use_velum) {
    SKFLT sum;
    tr->nose_diameter[0] = tr->velum;
    tr->noseA[0] = tr->nose_diameter[0] * tr->nose_diameter[0];
    sum = tr->A[tr->nose_start] + tr->A[tr->nose_start + 1] + tr->noseA[0];
    tr->reflection_left = (SKFLT)(2 * tr->A[tr->nose_start] - sum) / sum;
    tr->reflection_right = (SKFLT)(2 * tr->A[tr->nose_start + 1] - sum) / sum;
    tr->reflection_nose = (SKFLT)(2 * tr->noseA[0] - sum) / sum;
}
#line 713 "tract.org"
}
#line 786 "tract.org"
static SKFLT tract_compute(sk_tract *tr, SKFLT in)
{
    SKFLT  r, w;
    int i;
    SKFLT out;

    out = 0;

#line 809 "tract.org"
tr->junction_outR[0] = tr->L[0] * tr->glottal_reflection + in;
tr->junction_outL[tr->n - 1] = tr->R[tr->n - 1] * tr->lip_reflection;
#line 786 "tract.org"
#line 830 "tract.org"
for(i = 1; i < tr->n; i++) {
    r = tr->reflection[i];
    w = r * (tr->R[i - 1] + tr->L[i]);
    tr->junction_outR[i] = tr->R[i - 1] - w;
    tr->junction_outL[i - 1] = tr->L[i] + w;
}
#line 617 "tract.org"
if (tr->use_velum) {
    i = tr->nose_start;
    r = tr->reflection_left;

    tr->junction_outL[i - 1] = r*tr->R[i-1] +
        (1+r)*(tr->noseL[0]+tr->L[i]);

    r = tr->reflection_right;
    tr->junction_outR[i] = r*tr->L[i] +
        (1+r)*(tr->R[i-1]+tr->noseL[0]);

    r = tr->reflection_nose;
    tr->nose_junc_outR[0] =
        r * tr->noseL[0]+(1+r)*(tr->L[i]+tr->R[i-1]);
    tr->nose_junc_outL[28 - 1] =
        tr->noseR[28 - 1] * tr->lip_reflection;

    for(i = 1; i < 28; i++) {
        w = tr->nose_reflection[i] * (tr->noseR[i-1] + tr->noseL[i]);
        tr->nose_junc_outR[i] = tr->noseR[i - 1] - w;
        tr->nose_junc_outL[i - 1] = tr->noseL[i] + w;
    }

    for(i = 0; i < 28; i++) {
        tr->noseR[i] = tr->nose_junc_outR[i];
        tr->noseL[i] = tr->nose_junc_outL[i];
    }
}
#line 830 "tract.org"
#line 786 "tract.org"
#line 844 "tract.org"
for(i = 0; i < tr->n; i++) {
    tr->R[i] = tr->junction_outR[i]*0.999;
    tr->L[i] = tr->junction_outL[i]*0.999;
}
#line 786 "tract.org"
#line 855 "tract.org"
out = tr->R[tr->n - 1];
#line 786 "tract.org"
#line 649 "tract.org"
if (tr->use_velum) {
    out += tr->noseR[28 - 1];
}
#line 799 "tract.org"

    return out;
}
#line 49 "tract.org"
