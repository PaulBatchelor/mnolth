#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"

#include "uxn/uxn.h"
#include "gestvm.h"

static void delgestvm(void *ptr)
{
    free(ptr);
}

static lil_value_t l_gestvmnew(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    int rc;
    const char *key;
    gestvm_uxn *gu;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "gestvmnew", argc, 1);

    core = lil_get_data(lil);

    key = lil_to_string(argv[0]);
    gu = malloc(gestvm_uxn_sizeof());
    gestvm_uxn_init(gu);

    rc = sk_core_append(core, key, strlen(key), gu, delgestvm);

    SKLIL_ERROR_CHECK(lil, rc, "gestvmnew didn't work out.");

    return NULL;
}

static void compute(gf_node *node)
{
    int n;
    int blksize;
    gestvm *gvm;
    gf_cable *cabcnd;
    gf_cable *cabout;

    blksize = gf_node_blksize(node);
    gvm = gf_node_get_data(node);

    gf_node_get_cable(node, 0, &cabcnd);
    gf_node_get_cable(node, 1, &cabout);

    for (n = 0; n < blksize; n++) {
        SKFLT cnd, out;

        cnd = gf_cable_get(cabcnd, n);

        out = gestvm_tick(gvm, cnd);
        gf_cable_set(cabout, n, out);
    }
}

static void compute_ext(gf_node *node)
{
    int n;
    int blksize;
    gestvm *gvm;
    gf_cable *cabcnd;
    gf_cable *cabext;
    gf_cable *cabout;

    blksize = gf_node_blksize(node);
    gvm = gf_node_get_data(node);

    gf_node_get_cable(node, 0, &cabcnd);
    gf_node_get_cable(node, 1, &cabext);
    gf_node_get_cable(node, 2, &cabout);

    for (n = 0; n < blksize; n++) {
        SKFLT cnd, ext, out;

        cnd = gf_cable_get(cabcnd, n);
        ext = gf_cable_get(cabext, n);

        gestvm_extscale(gvm, ext);
        out = gestvm_tick(gvm, cnd);
        gf_cable_set(cabout, n, out);
    }
}

static void destroy(gf_node *node)
{
    gf_patch *patch;
    int rc;
    void *ud;
    rc = gf_node_get_patch(node, &patch);
    if (rc != GF_OK) return;
    gf_node_cables_free(node);
    ud = gf_node_get_data(node);
    gf_memory_free(patch, &ud);
}

static int node_gestvm(sk_core *core,
                       unsigned int ptr,
                       int use_ext)
{
    int rc;
    void *ud;
    gf_patch *patch;
    gestvm_uxn *gu;
    gf_node *node;
    sk_param cnd, ext;
    gestvm *gvm;

    if (use_ext) {
        rc = sk_param_get(core, &ext);
        SK_ERROR_CHECK(rc);
    }

    rc = sk_param_get(core, &cnd);
    SK_ERROR_CHECK(rc);

    rc = sk_core_generic_pop(core, &ud);
    SK_ERROR_CHECK(rc);
    gu = ud;

    patch = sk_core_patch(core);
    rc = gf_memory_alloc(patch, gestvm_sizeof(), &ud);

    if (rc) return 1;

    gvm = ud;

    gestvm_init(gvm, gu);
    gestvm_pointer(gvm, ptr);

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    if (use_ext) {
        rc = gf_node_cables_alloc(node, 3);
        SK_GF_ERROR_CHECK(rc);
        gf_node_set_block(node, 2);
    } else {
        rc = gf_node_cables_alloc(node, 2);
        SK_GF_ERROR_CHECK(rc);
        gf_node_set_block(node, 1);
    }

    gf_node_set_data(node, gvm);

    if (use_ext) {
        gf_node_set_compute(node, compute_ext);
    } else {
        gf_node_set_compute(node, compute);
    }

    gf_node_set_destroy(node, destroy);

    if (use_ext) {
        sk_param_set(core, node, &cnd, 0);
        sk_param_set(core, node, &ext, 1);
        sk_param_out(core, node, 2);
    } else {
        sk_param_set(core, node, &cnd, 0);
        sk_param_out(core, node, 1);
    }

    gestvm_uxn_set(gu, gvm);
    return 0;
}

int sk_node_gestvm(sk_core *core, unsigned int ptr)
{
    return node_gestvm(core, ptr, 0);
}

int sk_node_gestvm_ext(sk_core *core, unsigned int ptr)
{
    return node_gestvm(core, ptr, 1);
}

static lil_value_t l_gestvmnode(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    sk_core *core;
    int rc;
    unsigned int ptr;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gestvmnode", argc, 3);

    ptr = lil_to_integer(argv[1]);

    rc = sklil_param(core, argv[2]);
    SKLIL_PARAM_CHECK(lil, rc, "gestvmnode");

    if (argc > 3) {
        rc = sklil_param(core, argv[3]);
        SKLIL_PARAM_CHECK(lil, rc, "gestvmnode");
        rc = sk_node_gestvm_ext(core, ptr);
    } else {
        rc = sk_node_gestvm(core, ptr);
    }

    SKLIL_ERROR_CHECK(lil, rc, "gestvmnode didn't work out.");
    return NULL;
}

static lil_value_t l_gestvmsym(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    const char *rom, *sym;
    unsigned int addr;

    SKLIL_ARITY_CHECK(lil, "gestvmsym", argc, 2);

    rom = lil_to_string(argv[0]);
    sym = lil_to_string(argv[1]);

    addr = gestvm_lookup(rom, sym);

    SKLIL_ERROR_CHECK(lil, (addr == 0), "Problem finding symbol");

    return lil_alloc_integer(addr);
}

static lil_value_t l_gestvmload(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    const char *file;
    gestvm_uxn *gu;
    int rc;
    sk_core *core;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "gestvmload", argc, 2);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get gestvm");

    gu = ud;

    file = lil_to_string(argv[1]);

    rc = gestvm_load(gu, file);

    SKLIL_ERROR_CHECK(lil, rc, "could not load file");

    return NULL;
}

static lil_value_t l_gestvmeval(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    unsigned int addr;
    gestvm_uxn *gu;
    int rc;
    sk_core *core;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "gestvmeval", argc, 2);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get gestvm");

    gu = ud;

    addr = lil_to_integer(argv[1]);

    gestvm_eval(gu, NULL, addr);

    return NULL;
}

static lil_value_t l_gestvmlast(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    int rc;
    gestvm_uxn *gu;
    void *ud;
    gestvm *gvm;

    SKLIL_ARITY_CHECK(lil, "gestvmlast", argc, 1);

    core = lil_get_data(lil);
    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not pop gestvm");
    gu = ud;

    gvm = gestvm_uxn_get(gu);

    rc = sk_core_generic_push(core, gvm);

    SKLIL_ERROR_CHECK(lil, rc, "could not push gestvm");
    return NULL;
}

static void compute_weight(gf_node *node)
{
    int n;
    int blksize;
    gestvm_weight *gw;
    gf_cable *cabpos;
    gf_cable *cabneg;
    gf_cable *cabout;

    blksize = gf_node_blksize(node);
    gw = gf_node_get_data(node);

    gf_node_get_cable(node, 0, &cabneg);
    gf_node_get_cable(node, 1, &cabpos);
    gf_node_get_cable(node, 2, &cabout);

    for (n = 0; n < blksize; n++) {
        SKFLT neg, pos, out;

        neg = gf_cable_get(cabneg, n);
        pos = gf_cable_get(cabneg, n);

        gestvm_weight_ampneg(gw, neg);
        gestvm_weight_amppos(gw, pos);
        out = gestvm_weight_tick(gw);
        gf_cable_set(cabout, n, out);
    }
}

int sk_node_gestvmweight(sk_core *core)
{
    int rc;
    void *ud;
    gf_patch *patch;
    gestvm_uxn *gu;
    gf_node *node;
    sk_param neg;
    sk_param pos;
    gestvm *gvm;
    gestvm_weight *gw;

    rc = sk_param_get(core, &pos);
    SK_ERROR_CHECK(rc);

    rc = sk_param_get(core, &neg);
    SK_ERROR_CHECK(rc);

    rc = sk_core_generic_pop(core, &ud);
    SK_ERROR_CHECK(rc);
    gu = ud;

    patch = sk_core_patch(core);
    rc = gf_memory_alloc(patch, gestvm_weight_sizeof(), &ud);

    if (rc) return 1;

    gw = ud;
    gvm = gestvm_uxn_get(gu);

    gestvm_weight_init(gw, gvm, gf_patch_srate_get(patch));

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_cables_alloc(node, 3);
    SK_GF_ERROR_CHECK(rc);
    gf_node_set_block(node, 2);

    gf_node_set_data(node, gw);
    gf_node_set_compute(node, compute_weight);
    gf_node_set_destroy(node, destroy);

    sk_param_set(core, node, &neg, 0);
    sk_param_set(core, node, &pos, 1);
    sk_param_out(core, node, 2);
    return 0;
}

static lil_value_t l_gestvmweight(lil_t lil,
                                  size_t argc,
                                  lil_value_t *argv)
{
    sk_core *core;
    int rc;
    int i;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gestvmweight", argc, 3);


    for (i = 0; i < 3; i++) {
        rc = sklil_param(core, argv[i]);
        SKLIL_PARAM_CHECK(lil, rc, "gestvmweight");
    }

    rc = sk_node_gestvmweight(core);
    SKLIL_ERROR_CHECK(lil, rc, "gestvmweight didn't work out.");
    return NULL;
}

void load_gestvm(lil_t lil)
{
    lil_register(lil, "gestvmnew", l_gestvmnew);
    lil_register(lil, "gestvmnode", l_gestvmnode);
    lil_register(lil, "gestvmload", l_gestvmload);
    lil_register(lil, "gestvmsym", l_gestvmsym);
    lil_register(lil, "gestvmlast", l_gestvmlast);
    lil_register(lil, "gestvmeval", l_gestvmeval);
    lil_register(lil, "gestvmweight", l_gestvmweight);
}
