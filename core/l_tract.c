#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"

#include "gest/gest.h"
#include "gest/gestlang.h"

#include "sndkit/dsp/tract.h"
#define SK_SMOOTHER_PRIV
#include "sndkit/dsp/smoother.h"

int gf_node_tract(gf_node *node, sk_tract *tr);
sk_tract * gf_node_tractnew(gf_patch *patch);

static lil_value_t tract_new(lil_t lil, size_t argc, lil_value_t *argv)
{
    gf_patch *patch;
    sk_core *core;
    int rc;
    sk_tract *tr;

    core = (sk_core *)lil_get_data(lil);

    patch = sk_core_patch(core);

    tr = gf_node_tractnew(patch);

    rc = sk_core_generic_push(core, tr);

    SKLIL_ERROR_CHECK(lil, rc, "could not push tract data");

    return NULL;
}

struct gest_tract {
    gf_patch *patch;
    sk_tract *tr;
    sk_smoother smooth[44];
    int init;
    sk_table *last;
};

static void free_gest_tract(gf_pointer *p)
{
    struct gest_tract *gt;
    void *ud;
    gf_patch *patch;

    gt = gf_pointer_data(p);

    ud = gt;
    patch = gt->patch;

    gf_memory_free(patch, &ud);
}

static lil_value_t tract_gest(lil_t lil, size_t argc, lil_value_t *argv)
{
    int rc;
    gf_patch *patch;
    sk_core *core;
    sk_tract *tr;
    void *ud;
    struct gest_tract *gt;
    gest_d *gest;
    int i;
    int sr;

    SKLIL_ARITY_CHECK(lil, "tract_gest", argc, 2);

    core = (sk_core *)lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Could get tract data.");
    tr = ud;

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Could get gest data.");
    gest = ud;

    patch = sk_core_patch(core);

    rc = gf_memory_alloc(patch, sizeof(struct gest_tract), &ud);

    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "Could not allocate memory.");

    gt = ud;

    gt->patch = patch;
    gt->tr = tr;
    gf_patch_append_userdata(patch, free_gest_tract, gt);
    sr = gf_patch_srate_get(patch);

    sk_tract_use_diameters(tr, 0);
    for (i = 0; i < 44; i++) {
        sk_smoother_init(&gt->smooth[i], sr);
        sk_smoother_time(&gt->smooth[i], 0.001);
    }

    gt->init = 1;
    gt->last = NULL;

    gest_data_set(gest, gt);

    rc = sk_core_generic_push(core, gest);
    SKLIL_ERROR_CHECK(lil, rc, "Could not push gest data");
    return NULL;
}

static SKFLT shape_mix(gest_d *g, SKFLT x, SKFLT y, SKFLT a)
{
    struct gest_tract *gt;
    gest_target *cur;
    gest_target *next;
    sk_tract *tr;
    sk_table *shpa;
    SKFLT *at;
    sk_table *shpb;
    SKFLT *bt;

    SKFLT *areas;
    int i;
    SKFLT oma; /* one minus a(lpha) */

    cur = gest_curtarget(g);
    next = gest_nxttarget(g);

    shpa = gest_target_data_get(cur);

    gt = gest_data_get(g);

    if (shpa == NULL) {
        if (gt->last == NULL) return (1 - a)*x + a*y;
        shpa = gt->last;
    }

    if (next != NULL) {
        shpb = gest_target_data_get(next);
        if (shpb == NULL) shpb = shpa;
    } else {
        shpb = shpa;
    }

    tr = gt->tr;
    areas = sk_tract_areas(tr);

    oma = 1.0 - a;

    at = sk_table_data(shpa);
    bt = sk_table_data(shpb);

    for (i = 0; i < 44; i++) {
        SKFLT out;
        out = oma*at[i] + a*bt[i];

        if (gt->init) {
            gt->init = 0;
            sk_smoother_reset(&gt->smooth[i], out);
        }

        areas[i] = sk_smoother_tick(&gt->smooth[i], out);

    }

    gt->init = 0;

    gt->last = shpb;

    return (1 - a)*x + a*y;
}

static lil_value_t tract_target(lil_t lil, size_t argc, lil_value_t *argv)
{
    gest_d *gest;
    gest_target *t;
    const char *key;
    sk_table *shape;
    int rc;
    sk_core *core;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "tract_target", argc, 1);

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Could not pop gest data");
    gest = ud;

    key = lil_to_string(argv[0]);

    rc = sk_core_lookup(core, key, strlen(key), &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Could not find table");
    shape = ud;

    t = gest_curtarget(gest);

    if (sk_table_size(shape) < 44) {
        lil_set_error(lil, "ftable size must be at least 44");
        return NULL;
    }

    gest_target_mix(t, shape_mix);
    gest_target_data_set(t, (void *)shape);

    rc = sk_core_generic_push(core, gest);
    SKLIL_ERROR_CHECK(lil, rc, "Could not push gest data");
    return NULL;
}

static lil_value_t tract_node(lil_t lil, size_t argc, lil_value_t *argv)
{
    int rc;
    gf_patch *patch;
    sk_core *core;
    sk_tract *tr;
    void *ud;
    gf_node *node;
    sk_param in;

    SKLIL_ARITY_CHECK(lil, "tract_node", argc, 1);
    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get tract data");

    rc = sk_param_get(core, &in);
    SKLIL_ERROR_CHECK(lil, rc, "could not get input signal");

    tr = ud;

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "Could not make node.");

    rc = gf_node_tract(node, tr);
    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "Problems making tract.");

    sk_param_set(core, node, &in, 0);
    sk_param_out(core, node, 1);
    return NULL;
}

static int trfun(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_target *t;
    sk_table *shape;
    shape = ud;
    t = gest_curtarget(g);

    gest_target_mix(t, shape_mix);
    gest_target_data_set(t, (void *)shape);
    return 0;
}

static lil_value_t tract_glang(lil_t lil, size_t argc, lil_value_t *argv)
{
    int rc;
    sk_core *core;
    const char *name;
    sk_table *shape;
    gestlang_d *gl;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "tract_glang", argc, 3);

    core = lil_get_data(lil);

    /* TODO use table_pop */
    /* rc = sk_core_generic_pop(core, &ud); */
    rc = sk_core_table_pop(core, &shape);
    SKLIL_ERROR_CHECK(lil, rc, "could not get shape");
    /* shape = ud; */

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gestlang");
    gl = ud;

    if (sk_table_size(shape) < 44) {
        lil_set_error(lil, "ftable size must be at least 44");
        return NULL;
    }

    name = lil_to_string(argv[2]);

    gestlang_add(gl, name, strlen(name), 0, trfun, shape, NULL);

    return NULL;
}

void lil_load_tract(lil_t lil)
{
    lil_register(lil, "tract_new", tract_new);
    lil_register(lil, "tract_gest", tract_gest);
    lil_register(lil, "tract_target", tract_target);
    lil_register(lil, "tract_node", tract_node);
    lil_register(lil, "tract_glang", tract_glang);
}
