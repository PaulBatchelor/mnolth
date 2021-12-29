#line 38 "glottis.org"
#ifndef SK_GLOTTIS_H
#define SK_GLOTTIS_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 65 "glottis.org"
typedef struct sk_glottis sk_glottis;
#line 38 "glottis.org"
#line 58 "glottis.org"
void sk_glottis_init(sk_glottis *glot, SKFLT sr);
#line 112 "glottis.org"
void sk_glottis_freq(sk_glottis *glot, SKFLT freq);
#line 127 "glottis.org"
void sk_glottis_tenseness(sk_glottis *glot, SKFLT tenseness);
#line 348 "glottis.org"
void sk_glottis_srand(sk_glottis *glot, unsigned long s);
#line 394 "glottis.org"
SKFLT sk_glottis_tick(sk_glottis *glot);
#line 47 "glottis.org"

#ifdef SK_GLOTTIS_PRIV
#line 87 "glottis.org"
struct sk_glottis {
    SKFLT freq;
    SKFLT tenseness;
    SKFLT Rd;
    SKFLT waveform_length;
    SKFLT time_in_waveform;

    SKFLT alpha;
    SKFLT E0;
    SKFLT epsilon;
    SKFLT shift;
    SKFLT delta;
    SKFLT Te;
    SKFLT omega;

    SKFLT T;
#line 340 "glottis.org"
unsigned long rng;
#line 104 "glottis.org"
};
#line 50 "glottis.org"
#endif
#endif
