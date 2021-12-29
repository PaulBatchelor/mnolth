#line 29 "bitnoise.org"
/* tangled from sndkit. do not edit by hand */
#ifndef SK_BITNOISE_H
#define SK_BITNOISE_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 52 "bitnoise.org"
typedef struct sk_bitnoise sk_bitnoise;
#line 29 "bitnoise.org"
#line 67 "bitnoise.org"
void sk_bitnoise_init(sk_bitnoise *bn, int sr);
#line 170 "bitnoise.org"
void sk_bitnoise_rate(sk_bitnoise *bn, SKFLT rate);
#line 201 "bitnoise.org"
void sk_bitnoise_mode(sk_bitnoise *bn, int mode);
#line 226 "bitnoise.org"
SKFLT sk_bitnoise_tick(sk_bitnoise *bn);
#line 39 "bitnoise.org"

#ifdef SK_BITNOISE_PRIV
#line 57 "bitnoise.org"
struct sk_bitnoise {
#line 100 "bitnoise.org"
SKFLT maxlens;
#line 113 "bitnoise.org"
uint32_t phs;
#line 125 "bitnoise.org"
uint16_t lfsr;
#line 140 "bitnoise.org"
int pos;
#line 153 "bitnoise.org"
SKFLT saved;
#line 183 "bitnoise.org"
SKFLT rate;
#line 214 "bitnoise.org"
int m;
#line 59 "bitnoise.org"
};
#line 42 "bitnoise.org"
#endif

#endif
