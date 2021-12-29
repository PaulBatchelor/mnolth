#line 58 "gest.org"
#ifndef GEST_H
#define GEST_H

#ifndef SKFLT
#define SKFLT float
#endif

#line 93 "gest.org"
typedef struct gest_d gest_d;
#line 533 "gest.org"
typedef struct gest_state gest_state;
#line 1983 "gest.org"
typedef struct gest_phrase gest_phrase;
#line 2061 "gest.org"
typedef struct gest_node gest_node;
#line 3229 "gest.org"
typedef struct gest_collection gest_collection;
#line 3396 "gest.org"
typedef struct gest_target gest_target;
#line 3792 "gest.org"
typedef struct gest_behavior gest_behavior;
typedef SKFLT (* gest_bfunc) (gest_d *, SKFLT, void *);
#line 4042 "gest.org"
typedef struct gest_metatarget gest_metatarget;
#line 4177 "gest.org"
typedef struct gest_metabehavior gest_metabehavior;
#line 4332 "gest.org"
typedef struct gest_metanode gest_metanode;
#line 4533 "gest.org"
typedef struct gest_metaphrase gest_metaphrase;
#line 4823 "gest.org"
typedef struct gest_action gest_action;
#line 4844 "gest.org"
typedef struct gest_actionlist gest_actionlist;
#line 4979 "gest.org"
typedef struct {
    SKFLT cur;
    SKFLT nxt;
    int pos;
} gest_scalar;
#line 66 "gest.org"

#ifdef GEST_PRIVATE
#line 93 "gest.org"
#line 3396 "gest.org"
#line 3798 "gest.org"
struct gest_behavior {
    gest_bfunc tick;
    void *ud;
    gest_behavior * (*get)(gest_d *, gest_behavior *b);
    gest_metabehavior *meta;
};
#line 3396 "gest.org"
#line 4849 "gest.org"
struct gest_actionlist {
    gest_action *head;
    gest_action *tail;
    int sz;
};
#line 3414 "gest.org"
struct gest_target {
    SKFLT value;
    gest_behavior behavior;
    gest_behavior *curbehavior;
    void *ud; /* user data attached to this target */
    gest_target *next;
    gest_target* (*get)(gest_d *, gest_target *);
    gest_metatarget *meta;
#line 3721 "gest.org"
SKFLT (*mix)(gest_d *, SKFLT, SKFLT, SKFLT);
#line 4860 "gest.org"
gest_actionlist lst;
#line 3423 "gest.org"
};
#line 93 "gest.org"
#line 2066 "gest.org"
struct gest_node {
#line 2093 "gest.org"
int type;
#line 2115 "gest.org"
int modifier;
#line 2129 "gest.org"
int nchildren;
#line 2143 "gest.org"
gest_node *children;
#line 2157 "gest.org"
gest_target *target;
#line 2171 "gest.org"
gest_node *next;
#line 2185 "gest.org"
gest_node *parent;
#line 2197 "gest.org"
int id;
#line 2209 "gest.org"
gest_metanode *meta;
#line 2221 "gest.org"
gest_node* (*get)(gest_d *, gest_node *);
#line 2068 "gest.org"
};
#line 93 "gest.org"
#line 1988 "gest.org"
struct gest_phrase {
    gest_node *top;
    SKFLT mod;
    int beats;
    gest_phrase *next;
    gest_phrase* (*nextf)(gest_d *, gest_phrase *);
    gest_metaphrase *meta;
    gest_phrase* (*get)(gest_d *, gest_phrase *);
    void *ud;
#line 2044 "gest.org"
SKFLT (*skew)(SKFLT, SKFLT);
#line 1998 "gest.org"
};
#line 93 "gest.org"
#line 3229 "gest.org"
#line 3250 "gest.org"
struct gest_entry {
    void *ptr;
    void (*free)(void *);
    struct gest_entry *prev;
};
#line 3237 "gest.org"
struct gest_collection {
    struct gest_entry *tail;
    int nitems;
};
#line 93 "gest.org"
#line 542 "gest.org"
struct gest_state {
    gest_target *target;
    gest_node *node;
    gest_phrase *phrase;
    gest_behavior *behavior;
    int num;
    int den;
    int please_wait;
};
#line 103 "gest.org"
struct gest_d {
#line 154 "gest.org"
SKFLT last;
SKFLT last_inc;
#line 170 "gest.org"
SKFLT phs;
#line 187 "gest.org"
gest_target *curtarget;
gest_target *toptarget;
gest_target *nxttarget;
int targetondeck;
#line 206 "gest.org"
SKFLT curval;
SKFLT nxtval;
#line 274 "gest.org"
int num;
int den;
#line 302 "gest.org"
gest_phrase *phrase_top;
gest_phrase *phrase_selected;
#line 317 "gest.org"
gest_collection col;
#line 335 "gest.org"
gest_node *curnode;
#line 363 "gest.org"
gest_node *nxtnode;
#line 375 "gest.org"
int nodepos;
#line 394 "gest.org"
SKFLT inertia;
SKFLT mass;
#line 433 "gest.org"
int beat;
SKFLT t;
#line 445 "gest.org"
int please_wait;
#line 455 "gest.org"
SKFLT correction;
#line 465 "gest.org"
void *ud;
#line 498 "gest.org"
SKFLT prevout;
#line 511 "gest.org"
gest_state nxtstate;
#line 579 "gest.org"
gest_phrase *saved;
#line 593 "gest.org"
SKFLT (*mix)(gest_d *, SKFLT, SKFLT, SKFLT);
#line 609 "gest.org"
int squawk;
#line 652 "gest.org"
int spillover;
SKFLT tolerance;
#line 4101 "gest.org"
#define GEST_MTSTACK_SIZE 4
gest_metatarget *mtstack[GEST_MTSTACK_SIZE];
int mtpos;
#line 4230 "gest.org"
#define GEST_MBSTACK_SIZE 4
gest_metabehavior *mbstack[GEST_MBSTACK_SIZE];
int mbpos;
#line 4391 "gest.org"
#define GEST_MNSTACK_SIZE 4
gest_metanode *mnstack[GEST_MNSTACK_SIZE];
int mnpos;
#line 4586 "gest.org"
#define GEST_MPSTACK_SIZE 4
gest_metaphrase *mpstack[GEST_MPSTACK_SIZE];
int mppos;
#line 4733 "gest.org"
unsigned long rng;
#line 105 "gest.org"
};
#line 4047 "gest.org"
struct gest_metatarget {
    int pos;
    int size;
    gest_target *parent;
    gest_target **targets;
};
#line 4188 "gest.org"
struct gest_metabehavior {
    int pos;
    int size;
    gest_behavior *parent;
    gest_behavior *behaviors;
};
#line 4337 "gest.org"
struct gest_metanode {
    int pos;
    int size;
    gest_node *parent;
    gest_node **nodes;
};
#line 4538 "gest.org"
struct gest_metaphrase {
    int pos;
    int size;
    gest_phrase *parent;
    gest_phrase **phrases;
};
#line 4832 "gest.org"
struct gest_action {
    void (*fun)(gest_d*, void*, int);
    void *data;
    gest_action *next;
};
#line 69 "gest.org"
#endif

#line 110 "gest.org"
void gest_init(gest_d *g);
#line 125 "gest.org"
void gest_cleanup(gest_d *g);
#line 140 "gest.org"
size_t gest_sizeof(void);
#line 236 "gest.org"
gest_target * gest_curtarget(gest_d *g);
#line 249 "gest.org"
gest_target * gest_nxttarget(gest_d *g);
#line 411 "gest.org"
SKFLT gest_mass_get(gest_d *g);
SKFLT gest_inertia_get(gest_d *g);
#line 475 "gest.org"
void gest_data_set(gest_d *g, void *ud);
void * gest_data_get(gest_d *g);
#line 620 "gest.org"
void gest_squawk(gest_d *g, int squawk);
#line 665 "gest.org"
void gest_tolerance(gest_d *g, SKFLT tolerance);
#line 683 "gest.org"
void gest_begin(gest_d *g, int beats, int div);
#line 723 "gest.org"
int gest_end(gest_d *g);
#line 753 "gest.org"
int gest_polyramp(gest_d *g, int div);
#line 782 "gest.org"
int gest_monoramp(gest_d *g, int nbeats);
#line 808 "gest.org"
int gest_addtarget(gest_d *g, SKFLT val);
#line 844 "gest.org"
void gest_finish(gest_d *g);
#line 883 "gest.org"
void gest_loopit(gest_d *g);
#line 906 "gest.org"
int gest_addmetatarget(gest_d *g, int sz);
#line 948 "gest.org"
int gest_addmetabehavior(gest_d *g, int sz);
#line 1000 "gest.org"
int gest_addmetanode(gest_d *g, int nbeats, int sz);
#line 1043 "gest.org"
int gest_addmetaphrase(gest_d *g, int sz);
#line 1082 "gest.org"
int gest_randtarget(gest_d *g);
#line 1104 "gest.org"
int gest_randbehavior(gest_d *g);
#line 1127 "gest.org"
int gest_randnode(gest_d *g);
#line 1150 "gest.org"
int gest_randphrase(gest_d *g);
#line 1173 "gest.org"
int gest_setscalar(gest_d *g, gest_scalar *s, SKFLT val);
#line 1192 "gest.org"
int gest_repeat(gest_d *g, int ntimes);
#line 1251 "gest.org"
int gest_mark(gest_d *g);
#line 1269 "gest.org"
int gest_return(gest_d *g, int ntimes);
#line 1332 "gest.org"
int gest_skewquad(gest_d *g, int dir);
#line 1383 "gest.org"
int gest_skewexp(gest_d *g, int dir);
#line 1434 "gest.org"
int gest_skewshuf(gest_d *g);
#line 1475 "gest.org"
int gest_ramp(gest_d *g);
#line 1504 "gest.org"
int gest_invramp(gest_d *g);
#line 1535 "gest.org"
void gest_behavior_linear(gest_d *g);
#line 1556 "gest.org"
void gest_behavior_step(gest_d *g);
#line 1582 "gest.org"
void gest_behavior_gliss(gest_d *g);
#line 1613 "gest.org"
void gest_behavior_smallgliss(gest_d *g);
#line 1641 "gest.org"
void gest_behavior_mediumgliss(gest_d *g);
#line 1670 "gest.org"
void gest_behavior_exponential(gest_d *g, SKFLT slope);
#line 1675 "gest.org"
void gest_behavior_step(gest_d *g);
#line 1704 "gest.org"
void gest_behavior_bezier(gest_d *g, SKFLT cx, SKFLT cy);
#line 1792 "gest.org"
void gest_behavior_smoothstep(gest_d *g);
#line 1819 "gest.org"
void gest_behavior_biramp(gest_d *g, SKFLT pos);
#line 1824 "gest.org"
void gest_behavior_biramp(gest_d *g, SKFLT pos);
#line 1859 "gest.org"
void gest_behavior_gate(gest_d *g, SKFLT pos);
#line 1893 "gest.org"
void gest_mass(gest_d *g, SKFLT mass);
#line 1908 "gest.org"
void gest_inertia(gest_d *g, SKFLT inertia);
#line 1924 "gest.org"
int gest_shrink(gest_d *g, SKFLT amt);
int gest_grow(gest_d *g, SKFLT amt);
#line 3428 "gest.org"
void gest_target_init(gest_target *t);
#line 3751 "gest.org"
void gest_target_mix(gest_target *t,
                     SKFLT (*mix)(gest_d *, SKFLT, SKFLT, SKFLT));
#line 3766 "gest.org"
void gest_target_data_set(gest_target *t, void *ud);
void* gest_target_data_get(gest_target *t);
#line 3810 "gest.org"
void gest_behavior_init(gest_behavior *b);
#line 3828 "gest.org"
void gest_behavior_set(gest_behavior *b, gest_bfunc tick, void *ud);
#line 3844 "gest.org"
SKFLT gest_behavior_tick(gest_behavior *b, gest_d *g, SKFLT a);
#line 3896 "gest.org"
SKFLT gest_tick(gest_d *g, SKFLT conductor, int blkpos);
#line 4740 "gest.org"
void gest_seed(gest_d *g, unsigned long rng);
#line 4764 "gest.org"
unsigned long gest_rand(gest_d *g);
#line 4781 "gest.org"
SKFLT gest_randf(gest_d *g);
#line 4801 "gest.org"
int gest_randi(gest_d *g, int N);
#line 4872 "gest.org"
void gest_actionlist_init(gest_actionlist *lst);
#line 4997 "gest.org"
void gest_scalar_init(gest_scalar *s);
#line 72 "gest.org"
#endif
