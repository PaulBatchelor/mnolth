#line 42 "qgliss.org"
#define SK_QGLISS_PRIV
#include "qgliss.h"
#line 60 "qgliss.org"
void sk_qgliss_gliss(sk_qgliss *qg, SKFLT gliss)
{
    qg->gliss = gliss;
}
#line 82 "qgliss.org"
void sk_qgliss_init(sk_qgliss *qg, SKFLT *tab, int sz)
{
#line 108 "qgliss.org"
qg->init = 1;
#line 121 "qgliss.org"
qg->gliss = 0.5;
#line 137 "qgliss.org"
qg->gl = qg->gliss;
qg->igl = 1.0 / qg->gl;
#line 150 "qgliss.org"
qg->phs = -1;
#line 164 "qgliss.org"
qg->prv = -1;
qg->nxt = -1;
#line 179 "qgliss.org"
qg->tab = tab;
qg->sz = sz;
#line 85 "qgliss.org"
}
#line 190 "qgliss.org"
#line 321 "qgliss.org"
static SKFLT getval(SKFLT *tab, int sz, SKFLT in)
{
    int i;
    int pos;

    in *= sz;

    pos = 0;

    for (i = 1; i < sz; i++) {
        if (in < i) break;
        pos++;
    }

    return tab[pos];
}
#line 196 "qgliss.org"
SKFLT sk_qgliss_tick(sk_qgliss *qg, SKFLT in, SKFLT phs)
{
    SKFLT out;
    SKFLT p, n;
    out = 0;

#line 226 "qgliss.org"
if (qg->init) {
    qg->init = 0;
    qg->prv = getval(qg->tab, qg->sz, in);
    qg->nxt = qg->prv;
    qg->phs = phs;
    if (qg->gliss > 0 && qg->gliss < 1.0) {
        qg->gl = qg->gliss;
        qg->igl = 1.0 / (1 - qg->gliss);
    }
    return qg->prv;
}
#line 196 "qgliss.org"
#line 256 "qgliss.org"
if (phs < qg->phs) {
    /* reset */
    qg->prv = qg->nxt;
    qg->nxt = getval(qg->tab, qg->sz, in);

    if (qg->gliss > 0 && qg->gliss < 1.0) {
        qg->gl = qg->gliss;
        qg->igl = 1.0 / (1.0 - qg->gliss);
    }
}
#line 204 "qgliss.org"

    p = qg->prv;
    n = qg->nxt;

#line 287 "qgliss.org"
qg->phs = phs; /* cache phasor */

if (phs < qg->gl) {
    phs = 0;
} else {
    phs -= qg->gl; /* bias */
    if (phs < 0) phs = 0; /* clamp negative values */
    phs *= qg->igl; /* scale */
    phs = phs * phs * phs; /* apply cubic function */
}
#line 209 "qgliss.org"

#line 305 "qgliss.org"
out = (1 - phs)*p + phs*n;
#line 211 "qgliss.org"
    return out;
}
#line 42 "qgliss.org"
