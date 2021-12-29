#line 42 "vowel.org"
#include <math.h>
#define SK_VOWEL_PRIV
#include "vowel.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 384 "vowel.org"
const sk_vowel_formant sk_vowel_bass_a[] = {
    {600, 0, 60},
    {1040, -7, 70},
    {2250, -9, 110},
    {2450, -9, 120},
    {2750, -20, 130},
};

const sk_vowel_formant sk_vowel_bass_e[] = {
    {400, 0, 40},
    {1620, -12, 80},
    {2400, -9, 100},
    {2800, -12, 120},
    {3100, -18, 120},
};

const sk_vowel_formant sk_vowel_bass_i[] = {
    {250, 0, 60},
    {1750, -30, 90},
    {2600, -16, 100},
    {3050, -22, 120},
    {3340, -28, 120},
};

const sk_vowel_formant sk_vowel_bass_o[] = {
    {400, 0, 40},
    {750, -11, 80},
    {2400, -21, 100},
    {2600, -20, 120},
    {2900, -40, 120},
};

const sk_vowel_formant sk_vowel_bass_u[] = {
    {350, 0, 40},
    {600, -20, 80},
    {2400, -32, 100},
    {2675, -28, 120},
    {2950, -36, 120},
};

static const sk_vowel_formant *vowel_bass[] = {
    sk_vowel_bass_a,
    sk_vowel_bass_e,
    sk_vowel_bass_i,
    sk_vowel_bass_o,
    sk_vowel_bass_u
};
#line 444 "vowel.org"
const sk_vowel_formant sk_vowel_tenor_a[] = {
    {650, 0, 80},
    {1080, -6, 90},
    {2650, -7, 120},
    {2900, -8, 130},
    {3250, -22, 140},
};

const sk_vowel_formant sk_vowel_tenor_e[] = {
    {440, 0, 70},
    {1700, -14, 80},
    {2600, -12, 100},
    {3200, -14, 120},
    {3580, -20, 120},
};

const sk_vowel_formant sk_vowel_tenor_i[] = {
    {290, 0, 40},
    {1870, -15, 90},
    {2800, -18, 100},
    {3250, -20, 120},
    {3540, -30, 120},
};

const sk_vowel_formant sk_vowel_tenor_o[] = {
    {400, 0, 70},
    {800, -10, 80},
    {2600, -12, 100},
    {2800, -12, 130},
    {3000, -26, 135},
};

const sk_vowel_formant sk_vowel_tenor_u[] = {
    {350, 0, 40},
    {600, -20, 60},
    {2700, -17, 100},
    {2900, -14, 120},
    {3300, -26, 120},
};

static const sk_vowel_formant *vowel_tenor[] = {
    sk_vowel_tenor_a,
    sk_vowel_tenor_e,
    sk_vowel_tenor_i,
    sk_vowel_tenor_o,
    sk_vowel_tenor_u
};
#line 504 "vowel.org"
const sk_vowel_formant sk_vowel_countertenor_a[] = {
    {660, 0, 80},
    {1120, -6, 90},
    {2750, -23, 120},
    {3000, -24, 130},
    {3350, -38, 140},
};

const sk_vowel_formant sk_vowel_countertenor_e[] = {
    {440, 0, 70},
    {1800, -14, 80},
    {2700, -18, 100},
    {3000, -20, 120},
    {3300, -20, 120},
};

const sk_vowel_formant sk_vowel_countertenor_i[] = {
    {270, 0, 40},
    {1850, -24, 90},
    {2900, -24, 100},
    {3350, -36, 120},
    {3590, -36, 120},
};

const sk_vowel_formant sk_vowel_countertenor_o[] = {
    {430, 0, 40},
    {820, -10, 80},
    {2700, -26, 100},
    {3000, -22, 120},
    {3300, -34, 120},
};

const sk_vowel_formant sk_vowel_countertenor_u[] = {
    {370, 0, 40},
    {630, -20, 60},
    {2750, -23, 100},
    {3000, -30, 120},
    {3400, -34, 120},
};

const static sk_vowel_formant *vowel_countertenor[] = {
    sk_vowel_countertenor_a,
    sk_vowel_countertenor_e,
    sk_vowel_countertenor_i,
    sk_vowel_countertenor_o,
    sk_vowel_countertenor_u
};
#line 564 "vowel.org"
const sk_vowel_formant sk_vowel_alto_a[] = {
    {800, 0, 80},
    {1150, -4, 90},
    {2800, -20, 120},
    {3500, -36, 130},
    {4950, -60, 140},
};

const sk_vowel_formant sk_vowel_alto_e[] = {
    {400, 0, 60},
    {1600, -24, 80},
    {2700, -30, 120},
    {3300, -35, 150},
    {4950, -60, 200},
};

const sk_vowel_formant sk_vowel_alto_i[] = {
    {350, 0, 50},
    {1700, -20, 100},
    {2700, -30, 120},
    {3700, -36, 150},
    {4950, -60, 200},
};

const sk_vowel_formant sk_vowel_alto_o[] = {
    {450, 0, 70},
    {800, -9, 80},
    {2830, -16, 100},
    {3500, -28, 130},
    {4950, -55, 135},
};

const sk_vowel_formant sk_vowel_alto_u[] = {
    {325, 0, 50},
    {700, -12, 60},
    {2530, -30, 170},
    {3500, -40, 180},
    {4950, -64, 200},
};

static const sk_vowel_formant *vowel_alto[] = {
    sk_vowel_alto_a,
    sk_vowel_alto_e,
    sk_vowel_alto_i,
    sk_vowel_alto_o,
    sk_vowel_alto_u
};
#line 624 "vowel.org"
const sk_vowel_formant sk_vowel_soprano_a[] = {
    {800, 0, 80},
    {1150, -6, 90},
    {2900, -32, 120},
    {3900, -20, 130},
    {4950, -50, 140},
};

const sk_vowel_formant sk_vowel_soprano_e[] = {
    {350, 0, 60},
    {2000, -20, 100},
    {2800, -15, 120},
    {3600, -40, 150},
    {4950, -56, 200},
};

const sk_vowel_formant sk_vowel_soprano_i[] = {
    {270, 0, 60},
    {2140, -12, 90},
    {2950, -26, 100},
    {3900, -26, 120},
    {4950, -44, 120},
};

const sk_vowel_formant sk_vowel_soprano_o[] = {
    {450, 0, 40},
    {800, -11, 80},
    {2830, -22, 100},
    {3800, -22, 120},
    {4950, -50, 120},
};

const sk_vowel_formant sk_vowel_soprano_u[] = {
    {325, 0, 50},
    {700, -16, 60},
    {2700, -35, 170},
    {3800, -40, 180},
    {4950, -60, 200},
};

static const sk_vowel_formant *vowel_soprano[] = {
    sk_vowel_soprano_a,
    sk_vowel_soprano_e,
    sk_vowel_soprano_i,
    sk_vowel_soprano_o,
    sk_vowel_soprano_u
};
#line 42 "vowel.org"
#line 682 "vowel.org"
const sk_vowel_formant **sk_vowel_parts[] = {
    vowel_bass,
    vowel_tenor,
    vowel_countertenor,
    vowel_alto,
    vowel_soprano,
};
#line 42 "vowel.org"
#line 92 "vowel.org"
static void formant_filter_init(struct formant_filter *ff);
#line 113 "vowel.org"
static SKFLT formant_filter_tick(struct formant_filter *ff,
                                 int sr, SKFLT in);
#line 42 "vowel.org"
#line 101 "vowel.org"
static void formant_filter_init(struct formant_filter *ff)
{
#line 163 "vowel.org"
{
    int i;

    for (i = 0; i < 3; i++) {
        ff->b[i] = 0;
        ff->x[i] = 0;
    }

    for (i = 0; i < 2; i++) {
        ff->a[i] = 0;
        ff->y[i] = 0;
    }
}
#line 190 "vowel.org"
ff->freq = 0;
ff->pfreq = -1;
ff->gain = 0;
ff->pgain = -1;
ff->Q = 0;
ff->pQ = -1;
#line 104 "vowel.org"
}
#line 120 "vowel.org"
static SKFLT formant_filter_tick(struct formant_filter *ff,
                                 int sr, SKFLT in)
{
    SKFLT out;
    SKFLT *x, *y, *a, *b;
    int update;

    out = 0;

#line 205 "vowel.org"
update =
    ff->gain != ff->pgain ||
    ff->Q != ff->pQ ||
    ff->freq != ff->pfreq;

if (update) {
    SKFLT b2, b1, b0;
    SKFLT a1, a0;
    SKFLT wc;
    SKFLT c, csq, d;

    ff->pgain = ff->gain;
    ff->pQ = ff->Q;
    ff->pfreq = ff->freq;

    wc = ff->freq * 2 * M_PI;

    if (ff->Q == 0) ff->Q = 0.0000001;

    a1 = 1.0 / ff->Q;
    a0 = 1.0;
    b2 = 0.0;
    b1 = ff->gain;
    b0 = 0.0;

    c = 1.0 / tan(wc*0.5/(SKFLT)sr);
    csq = c*c;
    d = a0 + a1 * c + csq;

    ff->b[0] = (b0 + b1 * c + b2 * csq) / d;
    ff->b[1] = 2.0 * (b0 - b2 * csq) / d;
    ff->b[2] = (b0 - b1*c + b2*csq) / d;

    ff->a[0] = 2 * (a0 - csq) / d;
    ff->a[1] = (a0 - a1*c + csq) / d;
}
#line 120 "vowel.org"
#line 260 "vowel.org"
x = ff->x;
y = ff->y;
a = ff->a;
b = ff->b;

x[0] = in;

out =
    b[0]*x[0] + b[1]*x[1] + b[2]*x[2]
    -a[0]*y[0] - a[1]*y[1];
#line 120 "vowel.org"
#line 278 "vowel.org"
y[1] = y[0];
y[0] = out;

x[2] = x[1];
x[1] = x[0];
#line 132 "vowel.org"

    return out;
}
#line 312 "vowel.org"
void sk_vowel_init(sk_vowel *vow, int sr)
{
    int i;
    vow->sr = sr;

    for (i = 0; i < 5; i++) {
        formant_filter_init(&vow->filt[i]);
    }
}
#line 334 "vowel.org"
SKFLT sk_vowel_tick(sk_vowel *vow, SKFLT in)
{
    SKFLT out;
    int i;

    out = 0;
    for (i = 0; i < 5; i++) {
        out += formant_filter_tick(&vow->filt[i], vow->sr, in);
    }

    out *= 0.2;

    return out;
}
#line 710 "vowel.org"
static void formant_lerp(const sk_vowel_formant *ph1,
                         const sk_vowel_formant *ph2,
                         sk_vowel_formant *out,
                         SKFLT pos)
{
    out->freq =
        (1 - pos)*ph1->freq + pos*ph2->freq;
    out->amp =
        (1 - pos)*ph1->amp + pos*ph2->amp;
    out->bw =
        (1 - pos)*ph1->bw + pos*ph2->bw;
}

void sk_vowel_mix(const sk_vowel_formant *ph1,
                  const sk_vowel_formant *ph2,
                  sk_vowel_formant *out,
                  int size,
                  SKFLT pos)
{
    int i;

    for (i = 0; i < size; i++) {
        formant_lerp(&ph1[i], &ph2[i], &out[i], pos);
    }
}
#line 756 "vowel.org"
void sk_vowel_morph(sk_vowel_formant *out,
                    sk_vowel_formant *tmp,
                    int size,
                    SKFLT pos, SKFLT part)
{
    const sk_vowel_formant *ph[2];
    const sk_vowel_formant **voice[2];
    int ipos;
    int ipart;

    pos *= (size - 2);
    part *= 4;

    ipos = floor(pos);
    ipart = floor(part);

    if (ipart >= 4) {
        voice[0] = sk_vowel_parts[4];
        voice[1] = voice[0];
    } else {
        voice[0] = sk_vowel_parts[ipart];
        voice[1] = sk_vowel_parts[ipart + 1];
    }

    if (ipos >= 4) {
        ph[0] = voice[0][4];
        ph[1] = ph[0];
    } else {
        ph[0] = voice[0][ipos];
        ph[1] = voice[0][ipos + 1];
    }

    pos = pos - ipos;

    sk_vowel_mix(ph[0], ph[1], tmp, size, pos);

    if (ipos >= 4) {
        ph[0] = voice[1][4];
        ph[1] = ph[0];
    } else {
        ph[0] = voice[1][ipos];
        ph[1] = voice[1][ipos + 1];
    }

    sk_vowel_mix(ph[0], ph[1], out, size, pos);

    pos = part - ipart;

    sk_vowel_mix(tmp, out, out, size, pos);
}
#line 819 "vowel.org"
#define DB2LIN(db) (pow(10.0, 0.05 * (db)));

void sk_vowel_set_filter(sk_vowel *v, int pos,
                         SKFLT freq, SKFLT gain, SKFLT Q)
{
    if (pos < 0 || pos >= 5) return;

    v->filt[pos].freq = freq;
    v->filt[pos].gain = DB2LIN(gain);
    v->filt[pos].Q = Q;
}

void sk_vowel_set_phoneme(sk_vowel *v,
                          sk_vowel_formant *ph,
                          int nformants)
{
    int n;

    for (n = 0; n < nformants; n++) {
        sk_vowel_set_filter(v, n,
                            ph[n].freq,
                            ph[n].amp, ph[n].freq/ph[n].bw);
    }
}
#line 866 "vowel.org"
sk_vowel * sk_vowel_vowel(sk_vowel_withphoneme *vowph)
{
    return &vowph->vowel;
}

sk_vowel_formant * sk_vowel_phoneme(sk_vowel_withphoneme *vowph)
{
    return vowph->phoneme;
}
#line 42 "vowel.org"
