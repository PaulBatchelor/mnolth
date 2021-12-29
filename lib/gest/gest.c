#line 77 "gest.org"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define GEST_PRIVATE
#include "gest.h"
#line 2103 "gest.org"
enum  {
    NODE_NOTHING,
    NODE_POLYRAMP,
    NODE_MONORAMP
};
#line 77 "gest.org"
#line 220 "gest.org"
static void set_curtarget(gest_d *g, gest_target *target, int pos);
#line 347 "gest.org"
static void set_curnode(gest_d *g, gest_node *node);
#line 557 "gest.org"
static void init_state(gest_state *s);
#line 2011 "gest.org"
static void phrase_init(gest_d *g,
                        gest_phrase *phrase,
                        int beats,
                        int div);
#line 2077 "gest.org"
static void gest_node_init(gest_node *n);
#line 2267 "gest.org"
static gest_node * mkpolyramp(gest_d *g,
                              gest_node *parent,
                              int div);
#line 2328 "gest.org"
static gest_node * mkmonoramp(gest_d *g,
                              gest_node *parent,
                              int ninputs);
#line 2375 "gest.org"
static int node_count(gest_node *node, gest_node **last);
#line 2418 "gest.org"
static void append_node(gest_node *parent,
                        gest_node *node,
                        gest_node *last);
#line 2443 "gest.org"
static gest_node * dive_to_target(gest_d *g,
                                  gest_node *node);
#line 2483 "gest.org"
static void revert_modifier(gest_d *g, gest_node *node);
static void apply_modifier(gest_d *g, gest_node *node);
#line 2534 "gest.org"
static gest_node* acquire_children(gest_d *g, gest_node *n);
static gest_node* get_node(gest_d *g, gest_node *n);
#line 2566 "gest.org"
static gest_target *node_target(gest_d *g, gest_node *node);
#line 2606 "gest.org"
static SKFLT ramptree_step(gest_d *g,
                           SKFLT inc,
                           int reset,
                           int blkpos);
#line 2862 "gest.org"
static void find_next_node(gest_d *g,
                           gest_node *top,
                           gest_state *state);
#line 3045 "gest.org"
static gest_phrase* get_phrase(gest_d *g, gest_phrase *ph);
#line 3066 "gest.org"
static gest_phrase* next_phrase(gest_d *g, gest_phrase *ph);
#line 3261 "gest.org"
static void collection_init(gest_collection *col);
#line 3285 "gest.org"
static void * collection_alloc_dtor(gest_collection *col,
                                    size_t sz,
                                    void (*free)(void*));
#line 3316 "gest.org"
static void * collection_alloc(gest_collection *col, size_t sz);
#line 3335 "gest.org"
static void * gest_alloc(gest_d *gest, size_t sz);
#line 3352 "gest.org"
static void collection_cleanup(gest_collection *col);
#line 3456 "gest.org"
static gest_target * mktarget(gest_d *g);
#line 3662 "gest.org"
static void update_mass(gest_d *g, void *ud, int pos);
static void update_inertia(gest_d *g, void *ud, int pos);
#line 3688 "gest.org"
static void action_mass(gest_d *g, gest_target *t, SKFLT mass);
static void action_inertia(gest_d *g, gest_target *t, SKFLT interia);
#line 3735 "gest.org"
static SKFLT default_mix(gest_d *g, SKFLT x, SKFLT y, SKFLT a);
#line 3857 "gest.org"
static gest_behavior * target_behavior(gest_d *g, gest_target *t);
#line 4060 "gest.org"
static gest_metatarget * metatarget_alloc(gest_d *g, int sz);
#line 4128 "gest.org"
static gest_target *get_seq(gest_d *g, gest_target *t);
#line 4150 "gest.org"
static gest_target *target_random(gest_d *g, gest_target *t);
#line 4201 "gest.org"
static gest_metabehavior * metabehavior_alloc(gest_d *g, int sz);
#line 4256 "gest.org"
static void set_behavior(gest_d *g, gest_bfunc tick, void *ud);
#line 4287 "gest.org"
static gest_behavior* behave_seq(gest_d *g, gest_behavior *b);
#line 4306 "gest.org"
static gest_behavior* behave_random(gest_d *g, gest_behavior *b);
#line 4349 "gest.org"
static gest_metanode* metanode_alloc(gest_d *g,
                                     gest_node *parent,
                                     int sz);
#line 4488 "gest.org"
static gest_node* node_seq(gest_d *g, gest_node *n);
#line 4507 "gest.org"
static gest_node* node_random(gest_d *g, gest_node *n);
#line 4551 "gest.org"
static gest_metaphrase * metaphrase_alloc(gest_d *g, int sz);
#line 4691 "gest.org"
static gest_phrase* phrase_seq(gest_d *g, gest_phrase *ph);
#line 4710 "gest.org"
static gest_phrase* phrase_random(gest_d *g, gest_phrase *ph);
#line 4891 "gest.org"
static void append_action(gest_d *g,
                          gest_target *t,
                          void (*fun)(gest_d*, void*, int),
                          void *data);
#line 4937 "gest.org"
static void do_actions(gest_d *g, gest_target *t, int pos);
#line 5028 "gest.org"
static void action_scalar(gest_d *g,
                          gest_target *t,
                          gest_scalar *s,
                          SKFLT val);
#line 77 "gest.org"
#line 115 "gest.org"
void gest_init(gest_d *g)
{
#line 160 "gest.org"
g->last = 0;
g->last_inc = 0;
#line 175 "gest.org"
g->phs = -1;
#line 195 "gest.org"
g->curtarget = NULL;
g->toptarget = NULL;
g->nxttarget = NULL;
g->targetondeck = 0;
#line 212 "gest.org"
g->curval = 0;
g->nxtval = 0;
#line 280 "gest.org"
g->num = 1;
g->den = 1;
#line 308 "gest.org"
g->phrase_top = NULL;
g->phrase_selected = NULL;
#line 322 "gest.org"
collection_init(&g->col);
#line 340 "gest.org"
g->curnode = NULL;
#line 368 "gest.org"
g->nxtnode = NULL;
#line 380 "gest.org"
g->nodepos = 0;
#line 403 "gest.org"
g->inertia = 0;
g->mass = 0;
#line 439 "gest.org"
g->beat = 0;
g->t = 0;
#line 450 "gest.org"
g->please_wait = 0;
#line 460 "gest.org"
g->correction = 1.0;
#line 470 "gest.org"
g->ud = NULL;
#line 503 "gest.org"
g->prevout = 0;
#line 516 "gest.org"
init_state(&g->nxtstate);
#line 584 "gest.org"
g->saved = NULL;
#line 598 "gest.org"
g->mix = default_mix;
#line 614 "gest.org"
gest_squawk(g, 0);
#line 658 "gest.org"
g->spillover = 0;
gest_tolerance(g, 0);
#line 4108 "gest.org"
{
    int i;

    for (i = 0; i < GEST_MTSTACK_SIZE; i++) {
        g->mtstack[i] = NULL;
    }

    g->mtpos = 0;
}
#line 4237 "gest.org"
{
    int i;

    for (i = 0; i < GEST_MBSTACK_SIZE; i++) {
        g->mbstack[i] = NULL;
    }

    g->mbpos = 0;
}
#line 4398 "gest.org"
{
    int i;

    for (i = 0; i < GEST_MNSTACK_SIZE; i++) {
        g->mnstack[i] = NULL;
    }

    g->mnpos = 0;
}
#line 4593 "gest.org"
{
    int i;

    for (i = 0; i < GEST_MPSTACK_SIZE; i++) {
        g->mpstack[i] = NULL;
    }

    g->mppos = 0;
}
#line 4757 "gest.org"
gest_seed(g, rand());
#line 118 "gest.org"
}
#line 130 "gest.org"
void gest_cleanup(gest_d *g)
{
#line 327 "gest.org"
collection_cleanup(&g->col);
#line 133 "gest.org"
}
#line 145 "gest.org"
size_t gest_sizeof(void)
{
    return sizeof(gest_d);
}
#line 225 "gest.org"
static void set_curtarget(gest_d *g, gest_target *target, int pos)
{
    g->curtarget = target;
    g->curval = target->value;
    do_actions(g, target, pos);
}
#line 241 "gest.org"
gest_target * gest_curtarget(gest_d *g)
{
    return g->curtarget;
}
#line 254 "gest.org"
gest_target * gest_nxttarget(gest_d *g)
{
    return g->nxttarget;
}
#line 352 "gest.org"
static void set_curnode(gest_d *g, gest_node *node)
{
    g->curnode = node;
}
#line 417 "gest.org"
SKFLT gest_mass_get(gest_d *g)
{
    return g->mass;
}

SKFLT gest_inertia_get(gest_d *g)
{
    return g->inertia;
}
#line 481 "gest.org"
void gest_data_set(gest_d *g, void *ud)
{
    g->ud = ud;
}

void * gest_data_get(gest_d *g)
{
    return g->ud;
}
#line 562 "gest.org"
static void init_state(gest_state *s)
{
    s->num = 1;
    s->den = 1;
    s->phrase = NULL;
    s->target = NULL;
    s->node = NULL;
    s->behavior = NULL;
    s->please_wait = 0;
}
#line 625 "gest.org"
void gest_squawk(gest_d *g, int squawk)
{
    g->squawk = squawk;
}
#line 670 "gest.org"
void gest_tolerance(gest_d *g, SKFLT tolerance)
{
    g->tolerance = tolerance;
}
#line 694 "gest.org"
void gest_begin(gest_d *g, int beats, int div)
{
    gest_phrase *phrase;

    phrase = gest_alloc(g, sizeof(gest_phrase));
    phrase_init(g, phrase, beats, div);

    if (g->phrase_top == NULL) {
        g->phrase_top = phrase;
    }

#line 4628 "gest.org"
if (g->mppos > 0) {
    gest_metaphrase *mp;
    mp = g->mpstack[g->mppos - 1];

    if (mp->pos >= mp->size) {
        int i;

        /* pop from stack */
        g->mppos--;
        g->mpstack[g->mppos] = NULL;

        /* current phrase last phrase in metaphrase, clear it */
        g->phrase_selected = NULL;

        /* all phrases should point to newly made phrase */
        for (i = 0; i < mp->size; i++) {
            mp->phrases[i]->next = phrase;
        }

    } else {
        /* append to next available slot */
        mp->phrases[mp->pos] = phrase;
    }

}
#line 706 "gest.org"

    if (g->phrase_selected != NULL) {
        g->phrase_selected->next = phrase;
    }

    g->phrase_selected = phrase;

    /* TODO make this a targetondeck flag */
    set_curnode(g, g->phrase_selected->top);
}
#line 732 "gest.org"
int gest_end(gest_d *g)
{
    int count;
    gest_node *top;

    top = g->phrase_selected->top;

    count = node_count(top, NULL);

    if (count != top->modifier) return 1;

#line 4657 "gest.org"
if (g->mppos > 0) {
    gest_metaphrase *mp;
    mp = g->mpstack[g->mppos - 1];
    mp->pos++;
}
#line 744 "gest.org"
    return 0;
}
#line 762 "gest.org"
int gest_polyramp(gest_d *g, int div)
{
    gest_node *n;

    n = mkpolyramp(g, g->curnode, div);

    if (n == NULL) {
        return 1;
    }

    set_curnode(g, n);

    return 0;
}
#line 787 "gest.org"
int gest_monoramp(gest_d *g, int nbeats)
{
    gest_node *n;

    n = mkmonoramp(g, g->curnode, nbeats);

    if (n == NULL) {
        return 1;
    }

    set_curnode(g, n);

    return 0;
}
#line 813 "gest.org"
int gest_addtarget(gest_d *g, SKFLT val)
{
    gest_target *t;

    t = mktarget(g);

    if (t == NULL) {
        return 1;
    }

    if (g->toptarget == NULL) {
        g->toptarget = t;
    }

    if (g->curtarget != NULL) {
        g->curtarget->next = t;
    }

    g->curtarget = t;
    t->value = val;
    gest_behavior_linear(g);

    return 0;
}
#line 849 "gest.org"
void gest_finish(gest_d *g)
{
    gest_node *top;
    gest_target *target;

    g->den = 1;
    g->num = 1;

    g->phrase_selected = g->phrase_top;

    top = dive_to_target(g, g->phrase_top->top);

    set_curnode(g, top);
    target = node_target(g, top);
    target->curbehavior = target_behavior(g, target);
    set_curtarget(g, target, 0);

    init_state(&g->nxtstate);
    find_next_node(g, g->curnode, &g->nxtstate);
    g->nxttarget = g->nxtstate.target;

    if (g->nxttarget != NULL) {
        g->nxtval = g->nxttarget->value;
    } else {
        g->nxtval = g->curval;
    }
}
#line 888 "gest.org"
void gest_loopit(gest_d *g)
{
    g->curtarget->next = g->toptarget;
    g->phrase_selected->next = g->phrase_top;
#line 4666 "gest.org"
if (g->mppos > 0) {
    gest_metaphrase *mp;
    mp = g->mpstack[g->mppos - 1];

    if (mp->pos >= mp->size) {
        int i;

        /* pop from stack */
        g->mppos--;
        g->mpstack[g->mppos] = NULL;

        /* current phrase last phrase in metaphrase, clear it */
        g->phrase_selected = NULL;

        /* all phrases should point to newly made phrase */
        for (i = 0; i < mp->size; i++) {
            mp->phrases[i]->next = g->phrase_top;
        }
    }

}
#line 893 "gest.org"
}
#line 918 "gest.org"
int gest_addmetatarget(gest_d *g, int sz)
{
    int rc;
    gest_target *t;
    gest_metatarget *mt;

    if (g->mtpos >= GEST_MTSTACK_SIZE) return 2;

    rc = gest_addtarget(g, -1);

    if (rc) return rc;

    t = g->curtarget;

    mt = metatarget_alloc(g, sz);

    t->get = get_seq;
    t->meta = mt;
    mt->parent = t;

    /* push to stack */
    g->mtstack[g->mtpos++] = mt;
    return 0;
}
#line 961 "gest.org"
int gest_addmetabehavior(gest_d *g, int sz)
{
    gest_target *t;
    gest_metabehavior *mb;
    gest_behavior *b;

    if (g->mbpos >= GEST_MBSTACK_SIZE) return 2;

    t = g->curtarget;

    b = &t->behavior;

    if (b->meta != NULL) {
        /* empty stack, shouldn't happen normally */
        if (g->mbpos <= 0) return 3;

        mb = b->meta;
        b = &mb->behaviors[mb->pos++];

        if (mb->pos >= mb->size) {
            g->mbstack[g->mbpos - 1] = NULL;
            g->mbpos--; /* don't be clever */
        }
    }

    mb = metabehavior_alloc(g, sz);

    b->get = behave_seq;
    b->meta = mb;
    mb->parent = b;

    /* push to stack */
    g->mbstack[g->mbpos++] = mb;
    return 0;
}
#line 1008 "gest.org"
int gest_addmetanode(gest_d *g, int nbeats, int sz)
{
    int rc;
    gest_node *n;
    gest_metanode *mn;

    if (g->mnpos >= GEST_MNSTACK_SIZE) return 1;

    rc = gest_monoramp(g, nbeats);

    if (rc) return rc;

    n = g->curnode;

    mn = metanode_alloc(g, n, sz);

    n->meta = mn;
    n->get = node_seq;
    mn->parent = n;

    set_curnode(g, mn->nodes[mn->pos++]);

    g->mnstack[g->mnpos++] = mn;

    return 0;
}
#line 1048 "gest.org"
int gest_addmetaphrase(gest_d *g, int sz)
{
    gest_metaphrase *mp;
    gest_phrase *ph;

    if (g->phrase_top == NULL) {
        fprintf(stderr, "Sorry, metaphrases can't yet be first\n");
        return 2;
    }

    if (g->mppos >= GEST_MPSTACK_SIZE) return 1;


    gest_begin(g, 1, 1);

    ph = g->phrase_selected;

    mp = metaphrase_alloc(g, sz);

    ph->get = phrase_seq;
    ph->meta = mp;
    mp->parent = ph;

    g->mpstack[g->mppos++] = mp;

    return 0;
}
#line 1087 "gest.org"
int gest_randtarget(gest_d *g)
{
    gest_metatarget *mt;
    if (g->mtpos <= 0) return 1;

    mt = g->mtstack[g->mtpos - 1];

    mt->parent->get = target_random;
    return 0;
}
#line 1109 "gest.org"
int gest_randbehavior(gest_d *g)
{
    gest_metabehavior *mb;

    if (g->mbpos <= 0) return 1;

    mb = g->mbstack[g->mbpos - 1];

    mb->parent->get = behave_random;
    return 0;
}
#line 1132 "gest.org"
int gest_randnode(gest_d *g)
{
    gest_metanode *mn;

    if (g->mnpos <= 0) return 1;

    mn = g->mnstack[g->mnpos - 1];

    mn->parent->get = node_random;
    return 0;
}
#line 1155 "gest.org"
int gest_randphrase(gest_d *g)
{
    gest_metaphrase *mp;

    if (g->mppos <= 0) return 1;

    mp = g->mpstack[g->mppos - 1];

    mp->parent->get = phrase_random;
    return 0;
}
#line 1178 "gest.org"
int gest_setscalar(gest_d *g, gest_scalar *s, SKFLT val)
{
    action_scalar(g, g->curtarget, s, val);
    return 0;
}
#line 1205 "gest.org"
struct repeat_data {
    int pos;
    int reps;
};

gest_phrase *repeater(gest_d *g, gest_phrase *ph)
{
    struct repeat_data *rd;

    if (ph->ud == NULL) return ph;

    rd = (struct repeat_data *)ph->ud;
    rd->pos++;

    if (rd->pos >= rd->reps) {
        rd->pos = 0;
        return ph->next;
    }

    return ph;
}

int gest_repeat(gest_d *g, int ntimes)
{
    gest_phrase *ph;
    struct repeat_data *rd;

    ph = g->phrase_selected;

    rd = gest_alloc(g, sizeof(struct repeat_data));
    rd->pos = 0;
    rd->reps = ntimes;

    ph->nextf = repeater;
    ph->ud = rd;

    return 0;
}
#line 1256 "gest.org"
int gest_mark(gest_d *g)
{
    g->saved = g->phrase_selected;
    return 0;
}
#line 1279 "gest.org"
struct return_d {
    gest_phrase *ph;
    int pos;
    int ntimes;
};

static gest_phrase* goback(gest_d *g, gest_phrase *ph)
{
    struct return_d *rd;

    rd = (struct return_d *)ph->ud;

    rd->pos++;

    if (rd->pos >= rd->ntimes) {
        rd->pos = 0;
        return next_phrase(g, ph);
    }

    return rd->ph;
}

int gest_return(gest_d *g, int ntimes)
{
    gest_phrase *ph;
    struct return_d *rd;

    if (g->saved == NULL) return 1;

    gest_begin(g, 0, 0);
    gest_end(g);

    rd = gest_alloc(g, sizeof(struct return_d));
    ph = g->phrase_selected;

    rd->ph = g->saved;
    rd->pos = 0;
    rd->ntimes = ntimes;

    ph->ud = rd;
    ph->get = goback;

    return 0;
}
#line 1341 "gest.org"
static SKFLT skewquad_in(SKFLT t, SKFLT inc)
{
    SKFLT x0, x1;

    x0 = t*t;
    t += inc;
    x1 = t*t;

    return x1 - x0;
}

static SKFLT skewquad_out(SKFLT t, SKFLT inc)
{
    SKFLT x0, x1;

    x0 = 1 - (1 - t) * (1 - t);
    t += inc;
    x1 = 1 - (1 - t) * (1 - t);

    return x1 - x0;
}

int gest_skewquad(gest_d *g, int dir)
{
    if (g->phrase_selected == NULL) return 1;

    if (dir == 0) {
        g->phrase_selected->skew = skewquad_in;
    } else {
        g->phrase_selected->skew = skewquad_out;
    }
    return 0;
}
#line 1388 "gest.org"
static SKFLT skewexp_in(SKFLT t, SKFLT inc)
{
    SKFLT x0, x1;
    SKFLT s;

    s = 1.0 / (1 - exp(1));

    x0 = (1 - exp(t)) * s;
    t += inc;
    x1 = (1 - exp(t)) * s;

    return x1 - x0;
}

static SKFLT skewexp_out(SKFLT t, SKFLT inc)
{
    SKFLT x0, x1;
    SKFLT s;

    s = 1.0 / (1 - exp(-1));

    x0 = (1 - exp(-t)) * s;
    t += inc;
    x1 = (1 - exp(-t)) * s;

    return x1 - x0;
}

int gest_skewexp(gest_d *g, int dir)
{
    if (g->phrase_selected == NULL) return 1;

    if (dir == 0) {
        g->phrase_selected->skew = skewexp_in;
    } else {
        g->phrase_selected->skew = skewexp_out;
    }
    return 0;
}
#line 1443 "gest.org"
static SKFLT skewshuf(SKFLT t, SKFLT inc)
{
    int pos;

    if (t >= 1.0) return t;

    pos = floor(t * 16);
    pos %= 16;

    if ((pos % 2) == 0) {
       inc *= (2.0 / 3.0);
    } else {
       inc *= (4.0 / 3.0);
    }

    return inc;
}

int gest_skewshuf(gest_d *g)
{
    if (g->phrase_selected == NULL) return 1;

    g->phrase_selected->skew = skewshuf;
    return 0;
}
#line 1480 "gest.org"
static SKFLT mix_ramp(gest_d *g, SKFLT x, SKFLT y, SKFLT a)
{
    return a;
}

int gest_ramp(gest_d *g)
{
    gest_target *t;

    t = g->curtarget;

    if (t == NULL) return 1;

    gest_target_mix(t, mix_ramp);

    return 0;
}
#line 1509 "gest.org"
static SKFLT mix_invramp(gest_d *g, SKFLT x, SKFLT y, SKFLT a)
{
    return 1 - a;
}

int gest_invramp(gest_d *g)
{
    gest_target *t;

    t = g->curtarget;

    if (t == NULL) return 1;

    gest_target_mix(t, mix_invramp);

    return 0;
}
#line 1540 "gest.org"
static SKFLT linear(gest_d *g, SKFLT a, void *ud)
{
    return a;
}

void gest_behavior_linear(gest_d *g)
{
    set_behavior(g, linear, NULL);
}
#line 1561 "gest.org"
static SKFLT step(gest_d *g, SKFLT a, void *ud)
{
    return 0;
}

void gest_behavior_step(gest_d *g)
{
    set_behavior(g, step, NULL);
}
#line 1587 "gest.org"
static SKFLT gliss(gest_d *g, SKFLT a, void *ud)
{
    if (a < 0.5) {
        a = 0;
    } else {
        a -= 0.5;
        if (a < 0) a = 0;
        a *= 2;
        a = a * a * a;
    }

    return a;
}

void gest_behavior_gliss(gest_d *g)
{
    set_behavior(g, gliss, NULL);
}
#line 1618 "gest.org"
static SKFLT smallgliss(gest_d *g, SKFLT a, void *ud)
{
    if (a < 0.97) {
        a = 0;
    } else {
        a -= 0.97;
        if (a < 0) a = 0;
        a /= 0.03;
        a = a * a * a;
    }
    return a;
}

void gest_behavior_smallgliss(gest_d *g)
{
    set_behavior(g, smallgliss, NULL);
}
#line 1646 "gest.org"
static SKFLT mediumgliss(gest_d *g, SKFLT a, void *ud)
{
    if (a < 0.75) {
        a = 0;
    } else {
        a -= 0.75;
        if (a < 0) a = 0;
        a /= 0.25;
        a = a * a * a;
    }
    return a;
}

void gest_behavior_mediumgliss(gest_d *g)
{
    set_behavior(g, mediumgliss, NULL);
}
#line 1680 "gest.org"
static SKFLT exponential(gest_d *g, SKFLT a, void *ud)
{
    SKFLT *s;

    s = ud;

    return (1.0 - exp(a * (*s))) / (1 - exp(*s));
}

void gest_behavior_exponential(gest_d *g, SKFLT slope)
{
    SKFLT *s;
    s = gest_alloc(g, sizeof(SKFLT));
    *s = slope;
    set_behavior(g, exponential, s);
}
#line 1714 "gest.org"
struct bezier_data {
    SKFLT cx;
    SKFLT cy;
};

/* https://pbat.ch/sndkit/bezier/ */

static SKFLT quadratic_equation(SKFLT a, SKFLT b, SKFLT c)
{
    SKFLT det; /* determinant */

    det = b*b - 4*a*c;

    if (det >= 0) {
        return ((-b + sqrt(det))/(2.0*a));
    } else {
        return 0;
    }
}

static SKFLT find_t(SKFLT x0, SKFLT x1, SKFLT x2, SKFLT x)
{
    SKFLT a, b, c;

    a = (x0 - 2.0 * x1 + x2);
    b = 2.0 * (-x0 + x1);
    c = x0 - x;

    if (a) {
        return quadratic_equation(a, b, c);
    } else {
        return (x - x0) / b;
    }
}

static SKFLT bezier_curve(SKFLT xpos, SKFLT cx, SKFLT cy)
{
    SKFLT x[3];
    SKFLT y[3];
    SKFLT t;
    SKFLT val;

    x[0] = 0;
    x[1] = cx;
    x[2] = 1;

    y[0] = 0;
    y[1] = cy;
    y[2] = 1;

    t = find_t(x[0], x[1], x[2], xpos);

    val = (1.0-t)*(1.0-t)*y[0] + 2.0*(1 - t)*t*y[1] + t*t*y[2];
    return val;
}

static SKFLT bezier(gest_d *g, SKFLT a, void *ud)
{

    struct bezier_data *bd;
    bd = ud;
    return bezier_curve(a, bd->cx, bd->cy);
}

void gest_behavior_bezier(gest_d *g, SKFLT cx, SKFLT cy)
{
    struct bezier_data *bd;
    bd = gest_alloc(g, sizeof(struct bezier_data));
    bd->cx = cx;
    bd->cy = cy;
    set_behavior(g, bezier, bd);
}
#line 1797 "gest.org"
static SKFLT smoothstep(gest_d *g, SKFLT a, void *ud)
{
    SKFLT out;

    if (a <= 0) out = 0;
    else if (a >= 1) out = 1;
    else out = a * a * (3 - 2 * a);
    return out;
}

void gest_behavior_smoothstep(gest_d *g)
{
    set_behavior(g, smoothstep, NULL);
}
#line 1829 "gest.org"
static SKFLT biramp(gest_d *g, SKFLT a, void *ud)
{
    SKFLT *pos;
    SKFLT out;

    pos = ud;

    if (*pos == 0 || *pos == 1) return *pos;

    if (a < *pos) out = a / *pos;
    else out = (1 - a) / (1 - *pos);

    return out;
}

void gest_behavior_biramp(gest_d *g, SKFLT pos)
{
    SKFLT *ppos;
    ppos = gest_alloc(g, sizeof(SKFLT));
    *ppos = pos;
    set_behavior(g, biramp, ppos);
}
#line 1864 "gest.org"
static SKFLT gatefun(gest_d *g, SKFLT a, void *ud)
{
    SKFLT *pos;
    SKFLT out;

    pos = ud;

    if (a <= *pos) out = 1.0;
    else out = 0.0;

    return out;
}

void gest_behavior_gate(gest_d *g, SKFLT pos)
{
    SKFLT *ppos;
    ppos = gest_alloc(g, sizeof(SKFLT));
    *ppos = pos;
    set_behavior(g, gatefun, ppos);
}
#line 1898 "gest.org"
void gest_mass(gest_d *g, SKFLT mass)
{
    action_mass(g, g->curtarget, mass);
}
#line 1913 "gest.org"
void gest_inertia(gest_d *g, SKFLT inertia)
{
    action_inertia(g, g->curtarget, inertia);
}
#line 1930 "gest.org"
static void grow_mass(gest_d *g, void *ud, int pos)
{
    SKFLT *amt;
    amt = (SKFLT *) ud;
    g->mass += g->mass * (*amt);
}

int gest_grow(gest_d *g, SKFLT amt)
{
    SKFLT *pamt;
    gest_target *t;

    if (g->curtarget == NULL) return 1;

    t = g->curtarget;

    pamt= gest_alloc(g, sizeof(SKFLT));
    *pamt = amt;

    append_action(g, t, grow_mass, pamt);
    return 0;
}

static void shrink_mass(gest_d *g, void *ud, int pos)
{
    SKFLT *amt;
    amt = (SKFLT *) ud;
    g->mass -= g->mass * (*amt);
}

int gest_shrink(gest_d *g, SKFLT amt)
{
    SKFLT *pamt;
    gest_target *t;

    if (g->curtarget == NULL) return 1;

    t = g->curtarget;

    pamt= gest_alloc(g, sizeof(SKFLT));
    *pamt = amt;

    append_action(g, t, shrink_mass, pamt);
    return 0;
}
#line 2019 "gest.org"
static void phrase_init(gest_d *g,
                        gest_phrase *phrase,
                        int beats,
                        int div)
{
    if (beats > 0) phrase->mod = 1.0 / beats;
    else phrase->mod = 0;
    phrase->beats = beats;
    phrase->next = NULL;
    phrase->top = mkpolyramp(g, NULL, div);
    phrase->meta = NULL;
    phrase->get = NULL;
    phrase->nextf = NULL;
#line 2049 "gest.org"
phrase->skew = NULL;
#line 2033 "gest.org"
}
#line 2082 "gest.org"
static void gest_node_init(gest_node *n)
{
#line 2098 "gest.org"
n->type = NODE_NOTHING;
#line 2120 "gest.org"
n->modifier = NODE_NOTHING;
#line 2134 "gest.org"
n->nchildren = 0;
#line 2148 "gest.org"
n->children = NULL;
#line 2162 "gest.org"
n->target = NULL;
#line 2176 "gest.org"
n->next = NULL;
#line 2190 "gest.org"
n->parent = NULL;
#line 2202 "gest.org"
n->id = -1;
#line 2214 "gest.org"
n->meta = NULL;
#line 2226 "gest.org"
n->get = NULL;
#line 2085 "gest.org"
}
#line 2279 "gest.org"
static gest_node * mkpolyramp(gest_d *g,
                              gest_node *parent,
                              int div)
{
    gest_node *n, *last;
    int total;

    /* check to see if parent node is full */

    total = 0;
    last = NULL;

    if (parent != NULL) {
        total = node_count(parent, &last);
        if (total >= parent->modifier) {
            return NULL;
        }
    }

    n = gest_alloc(g, sizeof(gest_node));
    gest_node_init(n);
    n->type = NODE_POLYRAMP;
    n->modifier = div;
    n->parent = parent;
    n->id = g->nodepos;
    g->nodepos++;

    if (parent != NULL) {
        append_node(parent, n, last);
    }

    if (parent == NULL) {
        n->parent = n;
    }

    return n;
}
#line 2338 "gest.org"
static gest_node * mkmonoramp(gest_d *g,
                              gest_node *parent,
                              int ninputs)
{
    gest_node *n, *last;
    int total;

    last = NULL;

    if (parent != NULL) {
        total = node_count(parent, &last);
        total += ninputs;
        if (total > parent->modifier) return NULL;
    }

    n = gest_alloc(g, sizeof(gest_node));
    gest_node_init(n);

    n->type = NODE_MONORAMP;
    n->modifier = ninputs;
    n->parent = parent;
    n->id = g->nodepos;
    g->nodepos++;

    if (parent != NULL) {
        append_node(parent, n, last);
    }

    return n;
}
#line 2380 "gest.org"
static int node_count(gest_node *node, gest_node **last)
{
    int total;
    int i;
    gest_node *child;

    total = 0;

    if (node == NULL) {
        return -1;
    }

    child = node->children;

    for (i = 0; i < node->nchildren; i++) {
        if (child->type == NODE_MONORAMP) {
            /* monoramps eat up M ramps */
            total += child->modifier;
        } else if (child->type == NODE_POLYRAMP) {
            /* polyramps always occupy one ramp */
            total++;
        }

        if (last != NULL && i == node->nchildren - 1) {
            *last = child;
        }

        child = child->next;
    }

    return total;
}
#line 2425 "gest.org"
static void append_node(gest_node *parent,
                        gest_node *node,
                        gest_node *last)
{
    if (last == NULL) {
        parent->children = node;
    } else {
        last->next = node;
    }

    parent->nchildren++;
}
#line 2455 "gest.org"
static gest_node * dive_to_target(gest_d *g,
                                  gest_node *node)
{
    if (node->meta != NULL) node = get_node(g, node);

    if (node->target != NULL) {
        apply_modifier(g, node);
        return node;
    }

    while (node->target == NULL) {
        apply_modifier(g, node);

        /* go to left-most child */
        node = acquire_children(g, node);
        if (node == NULL) break;
    }

    if (node->type == NODE_MONORAMP && node->modifier > 1) {
        apply_modifier(g, node);
    }

    return node;
}
#line 2502 "gest.org"
static void revert_modifier(gest_d *g, gest_node *node)
{
    if (node->meta != NULL) return;

    if (node->type == NODE_POLYRAMP) {
        g->num /= node->modifier;
    } else if (node->type == NODE_MONORAMP) {
        g->den /= node->modifier;
    }
}
#line 2516 "gest.org"
static void apply_modifier(gest_d *g, gest_node *node)
{
    if (node->type == NODE_POLYRAMP) {
        g->num *= node->modifier;
    } else if (node->type == NODE_MONORAMP) {
        g->den *= node->modifier;
    }
}
#line 2540 "gest.org"
static gest_node* get_node(gest_d *g, gest_node *n)
{
    while (n->get != NULL) {
        n = n->get(g, n);
    }

    return n;
}

static gest_node* acquire_children(gest_d *g, gest_node *n)
{
    gest_node *children;

    children = n->children;
    children = get_node(g, children);

    return children;
}
#line 2571 "gest.org"
static gest_target *node_target(gest_d *g, gest_node *node)
{
    gest_target *t;

    if (node == NULL) return NULL;

    t = node->target;

    if (node->target != NULL && node->target->get != NULL) {
        while (1) {
            t = t->get(g, t);
            if (t->meta == NULL) break;
        }
    }

    return t;
}
#line 2614 "gest.org"
static SKFLT ramptree_step(gest_d *g,
                           SKFLT inc,
                           int reset,
                           int blkpos)
{
    SKFLT out;
    SKFLT phs;

    out = 0;

    phs = g->phs;

    if (g->phrase_selected == NULL) return phs;

#line 3139 "gest.org"
if (g->please_wait && g->spillover) {

    /* Copied and pasted from beat-checkin code */
    /* may want to wrap this in a function later */
    g->beat = 0;
    g->t = 0;
    g->phs = 0;
    g->please_wait = 0;
    g->correction = 1.0;
    g->targetondeck = 1;
    g->phrase_selected = g->nxtstate.phrase;

    g->spillover = 0;
    /* reset if next state was told to wait */
    /* TODO: is this needed here? */
    g->nxtstate.please_wait = 0;

    if (g->squawk) {
        fprintf(stderr, "spillover turned off.\n");
    }

    return phs;
}
#line 2629 "gest.org"

#line 2684 "gest.org"
if (reset && g->spillover == 0) {
    int limit;
    SKFLT goal;

    limit = g->phrase_selected->beats;
    g->beat++;
    goal = (SKFLT) g->beat / limit;

    if (g->t > 0) g->correction = goal / g->t;

    if (g->beat >= limit) {
        SKFLT amt;

        amt = 1 - g->t;

        if (amt > 0 && amt < g->tolerance) {
#line 3121 "gest.org"
if (g->squawk) {
    fprintf(stderr, "spillover turned on\n");
}
g->spillover = 1;

#line 3213 "gest.org"
if (g->please_wait) {
    if (g->squawk) {
        fprintf(stderr, "spillover: please_wait already on\n");
    }
    g->phs = 1.0;
    return 1.0;
}
#line 3121 "gest.org"
#line 2701 "gest.org"
        } else {
            if (g->squawk && amt > 0) {
                fprintf(stderr, "Ramp undershot by %g\n", amt);
                fprintf(stderr, "phase is %g\n", phs);
            }

#line 3193 "gest.org"
if (g->please_wait) {
    if (g->squawk > 1) {
        fprintf(stderr, "please_wait set in undershoot\n");
    }
    phs = 1.0;
}
#line 2708 "gest.org"

            g->beat = 0;
            g->t = 0;
            g->phs = 0;
            g->please_wait = 0;
            g->correction = 1.0;
            g->targetondeck = 1;
            g->phrase_selected = g->nxtstate.phrase;


            /* reset if next state was told to wait */
            g->nxtstate.please_wait = 0;

            return phs;
        }
    }
} else if (g->please_wait) {
    return 1.0;
}
#line 2631 "gest.org"

    inc *= g->phrase_selected->mod * g->correction;

#line 2738 "gest.org"
out = g->phs;
#line 2631 "gest.org"
#line 2762 "gest.org"
if (g->targetondeck) {
    gest_state *s;

    s = &g->nxtstate;

    g->targetondeck = 0;

    if (s->please_wait) {
        g->please_wait = 1;
        return 1.0;
    }

    g->num = s->num;
    g->den = s->den;
    s->target->curbehavior = s->behavior;
    set_curtarget(g, s->target, blkpos);

    set_curnode(g, s->node);
    g->phrase_selected = s->phrase;

    init_state(&g->nxtstate);

    if (g->curnode != NULL) {
        find_next_node(g, g->curnode, &g->nxtstate);
    }

    g->nxttarget = g->nxtstate.target;

    if (g->nxttarget != NULL) {
        g->nxtval = g->nxttarget->value;
    } else {
        g->nxtval = g->curval;
    }
}
#line 2631 "gest.org"
#line 2811 "gest.org"
{
SKFLT i;
SKFLT myinc;

if (g->phrase_selected->skew == NULL) {
    myinc = inc;
} else {
    myinc = g->phrase_selected->skew(g->t, inc);
}

i = myinc * ((SKFLT)g->num / g->den);
phs += i;
}
#line 2631 "gest.org"
#line 2831 "gest.org"
if (phs > 1.0) {
#line 2844 "gest.org"
g->targetondeck = 1;
#line 2831 "gest.org"
#line 3094 "gest.org"
while (phs > 1) phs--;
phs = 0; /* just kidding, truncate */
#line 2834 "gest.org"
}
#line 2638 "gest.org"

    g->phs = phs;
    g->t += inc;

    return out;
}
#line 2987 "gest.org"
static void find_next_node(gest_d *g,
                           gest_node *top,
                           gest_state *state)
{
    gest_node *next;
    gest_node *last_reverted;
    gest_phrase *phrase;
    int num, den;
    int please_wait;

    next = NULL;
    last_reverted = NULL;

    num = g->num;
    den = g->den;
    phrase = g->phrase_selected;
    please_wait = 0;

    while (next == NULL) {
        if (top == g->phrase_selected->top) {
            /* are we at the end */
            /* if so, go to next phrase */
            if (top->next == NULL) {
#line 2882 "gest.org"
/* next node is in next phrase */
phrase = next_phrase(g, phrase);
phrase = get_phrase(g, phrase);

if (phrase != NULL) next = phrase->top;
else next = NULL;

g->num = 1;
g->den = 1;

if (next != NULL) {
    next = dive_to_target(g, next);
}

please_wait = 1;
#line 3011 "gest.org"
                break;
            } else {
                /* go to next child in top polyramp node */
#line 2908 "gest.org"
next = top->next;

if (next->target == NULL) {
    next = dive_to_target(g, next);
}
#line 3015 "gest.org"
            }
        } else if (top->next == NULL) {
#line 2937 "gest.org"
/* if top is a monoramp >1, revert it */
if (top != last_reverted) {
    if (top->type == NODE_MONORAMP && top->modifier > 1) {
        /* revert the monoramp */
        revert_modifier(g, top);
    }
}

revert_modifier(g, top->parent);
last_reverted = top->parent;
top = top->parent;
#line 3018 "gest.org"
        } else {
#line 2958 "gest.org"
next = top->next;

/* if top is a monoramp >1, revert it */

if (top != last_reverted) {
    if (top->type == NODE_MONORAMP && top->modifier > 1) {
        /* revert the monoramp */
        revert_modifier(g, top);
    }
}

/* dive_to_target applies modifiers */
/* continguous nodes on the same level don't have these */
if (next->target == NULL) {
    next = dive_to_target(g, next);
} else if (next->type == NODE_MONORAMP && next->modifier > 1) {
    /* next node is on same level but is monoramp */
    apply_modifier(g, next);
}
#line 3020 "gest.org"
        }
    }

    state->num = g->num;
    state->den = g->den;
    state->node = next;
    state->target = node_target(g, next);
    state->behavior = target_behavior(g, state->target);
    state->phrase = phrase;
    state->please_wait = please_wait;

    /* restore old numerator and denominator */

    g->num = num;
    g->den = den;
}
#line 3050 "gest.org"
static gest_phrase* get_phrase(gest_d *g, gest_phrase *ph)
{
    if (ph == NULL) return NULL;
    while (ph->get != NULL) ph = ph->get(g, ph);
    return ph;
}
#line 3071 "gest.org"
static gest_phrase* next_phrase(gest_d *g, gest_phrase *ph)
{
    gest_phrase *next;
    next = ph->next;

    if (ph->nextf != NULL) next = ph->nextf(g, ph);

    return next;
}
#line 3266 "gest.org"
static void collection_init(gest_collection *col)
{
    col->tail = NULL;
    col->nitems = 0;
}
#line 3292 "gest.org"
static void * collection_alloc_dtor(gest_collection *col,
                                    size_t sz,
                                    void (*free)(void*))
{
    struct gest_entry *ent;

    ent = malloc(sizeof(struct gest_entry));

    ent->ptr = calloc(1, sz);
    ent->free = free;

    ent->prev = col->tail;
    col->tail = ent;
    col->nitems++;

    return ent->ptr;
}
#line 3324 "gest.org"
static void * collection_alloc(gest_collection *col, size_t sz)
{
    return collection_alloc_dtor(col, sz, NULL);
}
#line 3340 "gest.org"
static void * gest_alloc(gest_d *gest, size_t sz)
{
    return collection_alloc(&gest->col, sz);
}
#line 3365 "gest.org"
static void collection_cleanup(gest_collection *col)
{
    int n;
    struct gest_entry *ent, *prev;

    ent = col->tail;

    for (n = 0; n < col->nitems; n++) {
        prev = ent->prev;
        if (ent->free != NULL) ent->free(ent->ptr);
        free(ent->ptr);
        free(ent);
        ent = prev;
    }
}
#line 3433 "gest.org"
void gest_target_init(gest_target *t)
{
    t->value = 0;
    t->ud = NULL;
    t->next = NULL;
    t->get = NULL;
    t->meta = NULL;
    gest_behavior_init(&t->behavior);
    t->curbehavior = &t->behavior;
#line 3730 "gest.org"
t->mix = default_mix;
#line 4865 "gest.org"
gest_actionlist_init(&t->lst);
#line 3443 "gest.org"
}
#line 3461 "gest.org"
static gest_target * mktarget(gest_d *g)
{
    gest_target *t;
    gest_node *last;
    gest_node *curnode;

    t = NULL;
    last = NULL;
    curnode = g->curnode;

#line 3495 "gest.org"
t = gest_alloc(g, sizeof(gest_target));
gest_target_init(t);
#line 3472 "gest.org"

    if (g->mtpos > 0) {
#line 3631 "gest.org"
gest_metatarget *mt;

/* get from top of stack */
mt = g->mtstack[g->mtpos - 1];

/* append to targets array */
mt->targets[mt->pos++] = t;

/* pop off stack if reached the end */
if (mt->pos >= mt->size) {
    g->mtpos--;
    g->mtstack[g->mtpos] = NULL;
    mt->pos = 0;
}
#line 3475 "gest.org"
        return t;
    }

    if (curnode == NULL) {
        return NULL;
    }

#line 3505 "gest.org"
{
    int size;

    size = node_count(curnode, &last);

    if (curnode != NULL && size >= curnode->modifier) {
        return NULL;
    }
}
#line 3475 "gest.org"
#line 3535 "gest.org"
{
    int rc;

    rc = last != NULL &&
        last->type == NODE_MONORAMP &&
        last->meta == NULL &&
        last->target == NULL &&
        last->nchildren == 0;

    if (rc) {
        last->target = t;
    } else {
        rc = curnode->type == NODE_MONORAMP &&
            curnode->target == NULL;

        if (rc) {
            curnode->target = t;
        } else {
            gest_node *mr;
            mr = mkmonoramp(g, curnode, 1);
            mr->target = t;
        }
    }
}
#line 3475 "gest.org"
#line 3572 "gest.org"
{
    gest_node *next;
    gest_node *curr;
    int metahunt;
    gest_node *mnpar;

    next = NULL;
    curr = curnode;
    metahunt = 0;
    mnpar = NULL;

#line 4427 "gest.org"
if (g->mnpos > 0) {
    gest_metanode *mn;
    metahunt = 1;

    mn = g->mnstack[g->mnpos - 1];

    if (mn != NULL) mnpar = mn->parent;
}
#line 3584 "gest.org"

    while (next == NULL) {
        int size;
        gest_node *last;
        int limit;

#line 4444 "gest.org"
if (metahunt && curr == mnpar) {
#line 4456 "gest.org"
gest_metanode *mn;
next = curr;
/* store children in metanode */

mn = g->mnstack[g->mnpos - 1];

next = mn->nodes[mn->pos++];

if (mn->pos >= mn->size) {
    g->mnpos--;
    g->mnstack[g->mnpos] = NULL;
    mn->pos = 0;
}

set_curnode(g, next);
break;
#line 4446 "gest.org"
}
#line 3591 "gest.org"

        /* is there any room in the current node? */

        size = node_count(curr, &last);

        if (curr->type == NODE_MONORAMP) {
            limit = curr->nchildren;
        } else {
            limit = curr->modifier;
        }

        /* no room ... */
        if (size >= limit) {
            /* onto the next... */

            /* we've reached the top */
            if (curr == curr->parent) break;

            /* try one level up */

            curr = curr->parent;

        } else {
            /* this node has room! */
            next = curr;
        }
    }

    set_curnode(g, next);
}
#line 3485 "gest.org"

    return t;
}
#line 3668 "gest.org"
static void update_mass(gest_d *g, void *ud, int pos)
{
    SKFLT *mass;
    mass = (SKFLT *) ud;
    g->mass = *mass;
}

static void update_inertia(gest_d *g, void *ud, int pos)
{
    SKFLT *inertia;
    inertia = (SKFLT *) ud;
    g->inertia = *inertia;
}
#line 3694 "gest.org"
static void action_mass(gest_d *g, gest_target *t, SKFLT mass)
{
    SKFLT *pmass;

    pmass = gest_alloc(g, sizeof(SKFLT));
    *pmass = mass;

    append_action(g, t, update_mass, pmass);
}

static void action_inertia(gest_d *g, gest_target *t, SKFLT inertia)
{
    SKFLT *pinertia;

    pinertia = gest_alloc(g, sizeof(SKFLT));
    *pinertia = inertia;

    append_action(g, t, update_inertia, pinertia);
}
#line 3740 "gest.org"
static SKFLT default_mix(gest_d *g, SKFLT x, SKFLT y, SKFLT a)
{
    return (1 - a)*x + a*y;
}
#line 3757 "gest.org"
void gest_target_mix(gest_target *t,
                     SKFLT (*mix)(gest_d *, SKFLT, SKFLT, SKFLT))
{
    t->mix = mix;
}
#line 3772 "gest.org"
void gest_target_data_set(gest_target *t, void *ud)
{
    t->ud = ud;
}

void* gest_target_data_get(gest_target *t)
{
    return t->ud;
}
#line 3815 "gest.org"
void gest_behavior_init(gest_behavior *b)
{
    b->tick = NULL;
    b->ud = NULL;
    b->get = NULL;
    b->meta = NULL;
}
#line 3833 "gest.org"
void gest_behavior_set(gest_behavior *b, gest_bfunc tick, void *ud)
{
    b->tick = tick;
    b->ud = ud;
}
#line 3849 "gest.org"
SKFLT gest_behavior_tick(gest_behavior *b, gest_d *g, SKFLT a)
{
    return b->tick(g, a, b->ud);
}
#line 3862 "gest.org"
static gest_behavior * target_behavior(gest_d *g, gest_target *t)
{
    gest_behavior *b;

    if (t == NULL) return NULL;

    b = &t->behavior;

    if (b != NULL && b->get != NULL) {
        while (1) {
            b = b->get(g, b);
            if (b->meta == NULL || b->get == NULL) break;
        }
    }

    return b;
}
#line 3901 "gest.org"
SKFLT gest_tick(gest_d *g, SKFLT conductor, int blkpos)
{
    SKFLT out;
    SKFLT inc;
    int reset;

    out = 0;

#line 3932 "gest.org"
if (conductor < 0) return g->prevout;
#line 3901 "gest.org"
#line 3950 "gest.org"
inc = conductor - g->last;
reset = 0;

if (conductor < g->last) {
    inc = g->last_inc;
    reset = 1;
}

g->last_inc = inc;

/* discount reset found at initial sample */
if (g->phs == -1) {
    g->phs = 0;
    reset = 0;
}

g->last = conductor;
#line 3901 "gest.org"
#line 3975 "gest.org"
out = ramptree_step(g, inc, reset, blkpos);
#line 3901 "gest.org"
#line 3984 "gest.org"
if (g->curtarget != NULL) {
    out = gest_behavior_tick(g->curtarget->curbehavior, g, out);
    if (g->curtarget->mix != NULL) {
        out = g->curtarget->mix(g, g->curval, g->nxtval, out);
    } else {
        out = g->mix(g, g->curval, g->nxtval, out);
    }
}
#line 3901 "gest.org"
#line 3999 "gest.org"
if (g->squawk > 1) {
    SKFLT diff;

    diff = out - g->prevout;
    if (fabs(diff) > 0.1) {
        fprintf(stderr, "large jump: %g\n", diff);

        if (reset) {
            fprintf(stderr, "this happened at a reset.\n");
        }
    }
}

g->prevout = out;
#line 3914 "gest.org"

    g->prevout = out;
    return out;
}
#line 4065 "gest.org"
static gest_metatarget * metatarget_alloc(gest_d *g, int sz)
{
    gest_metatarget *mt;
    int t;

    mt = gest_alloc(g, sizeof(gest_metatarget));
    mt->size = sz;
    mt->pos = 0;
    mt->targets = gest_alloc(g, sizeof(gest_target) * sz);

    for (t = 0; t < sz; t++) {
        mt->targets[t] = NULL;
    }

    return mt;
}
#line 4133 "gest.org"
static gest_target *get_seq(gest_d *g, gest_target *t)
{
    gest_metatarget *mt;

    mt = t->meta;

    if (mt->pos >= mt->size) mt->pos = 0;

    return mt->targets[mt->pos++];
}
#line 4155 "gest.org"
static gest_target *target_random(gest_d *g, gest_target *t)
{
    gest_metatarget *mt;

    mt = t->meta;

    return mt->targets[gest_randi(g, mt->size)];
}
#line 4206 "gest.org"
static gest_metabehavior * metabehavior_alloc(gest_d *g, int sz)
{
    gest_metabehavior *mb;
    int b;

    mb = gest_alloc(g, sizeof(gest_metatarget));
    mb->size = sz;
    mb->pos = 0;
    mb->behaviors = gest_alloc(g, sizeof(gest_behavior) * sz);

    for (b = 0; b < sz; b++) {
        gest_behavior_init(&mb->behaviors[b]);
        gest_behavior_set(&mb->behaviors[b], linear, NULL);
    }

    return mb;
}
#line 4261 "gest.org"
static void set_behavior(gest_d *g, gest_bfunc tick, void *ud)
{
    gest_behavior *b;

    b = &g->curtarget->behavior;

    if (g->mbpos > 0) {
        gest_metabehavior *mb;

        mb = g->mbstack[g->mbpos - 1];

        b = &mb->behaviors[mb->pos++];

        if (mb->pos >= mb->size) {
            mb->pos = 0;
            g->mbstack[g->mbpos - 1] = NULL;
            g->mbpos--; /* don't be clever */
        }
    }

    gest_behavior_set(b, tick, ud);
}
#line 4292 "gest.org"
static gest_behavior* behave_seq(gest_d *g, gest_behavior *b)
{
    gest_metabehavior *mb;

    mb = b->meta;

    if (mb->pos >= mb->size) mb->pos = 0;

    return &mb->behaviors[mb->pos++];
}
#line 4311 "gest.org"
static gest_behavior* behave_random(gest_d *g, gest_behavior *b)
{
    gest_metabehavior *mb;

    mb = b->meta;

    return &mb->behaviors[gest_randi(g, mb->size)];
}
#line 4356 "gest.org"
static gest_metanode* metanode_alloc(gest_d *g,
                                     gest_node *parent,
                                     int sz)
{
    gest_metanode *mn;
    int i;

    mn = gest_alloc(g, sizeof(gest_metanode));
    mn->size = sz;
    mn->pos = 0;
    mn->parent = NULL;
    mn->nodes = gest_alloc(g, sizeof(gest_node *) * sz);

    for (i = 0; i < sz; i++) {
        gest_node *n;

        n = gest_alloc(g, sizeof(gest_node));
        gest_node_init(n);
        n->type = parent->type;
        n->modifier = parent->modifier;
        n->parent = parent;
        n->id = g->nodepos;
        g->nodepos++;
        mn->nodes[i] = n;
    }

    return mn;
}
#line 4493 "gest.org"
static gest_node* node_seq(gest_d *g, gest_node *n)
{
    gest_metanode *mn;

    mn = n->meta;

    if (mn->pos >= mn->size) mn->pos = 0;

    return mn->nodes[mn->pos++];
}
#line 4512 "gest.org"
static gest_node* node_random(gest_d *g, gest_node *n)
{
    gest_metanode *mn;

    mn = n->meta;

    return mn->nodes[gest_randi(g, mn->size)];
}
#line 4556 "gest.org"
static gest_metaphrase * metaphrase_alloc(gest_d *g, int sz)
{
    gest_metaphrase *mp;
    int i;

    mp = gest_alloc(g, sizeof(gest_metaphrase));
    mp->size = sz;
    mp->pos = 0;
    mp->phrases = gest_alloc(g, sizeof(gest_phrase *) * sz);

    for (i = 0; i < sz; i++) {
        mp->phrases[i] = NULL;
    }

    return mp;
}
#line 4696 "gest.org"
static gest_phrase* phrase_seq(gest_d *g, gest_phrase *ph)
{
    gest_metaphrase *mp;

    mp = ph->meta;

    if (mp->pos >= mp->size) mp->pos = 0;

    return mp->phrases[mp->pos++];
}
#line 4715 "gest.org"
static gest_phrase* phrase_random(gest_d *g, gest_phrase *ph)
{
    gest_metaphrase *mp;
    mp = ph->meta;
    return mp->phrases[gest_randi(g, mp->size)];
}
#line 4745 "gest.org"
void gest_seed(gest_d *g, unsigned long rng)
{
    g->rng = rng;
}
#line 4769 "gest.org"
unsigned long gest_rand(gest_d *g)
{
    g->rng = (1103515245 * g->rng + 12345) % 2147483648;
    return g->rng;
}
#line 4786 "gest.org"
SKFLT gest_randf(gest_d *g)
{
    unsigned long x;

    x = gest_rand(g);

    return (SKFLT)x / 2147483648.0;
}
#line 4806 "gest.org"
int gest_randi(gest_d *g, int N)
{
    int x;

    x = floor(gest_randf(g) * N);

    if (x >= N) x = N - 1;

    return x;
}
#line 4877 "gest.org"
void gest_actionlist_init(gest_actionlist *lst)
{
    lst->head = NULL;
    lst->tail = NULL;
    lst->sz = 0;
}
#line 4899 "gest.org"
static void append_action(gest_d *g,
                          gest_target *t,
                          void (*fun)(gest_d*, void*, int),
                          void *data)
{
    gest_action *a;
    gest_actionlist *lst;

    lst = &t->lst;
    a = gest_alloc(g, sizeof(gest_action));

    a->data = data;
    a->fun = fun;
    a->next = NULL;

    if (lst->head == NULL) {
        lst->head = a;
    }

    if (lst->tail != NULL) lst->tail->next = a;
    lst->tail = a;
    lst->sz++;
}
#line 4942 "gest.org"
static void do_actions(gest_d *g, gest_target *t, int pos)
{
    gest_actionlist *lst;
    gest_action *a;
    int i;

    lst = &t->lst;

    a = lst->head;

    for (i = 0; i < lst->sz; i++) {
        a->fun(g, a->data, pos);
        a = a->next;
    }
}
#line 5002 "gest.org"
void gest_scalar_init(gest_scalar *s)
{
    s->cur = 0;
    s->nxt = 0;
    s->pos = -1;
}
#line 5036 "gest.org"
struct action_scalar_d {
   gest_scalar *s;
   SKFLT val;
};

static void scalar_cb(gest_d *g, void *ud, int pos)
{
    struct action_scalar_d *ad;
    ad = (struct action_scalar_d *)ud;

    ad->s->nxt = ad->val;
    ad->s->pos = pos;
}

static void action_scalar(gest_d *g,
                          gest_target *t,
                          gest_scalar *s,
                          SKFLT val)
{
    struct action_scalar_d *ad;

    ad = gest_alloc(g, sizeof(struct action_scalar_d));

    ad->s = s;
    ad->val = val;

    append_action(g, t, scalar_cb, ad);
}
#line 77 "gest.org"
