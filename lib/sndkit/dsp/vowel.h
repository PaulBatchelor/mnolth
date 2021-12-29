#line 58 "vowel.org"
#ifndef SK_VOWEL_H
#define SK_VOWEL_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 293 "vowel.org"
typedef struct sk_vowel sk_vowel;
#line 368 "vowel.org"
typedef struct {
    SKFLT freq, amp, bw;
} sk_vowel_formant;
#line 847 "vowel.org"
typedef struct sk_vowel_withphoneme sk_vowel_withphoneme;
#line 58 "vowel.org"
#line 375 "vowel.org"
extern const sk_vowel_formant sk_vowel_bass_a[];
extern const sk_vowel_formant sk_vowel_bass_o[];
extern const sk_vowel_formant sk_vowel_bass_i[];
extern const sk_vowel_formant sk_vowel_bass_e[];
extern const sk_vowel_formant sk_vowel_bass_u[];
#line 435 "vowel.org"
extern const sk_vowel_formant sk_vowel_alto_a[];
extern const sk_vowel_formant sk_vowel_alto_o[];
extern const sk_vowel_formant sk_vowel_alto_i[];
extern const sk_vowel_formant sk_vowel_alto_e[];
extern const sk_vowel_formant sk_vowel_alto_u[];
#line 495 "vowel.org"
extern const sk_vowel_formant sk_vowel_countertenor_a[];
extern const sk_vowel_formant sk_vowel_countertenor_o[];
extern const sk_vowel_formant sk_vowel_countertenor_i[];
extern const sk_vowel_formant sk_vowel_countertenor_e[];
extern const sk_vowel_formant sk_vowel_countertenor_u[];
#line 555 "vowel.org"
extern const sk_vowel_formant sk_vowel_alto_a[];
extern const sk_vowel_formant sk_vowel_alto_o[];
extern const sk_vowel_formant sk_vowel_alto_i[];
extern const sk_vowel_formant sk_vowel_alto_e[];
extern const sk_vowel_formant sk_vowel_alto_u[];
#line 615 "vowel.org"
extern const sk_vowel_formant sk_vowel_soprano_a[];
extern const sk_vowel_formant sk_vowel_soprano_o[];
extern const sk_vowel_formant sk_vowel_soprano_i[];
extern const sk_vowel_formant sk_vowel_soprano_e[];
extern const sk_vowel_formant sk_vowel_soprano_u[];
#line 677 "vowel.org"
extern const sk_vowel_formant **sk_vowel_parts[];
#line 67 "vowel.org"
#ifdef SK_VOWEL_PRIV
#line 293 "vowel.org"
#line 81 "vowel.org"
struct formant_filter {
#line 157 "vowel.org"
SKFLT b[3], x[3];
SKFLT a[2], y[2];
#line 183 "vowel.org"
SKFLT freq, pfreq;
SKFLT gain, pgain;
SKFLT Q, pQ;
#line 83 "vowel.org"
};
#line 299 "vowel.org"
struct sk_vowel {
    int sr;
    struct formant_filter filt[5];
};
#line 852 "vowel.org"
struct sk_vowel_withphoneme {
    sk_vowel vowel;
    sk_vowel_formant phoneme[5];
};
#line 69 "vowel.org"
#endif
#line 307 "vowel.org"
void sk_vowel_init(sk_vowel *vow, int sr);
#line 329 "vowel.org"
SKFLT sk_vowel_tick(sk_vowel *vow, SKFLT in);
#line 701 "vowel.org"
void sk_vowel_mix(const sk_vowel_formant *ph1,
                  const sk_vowel_formant *ph2,
                  sk_vowel_formant *out,
                  int size,
                  SKFLT pos);
#line 748 "vowel.org"
void sk_vowel_morph(sk_vowel_formant *out,
                    sk_vowel_formant *tmp,
                    int size,
                    SKFLT pos, SKFLT part);
#line 810 "vowel.org"
void sk_vowel_set_filter(sk_vowel *v, int pos,
                         SKFLT freq, SKFLT gain, SKFLT Q);
void sk_vowel_set_phoneme(sk_vowel *v,
                          sk_vowel_formant *ph,
                          int nformants);
#line 860 "vowel.org"
sk_vowel * sk_vowel_vowel(sk_vowel_withphoneme * vowph);
sk_vowel_formant * sk_vowel_phoneme(sk_vowel_withphoneme *vowph);
#line 71 "vowel.org"
#endif
