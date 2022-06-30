#line 157 "rephasor.org"
#include <math.h>
#define SK_REPHASOR_PRIV
#include "rephasor.h"
#line 223 "rephasor.org"
void sk_rephasor_init(sk_rephasor *rp)
{
    rp->pr = 0;
    rp->pc[0] = 0;
    rp->pc[1] = 0;
    rp->pe[0] = 0;
    rp->pe[1] = 0;
    rp->c = 1.0;
    rp->s = 1.0;
    rp->si = 1.0;

    rp->ir = 0.0;
    rp->ic = 0.0;
}
#line 251 "rephasor.org"
void sk_rephasor_scale(sk_rephasor *rp, SKFLT scale)
{
    if (scale != rp->s) {
        rp->s = scale;
        rp->si = 1.0 / scale;
    }
}
#line 275 "rephasor.org"
/* implementation of a truncated phasor */

static SKFLT phasor(SKFLT phs, SKFLT inc)
{
    phs += inc;

    if (phs > 1.0) return 0;

    return phs;
}

SKFLT sk_rephasor_tick(sk_rephasor *rp, SKFLT ext)
{
    SKFLT pr, pc;
    SKFLT out;


    /* delta function of \theta_e */
    if (ext > rp->pe[0]) {
        rp->ir = ext - rp->pe[0];
    }

    /* compute main rephasor \theta_r */
    pr = phasor(rp->pr, rp->s * rp->ir * rp->c);

    /* delta function of \theta_r */
    if (pr > rp->pr) {
        rp->ic = pr - rp->pr;
    }

    /* compute rephasor \theta_c */
    pc = phasor(rp->pc[0], rp->si * rp->ic);

    /* compute correction coefficient */
    if (rp->pc[1] != 0) {
        rp->c = rp->pe[1] / rp->pc[1];
    }

#line 343 "rephasor.org"
if (rp->c > 2.0 || rp->c < 0.5) rp->c = 1.0;
#line 314 "rephasor.org"

    out = pr;

    /* update state */

    rp->pr = pr;

    rp->pc[1] = rp->pc[0];
    rp->pc[0] = pc;

    rp->pe[1] = rp->pe[0];
    rp->pe[0] = ext;

    return out;
}
#line 362 "rephasor.org"
SKFLT sk_rephasor_tick_nosync(sk_rephasor *rp, SKFLT ext)
{
    SKFLT out;

    if (ext > rp->pe[0]) {
        rp->ir = ext - rp->pe[0];
    }

    rp->pr = phasor(rp->pr, rp->s * rp->ir);
    rp->pe[0] = ext;

    out = rp->pr;

    return out;
}
#line 157 "rephasor.org"
