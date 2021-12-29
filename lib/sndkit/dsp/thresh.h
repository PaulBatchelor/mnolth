#line 13 "thresh.org"
#ifndef SK_THRESH_H
#define SK_THRESH_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 72 "thresh.org"
typedef struct sk_thresh sk_thresh;
#line 21 "thresh.org"

#ifdef SK_THRESH_PRIV
#line 77 "thresh.org"
struct sk_thresh {
    SKFLT value;
    int mode;
    SKFLT prev;
    int init;
};
#line 24 "thresh.org"
#endif

#line 39 "thresh.org"
SKFLT sk_thresh_stateless(SKFLT in, SKFLT prev, SKFLT thresh, int mode);
#line 87 "thresh.org"
void sk_thresh_init(sk_thresh *th);
void sk_thresh_mode(sk_thresh *th, int mode);
void sk_thresh_value(sk_thresh *th, SKFLT value);
SKFLT sk_thresh_tick(sk_thresh *th, SKFLT in);
#line 27 "thresh.org"
#endif
