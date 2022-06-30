#line 52 "shelf.org"
#ifndef SK_SHELF_H
#define SK_SHELF_H

#ifndef SKFLT
#define SKFLT float
#endif

#ifdef SK_SHELF_PRIV
#line 105 "shelf.org"
struct sk_shelf {
    int sr;
    SKFLT a[2];
    SKFLT b[3];
    SKFLT x[2];
    SKFLT y[2];
    SKFLT gain;
    SKFLT freq;
    SKFLT slope;
    int changed;
    SKFLT T;
};
#line 61 "shelf.org"
#endif

#line 88 "shelf.org"
typedef struct sk_shelf sk_shelf;
#line 61 "shelf.org"
#line 93 "shelf.org"
void sk_shelf_init(sk_shelf *shf, int sr);
#line 147 "shelf.org"
void sk_shelf_frequency(sk_shelf *shf, SKFLT freq);
void sk_shelf_gain(sk_shelf *shf, SKFLT gain);
void sk_shelf_slope(sk_shelf *shf, SKFLT slope);
#line 235 "shelf.org"
SKFLT sk_shelf_high_tick(sk_shelf *shf, SKFLT in);
#line 358 "shelf.org"
SKFLT sk_shelf_low_tick(sk_shelf *shf, SKFLT in);
#line 65 "shelf.org"
#endif
