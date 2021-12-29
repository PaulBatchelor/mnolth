#line 19 "biramp.org"
#ifndef SK_BIRAMP_H
#define SK_BIRAMP_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 42 "biramp.org"
SKFLT sk_biramp_stateless(SKFLT phs, SKFLT pos);
#line 78 "biramp.org"
SKFLT sk_biramp_stateful(SKFLT phs, SKFLT pos,
                         SKFLT *lphs, SKFLT *lpos);
#line 131 "biramp.org"
SKFLT sk_biramp_flipper(SKFLT phs, SKFLT *lphs, int *flipit);
#line 27 "biramp.org"
#endif
