#line 24 "clkphs.org"
#include <stdio.h>
#define SK_CLKPHS_PRIV
#include "clkphs.h"

#line 62 "clkphs.org"
void sk_clkphs_init(sk_clkphs *c)
{
#line 94 "clkphs.org"
c->counter = 0;
#line 108 "clkphs.org"
c->inc = 0;
#line 121 "clkphs.org"
c->phs = 0;
#line 142 "clkphs.org"
c->start = -1;
#line 156 "clkphs.org"
c->wait = 0;
#line 171 "clkphs.org"
c->spillover = 0;
#line 190 "clkphs.org"
c->correction = 1.0;
#line 65 "clkphs.org"
    c->correction = 1.0;
}
#line 204 "clkphs.org"
SKFLT sk_clkphs_tick(sk_clkphs *c, SKFLT clk)
{
    SKFLT out;
    SKFLT phs;

    out = 0;

#line 228 "clkphs.org"
if (clk != 0) {
#line 241 "clkphs.org"
if (c->start == -1) {
    /* start initial count */
    c->start = 1;
    c->counter = 0;
    return -1;
}
#line 228 "clkphs.org"
#line 258 "clkphs.org"
else if (c->start == 1) {
    /* first counter finished */
    c->start = 0;
    c->phs = 0;
    c->inc = 1.0 / c->counter;
    c->counter = 0;
}
#line 228 "clkphs.org"
#line 273 "clkphs.org"
else {
    /* reset phasor and flags */
    c->inc = 1.0 / c->counter;
    c->counter = 0;
    c->correction = 1.0;
    c->wait = 0;

#line 302 "clkphs.org"
if (c->spillover) {
    /* too much spillage. abandon ship */
    c->spillover = 0;
    c->phs = 0;
}
#line 281 "clkphs.org"

#line 316 "clkphs.org"
if (c->phs >= 1.0) {
    c->phs -= 1.0;
} else if (c->phs < 0.0) {
    c->phs += 1.0;
} else {
#line 334 "clkphs.org"
/* too slow! spill-over mode */
c->spillover = 1;

if (c->phs != 0) {
#line 352 "clkphs.org"
c->correction = 1.0 / c->phs;
#line 339 "clkphs.org"
} else {
#line 363 "clkphs.org"
c->correction = 1.0;
c->spillover = 0;
#line 341 "clkphs.org"
}
#line 322 "clkphs.org"
}
#line 283 "clkphs.org"
}
#line 232 "clkphs.org"
}
#line 204 "clkphs.org"
#line 374 "clkphs.org"
c->counter++;
#line 204 "clkphs.org"
#line 383 "clkphs.org"
if (c->start != 0) return -1;
if (c->wait) return 1;
#line 204 "clkphs.org"
#line 392 "clkphs.org"
out = c->phs;
#line 204 "clkphs.org"
#line 400 "clkphs.org"
phs = c->phs;

#line 413 "clkphs.org"
phs += c->inc*c->correction;
#line 400 "clkphs.org"
#line 421 "clkphs.org"
if (phs >= 1.0) {
#line 448 "clkphs.org"
if (c->spillover) {
    /* now back to our regularly scheduled program */
    c->spillover = 0;
    phs -= 1.0;
}
#line 421 "clkphs.org"
#line 435 "clkphs.org"
else {
    c->wait = 1;
}
#line 424 "clkphs.org"
}
#line 400 "clkphs.org"
#line 459 "clkphs.org"
c->phs = phs;
#line 400 "clkphs.org"
#line 216 "clkphs.org"


    return out;
}
#line 24 "clkphs.org"
