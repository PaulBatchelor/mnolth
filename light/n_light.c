#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include "graforge.h"
#include "core.h"
#include "light.h"

struct light_n {
    gf_cable *in;
    mno_light *light;
};

static void compute(gf_node *node)
{
    struct light_n *li_n;
    GFFLT in;

    li_n = (struct light_n *)gf_node_get_data(node);

    /* control rate, only take first sample of block */
    in = gf_cable_get(li_n->in, 0);
    mno_light_set(li_n->light, in);
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

int sk_node_light(sk_core *core)
{
    gf_patch *patch;
    gf_node *node;
    int rc;
    sk_param in;
    void *ud;
    struct light_n *li_n;
    mno_light *light;

    rc = sk_param_get(core, &in);
    SK_ERROR_CHECK(rc);

    rc = sk_core_generic_pop(core, &ud);
    SK_ERROR_CHECK(rc);
    light = ud;

    patch = sk_core_patch(core);

    rc = gf_memory_alloc(patch, sizeof(struct light_n), &ud);
    SK_GF_ERROR_CHECK(rc);
    li_n = (struct light_n *)ud;
    li_n->light = light;

    rc = gf_patch_new_node(patch, &node);
    SK_GF_ERROR_CHECK(rc);

    rc = gf_node_cables_alloc(node, 1);
    SK_GF_ERROR_CHECK(rc);

    gf_node_get_cable(node, 0, &li_n->in);

    gf_node_set_data(node, li_n);
    gf_node_set_compute(node, compute);
    gf_node_set_destroy(node, destroy);

    sk_param_set(core, node, &in, 0);
    return 0;
}
