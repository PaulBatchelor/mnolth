#line 32 "gen.org"
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "gen.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#line 55 "gen.org"
void sk_gen_sine(SKFLT *tab, unsigned long sz)
{
    int i;
    SKFLT step;

    if (sz <= 0) return;

    step = 2 * M_PI / sz;

    for (i = 0; i < sz; i++) {
        tab[i] = sin(i * step);
    }
}
#line 82 "gen.org"
void sk_gen_saw(SKFLT *tab, unsigned long sz)
{
    int i;
    SKFLT step;

    if (sz <= 0) return;

    step = 1.0 / sz;

    for (i = 0; i < sz; i++) {
        tab[i] = 2.0*(i*step) - 1.0;
    }
}
#line 110 "gen.org"
static char * tokenize(char **next, int *size)
{
    char *token;
    char *str;
    char *peak;

    if (*size <= 0) return NULL;
    token = *next;
    str = *next;

    peak = str + 1;

    while ((*size)--) {
        if (*str == ' ') {
            *str = 0;
            if (*peak != ' ') break;
        }
        str = str + 1;
        peak = str + 1;
    }
    *next = peak;
    return token;
}

void sk_gen_vals(SKFLT **ptab, int *psz, const char *string)
{
    int size;
    char *str;
    char *out;
    char *ptr;
    int j;
    int sz;
    SKFLT *tab;

    size = strlen(string);
    str = malloc(sizeof(char) * size + 1);
    strcpy(str, string);
    ptr = str;
    j = 0;

    tab = *ptab;
    sz = *psz;

    while (size > 0) {
        out = tokenize(&str, &size);
        if (sz < j + 1) {
            tab = realloc(tab, sizeof(SKFLT) * (sz + 2));
            /* zero out new tables */
            tab[sz] = 0;
            tab[sz + 1] = 0;
            sz++;
        }
        tab[j] = atof(out);
        j++;
    }

    *ptab = tab;
    *psz = sz;
    free(ptr);
}
#line 179 "gen.org"
void sk_gen_sinesum(SKFLT *tab, int sz, const char *argstring)
{
    SKFLT *args;
    int argsz;
    int phs;
    SKFLT amp;
    int flen;
    SKFLT tpdlen;
    int i, n;

    args = malloc(sizeof(SKFLT));
    args[0] = 0;
    argsz = 1;

    sk_gen_vals(&args, &argsz, argstring);
    flen = sz;
    tpdlen = 2.0 * M_PI / (SKFLT) flen;

    for (i = argsz; i > 0; i--) {
        amp = args[i - 1];
        if (amp != 0) {
            for (phs = 0, n = 0; n < sz; n++) {
                tab[n] += sin(phs * tpdlen) * amp;
                phs += i;
                phs %= flen;
            }
        }
    }

    free(args);
}
#line 32 "gen.org"
