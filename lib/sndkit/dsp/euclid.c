#line 63 "euclid.org"
#include <stdint.h>
#define SK_EUCLID_PRIV
#include "euclid.h"

#line 395 "euclid.org"
static void append(sk_euclid_bitpat *a, sk_euclid_bitpat *b);
#line 63 "euclid.org"
#line 165 "euclid.org"
uint32_t sk_euclid_simple(int p, int n, int r)
{
    int bucket;
    int i;
    uint32_t out;

    out = 0;

    if (n > 32) return 0;

    bucket = 0;

    for (i = 0; i < n; i++) {
        int pulse;
        int bitpos;

        bitpos = (i + r) % n;
        bucket += p;
        pulse = bucket >= n;
        bucket %= n;

        out |= (pulse << bitpos);
    }

    return out;
}
#line 213 "euclid.org"
int euclid_simple_stateless(int p, int n, int r, int i)
{
    return (((p * (i + r)) % n) + p) >= n;
}
#line 229 "euclid.org"
int euclid_simple_stateless2(int p, int n, int r, int i)
{
    return (p * (i + r)) % n < p;
}
#line 409 "euclid.org"
static void append(sk_euclid_bitpat *a, sk_euclid_bitpat *b)
{
    a->pat |= b->pat << a->nbits;
    a->nbits += b->nbits;
}
#line 442 "euclid.org"
unsigned long sk_euclid_pattern(int p, int n)
{
    sk_euclid_bitpat a, b;
    unsigned long out;
    int i, k;
    int bitpos;

    if (n < 1 || n > 32) return 0;

    a.pat = 1;
    a.nbits = 1;
    a.npats = p;

    b.pat = 0;
    b.nbits = 1;
    b.npats = n - p;

    while (b.npats > 1) {
        uint16_t prev_pat;
        uint8_t prev_nbits;

        if (a.npats > b.npats) {
            uint8_t tmp;

            tmp = a.npats;
            a.npats = b.npats;
            b.npats = tmp - b.npats;

            prev_pat = a.pat;
            prev_nbits = a.nbits;
        } else {
            b.npats = b.npats - a.npats;
            prev_pat = b.pat;
            prev_nbits = b.nbits;
        }

        append(&a, &b);

        b.pat = prev_pat;
        b.nbits = prev_nbits;
    }

    out = 0;

    /* write bitpatterns to bits */

    bitpos = 0;
    for (k = 0; k < a.npats; k++) {
        for (i = 0; i < a.nbits; i++) {
            int bit;
            bit = (a.pat & (1 << i)) > 0;
            out |= (1 << bitpos) * bit;
            bitpos++;
        }
    }

    for (k = 0; k < b.npats; k++) {
        for (i = 0; i < b.nbits; i++) {
            int bit;
            bit = (b.pat & (1 << i)) > 0;
            out |= (1 << bitpos) * bit;
            bitpos++;
        }
    }

    return out;
}
#line 608 "euclid.org"
void sk_euclid_init(sk_euclid *e)
{
    sk_euclid_pulses(e, 1);
    sk_euclid_length(e, 4);
    e->changed = 0;
    e->bits = 0;
    e->pos = 0;
}
#line 631 "euclid.org"
void sk_euclid_pulses(sk_euclid *e, int p)
{
    if (p != e->pulses && p > 0 && p <= 32) {
        e->pulses = p;
        e->changed = 1;
    }
}

void sk_euclid_length(sk_euclid *e, int n)
{
    if (n != e->len && n > 1 && n <= 32) {
        e->len = n;
        e->changed = 1;
    }
}
#line 672 "euclid.org"
SKFLT sk_euclid_tick(sk_euclid *e, SKFLT trig)
{
    SKFLT out;

    out = 0;

    if (trig) {

        /* update pattern if needed */

        if (e->changed) {
            e->bits = sk_euclid_pattern(e->pulses, e->len);

            /* wraparound if position is greater than length */
            e->pos %= e->len;
            e->changed = 0;
        }

        /* compute output */

        out = (e->bits & (1 << e->pos)) > 0;

        /* updated sequence position */
        e->pos = (e->pos + 1) % e->len;
    }

    return out;
}
#line 63 "euclid.org"
