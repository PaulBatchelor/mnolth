#line 157 "gestvm.org"
#ifndef GESTVM_H
#define GESTVM_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 214 "gestvm.org"
typedef struct gestvm gestvm;
#line 311 "gestvm.org"
typedef struct gestvm_rephasor gestvm_rephasor;
#line 794 "gestvm.org"
typedef struct gestvm_uxn gestvm_uxn;
#line 1401 "gestvm.org"
typedef SKFLT (*gestvm_behavior)(gestvm *, SKFLT);
#line 1788 "gestvm.org"
typedef struct gestvm_weight gestvm_weight;
#line 165 "gestvm.org"

#ifdef GESTVM_PRIV
#line 177 "gestvm.org"
#line 322 "gestvm.org"
struct gestvm_rephasor {
    SKFLT pr; /* rephasor */
    SKFLT pc[2]; /* comparison phasor */
    SKFLT pe[2]; /* external phasor */
    SKFLT c; /* correction */
    SKFLT s; /* scaling */
    SKFLT si; /* scaling, inverted */

    SKFLT ir; /* rephasor incrementor */
    SKFLT ic; /* comparison rephasor */
};
#line 177 "gestvm.org"
#line 219 "gestvm.org"
struct gestvm {
#line 484 "gestvm.org"
gestvm_rephasor skew;
#line 533 "gestvm.org"
int skewdur;
#line 596 "gestvm.org"
gestvm_behavior skewer;
#line 641 "gestvm.org"
int update_skewer;
#line 721 "gestvm.org"
gestvm_rephasor rephasor;
int num, den;
int update_rephasor;
SKFLT extscale, pextscale;
#line 863 "gestvm.org"
gestvm_uxn *u;
#line 1025 "gestvm.org"
unsigned int ptr;
#line 1272 "gestvm.org"
SKFLT lphs;
#line 1325 "gestvm.org"
int empty_value;
#line 1388 "gestvm.org"
SKFLT cur, nxt;
#line 1406 "gestvm.org"
gestvm_behavior behavior;
#line 1421 "gestvm.org"
SKFLT a;
#line 1431 "gestvm.org"
SKFLT cnd;
#line 1724 "gestvm.org"
int pselect;
int params[4];
#line 1770 "gestvm.org"
SKFLT inertia;
SKFLT mass;
#line 2018 "gestvm.org"
int interp;
#line 2028 "gestvm.org"
sk_table **tablist;
int ntables;
#line 2093 "gestvm.org"
int adder;
#line 2103 "gestvm.org"
Stack rst, wst;
#line 221 "gestvm.org"
};
#line 177 "gestvm.org"
#line 799 "gestvm.org"
struct gestvm_uxn {
    Uxn u;
    gestvm *gvm;
};
#line 177 "gestvm.org"
#line 1793 "gestvm.org"
struct gestvm_weight {
    gestvm *gvm;
    int sr;
    SKFLT a;
    SKFLT inertia;
    SKFLT prev;
    SKFLT scale;
    SKFLT ampneg;
    SKFLT amppos;
};
#line 168 "gestvm.org"
#endif

#line 229 "gestvm.org"
void gestvm_init(gestvm *gvm, gestvm_uxn *u);
#line 245 "gestvm.org"
size_t gestvm_sizeof(void);
#line 262 "gestvm.org"
SKFLT gestvm_tick(gestvm *gvm, SKFLT cnd);
#line 340 "gestvm.org"
void gestvm_rephasor_scale(gestvm_rephasor *rp, SKFLT scale);
#line 363 "gestvm.org"
void gestvm_rephasor_init(gestvm_rephasor *rp);
#line 390 "gestvm.org"
SKFLT gestvm_rephasor_tick(gestvm_rephasor *rp, SKFLT ext);
#line 692 "gestvm.org"
SKFLT gestvm_last_conductor(gestvm *gvm);
#line 744 "gestvm.org"
void gestvm_extscale(gestvm *gvm, SKFLT extscale);
#line 809 "gestvm.org"
void gestvm_uxn_init(gestvm_uxn *u);
#line 828 "gestvm.org"
size_t gestvm_uxn_sizeof(void);
#line 841 "gestvm.org"
void gestvm_uxn_set(gestvm_uxn *gu, gestvm *gvm);
gestvm *gestvm_uxn_get(gestvm_uxn *gu);
#line 875 "gestvm.org"
int gestvm_load(gestvm_uxn *gu, const char *rom);
#line 932 "gestvm.org"
unsigned int gestvm_lookup(const char *rom, const char *sym);
#line 1037 "gestvm.org"
void gestvm_pointer(gestvm *gvm, unsigned int ptr);
#line 1362 "gestvm.org"
void gestvm_eval(gestvm_uxn *gu, gestvm *gvm, unsigned int addr);
#line 1698 "gestvm.org"
void gestvm_get_last_values(gestvm *gvm,
                            SKFLT *x,
                            SKFLT *y,
                            SKFLT *a);
#line 1810 "gestvm.org"
size_t gestvm_weight_sizeof(void);
#line 1823 "gestvm.org"
void gestvm_weight_init(gestvm_weight *gw, gestvm *gvm, int sr);
#line 1849 "gestvm.org"
void gestvm_weight_amppos(gestvm_weight *gw, SKFLT amp);
void gestvm_weight_ampneg(gestvm_weight *gw, SKFLT amp);
#line 1870 "gestvm.org"
SKFLT gestvm_weight_tick(gestvm_weight *gw);
#line 2040 "gestvm.org"
void gestvm_tablist(gestvm *gvm, sk_table **tablist, int ntables);
#line 171 "gestvm.org"

#endif
