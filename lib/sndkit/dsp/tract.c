#line 47 "tract.org"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SK_TRACT_PRIV
#include "tract.h"

#line 484 "tract.org"
static void calculate_reflections(sk_tract *tr);
#line 567 "tract.org"
static SKFLT tract_compute(sk_tract *tr, SKFLT in);
#line 47 "tract.org"
#line 100 "tract.org"
void sk_tract_init(sk_tract *tr)
{
#line 119 "tract.org"
tr->glottal_reflection = 0.75;
tr->lip_reflection = -0.85;
#line 133 "tract.org"
tr->n = 44;
#line 146 "tract.org"
memset(tr->diameter, 0, 44 * sizeof(SKFLT));
#line 155 "tract.org"
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
#line 146 "tract.org"
#line 186 "tract.org"
memset(tr->A, 0, 44 * sizeof(SKFLT));
memset(tr->reflection, 0, 44 * sizeof(SKFLT));
#line 207 "tract.org"
memset(tr->junction_outL, 0, 44 * sizeof(SKFLT));
memset(tr->L, 0, 44 * sizeof(SKFLT));
memset(tr->junction_outR, 0, 44 * sizeof(SKFLT));
memset(tr->R, 0, 44 * sizeof(SKFLT));
#line 226 "tract.org"
tr->shape = NULL;
tr->ud = NULL;
#line 241 "tract.org"
sk_tract_use_diameters(tr, 1);
#line 103 "tract.org"
    calculate_reflections(tr);
}
#line 254 "tract.org"
void sk_tract_use_diameters(sk_tract *tr, int mode)
{
    tr->use_diameters = mode;
}
#line 271 "tract.org"
SKFLT* sk_tract_diameters(sk_tract *tr)
{
    return tr->diameter;
}
#line 287 "tract.org"
int sk_tract_size(sk_tract *tr)
{
    return tr->n;
}
#line 321 "tract.org"
void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud)
{
    tract->shape = shape;
    tract->ud = ud;
}
#line 350 "tract.org"
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
#line 408 "tract.org"
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
#line 435 "tract.org"
SKFLT* sk_tract_areas(sk_tract *tr)
{
    return tr->A;
}
#line 462 "tract.org"
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
#line 489 "tract.org"
static void calculate_reflections(sk_tract *tr)
{
    int i;
    SKFLT *diam;

    diam = tr->diameter;

#line 509 "tract.org"
if (tr->shape != NULL) {
    if (tr->use_diameters)
        tr->shape(tr, tr->diameter, tr->ud);
    else
        tr->shape(tr, tr->A, tr->ud);
}
#line 489 "tract.org"
#line 523 "tract.org"
if (tr->use_diameters) {
    for(i = 0; i < tr->n; i++) {
        tr->A[i] = diam[i] * diam[i];
    }
}
#line 489 "tract.org"
#line 552 "tract.org"
for(i = 1; i < tr->n; i++) {
    if(tr->A[i] == 0) {
        tr->reflection[i] = 0.999; /* to prevent bad behavior if 0 */
    } else {
        tr->reflection[i] =
            (tr->A[i - 1] - tr->A[i]) / (tr->A[i - 1] + tr->A[i]);
    }
}
#line 499 "tract.org"
}
#line 572 "tract.org"
static SKFLT tract_compute(sk_tract *tr, SKFLT in)
{
    SKFLT  r, w;
    int i;
    SKFLT out;

    out = 0;

#line 594 "tract.org"
tr->junction_outR[0] = tr->L[0] * tr->glottal_reflection + in;
tr->junction_outL[tr->n - 1] = tr->R[tr->n - 1] * tr->lip_reflection;
#line 572 "tract.org"
#line 615 "tract.org"
for(i = 1; i < tr->n; i++) {
    r = tr->reflection[i];
    w = r * (tr->R[i - 1] + tr->L[i]);
    tr->junction_outR[i] = tr->R[i - 1] - w;
    tr->junction_outL[i - 1] = tr->L[i] + w;
}
#line 572 "tract.org"
#line 628 "tract.org"
for(i = 0; i < tr->n; i++) {
    tr->R[i] = tr->junction_outR[i]*0.999;
    tr->L[i] = tr->junction_outL[i]*0.999;
}
#line 572 "tract.org"
#line 639 "tract.org"
out = tr->R[tr->n - 1];
#line 584 "tract.org"

    return out;
}
#line 47 "tract.org"
