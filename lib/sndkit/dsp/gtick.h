#line 22 "gtick.org"
#ifndef SK_GTICK_H
#define SK_GTICK_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 51 "gtick.org"
typedef struct sk_gtick sk_gtick;
#line 30 "gtick.org"

#ifdef SK_GTICK_PRIV
#line 56 "gtick.org"
struct sk_gtick {
    SKFLT prev;
};
#line 33 "gtick.org"
#endif

#line 65 "gtick.org"
void sk_gtick_init(sk_gtick *gt);
#line 81 "gtick.org"
SKFLT sk_gtick_tick(sk_gtick *gt, SKFLT in);
#line 36 "gtick.org"

#endif
