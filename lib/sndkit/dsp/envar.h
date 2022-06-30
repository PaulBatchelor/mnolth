#line 31 "envar.org"
#ifndef SK_ENVAR_H
#define SK_ENVAR_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 67 "envar.org"
typedef struct sk_envar sk_envar;
#line 39 "envar.org"

#ifdef SK_ENVAR_PRIV
#line 67 "envar.org"
#line 234 "envar.org"
struct envar_timing_param {
    SKFLT cur;
    SKFLT prev;
    SKFLT cached;
};
#line 73 "envar.org"
struct sk_envar {
    int sr;
#line 87 "envar.org"
SKFLT y;
#line 102 "envar.org"
SKFLT a1;
#line 182 "envar.org"
SKFLT pgate;
int state;
#line 261 "envar.org"
struct envar_timing_param atk;
struct envar_timing_param rel;
#line 76 "envar.org"
};
#line 42 "envar.org"
#endif

#line 53 "envar.org"
void sk_envar_init(sk_envar *env, int sr);
#line 202 "envar.org"
void sk_envar_attack(sk_envar *env, SKFLT atk);
void sk_envar_release(sk_envar *env, SKFLT rel);
#line 283 "envar.org"
SKFLT sk_envar_tick(sk_envar *env, SKFLT gate);
#line 45 "envar.org"
#endif
