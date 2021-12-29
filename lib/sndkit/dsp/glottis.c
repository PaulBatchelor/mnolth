#line 21 "glottis.org"
#include <math.h>
#include <stdlib.h>
#define SK_GLOTTIS_PRIV
#include "glottis.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 387 "glottis.org"
#define LCG_MAX 2147483648
#line 31 "glottis.org"

#line 144 "glottis.org"
static void setup_waveform(sk_glottis *glot);
#line 370 "glottis.org"
static unsigned long glot_lcg(sk_glottis *glot);
#line 31 "glottis.org"
#line 71 "glottis.org"
void sk_glottis_init(sk_glottis *glot, SKFLT sr)
{
    glot->freq = 140; /* 140Hz frequency by default */
    glot->tenseness = 0.6; /* value between 0 and 1 */
    glot->T = 1.0/sr; /* big T */
    glot->time_in_waveform = 0;
    setup_waveform(glot);
#line 363 "glottis.org"
sk_glottis_srand(glot, 0);
#line 79 "glottis.org"
}
#line 117 "glottis.org"
void sk_glottis_freq(sk_glottis *glot, SKFLT freq)
{
    glot->freq = freq;
}
#line 132 "glottis.org"
void sk_glottis_tenseness(sk_glottis *glot, SKFLT tenseness)
{
    glot->tenseness = tenseness;
}
#line 149 "glottis.org"
static void setup_waveform(sk_glottis *glot)
{
    SKFLT Rd;
    SKFLT Ra;
    SKFLT Rk;
    SKFLT Rg;

    SKFLT Ta;
    SKFLT Tp;
    SKFLT Te;

    SKFLT epsilon;
    SKFLT shift;
    SKFLT delta;
    SKFLT rhs_integral;

    SKFLT lower_integral;
    SKFLT upper_integral;

    SKFLT omega;
    SKFLT s;
    SKFLT y;
    SKFLT z;

    SKFLT alpha;
    SKFLT E0;

#line 204 "glottis.org"
glot->waveform_length = 1.0 / glot->freq;
#line 149 "glottis.org"
#line 221 "glottis.org"
glot->Rd = 3 * (1 - glot->tenseness);
Rd = glot->Rd;
if (Rd < 0.5) Rd = 0.5;
if (Rd > 2.7) Rd = 2.7;
#line 149 "glottis.org"
#line 249 "glottis.org"
Ra = -0.01 + 0.048*Rd;
Rk = 0.224 + 0.118*Rd;
#line 149 "glottis.org"
#line 273 "glottis.org"
Rg = (Rk/4)*(0.5 + 1.2*Rk)/(0.11*Rd-Ra*(0.5+1.2*Rk));
#line 149 "glottis.org"
#line 291 "glottis.org"
Ta = Ra;
Tp = (SKFLT)1.0 / (2*Rg);
Te = Tp + Tp*Rk;
#line 149 "glottis.org"
#line 302 "glottis.org"
epsilon = (SKFLT)1.0 / Ta;
shift = exp(-epsilon * (1 - Te));
delta = 1 - shift;
#line 149 "glottis.org"
#line 312 "glottis.org"
rhs_integral = (SKFLT)(1.0/epsilon) * (shift-1) + (1-Te)*shift;
rhs_integral = rhs_integral / delta;
lower_integral = - (Te - Tp) / 2 + rhs_integral;
upper_integral = -lower_integral;
#line 183 "glottis.org"

    omega = M_PI / Tp;
    s = sin(omega * Te);

    y = -M_PI * s * upper_integral / (Tp*2);
    z = log(y);
    alpha = z / (Tp/2 - Te);
    E0 = -1 / (s * exp(alpha*Te));

#line 325 "glottis.org"
glot->alpha = alpha;
glot->E0 = E0;
glot->epsilon = epsilon;
glot->shift = shift;
glot->delta = delta;
glot->Te = Te;
glot->omega = omega;
#line 193 "glottis.org"
}
#line 353 "glottis.org"
void sk_glottis_srand(sk_glottis *glot, unsigned long s)
{
    glot->rng = 0;
}
#line 375 "glottis.org"
static unsigned long glot_lcg(sk_glottis *glot)
{
    glot->rng = (1103515245 * glot->rng + 12345) % LCG_MAX;
    return glot->rng;
}
#line 399 "glottis.org"
SKFLT sk_glottis_tick(sk_glottis *glot)
{
    SKFLT out;
    SKFLT aspiration;
    SKFLT noise;
    SKFLT t;

    out = 0;

#line 423 "glottis.org"
glot->time_in_waveform += glot->T;
#line 399 "glottis.org"
#line 431 "glottis.org"
if (glot->time_in_waveform > glot->waveform_length) {
    glot->time_in_waveform -= glot->waveform_length;
    setup_waveform(glot);

}
#line 410 "glottis.org"

#line 445 "glottis.org"
t = (glot->time_in_waveform / glot->waveform_length);

if (t > glot->Te) {
    out = (-exp(-glot->epsilon * (t-glot->Te)) + glot->shift) / glot->delta;
} else {
    out = glot->E0 * exp(glot->alpha * t) * sin(glot->omega * t);
}
#line 410 "glottis.org"
#line 466 "glottis.org"
noise = 2.0 * ((SKFLT) glot_lcg(glot) / LCG_MAX) - 1;
aspiration = (1 - sqrt(glot->tenseness)) * 0.3 * noise;
aspiration *= 0.2;
out += aspiration;
#line 413 "glottis.org"

    return out;
}
#line 31 "glottis.org"
