#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "hotswap.h"

static void delhs(void *ud)
{
    mno_hotswap *hs;

    hs = ud;
    mno_hotswap_del(hs);
    free(hs);
}

static lil_value_t l_hsnew(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    const char *key;
    int rc;
    mno_hotswap *hs;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hsnew", argc, 1);

    key = lil_to_string(argv[0]);

    hs = malloc(mno_hotswap_sizeof());

    mno_hotswap_init(hs, sk_core_patch(core));

    rc = sk_core_append(core, key, strlen(key), hs, delhs);

    SKLIL_ERROR_CHECK(lil, rc, "Couldn't create key.");

    return NULL;
}

static lil_value_t l_hsout(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    gf_patch *patch;
    sk_param in;
    gf_node *node;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hsout", argc, 1);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    rc = sk_param_get(core, &in);
    SKLIL_ERROR_CHECK(lil, rc, "could not get cable");

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "could not make node");

    mno_hotswap_out(node, hs);

    sk_param_set(core, node, &in, 0);

    return NULL;
}

static lil_value_t l_hsswp(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hsswp", argc, 1);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    mno_hotswap_please(hs);

    return NULL;
}

static lil_value_t l_hstog(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hstog", argc, 1);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    mno_hotswap_playtog(hs);

    return NULL;
}

static lil_value_t l_hsnode(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    gf_patch *patch;
    gf_node *node;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hsnode", argc, 1);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "could not make node");

    mno_hotswap_node(node, hs);

    return NULL;
}

static lil_value_t l_hsget(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    gf_patch *patch;
    gf_node *node;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hsget", argc, 1);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    patch = sk_core_patch(core);

    rc = gf_patch_new_node(patch, &node);
    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "could not make node");

    mno_hotswap_get(node, hs);

    sk_param_out(core, node, 0);

    return NULL;
}

static lil_value_t l_hscf(lil_t lil,
                          size_t argc,
                          lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    void *ud;
    int mode;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hscf", argc, 2);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    mode = lil_to_integer(argv[1]);

    mno_hotswap_crossfade(hs, mode);

    return NULL;
}

static lil_value_t l_hscfsz(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    int rc;
    mno_hotswap *hs;
    void *ud;
    double t;
    int ts;
    gf_patch *patch;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "hscfcz", argc, 2);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get hotswap");
    hs = ud;

    t = lil_to_double(argv[1]);
    patch = sk_core_patch(core);
    ts = floor(t * gf_patch_srate_get(patch));

    mno_hotswap_cfsize(hs, ts);

    return NULL;
}

static void delsubpatch(void *ptr)
{
    gf_subpatch *subpatch;

    subpatch = (gf_subpatch *)ptr;

    gf_subpatch_destroy(subpatch);
    gf_subpatch_free(subpatch);

    free(subpatch);
}

static lil_value_t l_subpatch(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int rc;
    sk_core *core;
    const char *key;
    gf_subpatch *subpatch;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "subpatch", argc, 1);

    key = lil_to_string(argv[0]);

    subpatch = malloc(sizeof(gf_subpatch));

    gf_subpatch_init(subpatch);

    rc = sk_core_append(core, key, strlen(key), subpatch, delsubpatch);

    SKLIL_ERROR_CHECK(lil, rc, "could not add subpatch key");

    return NULL;
}

/* needed if a subpatch is the top-level */

static void subpatch_compute(gf_node *node)
{
    gf_subpatch *subpatch;

    subpatch = gf_node_get_data(node);

    gf_subpatch_compute(subpatch);
}

static void deltop(void *ptr)
{
    free(ptr);
}

static lil_value_t l_subpatcher(lil_t lil,
                                size_t argc,
                                lil_value_t *argv)
{
    int rc;
    sk_core *core;
    gf_subpatch *subpatch;
    void *ud;
    gf_patch *patch;
    gf_node *node;
    const char *key;
    gf_subpatch *top;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "subpatcher", argc, 2);

    key = lil_to_string(argv[0]);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get subpatch");
    subpatch = ud;

    top = malloc(sizeof(gf_subpatch));

    rc = sk_core_append(core, key, strlen(key), top, deltop);
    SKLIL_ERROR_CHECK(lil, rc, "could not append subpatcher");


    patch = sk_core_patch(core);

    gf_patch_clear(patch);

    rc = gf_patch_new_node(patch, &node);

    SKLIL_ERROR_CHECK(lil, rc != GF_OK, "could not make node");

    gf_node_set_compute(node, subpatch_compute);
    gf_node_set_data(node, subpatch);
    gf_subpatch_save(patch, top);

    return NULL;
}

static lil_value_t l_psave(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int rc;
    gf_subpatch *subpatch;
    gf_patch *patch;
    sk_core *core;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "psave", argc, 1);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get subpatch");

    subpatch = ud;

    patch = sk_core_patch(core);
    gf_subpatch_save(patch, subpatch);

    return NULL;
}

static lil_value_t l_pload(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int rc;
    gf_subpatch *subpatch;
    gf_patch *patch;
    sk_core *core;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "pload", argc, 1);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);

    SKLIL_ERROR_CHECK(lil, rc, "could not get subpatch");

    subpatch = ud;

    patch = sk_core_patch(core);
    gf_subpatch_restore(patch, subpatch);
    return NULL;
}

static lil_value_t l_pclear(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    gf_patch *patch;
    sk_core *core;
    core = lil_get_data(lil);
    patch = sk_core_patch(core);
    gf_patch_clear(patch);
    return NULL;
}

void lil_load_hotswap(lil_t lil)
{
    lil_register(lil, "hsnew", l_hsnew);
    lil_register(lil, "hsswp", l_hsswp);
    lil_register(lil, "hsout", l_hsout);
    lil_register(lil, "hsnode", l_hsnode);
    lil_register(lil, "hsget", l_hsget);
    lil_register(lil, "hstog", l_hstog);
    lil_register(lil, "hscf", l_hscf);
    lil_register(lil, "hscfsz", l_hscfsz);
    lil_register(lil, "subpatch", l_subpatch);
    lil_register(lil, "subpatcher", l_subpatcher);
    lil_register(lil, "psave", l_psave);
    lil_register(lil, "pload", l_pload);
    lil_register(lil, "pclear", l_pclear);
}
