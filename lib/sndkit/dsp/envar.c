#line 18 "envar.org"
#include <math.h>
#include <stddef.h>

#define SK_ENVAR_PRIV
#include "envar.h"

#line 194 "envar.org"
enum {ATTACK, RELEASE};
#line 18 "envar.org"
#line 243 "envar.org"
static void init_param(struct envar_timing_param *p, SKFLT t);
#line 18 "envar.org"
#line 58 "envar.org"
void sk_envar_init(sk_envar *env, int sr)
{
    env->sr = sr;
#line 92 "envar.org"
env->y = 0;
#line 107 "envar.org"
env->a1 = 0;
#line 188 "envar.org"
env->pgate = 0;
env->state = ATTACK;
#line 267 "envar.org"
init_param(&env->atk, 0.01);
init_param(&env->rel, 0.01);
#line 62 "envar.org"
}
#line 208 "envar.org"
void sk_envar_attack(sk_envar *env, SKFLT atk)
{
    env->atk.cur = atk;
}

void sk_envar_release(sk_envar *env, SKFLT rel)
{
    env->rel.cur = rel;
}
#line 251 "envar.org"
static void init_param(struct envar_timing_param *p, SKFLT t)
{
    p->cur = t;
    p->prev = -t;
    p->cached = 0;
}
#line 293 "envar.org"
SKFLT sk_envar_tick(sk_envar *env, SKFLT gate)
{
    SKFLT out;
    struct envar_timing_param *p;
    out = 0;
    p = NULL;
#line 315 "envar.org"
if (gate > 0.5 && env->pgate <= 0.5) {
    env->state = ATTACK;
} else if (gate < 0.5 && env->pgate >= 0.5) {
    env->state = RELEASE;
}
env->pgate = gate;
#line 293 "envar.org"
#line 329 "envar.org"
if (env->state == ATTACK) p = &env->atk;
else p = &env->rel;

if (p->cur != p->prev) {
    SKFLT tau;
    p->prev = p->cur;
    tau = p->cur / log(1000.0);
    tau *= env->sr;

    if (tau > 0) p->cached = exp(-1.0/tau);
}
#line 293 "envar.org"
#line 358 "envar.org"
{
    SKFLT a1;
    SKFLT b0;
    SKFLT y;

    a1 = p->cached;
    b0 = 1 - a1;
    y = env->y;

    out = b0*gate + a1*y;
}
#line 293 "envar.org"
#line 376 "envar.org"
env->y = out;
#line 303 "envar.org"
    return out;
}
#line 18 "envar.org"
