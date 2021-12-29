#line 22 "env.org"
#ifndef SK_ENV_H
#define SK_ENV_H
#ifndef SKFLT
#define SKFLT float
#endif

#line 66 "env.org"
typedef struct sk_env sk_env;
#line 22 "env.org"
#line 51 "env.org"
void sk_env_init(sk_env *env, int sr);
#line 159 "env.org"
void sk_env_attack(sk_env *env, SKFLT atk);
#line 188 "env.org"
void sk_env_release(sk_env *env, SKFLT rel);
#line 217 "env.org"
void sk_env_hold(sk_env *env, SKFLT hold);
#line 263 "env.org"
SKFLT sk_env_tick(sk_env *env, SKFLT trig);
#line 30 "env.org"

#ifdef SK_ENV_PRIV
#line 71 "env.org"
struct sk_env {
    int sr;
#line 87 "env.org"
float timer;
float inc;
#line 104 "env.org"
SKFLT atk_env;
SKFLT rel_env;
#line 120 "env.org"
int mode;
#line 143 "env.org"
SKFLT prev;
#line 172 "env.org"
SKFLT atk;
SKFLT patk;
#line 201 "env.org"
SKFLT rel;
SKFLT prel;
#line 230 "env.org"
SKFLT hold;
SKFLT phold;
#line 74 "env.org"
};
#line 33 "env.org"
#endif

#endif
