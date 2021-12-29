#line 44 "core.org"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graforge.h"
#define SK_CORE_PRIV
#include "core.h"
#line 1633 "core.org"
static int hash(const char *str, int sz, int mod);
#line 44 "core.org"
#line 329 "core.org"
enum {
   SK_TYPE_NONE,
   SK_TYPE_CONSTANT,
   SK_TYPE_CABLE,
   SK_TYPE_TABLE,
   SK_TYPE_GENERIC
};
#line 44 "core.org"
#line 67 "core.org"
sk_core * sk_core_new(int sr)
{
    sk_core *core;
    gf_patch *patch;

    core = malloc(sizeof(sk_core));
    core->patch = malloc(gf_patch_size());

    patch = core->patch;
    gf_patch_init(patch, 64);
    gf_patch_alloc(patch, 8, 10);
    gf_patch_srate_set(patch, sr);

    sk_stack_init(&core->stack);
    sk_regtbl_init(&core->regtbl);
    sk_dict_init(&core->dict);

    sk_core_srand(core, 0);
    return core;
}
#line 91 "core.org"
void sk_core_del(sk_core *core)
{
    if (core == NULL) return;

    gf_patch_destroy(core->patch);
    gf_patch_free_nodes(core->patch);
    free(core->patch);
    sk_dict_clean(&core->dict);
    free(core);
    core = NULL;
}
#line 130 "core.org"
void sk_core_compute(sk_core *core)
{
    gf_patch_compute(core->patch);
}
#line 148 "core.org"
size_t sk_core_computes(sk_core *core, SKFLT secs)
{
    size_t nblocks, n;
    nblocks = sk_core_seconds_to_blocks(core, secs);

    for (n = 0; n < nblocks; n++) {
        sk_core_compute(core);
    }

    return nblocks;
}
#line 172 "core.org"
gf_patch * sk_core_patch(sk_core *core)
{
    return core->patch;
}
#line 188 "core.org"
size_t sk_core_seconds_to_blocks(sk_core *core, SKFLT secs)
{
    size_t nblocks;
    int sr;

    sr = gf_patch_srate_get(core->patch);
    nblocks = floor((sr * secs) / 64) + 1;

    return nblocks;
}
#line 210 "core.org"
int sk_core_blkset(sk_core *core, int sz)
{
    return gf_patch_blksize_set(core->patch, sz);
}
#line 223 "core.org"
sk_stack* sk_core_stack(sk_core *core)
{
    return &core->stack;
}
#line 260 "core.org"
void sk_stacklet_init(sk_stacklet *s)
{
    s->type = SK_TYPE_NONE;
    s->f = 0;
    s->ptr = NULL;
}
#line 275 "core.org"
SKFLT sk_stacklet_constant(sk_stacklet *s)
{
    return s->f;
}
#line 313 "core.org"
void sk_stack_init(sk_stack *s)
{
    int i;

    for (i = 0; i < SK_STACKSIZE; i++) {
        sk_stacklet_init(&s->stack[i]);
    }

    s->pos = -1;
}
#line 349 "core.org"
int sk_stacklet_isnone(sk_stacklet *s)
{
    return s->type == SK_TYPE_NONE;
}

int sk_stacklet_isconstant(sk_stacklet *s)
{
    return s->type == SK_TYPE_CONSTANT;
}

int sk_stacklet_iscable(sk_stacklet *s)
{
    return s->type == SK_TYPE_CABLE;
}

int sk_stacklet_istable(sk_stacklet *s)
{
    return s->type == SK_TYPE_TABLE;
}

int sk_stacklet_isgeneric(sk_stacklet *s)
{
    return s->type == SK_TYPE_GENERIC;
}
#line 393 "core.org"
int sk_stack_pop(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;
    /* no items on stack */
    if (stack->pos < 0) return 1;

    /* stack overflow */
    if (stack->pos >= SK_STACKSIZE) return 2;

    s = &stack->stack[stack->pos];
    stack->pos--;

    *out = s;
    return 0;
}
#line 412 "core.org"
int sk_stack_push(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;

    if (stack->pos >= (SK_STACKSIZE - 1)) return 1;
    stack->pos++;
    s = &stack->stack[stack->pos];

    sk_stacklet_init(s);
    *out = s;
    return 0;
}
#line 436 "core.org"
int sk_stack_peak(sk_stack *stack, sk_stacklet **out)
{
    sk_stacklet *s;
    if (stack->pos < 0) return 1;
    if (stack->pos >= SK_STACKSIZE) return 2;

    s = &stack->stack[stack->pos];
    *out = s;
    return 0;
}
#line 461 "core.org"
int sk_stack_dup(sk_stack *stack, sk_stacklet **out)
{
    int rc;
    sk_stacklet *a, *b;

    rc = sk_stack_peak(stack, &a);
    SK_ERROR_CHECK(rc);

    rc = sk_stack_push(stack, &b);
    SK_ERROR_CHECK(rc);

    *b = *a;

    if (out != NULL) *out = b;

    return 0;
}
#line 492 "core.org"
int sk_core_dup(sk_core *core)
{
    sk_stacklet *s;
    int rc;

    rc = sk_stack_dup(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CABLE) {
        gf_stack *stack;
        stack = gf_patch_stack(core->patch);
        gf_stack_dup(stack);
    }

    return 0;
}
#line 524 "core.org"
int sk_stack_drop(sk_stack *stack, sk_stacklet **out)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(stack, &s);

    SK_ERROR_CHECK(rc);

    if (out != NULL) *out = s;
    return 0;
}
#line 549 "core.org"
int sk_core_drop(sk_core *core)
{
    int rc;
    sk_stacklet *s;
    rc = sk_stack_drop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CABLE) {
        gf_stack *stack;
        stack = gf_patch_stack(core->patch);
        gf_stack_pop(stack, NULL);
    }

    return 0;
}
#line 584 "core.org"
int sk_stack_swap(sk_stack *stack,
                  sk_stacklet **out1,
                  sk_stacklet **out2)
{
    sk_stacklet tmp;
    int pos;

    pos = stack->pos;
    if (pos < 1) return 1;


    tmp = stack->stack[pos];

    stack->stack[pos] = stack->stack[pos - 1];
    stack->stack[pos - 1] = tmp;

    if (out1 != NULL) *out1 = &stack->stack[pos - 1];
    if (out2 != NULL) *out2 = &stack->stack[pos];

    return 0;
}
#line 617 "core.org"
int sk_core_swap(sk_core *core)
{
    int rc;
    sk_stacklet *s[2];
    rc = sk_stack_swap(&core->stack, &s[0], &s[1]);

    SK_ERROR_CHECK(rc);

    if (s[0]->type == SK_TYPE_CABLE && s[1]->type == SK_TYPE_CABLE) {
        gf_stack *stack;
        stack = gf_patch_stack(core->patch);
        gf_stack_swap(stack);
    }

    return 0;
}
#line 665 "core.org"
gf_cable* sk_param_cable(sk_param *p)
{
    return p->data.c;
}

SKFLT sk_param_constant(sk_param *p)
{
    return p->data.f;
}
#line 691 "core.org"
int sk_param_get(sk_core *core, sk_param *p)
{
    sk_stack *stk;
    sk_stacklet *s;
    int rc;

    stk = &core->stack;

    rc = sk_stack_pop(stk, &s);
    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CONSTANT) {
        p->type = 0;
        p->data.f = s->f;
    } else if (s->type == SK_TYPE_CABLE) {
        p->type = 1;
        p->data.c = (gf_cable *)s->ptr;
        gf_cable_pop(p->data.c);
    } else {
        /* Wrong type! */
        return 1;
    }

    return 0;
}
#line 728 "core.org"
int sk_param_get_constant(sk_core *core, SKFLT *val)
{
    sk_stack *stk;
    sk_stacklet *s;
    int rc;

    stk = &core->stack;

    rc = sk_stack_pop(stk, &s);
    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_CONSTANT) {
        /* Wrong type! */
        return 1;
    }

    *val = s->f;
    return 0;
}
#line 759 "core.org"
int sk_param_get_cable(sk_core *core, sk_param *p)
{
    sk_stack *stk;
    sk_stacklet *s;
    int rc;

    stk = &core->stack;

    rc = sk_stack_pop(stk, &s);
    SK_ERROR_CHECK(rc);

    if (s->type == SK_TYPE_CABLE) {
        p->type = 1;
        p->data.c = (gf_cable *)s->ptr;
        gf_cable_pop(p->data.c);
    } else {
        /* Wrong type! */
        return 1;
    }

    return 0;
}
#line 795 "core.org"
int sk_param_set(sk_core *core,
                 gf_node *node,
                 sk_param *p,
                 int cid)
{
    gf_cable *c;

    gf_node_get_cable(node, cid, &c);
    if (p->type == 0) {
        gf_cable_set_value(c, p->data.f);
    } else {
        int rc;
        rc = gf_cable_connect(p->data.c, c);
        SK_GF_ERROR_CHECK(rc);
    }
    return 0;
}
#line 830 "core.org"
int sk_core_constant(sk_core *core, SKFLT x)
{
    int rc;
    sk_stacklet *s;
    sk_stack *stk;

    stk = &core->stack;

    rc = sk_stack_push(stk, &s);
    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_CONSTANT;
    s->f = x;

    return 0;
}
#line 866 "core.org"
int sk_param_out(sk_core *core,
                 gf_node *node,
                 int cid)
{
    gf_cable *c;
    sk_stacklet *s;
    sk_stack *stk;
    int rc;

    stk = &core->stack;

    rc = sk_stack_push(stk, &s);
    SK_ERROR_CHECK(rc);
    rc = gf_node_get_cable(node, cid, &c);
    SK_GF_ERROR_CHECK(rc);

    s->type = SK_TYPE_CABLE;
    s->ptr = c;

    return 0;
}
#line 901 "core.org"
int sk_core_generic_push(sk_core *core, void *ptr)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_GENERIC;
    s->ptr = ptr;

    return rc;
}
#line 918 "core.org"
int sk_core_generic_pop(sk_core *core, void **ptr)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_GENERIC) {
        return 1;
    }

    if (ptr != NULL) *ptr = s->ptr;

    return rc;
}
#line 944 "core.org"
int sk_core_stackpos(sk_core *core)
{
    return core->stack.pos;
}
#line 990 "core.org"
void sk_register_entry_init(sk_register_entry *e)
{
    sk_stacklet_init(&e->data);
    e->flags = 0;
}
#line 1024 "core.org"
void sk_regtbl_init(sk_regtbl *rs)
{
    int i;

    for (i = 0; i < SK_REGSIZE; i++) {
        sk_register_entry_init(&rs->r[i]);
    }
}
#line 1053 "core.org"
int sk_register_get(sk_regtbl *rt, int pos, sk_stacklet *s)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    *s = rt->r[pos].data;
    return 0;
}
#line 1068 "core.org"
int sk_core_regget(sk_core *core, int pos)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);
    SK_ERROR_CHECK(rc);
    rc = sk_register_get(&core->regtbl, pos, s);
    SK_ERROR_CHECK(rc);

    /* also push to buffer stack if cable */
    if (s->type == SK_TYPE_CABLE) {
        gf_cable *c;
        gf_buffer *b;
        gf_stack *bstack;
        c = (gf_cable *) s->ptr;
        b = gf_cable_get_buffer(c);
        bstack = gf_patch_stack(core->patch);
        gf_stack_push_buffer(bstack, b);
    }

    return 0;
}
#line 1095 "core.org"
int sk_register_set(sk_regtbl *rt, int pos, sk_stacklet *s)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].data = *s;
    return 0;
}
#line 1106 "core.org"
int sk_core_regset(sk_core *core, int pos)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);
    SK_ERROR_CHECK(rc);
    rc = sk_register_set(&core->regtbl, pos, s);
    SK_ERROR_CHECK(rc);

    return 0;
}
#line 1131 "core.org"
int sk_register_mark(sk_regtbl *rt, int pos)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].flags |= 1;
    return 0;
}
#line 1142 "core.org"
int sk_core_regmrk(sk_core *core, int pos)
{
    return sk_register_mark(&core->regtbl, pos);
}
#line 1160 "core.org"
int sk_register_clear(sk_regtbl *rt, int pos)
{
    if (pos < 0 || pos >= SK_REGSIZE) return 1;

    rt->r[pos].flags = 0;
    return 0;
}
#line 1171 "core.org"
int sk_core_regclr(sk_core *core, int pos)
{
    return sk_register_clear(&core->regtbl, pos);
}
#line 1195 "core.org"
int sk_register_nxtfree(sk_regtbl *rt, int start)
{
    int pos;
    int i;
    sk_register_entry *reg;

    reg = rt->r;

    if (start < 0 || start >= SK_REGSIZE) start = 0;

    pos = start;

    for (i = 0; i < SK_REGSIZE; i++) {
        if (!(reg[pos].flags & 1)) return pos;
        pos = (pos + 1) % SK_REGSIZE;
    }

    return -1;
}
#line 1222 "core.org"
int sk_core_regnxt(sk_core *core, int start, int *pos)
{
    *pos = sk_register_nxtfree(&core->regtbl, start);

    if (*pos < 0) return 1;

    return 0;
}
#line 1261 "core.org"
int sk_core_hold(sk_core *core)
{
    int rc;
    sk_stacklet *s;
    rc = sk_stack_peak(&core->stack, &s);
    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_CABLE) {
        /* Wrong type, kiddo */
        return 2;
    }

    rc = gf_patch_bhold(core->patch, NULL);
    SK_GF_ERROR_CHECK(rc);

    return 0;
}
#line 1282 "core.org"
int sk_core_unhold(sk_core *core)
{
    sk_param cable;
    gf_buffer *buf;
    gf_cable *c;
    int rc;

    rc = sk_param_get(core, &cable);
    SK_ERROR_CHECK(rc);

    if (cable.type != 1) {
        /* Your princess is is another castle. */
        return 2;
    }

    c = cable.data.c;
    buf = gf_cable_get_buffer(c);
    rc = gf_patch_bunhold(core->patch, buf);
    SK_GF_ERROR_CHECK(rc);

    return 0;
}
#line 1339 "core.org"
static void free_table(gf_pointer *p)
{
    sk_table *tab;

    tab = gf_pointer_data(p);

    free(tab->tab);
    free(tab);
}

int sk_core_table_new(sk_core *core, unsigned long sz)
{
    sk_table *tab;
    int rc;
    SKFLT *data;

    tab = malloc(sizeof(sk_table));

    if (tab == NULL) return 1;
    data = malloc(sz * sizeof(SKFLT));
    memset(data, 0, sz * sizeof(SKFLT));

    sk_table_init(tab, data, sz);

    gf_patch_append_userdata(core->patch, free_table, tab);

    rc = sk_core_table_push(core, tab);
    SK_ERROR_CHECK(rc);

    return 0;
}
#line 1379 "core.org"
void sk_table_init(sk_table *tab, SKFLT *data, unsigned long sz)
{
    tab->tab = data;
    tab->sz = sz;
}
#line 1396 "core.org"
size_t sk_table_size(sk_table *t)
{
    return t->sz;
}

SKFLT* sk_table_data(sk_table *t)
{
    return t->tab;
}
#line 1417 "core.org"
int sk_core_table_push(sk_core *core, sk_table *tab)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_push(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    s->type = SK_TYPE_TABLE;
    s->ptr = tab;

    return rc;
}
#line 1435 "core.org"
int sk_core_table_pop(sk_core *core, sk_table **tab)
{
    int rc;
    sk_stacklet *s;

    rc = sk_stack_pop(&core->stack, &s);

    SK_ERROR_CHECK(rc);

    if (s->type != SK_TYPE_TABLE) {
        printf("uh oh type is %d, %g\n", s->type, s->f);
        printf("%d items on stack\n", core->stack.pos);
        return 1;
    }

    *tab = (sk_table *)s->ptr;

    return rc;
}
#line 1476 "core.org"
int sk_table_dump(sk_table *tab, const char *filename)
{
    FILE *fp;

    fp = fopen(filename, "w");

    if (fp == NULL) return 1;

    fwrite(tab->tab, sizeof(SKFLT), tab->sz, fp);

    fclose(fp);

    return 0;
}
#line 1494 "core.org"
int sk_core_tabdump(sk_core *core, const char *filename)
{
    int rc;
    sk_table *tab;

    rc = sk_core_table_pop(core, &tab);
    SK_ERROR_CHECK(rc);

    return sk_table_dump(tab, filename);
}
#line 1535 "core.org"
void sk_core_srand(sk_core *core, unsigned long val)
{
    core->rng = val;
}
#line 1557 "core.org"
unsigned long sk_core_rand(sk_core *core)
{
    core->rng = (1103515245 * core->rng + 12345) % SK_CORE_RANDMAX;
    return core->rng;
}
#line 1573 "core.org"
SKFLT sk_core_randf(sk_core *core)
{
    return (SKFLT)sk_core_rand(core) / SK_CORE_RANDMAX;
}
#line 1623 "core.org"
size_t sk_dict_sizeof(void)
{
    return sizeof(sk_dict);
}
#line 1638 "core.org"
static int hash(const char *str, int sz, int mod)
{
    unsigned long h;
    int i;

    h = 5381;
    i = 0;

    for(i = 0; i < sz; i++) {
        h = ((h << 5) + h) ^ str[i];
        h %= 0x7FFFFFFF;
    }

    return h % mod;
}
#line 1662 "core.org"
void sk_dict_init(sk_dict *d)
{
    int n;

    for (n = 0; n < 64; n++) {
        d->ent[n] = NULL;
    }

    d->sz = 0;
}
#line 1681 "core.org"
int sk_dict_clean(sk_dict *d)
{
    int e;

    for (e = 0; e < 64; e++) {
        struct dict_entry *ent, *nxt;
        ent = d->ent[e];

        while (ent != NULL) {
            nxt = ent->nxt;
            if (ent->del != NULL) ent->del(ent->val);
            free(ent->key);
            free(ent);
            ent = nxt;
        }
    }

    return 0;
}
#line 1713 "core.org"
int sk_dict_append(sk_dict *d,
                   const char *key,
                   int sz,
                   void *val,
                   void (*del)(void*))
{
    int pos;
    struct dict_entry *ent;
    int i;

    pos = hash(key, sz, 64);

    ent = d->ent[pos];

    while (ent != NULL) {
        if (ent->sz == sz && !strcmp(key, ent->key)) {
            return 1;
        }
        ent = ent->nxt;
    }

    ent = malloc(sizeof(struct dict_entry));
    ent->key = malloc(sz + 1);
    ent->sz = sz;

    for (i = 0; i < sz; i++) ent->key[i] = key[i];
    ent->key[sz] = '\0';

    ent->val = val;
    ent->del = del;
    ent->nxt = d->ent[pos];
    d->ent[pos] = ent;
    return 0;
}
#line 1762 "core.org"
int sk_core_append(sk_core *core,
                   const char *key,
                   int sz,
                   void *p,
                   void (*del)(void*))
{
    return sk_dict_append(&core->dict, key, sz, p, del);
}
#line 1782 "core.org"
int sk_dict_lookup(sk_dict *d,
                   const char *key,
                   int sz,
                   void **p)
{
    int pos;
    struct dict_entry *ent;

    pos = hash(key, sz, 64);

    ent = d->ent[pos];

    while (ent != NULL) {
        if (ent->sz == sz && !strncmp(key, ent->key, sz)) {
            *p = ent->val;
            return 0;
        }
        ent = ent->nxt;
    }

    return 1;
}
#line 1819 "core.org"
int sk_core_lookup(sk_core *core,
                   const char *key,
                   int sz,
                   void **p)
{
    return sk_dict_lookup(&core->dict, key, sz, p);
}
#line 1835 "core.org"
int sk_dict_remove(sk_dict *d, const char *key, int sz)
{
    int pos;
    struct dict_entry *ent, *prv;

    pos = hash(key, sz, 64);

    ent = d->ent[pos];

    prv = NULL;

    while (ent != NULL) {
        if (ent->sz == sz && !strncmp(key, ent->key, sz)) {

            if (prv == NULL) {
                d->ent[pos] = ent->nxt;
            } else {
                prv->nxt = ent->nxt;
            }
            if (ent->del) ent->del(ent->val);
            free(ent->key);
            free(ent);
            return 0;
        }
        prv = ent;
        ent = ent->nxt;
    }
    return 1;
}
#line 1878 "core.org"
int sk_core_remove(sk_core *core,
                   const char *key,
                   int sz)
{
    return sk_dict_remove(&core->dict, key, sz);
}
#line 1895 "core.org"
int sk_core_grab(sk_core *core,
                 const char *key,
                 int sz)
{
    int rc;
    void *ptr;

    rc = sk_core_lookup(core, key, sz, &ptr);

    if (rc) return 1;

    rc = sk_core_generic_push(core, ptr);

    if (rc) return 2;

    return 0;
}
#line 1927 "core.org"
static void deltab(void *ptr)
{
    sk_table *tab;

    tab = ptr;

    free(tab->tab);
    free(tab);
}

int sk_core_append_table(sk_core *core,
                         const char *key,
                         int sz,
                         int tabsz)
{
    int rc;
    sk_table *tab;

    tab = calloc(1, sizeof(sk_table));

    tab->tab = calloc(tabsz, sizeof(SKFLT));
    tab->sz = tabsz;

    rc = sk_core_append(core, key, sz, tab, deltab);
    SK_ERROR_CHECK(rc);

    rc = sk_core_table_push(core, tab);
    SK_ERROR_CHECK(rc);

    return 0;
}
#line 44 "core.org"
