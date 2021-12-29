#line 35 "noise.org"
#define SK_NOISE_PRIV
#define SK_NOISE_RANDMAX 2147483648
#include "noise.h"

void sk_noise_init(sk_noise *n, unsigned long seed)
{
    n->rng = seed;
}

SKFLT sk_noise_tick(sk_noise *n)
{
    SKFLT out;

    out = (SKFLT)n->rng / SK_NOISE_RANDMAX;
    out *= 2;
    out -= 1;
    n->rng = (1103515245 * n->rng + 12345) % SK_NOISE_RANDMAX;

    return out;
}
