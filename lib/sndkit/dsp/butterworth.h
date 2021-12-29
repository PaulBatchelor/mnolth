#line 25 "butterworth.org"
#ifndef SK_BUTTERWORTH_H
#define SK_BUTLERWORTH_H

#ifndef SKFLT
#define SKFLT float
#endif

typedef struct sk_butterworth sk_butterworth;

void sk_butterworth_init(sk_butterworth *bw, int sr);
void sk_butterworth_freq(sk_butterworth *bw, SKFLT freq);

SKFLT sk_butlp_tick(sk_butterworth *bw, SKFLT in);
SKFLT sk_buthp_tick(sk_butterworth *bw, SKFLT in);

#ifdef SK_BUTTERWORTH_PRIV
struct sk_butterworth {
    SKFLT freq, lfreq;
    SKFLT a[7];
    SKFLT pidsr;
};
#endif

#endif
