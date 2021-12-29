#line 93 "peakeq.org"
#ifndef SK_PEAKEQ_H
#define SK_PEAKEQ_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 113 "peakeq.org"
typedef struct sk_peakeq sk_peakeq;
#line 93 "peakeq.org"
#line 128 "peakeq.org"
void sk_peakeq_init(sk_peakeq *eq, int sr);
#line 190 "peakeq.org"
void sk_peakeq_freq(sk_peakeq *eq, SKFLT freq);
#line 224 "peakeq.org"
void sk_peakeq_bandwidth(sk_peakeq *eq, SKFLT bw);
#line 258 "peakeq.org"
void sk_peakeq_gain(sk_peakeq *eq, SKFLT gain);
#line 283 "peakeq.org"
SKFLT sk_peakeq_tick(sk_peakeq *eq, SKFLT in);
#line 102 "peakeq.org"

#ifdef SK_PEAKEQ_PRIV
#line 118 "peakeq.org"
struct sk_peakeq {
#line 147 "peakeq.org"
SKFLT v[2];
#line 161 "peakeq.org"
SKFLT a;
SKFLT b;
#line 176 "peakeq.org"
int sr;
#line 206 "peakeq.org"
SKFLT freq;
SKFLT pfreq;
#line 240 "peakeq.org"
SKFLT bw;
SKFLT pbw;
#line 271 "peakeq.org"
SKFLT gain;
#line 120 "peakeq.org"
};
#line 105 "peakeq.org"
#endif
#endif
