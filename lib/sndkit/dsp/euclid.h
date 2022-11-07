#line 45 "euclid.org"
#ifndef SK_EUCLID_H
#define SK_EUCLID_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 369 "euclid.org"
typedef struct sk_euclid_bitpat sk_euclid_bitpat;
#line 572 "euclid.org"
typedef struct sk_euclid sk_euclid;
#line 52 "euclid.org"

#ifdef SK_EUCLID_PRIV
#line 382 "euclid.org"
struct sk_euclid_bitpat {
    uint16_t pat;
    uint8_t nbits;
    uint8_t npats;
};
#line 589 "euclid.org"
struct sk_euclid {
    int pos;
    int pulses;
    int len;
    unsigned long bits;
    int changed;
};
#line 55 "euclid.org"
#endif

#line 150 "euclid.org"
uint32_t sk_euclid_simple(int p, int n, int r);
#line 208 "euclid.org"
int euclid_simple_stateless(int p, int n, int r, int i);
#line 224 "euclid.org"
int euclid_simple_stateless2(int p, int n, int r, int i);
#line 430 "euclid.org"
unsigned long sk_euclid_pattern(int p, int n);
#line 603 "euclid.org"
void sk_euclid_init(sk_euclid *e);
#line 625 "euclid.org"
void sk_euclid_pulses(sk_euclid *e, int p);
void sk_euclid_length(sk_euclid *e, int n);
#line 656 "euclid.org"
SKFLT sk_euclid_tick(sk_euclid *e, SKFLT trig);
#line 58 "euclid.org"
#endif
