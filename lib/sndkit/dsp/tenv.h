#line 10 "tenv.org"
#ifndef SK_TENV_H
#define SK_TENV_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 37 "tenv.org"
typedef struct sk_tenv sk_tenv;
#line 18 "tenv.org"

#ifdef SK_TENV_PRIV
#line 42 "tenv.org"
struct sk_tenv {
    unsigned long pos, atk_end, rel_start, totaldur;
    SKFLT atk, rel, hold;
    SKFLT atk_slp, rel_slp;
    SKFLT last;
    int sigmode;
    SKFLT input;
    int started;
    int sr;
};
#line 21 "tenv.org"
#endif

#line 56 "tenv.org"
void sk_tenv_init(sk_tenv *te, int sr);
#line 84 "tenv.org"
SKFLT sk_tenv_tick(sk_tenv *te, SKFLT trig);
#line 161 "tenv.org"
void sk_tenv_attack(sk_tenv *te, SKFLT atk);
#line 174 "tenv.org"
void sk_tenv_hold(sk_tenv *te, SKFLT hold);
#line 187 "tenv.org"
void sk_tenv_release(sk_tenv *te, SKFLT rel);
#line 24 "tenv.org"

#endif
