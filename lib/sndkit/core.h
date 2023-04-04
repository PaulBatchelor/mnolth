#line 18 "core.org"
#ifndef SKGF_H
#define SKGF_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 106 "core.org"
typedef struct sk_core sk_core;
#line 265 "core.org"
typedef struct sk_stacklet sk_stacklet;
#line 322 "core.org"
typedef struct sk_stack sk_stack;
#line 698 "core.org"
typedef struct {
    char type;
    union {
        gf_cable *c;
        SKFLT f;
    } data;
} sk_param;
#line 1026 "core.org"
typedef struct sk_register_entry sk_register_entry;
#line 1064 "core.org"
typedef struct sk_regtbl sk_regtbl;
#line 1374 "core.org"
typedef struct sk_table sk_table;
#line 1698 "core.org"
typedef struct sk_dict sk_dict;
#line 26 "core.org"
#ifdef SK_CORE_PRIV
#line 270 "core.org"
struct sk_stacklet {
    int type;
    SKFLT f;
    void *ptr;
};
#line 331 "core.org"
#define SK_STACKSIZE 16
struct sk_stack {
    sk_stacklet stack[SK_STACKSIZE];
    int pos;
};
#line 270 "core.org"
#line 1034 "core.org"
struct sk_register_entry {
    sk_stacklet data;
    int flags;
};
#line 1069 "core.org"
#define SK_REGSIZE 16
struct sk_regtbl {
    sk_register_entry r[SK_REGSIZE];
};
#line 1379 "core.org"
struct sk_table {
    SKFLT *tab;
    unsigned long sz;
};
#line 1703 "core.org"
struct dict_entry {
    char *key;
    int sz;
    /* void *val; */
    sk_stacklet s;
    void (*del)(void*);
    struct dict_entry *nxt;
};

struct sk_dict {
    struct dict_entry *ent[64];
    int sz;
};
#line 26 "core.org"
#line 111 "core.org"
struct sk_core {
    gf_patch *patch;
    sk_stack stack;
    sk_regtbl regtbl;
    sk_dict dict;
    unsigned long rng;
};
#line 29 "core.org"
#endif
#line 1611 "core.org"
#define SK_ERROR_CHECK(rc) if (rc) return rc;
#line 1616 "core.org"
#define SK_GF_ERROR_CHECK(rc) if(rc != GF_OK) {\
    fprintf(stderr, "Error: %s\n", gf_error(rc));\
    return 1;\
}
#line 1653 "core.org"
#define SK_CORE_RANDMAX 2147483648
#line 29 "core.org"
#line 61 "core.org"
sk_core * sk_core_new(int sr);
void sk_core_del(sk_core *core);
#line 125 "core.org"
void sk_core_compute(sk_core *core);
#line 143 "core.org"
size_t sk_core_computes(sk_core *core, SKFLT secs);
#line 167 "core.org"
gf_patch * sk_core_patch(sk_core *core);
#line 183 "core.org"
size_t sk_core_seconds_to_blocks(sk_core *core, SKFLT secs);
#line 205 "core.org"
int sk_core_blkset(sk_core *core, int sz);
#line 218 "core.org"
sk_stack* sk_core_stack(sk_core *core);
#line 231 "core.org"
sk_dict* sk_core_dict(sk_core *core);
#line 246 "core.org"
void sk_core_srate_set(sk_core *core, int sr);
#line 283 "core.org"
void sk_stacklet_init(sk_stacklet *s);
#line 298 "core.org"
SKFLT sk_stacklet_constant(sk_stacklet *s);
void * sk_stacklet_userdata(sk_stacklet *s);
#line 342 "core.org"
void sk_stack_init(sk_stack *s);
#line 377 "core.org"
int sk_stacklet_isnone(sk_stacklet *s);
int sk_stacklet_isconstant(sk_stacklet *s);
int sk_stacklet_iscable(sk_stacklet *s);
int sk_stacklet_istable(sk_stacklet *s);
int sk_stacklet_isgeneric(sk_stacklet *s);
#line 424 "core.org"
int sk_stack_pop(sk_stack *stack, sk_stacklet **out);
int sk_stack_push(sk_stack *stack, sk_stacklet **out);
#line 468 "core.org"
int sk_stack_peak(sk_stack *stack, sk_stacklet **out);
#line 493 "core.org"
int sk_stack_dup(sk_stack *stack, sk_stacklet **out);
#line 524 "core.org"
int sk_core_dup(sk_core *core);
#line 556 "core.org"
int sk_stack_drop(sk_stack *stack, sk_stacklet **out);
#line 581 "core.org"
int sk_core_drop(sk_core *core);
#line 611 "core.org"
int sk_stack_swap(sk_stack *stack,
                  sk_stacklet **s1,
                  sk_stacklet **s2);
#line 649 "core.org"
int sk_core_swap(sk_core *core);
#line 674 "core.org"
int sk_core_stackpos(sk_core *core);
#line 709 "core.org"
gf_cable* sk_param_cable(sk_param *p);
SKFLT sk_param_constant(sk_param *p);
#line 730 "core.org"
int sk_param_get(sk_core *core, sk_param *p);
#line 773 "core.org"
int sk_param_get_constant(sk_core *core, SKFLT *val);
#line 804 "core.org"
int sk_param_get_cable(sk_core *core, sk_param *p);
#line 837 "core.org"
int sk_param_set(sk_core *core,
                 gf_node *node,
                 sk_param *p,
                 int cid);
#line 875 "core.org"
int sk_core_constant(sk_core *core, SKFLT x);
#line 903 "core.org"
int sk_param_out(sk_core *core,
                 gf_node *node,
                 int cid);
#line 945 "core.org"
int sk_core_generic_push(sk_core *core, void *ptr);
int sk_core_generic_pop(sk_core *core, void **ptr);
#line 993 "core.org"
int sk_param_isconstant(sk_param *p);
int sk_param_iscable(sk_param *p);
#line 1045 "core.org"
void sk_register_entry_init(sk_register_entry *e);
#line 1079 "core.org"
void sk_regtbl_init(sk_regtbl *rs);
#line 1105 "core.org"
int sk_core_regget(sk_core *core, int pos);
int sk_register_get(sk_regtbl *rt, int pos, sk_stacklet *s);
int sk_core_regset(sk_core *core, int pos);
int sk_register_set(sk_regtbl *rt, int pos, sk_stacklet *s);
#line 1185 "core.org"
int sk_core_regmrk(sk_core *core, int pos);
int sk_register_mark(sk_regtbl *rt, int pos);
#line 1214 "core.org"
int sk_core_regclr(sk_core *core, int pos);
int sk_register_clear(sk_regtbl *rt, int pos);
#line 1245 "core.org"
int sk_core_regnxt(sk_core *core, int start, int *pos);
int sk_register_nxtfree(sk_regtbl *rt, int start);
#line 1315 "core.org"
int sk_core_hold(sk_core *core);
int sk_core_unhold(sk_core *core);
#line 1393 "core.org"
int sk_core_table_new(sk_core *core, unsigned long sz);
int sk_core_table_init(sk_core *core, SKFLT *tab, unsigned long sz);
#line 1434 "core.org"
void sk_table_init(sk_table *tab, SKFLT *data, unsigned long sz);
#line 1450 "core.org"
size_t sk_table_size(sk_table *t);
SKFLT* sk_table_data(sk_table *t);
#line 1471 "core.org"
int sk_core_table_push(sk_core *core, sk_table *tab);
int sk_core_table_pop(sk_core *core, sk_table **tab);
#line 1523 "core.org"
int sk_table_dump(sk_table *tab, const char *filename);
int sk_core_tabdump(sk_core *core, const char *filename);
#line 1572 "core.org"
int sk_core_tabload(sk_core *core, const char *filename);
#line 1631 "core.org"
void sk_core_srand(sk_core *core, unsigned long val);
#line 1646 "core.org"
unsigned long sk_core_rand(sk_core *core);
#line 1669 "core.org"
SKFLT sk_core_randf(sk_core *core);
#line 1720 "core.org"
size_t sk_dict_sizeof(void);
#line 1759 "core.org"
void sk_dict_init(sk_dict *d);
#line 1778 "core.org"
int sk_dict_clean(sk_dict *d);
#line 1814 "core.org"
int sk_dict_append(sk_dict *d,
                   const char *key,
                   int sz,
                   void *p,
                   void (*del)(void*));

int sk_dict_sappend(sk_dict *d,
                    const char *key,
                    int sz,
                    void *p,
                    void (*del)(void*),
                    sk_stacklet **s);
#line 1888 "core.org"
int sk_core_append(sk_core *core,
                   const char *key,
                   int sz,
                   void *p,
                   void (*del)(void*));
#line 1909 "core.org"
int sk_dict_lookup_stacklet(sk_dict *d,
                            const char *key,
                            int sz,
                            sk_stacklet **s);

int sk_dict_lookup(sk_dict *d,
                   const char *key,
                   int sz,
                   void **p);
#line 1970 "core.org"
int sk_core_lookup(sk_core *core,
                   const char *key,
                   int sz,
                   void **p);
#line 1989 "core.org"
int sk_dict_remove(sk_dict *d, const char *key, int sz);
#line 2030 "core.org"
int sk_core_remove(sk_core *core,
                   const char *key,
                   int sz);
#line 2047 "core.org"
int sk_core_grab(sk_core *core,
                 const char *key,
                 int sz);
#line 2087 "core.org"
int sk_core_append_table(sk_core *core,
                         const char *key,
                         int sz,
                         int tabsz);
#line 32 "core.org"

#ifdef __plan9__
#pragma incomplete sk_core
#pragma incomplete sk_stack
#pragma incomplete sk_regtbl
#pragma incomplete sk_table
#endif
#endif
