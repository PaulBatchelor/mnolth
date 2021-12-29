#line 38 "rephasor.org"
#include <math.h>
#define SK_REPHASOR_PRIV
#include "rephasor.h"
#line 67 "rephasor.org"
void sk_rephasor_init(sk_rephasor *rp)
{
#line 89 "rephasor.org"
rp->extphs = -1;
rp->phs = -1;
#line 105 "rephasor.org"
sk_rephasor_scale(rp, 1.0);
#line 70 "rephasor.org"
}
#line 117 "rephasor.org"
void sk_rephasor_scale(sk_rephasor *rp, SKFLT scale)
{
    rp->scale = scale;
}
#line 133 "rephasor.org"
SKFLT sk_rephasor_tick(sk_rephasor *rp, SKFLT in)
{
    SKFLT out;
    SKFLT delta;
    out = 0;
#line 155 "rephasor.org"
if (rp->phs < 0) {
   rp->phs = in;
}

out = rp->phs;
#line 133 "rephasor.org"
#line 170 "rephasor.org"
if (rp->extphs < 0 || in < rp->extphs) delta = 0;
else delta = in - rp->extphs;
#line 133 "rephasor.org"
#line 179 "rephasor.org"
rp->extphs = in;
#line 133 "rephasor.org"
#line 188 "rephasor.org"
rp->phs += delta * rp->scale;
while (rp->phs >= 1.0) rp->phs -= 1.0;
while (rp->phs < 0.0) rp->phs += 1.0;
#line 142 "rephasor.org"
    return out;
}
#line 239 "rephasor.org"
void sk_rephasorx_init(sk_rephasorx *rpx)
{
    sk_rephasor_init(&rpx->rp);
    rpx->counter = -1;
    rpx->limit = -1;
    rpx->N = 1;
}
#line 257 "rephasor.org"
void sk_rephasorx_factor(sk_rephasorx *rpx, SKFLT N)
{
    rpx->N = N;
}
#line 287 "rephasor.org"
SKFLT sk_rephasorx_tick_div(sk_rephasorx *rpx, SKFLT in)
{
    SKFLT out;
    SKFLT delta;
    out = 0;

    if (rpx->counter < 0) {
        /* initialize */
        rpx->counter = 0;
        rpx->limit = floor(rpx->N);
        rpx->rp.scale = rpx->limit;
    }

    if (rpx->rp.phs < 0) {
        rpx->rp.phs = in;
    }

    out = rpx->rp.phs;

    if (rpx->rp.extphs < 0 || in < rpx->rp.extphs) delta = 0;
    else delta = in - rpx->rp.extphs;

    if (in < rpx->rp.extphs) {
        /* hard reset */
        rpx->rp.phs = in;

        rpx->counter = 0;
        rpx->limit = floor(rpx->N);
        rpx->rp.scale = rpx->limit;
    }

    if (rpx->counter < rpx->limit) {
        SKFLT phs;
        rpx->rp.extphs = in;

        phs = rpx->rp.phs;
        phs += delta * rpx->rp.scale;
        while (phs >= 1.0) phs -= 1.0;
        while (phs < 0.0) phs += 1.0;

        if (phs < rpx->rp.phs) {
            rpx->counter++;
        }

        rpx->rp.phs = phs;
    } else {
       /* hang out at 1, wait for external phasor to reset */
       rpx->rp.phs = 1;
    }

    return out;
}
#line 355 "rephasor.org"
SKFLT sk_rephasorx_tick_mul(sk_rephasorx *rpx, SKFLT in)
{
    SKFLT out;
    SKFLT delta;
    out = 0;

    if (rpx->counter < 0) {
        /* initialize */
        rpx->counter = 0;
        rpx->limit = floor(rpx->N);
        rpx->rp.scale = 1.0 / rpx->limit;
    }

    if (rpx->rp.phs < 0) {
        rpx->rp.phs = in;
    }

    out = rpx->rp.phs;

    if (rpx->rp.extphs < 0 || in < rpx->rp.extphs) delta = 0;
    else delta = in - rpx->rp.extphs;

    if (in < rpx->rp.extphs) {
        rpx->counter++;
    }

    if (rpx->counter >= rpx->limit) {
        /* hard reset */
        rpx->rp.phs = in;

        rpx->counter = 0;
        rpx->limit = floor(rpx->N);
        rpx->rp.scale = 1.0 / rpx->limit;
    }


    if (rpx->rp.extphs < 0 || in < rpx->rp.extphs) delta = 0;
    else delta = in - rpx->rp.extphs;
    rpx->rp.phs += delta * rpx->rp.scale;
    rpx->rp.extphs = in;

    /* hang at 1 and wait until external phasor resets */
    if (rpx->rp.phs > 1) rpx->rp.phs = 1;

    return out;
}
#line 38 "rephasor.org"
