#line 13 "noise.org"
#ifndef SK_NOISE_H
#define SK_NOISE_H

#ifndef SKFLT
#define SKFLT float
#endif

typedef struct sk_noise sk_noise;

#ifdef SK_NOISE_PRIV
struct sk_noise {
    unsigned long rng;
};
#endif

void sk_noise_init(sk_noise *n, unsigned long seed);
SKFLT sk_noise_tick(sk_noise *n);
#endif
