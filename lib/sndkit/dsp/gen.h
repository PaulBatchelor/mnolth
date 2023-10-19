#line 19 "gen.org"
#ifndef SK_GEN_SINE
#define SK_GEN_SINE

#ifndef SKFLT
#define SKFLT float
#endif

#line 52 "gen.org"
void sk_gen_sine(SKFLT *tab, unsigned long sz);
#line 79 "gen.org"
void sk_gen_saw(SKFLT *tab, unsigned long sz);
#line 107 "gen.org"
void sk_gen_vals(SKFLT **ptab, int *psz, const char *str);
#line 176 "gen.org"
void sk_gen_sinesum(SKFLT *tab,
                    int sz,
                    const char *argstring,
                    int normalize);
#line 234 "gen.org"
int sk_gen_line(SKFLT *tab, int sz, const char *argstring);
#line 27 "gen.org"
#endif
