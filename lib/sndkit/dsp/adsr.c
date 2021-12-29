#line 36 "adsr.org"
#include <math.h>
#define SK_ADSR_PRIV
#include "adsr.h"
#line 185 "adsr.org"
enum {CLEAR, ATTACK, DECAY, RELEASE};
#line 36 "adsr.org"
#line 85 "adsr.org"
void sk_adsr_init(sk_adsr *adsr, int sr)
{
    adsr->sr = sr;
    /* initial ADSR times and sustain */
    sk_adsr_attack(adsr, 0.1);
    sk_adsr_decay(adsr, 0.1);
    sk_adsr_sustain(adsr, 0.5);
    sk_adsr_release(adsr, 0.3);

    /* zero out timer, filter coefs, and memory */
    adsr->timer = 0;
    adsr->a = 0;
    adsr->b = 0;
    adsr->y = 0;
    adsr->x = 0;
    adsr->prev = 0;

    /* set up initial attack time timer */
    adsr->atk_time = adsr->atk * adsr->sr;

    /* initial state: CLEAR */
    adsr->mode = CLEAR;
}
#line 117 "adsr.org"
void sk_adsr_attack(sk_adsr *adsr, SKFLT atk)
{
    adsr->atk = atk;
}
#line 130 "adsr.org"
void sk_adsr_decay(sk_adsr *adsr, SKFLT dec)
{
    adsr->dec = dec;
}
#line 143 "adsr.org"
void sk_adsr_sustain(sk_adsr *adsr, SKFLT sus)
{
    adsr->sus = sus;
}
#line 156 "adsr.org"
void sk_adsr_release(sk_adsr *adsr, SKFLT rel)
{
    adsr->rel = rel;
}
#line 195 "adsr.org"
static SKFLT tau2pole(SKFLT tau, int sr)
{
    return exp(-1.0 / (tau * sr));
}

static SKFLT adsr_filter(sk_adsr *p)
{
    p->y = p->b * p->x  + p->a * p->y;
    return p->y;
}

SKFLT sk_adsr_tick(sk_adsr *adsr, SKFLT gate)
{
    SKFLT out;
    SKFLT pole;
    out = 0;

    if (adsr->prev < gate && adsr->mode != DECAY) {
        adsr->mode = ATTACK;
        adsr->timer = 0;

        /* 60% attack time was done empirically */
        pole = tau2pole(adsr->atk * 0.6, adsr->sr);
        adsr->atk_time = adsr->atk * adsr->sr;
        adsr->a = pole;
        adsr->b = 1 - pole;
    } else if (adsr->prev > gate) {
        adsr->mode = RELEASE;
        pole = tau2pole(adsr->rel, adsr->sr);
        adsr->a = pole;
        adsr->b = 1 - pole;
    }

    adsr->x = gate;
    adsr->prev = gate;

    switch (adsr->mode) {
        case CLEAR:
            out = 0;
            break;
        case ATTACK:
            adsr->timer++;
            out = adsr_filter(adsr);
            if (out > 0.99) {
                adsr->mode = DECAY;
                pole = tau2pole(adsr->dec, adsr->sr);
                adsr->a = pole;
                adsr->b = 1 - pole;
            }
            break;
        case DECAY:
        case RELEASE:
            adsr->x *= adsr->sus;
            out = adsr_filter(adsr);
        default:
            break;
    }

    return out;
}
#line 36 "adsr.org"
