#line 19 "vowshape.org"
#ifndef SK_VOWSHAPE_H
#define SK_VOWSHAPE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 35 "vowshape.org"
void sk_vowshape_constant(SKFLT *a,
                          int start, int len, SKFLT val);
#line 58 "vowshape.org"
void sk_vowshape_pyramid(SKFLT *a,
                         int center, int nsteps,
                         SKFLT beg, SKFLT end);
#line 91 "vowshape.org"
void sk_vowshape_parabola(SKFLT *a,
                          int center, int nsteps,
                          SKFLT beg, SKFLT end);
#line 126 "vowshape.org"
void sk_vowshape_exponential(SKFLT *a,
                             SKFLT slope,
                             int start,
                             int nsteps,
                             SKFLT beg, SKFLT end);
#line 199 "vowshape.org"
void sk_vowshape_drm(SKFLT *a, int len, SKFLT *vals);
#line 27 "vowshape.org"
#endif
