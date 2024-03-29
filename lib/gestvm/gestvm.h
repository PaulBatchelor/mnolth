#line 157 "gestvm.org"
#ifndef GESTVM_H
#define GESTVM_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 211 "gestvm.org"
typedef struct gestvm gestvm;
#line 308 "gestvm.org"
typedef struct gestvm_rephasor gestvm_rephasor;
#line 776 "gestvm.org"
typedef struct gestvm_uxn gestvm_uxn;
#line 1350 "gestvm.org"
typedef SKFLT (*gestvm_behavior)(gestvm *, SKFLT);
#line 1565 "gestvm.org"
typedef struct gestvm_weight gestvm_weight;
#line 165 "gestvm.org"

#ifdef GESTVM_PRIV
#line 177 "gestvm.org"
#line 319 "gestvm.org"
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
#line 216 "gestvm.org"
struct gestvm {
#line 481 "gestvm.org"
gestvm_rephasor skew;
#line 530 "gestvm.org"
int skewdur;
#line 593 "gestvm.org"
gestvm_behavior skewer;
#line 638 "gestvm.org"
int update_skewer;
#line 704 "gestvm.org"
gestvm_rephasor rephasor;
int num, den;
int update_rephasor;
SKFLT extscale, pextscale;
#line 844 "gestvm.org"
gestvm_uxn *u;
#line 1006 "gestvm.org"
unsigned int ptr;
#line 1232 "gestvm.org"
SKFLT lphs;
#line 1284 "gestvm.org"
int empty_value;
#line 1337 "gestvm.org"
SKFLT cur, nxt;
#line 1355 "gestvm.org"
gestvm_behavior behavior;
#line 1370 "gestvm.org"
SKFLT a;
#line 1547 "gestvm.org"
SKFLT inertia;
SKFLT mass;
#line 1795 "gestvm.org"
int interp;
#line 218 "gestvm.org"
};
#line 177 "gestvm.org"
#line 781 "gestvm.org"
struct gestvm_uxn {
    Uxn u;
    gestvm *gvm;
};
#line 177 "gestvm.org"
#line 1570 "gestvm.org"
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

#line 226 "gestvm.org"
void gestvm_init(gestvm *gvm, gestvm_uxn *u);
#line 242 "gestvm.org"
size_t gestvm_sizeof(void);
#line 259 "gestvm.org"
SKFLT gestvm_tick(gestvm *gvm, SKFLT cnd);
#line 337 "gestvm.org"
void gestvm_rephasor_scale(gestvm_rephasor *rp, SKFLT scale);
#line 360 "gestvm.org"
void gestvm_rephasor_init(gestvm_rephasor *rp);
#line 387 "gestvm.org"
SKFLT gestvm_rephasor_tick(gestvm_rephasor *rp, SKFLT ext);
#line 727 "gestvm.org"
void gestvm_extscale(gestvm *gvm, SKFLT extscale);
#line 791 "gestvm.org"
void gestvm_uxn_init(gestvm_uxn *u);
#line 809 "gestvm.org"
size_t gestvm_uxn_sizeof(void);
#line 822 "gestvm.org"
void gestvm_uxn_set(gestvm_uxn *gu, gestvm *gvm);
gestvm *gestvm_uxn_get(gestvm_uxn *gu);
#line 856 "gestvm.org"
int gestvm_load(gestvm_uxn *gu, const char *rom);
#line 913 "gestvm.org"
unsigned int gestvm_lookup(const char *rom, const char *sym);
#line 1018 "gestvm.org"
void gestvm_pointer(gestvm *gvm, unsigned int ptr);
#line 1320 "gestvm.org"
void gestvm_eval(gestvm_uxn *gu, unsigned int addr);
#line 1504 "gestvm.org"
void gestvm_get_last_values(gestvm *gvm,
                            SKFLT *x,
                            SKFLT *y,
                            SKFLT *a);
#line 1587 "gestvm.org"
size_t gestvm_weight_sizeof(void);
#line 1600 "gestvm.org"
void gestvm_weight_init(gestvm_weight *gw, gestvm *gvm, int sr);
#line 1626 "gestvm.org"
void gestvm_weight_amppos(gestvm_weight *gw, SKFLT amp);
void gestvm_weight_ampneg(gestvm_weight *gw, SKFLT amp);
#line 1647 "gestvm.org"
SKFLT gestvm_weight_tick(gestvm_weight *gw);
#line 171 "gestvm.org"

#endif
