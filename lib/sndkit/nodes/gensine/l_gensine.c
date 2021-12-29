#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lil/lil.h"
#include "graforge.h"
#include "core.h"
#include "sklil.h"

int sk_node_gensine(sk_core *core);

static lil_value_t gensine(lil_t lil, size_t argc, lil_value_t *argv)
{
    sk_core *core;
    int rc;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gensine", argc, 1);

    rc = sk_node_gensine(core);

    SKLIL_ERROR_CHECK(lil, rc, "gensine didn't work out.");

    return NULL;
}

void sklil_load_gensine(lil_t lil)
{
    lil_register(lil, "gensine", gensine);
}
