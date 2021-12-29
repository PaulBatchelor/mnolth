#line 17 "adsr.org"
#ifndef SK_ADSR_H
#define SK_ADSR_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 45 "adsr.org"
typedef struct sk_adsr sk_adsr;
#line 17 "adsr.org"
#line 80 "adsr.org"
void sk_adsr_init(sk_adsr *adsr, int sr);
#line 112 "adsr.org"
void sk_adsr_attack(sk_adsr *adsr, SKFLT atk);
#line 125 "adsr.org"
void sk_adsr_decay(sk_adsr *adsr, SKFLT dec);
#line 138 "adsr.org"
void sk_adsr_sustain(sk_adsr *adsr, SKFLT sus);
#line 151 "adsr.org"
void sk_adsr_release(sk_adsr *adsr, SKFLT rel);
#line 190 "adsr.org"
SKFLT sk_adsr_tick(sk_adsr *adsr, SKFLT gate);
#line 26 "adsr.org"

#ifdef SK_ADSR_PRIV
#line 50 "adsr.org"
struct sk_adsr {
    /* times */
    SKFLT atk;
    SKFLT dec;
    SKFLT sus;
    SKFLT rel;

    /* timer */
    unsigned long timer;
    unsigned long atk_time;

    /* filter coefficients */
    SKFLT a;
    SKFLT b;
    SKFLT y;
    SKFLT x;

    /* previous sample */
    SKFLT prev;

    /* envelope mode */
    int mode;

    /* sample rate */
    int sr;
};
#line 29 "adsr.org"
#endif

#endif
