#line 40 "env.org"
#include <math.h>
#define SK_ENV_PRIV
#include "env.h"
#line 125 "env.org"
enum {
    MODE_ZERO,
    MODE_ATTACK,
    MODE_HOLD,
    MODE_RELEASE
};
#line 255 "env.org"
#define EPS 5e-8
#line 40 "env.org"
#line 56 "env.org"
void sk_env_init(sk_env *env, int sr)
{
    env->sr = sr;
#line 93 "env.org"
env->timer = 0;
env->inc = 0;
#line 110 "env.org"
env->atk_env = 0;
env->rel_env = 0;
#line 135 "env.org"
env->mode = MODE_ZERO;
#line 148 "env.org"
env->prev = 0;
#line 180 "env.org"
sk_env_attack(env, 0.1);
env->patk = -1;
#line 209 "env.org"
sk_env_release(env, 0.1);
env->prel= -1;
#line 236 "env.org"
sk_env_hold(env, 0.1);
env->phold = -1;
#line 60 "env.org"
}
#line 164 "env.org"
void sk_env_attack(sk_env *env, SKFLT atk)
{
    env->atk = atk;
}
#line 193 "env.org"
void sk_env_release(sk_env *env, SKFLT rel)
{
    env->rel= rel;
}
#line 222 "env.org"
void sk_env_hold(sk_env *env, SKFLT hold)
{
    env->hold = hold;
}
#line 268 "env.org"
SKFLT sk_env_tick(sk_env *env, SKFLT trig)
{
    SKFLT out;
    out = 0;

#line 298 "env.org"
if (trig != 0) {
    env->mode = MODE_ATTACK;

    if (env->patk != env->atk) {
        env->patk = env->atk;
        env->atk_env = exp(-1.0 / (env->atk * env->sr));
    }
}
#line 274 "env.org"

    switch (env->mode) {
#line 394 "env.org"
case MODE_ZERO:
    break;
#line 274 "env.org"
#line 321 "env.org"
case MODE_ATTACK: {
    out = env->atk_env*env->prev + (1.0 - env->atk_env);

    if ((out - env->prev) <= EPS) {
        env->mode = MODE_HOLD;
        env->timer = 0;

        if (env->phold != env->hold) {
            if (env->hold <= 0) {
                env->inc = 1.0;
            } else {
                env->phold = env->hold;
                env->inc = 1.0 / (env->hold * env->sr);
            }
        }
    }

    env->prev = out;
    break;
}
#line 274 "env.org"
#line 354 "env.org"
case MODE_HOLD: {
    out = env->prev;
    env->timer += env->inc;

    if (env->timer >= 1.0) {
        env->mode = MODE_RELEASE;

        if (env->prel != env->rel) {
            env->prel = env->rel;
            env->rel_env = exp(-1 / (env->rel * env->sr));
        }
    }
    break;
}
#line 274 "env.org"
#line 376 "env.org"
case MODE_RELEASE: {
    out = env->rel_env*env->prev;
    env->prev = out;

    if (out <= EPS) {
       env->mode = MODE_ZERO;
    }
    break;
}
#line 280 "env.org"
        default:
            break;
    }
    return out;
}
#line 40 "env.org"
