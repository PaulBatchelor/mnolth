#line 30 "tenv.org"
#define SK_TENV_PRIV
#include "tenv.h"
#line 61 "tenv.org"
void sk_tenv_init(sk_tenv *te, int sr)
{
    te->pos = 0;
    te->last = 0;
    te->atk = 0.1;
    te->hold = 0.3;
    te->rel = 0.2;
    te->sigmode = 0;
    te->input = 0;

    te->sr = sr;
    te->atk_end = te->sr * te->atk;
    te->rel_start = te->sr * (te->atk + te->hold);
    te->atk_slp = 1.0 / te->atk_end;
    te->rel_slp = -1.0 / (te->sr * te->rel);
    te->totaldur = sr * (te->atk + te->hold + te->rel);

    te->started = 0;
}
#line 84 "tenv.org"
#line 148 "tenv.org"
static void reinit(sk_tenv *te)
{
    te->pos = 0;
    te->atk_end = te->sr * te->atk;
    te->rel_start = te->sr * (te->atk + te->hold);
    te->atk_slp = 1.0 / te->atk_end;
    te->rel_slp = -1.0 / (te->sr * te->rel);
    te->totaldur = te->sr * (te->atk + te->hold + te->rel);
}
#line 84 "tenv.org"
#line 109 "tenv.org"
static SKFLT envelope(sk_tenv *env)
{
    SKFLT sig;
    unsigned long pos;
    SKFLT out;

    out = 0;
    sig = 0;
    pos = env->pos;

    if (pos < env->atk_end) {
        sig = env->last + env->atk_slp;
    } else if (pos < env->rel_start) {
        sig = 1.0;
    } else if (pos < env->totaldur) {
        sig = env->last + env->rel_slp;
    } else{
        sig = 0.0;
    }

    sig = (sig > 1.0) ? 1.0 : sig;
    sig = (sig < 0.0) ? 0.0 : sig;

    /* Internal input signal mode */
    if (env->sigmode) {
        out = env->input * sig;
    } else {
        out = sig;
    }

    env->pos++;
    env->last = sig;

    return out;
}
#line 91 "tenv.org"
SKFLT sk_tenv_tick(sk_tenv *te, SKFLT trig)
{
    SKFLT out;
    out = 0;

    if (trig != 0) {
        reinit(te);
        te->started = 1;
    }

    if (te->started) out = envelope(te);

    return out;
}
#line 166 "tenv.org"
void sk_tenv_attack(sk_tenv *te, SKFLT atk)
{
    te->atk = atk;
}
#line 179 "tenv.org"
void sk_tenv_hold(sk_tenv *te, SKFLT hold)
{
    te->hold = hold;
}
#line 192 "tenv.org"
void sk_tenv_release(sk_tenv *te, SKFLT rel)
{
    te->rel = rel;
}
#line 30 "tenv.org"
