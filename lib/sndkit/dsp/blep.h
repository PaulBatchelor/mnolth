#line 61 "blep.org"
#ifndef SK_BLEP_H
#define SK_BLEP_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 112 "blep.org"
typedef struct sk_blep sk_blep;
#line 61 "blep.org"
#line 126 "blep.org"
void sk_blep_init(sk_blep *blep, int sr);
#line 231 "blep.org"
void sk_blep_freq(sk_blep *blep, SKFLT freq);
#line 303 "blep.org"
SKFLT sk_blep_saw(sk_blep *blep);
#line 342 "blep.org"
SKFLT sk_blep_square(sk_blep *blep);
#line 384 "blep.org"
SKFLT sk_blep_triangle(sk_blep *blep);
#line 70 "blep.org"

#ifdef SK_BLEP_PRIV
#line 117 "blep.org"
struct sk_blep {
#line 143 "blep.org"
SKFLT freq;
SKFLT pfreq;
#line 157 "blep.org"
SKFLT onedsr;
#line 171 "blep.org"
SKFLT inc;
SKFLT phs;
#line 192 "blep.org"
SKFLT A;
SKFLT prev;
#line 213 "blep.org"
SKFLT R, x, y;
#line 119 "blep.org"
};
#line 73 "blep.org"
#endif

#endif
