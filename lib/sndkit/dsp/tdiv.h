#line 11 "tdiv.org"
#ifndef SK_TDIV_H
#define SK_TDIV_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 40 "tdiv.org"
typedef struct sk_tdiv sk_tdiv;
#line 19 "tdiv.org"

#ifdef SK_TDIV_PRIV
#line 45 "tdiv.org"
struct sk_tdiv {
    int cnt;
    int div;
    int off;
};
#line 22 "tdiv.org"
#endif

#line 54 "tdiv.org"
void sk_tdiv_init(sk_tdiv *tdiv);
#line 72 "tdiv.org"
void sk_tdiv_offset(sk_tdiv *tdiv, int offset);
void sk_tdiv_divide(sk_tdiv *tdiv, int divide);
#line 116 "tdiv.org"
SKFLT sk_tdiv_tick(sk_tdiv *td, SKFLT trig);
#line 25 "tdiv.org"

#endif
