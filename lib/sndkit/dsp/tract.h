#line 64 "tract.org"
#ifndef SK_TRACT_H
#define SK_TRACT_H
#ifndef SKFLT
#define SKFLT float
#endif
#line 81 "tract.org"
typedef struct sk_tract sk_tract;
#line 64 "tract.org"
#line 95 "tract.org"
void sk_tract_init(sk_tract *tr);
#line 249 "tract.org"
void sk_tract_use_diameters(sk_tract *tr, int mode);
#line 266 "tract.org"
SKFLT* sk_tract_diameters(sk_tract *tr);
#line 282 "tract.org"
int sk_tract_size(sk_tract *tr);
#line 314 "tract.org"
void sk_tract_shaper(sk_tract *tract,
                     void (*shape)(sk_tract *, SKFLT *, void *),
                     void *ud);
#line 343 "tract.org"
void sk_tract_tongue_shape(sk_tract *tract,
                           SKFLT position,
                           SKFLT diameter);
#line 403 "tract.org"
void sk_tract_set_area_shape(sk_tract *tr, SKFLT *areas, int sz);
#line 430 "tract.org"
SKFLT* sk_tract_areas(sk_tract *tr);
#line 448 "tract.org"
SKFLT sk_tract_tick(sk_tract *tract, SKFLT in);
#line 71 "tract.org"
#ifdef SK_TRACT_PRIV
#line 86 "tract.org"
struct sk_tract {
#line 113 "tract.org"
SKFLT glottal_reflection;
SKFLT lip_reflection;
#line 128 "tract.org"
int n;
#line 141 "tract.org"
SKFLT diameter[44];
#line 180 "tract.org"
SKFLT A[44];
SKFLT reflection[44];
#line 199 "tract.org"
SKFLT junction_outL[44];
SKFLT L[44];
SKFLT junction_outR[44];
SKFLT R[44];
#line 220 "tract.org"
void *ud;
void (*shape)(sk_tract *, SKFLT *, void *);
#line 236 "tract.org"
int use_diameters;
#line 88 "tract.org"
};
#line 73 "tract.org"
#endif
#endif
