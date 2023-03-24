#line 66 "tract.org"
#ifndef SK_TRACT_H
#define SK_TRACT_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 83 "tract.org"
typedef struct sk_tract sk_tract;
#line 66 "tract.org"
#line 97 "tract.org"
void sk_tract_init(sk_tract *tr);
#line 251 "tract.org"
void sk_tract_use_diameters(sk_tract *tr, int mode);
#line 271 "tract.org"
SKFLT* sk_tract_diameters(sk_tract *tr);
#line 287 "tract.org"
int sk_tract_size(sk_tract *tr);
#line 319 "tract.org"
void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud);
#line 348 "tract.org"
void sk_tract_tongue_shape(sk_tract *tract,
                           SKFLT position,
                           SKFLT diameter);
#line 408 "tract.org"
void sk_tract_set_area_shape(sk_tract *tr, SKFLT *areas, int sz);
#line 435 "tract.org"
SKFLT* sk_tract_areas(sk_tract *tr);
#line 506 "tract.org"
void sk_tract_use_velum(sk_tract *tr, int mode);
#line 519 "tract.org"
void sk_tract_velum(sk_tract *tract, SKFLT velum);
#line 661 "tract.org"
SKFLT sk_tract_tick(sk_tract *tract, SKFLT in);
#line 73 "tract.org"
#ifdef SK_TRACT_PRIV
#line 88 "tract.org"
struct sk_tract {
#line 115 "tract.org"
SKFLT glottal_reflection;
SKFLT lip_reflection;
#line 130 "tract.org"
int n;
#line 143 "tract.org"
SKFLT diameter[44];
#line 182 "tract.org"
SKFLT A[44];
SKFLT reflection[44];
#line 201 "tract.org"
SKFLT junction_outL[44];
SKFLT L[44];
SKFLT junction_outR[44];
SKFLT R[44];
#line 222 "tract.org"
void *ud;
void (*shape)(sk_tract *, SKFLT *, void *);
#line 238 "tract.org"
int use_diameters;
#line 470 "tract.org"
    SKFLT nose_diameter[28];
    SKFLT noseL[28];
    SKFLT noseR[28];
    SKFLT noseA[28];
    SKFLT nose_reflection[28];
    SKFLT nose_junc_outL[28];
    SKFLT nose_junc_outR[28];
    SKFLT velum;
    SKFLT reflection_left;
    SKFLT reflection_right;
    SKFLT reflection_nose;
    int nose_start;
#line 496 "tract.org"
int use_velum;
#line 90 "tract.org"
};
#line 75 "tract.org"
#endif
#endif
