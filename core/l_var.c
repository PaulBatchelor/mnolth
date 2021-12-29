#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "var.h"

static lil_value_t l_varnew(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    int rc;
    const char *key;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "varnew", argc, 1);

    key = lil_to_string(argv[0]);

    rc = sk_var_new(core, key, strlen(key));

    SKLIL_ERROR_CHECK(lil, rc, "varnew didn't work.");

    return NULL;
}

static lil_value_t l_varset(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    int rc;
    const char *key;
    SKFLT val;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "varset", argc, 2);

    key = lil_to_string(argv[0]);
    val = lil_to_double(argv[1]);

    rc = sk_var_set(core, key, strlen(key), val);

    SKLIL_ERROR_CHECK(lil, rc, "varset didn't work.");

    return NULL;
}

static lil_value_t l_varcpy(lil_t lil, size_t argc, lil_value_t *argv)
{
    sk_core *core;
    int rc;
    const char *key;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "varcpy", argc, 2);

    rc = sklil_param(core, argv[0]);
    SKLIL_PARAM_CHECK(lil, rc, "varcpy");
    key = lil_to_string(argv[1]);

    rc = sk_node_varcpy(core, key, strlen(key));
    SKLIL_ERROR_CHECK(lil, rc, "varcpy didn't work out.");
    return NULL;
}

static lil_value_t l_vargrb(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    int rc;
    SKFLT *val;
    void *ud;

    core = lil_get_data(lil);
    SKLIL_ARITY_CHECK(lil, "vargrb", argc, 1);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Could not pop value.");

    val = (SKFLT *)ud;
    return lil_alloc_double(*val);
}

static lil_value_t l_varsig(lil_t lil, size_t argc, lil_value_t *argv)
{
    sk_core *core;
    int rc;
    const char *key;
    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "varsig", argc, 1);

    key = lil_to_string(argv[0]);

    rc = sk_node_varsig(core, key, strlen(key));
    SKLIL_ERROR_CHECK(lil, rc, "varsig didn't work out.");
    return NULL;
}

void lil_load_var(lil_t lil)
{
    lil_register(lil, "varnew", l_varnew);
    lil_register(lil, "varset", l_varset);
    lil_register(lil, "varcpy", l_varcpy);
    lil_register(lil, "vargrb", l_vargrb);
    lil_register(lil, "varsig", l_varsig);
}
