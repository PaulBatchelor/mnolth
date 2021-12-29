#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h> /* for rseed */
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "gest.h"
#include "gestlang.h"

static void delgl(void *ptr)
{
    gestlang_d *gl;

    gl = ptr;

    gestlang_bye(gl);

    free(gl);
}

static lil_value_t glang_new(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    int rc;
    sk_core *core;
    const char *key;
    gestlang_d *gl;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "glang_new", argc, 1);

    gl = malloc(gestlang_sizeof());
    gestlang_init(gl);
    gestlang_loader(gl);

    key = lil_to_string(argv[0]);

    rc = sk_core_append(core, key, strlen(key), gl, delgl);

    SKLIL_ERROR_CHECK(lil, rc, "could not add gestlang.");
    return NULL;
}

static lil_value_t glang_eval(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int rc;
    sk_core *core;
    const char *str;
    gestlang_d *gl;
    gest_d *gest;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "glang_new", argc, 3);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gestlang");
    gl = ud;

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gest");
    gest = ud;

    str = lil_to_string(argv[2]);

    rc = gestlang_parse(gl, gest, str, strlen(str));

    SKLIL_ERROR_CHECK(lil, rc != 0, "gestlang failed");

    return NULL;
}

static lil_value_t glang_load(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int rc;
    sk_core *core;
    const char *filename;
    gestlang_d *gl;
    gest_d *gest;
    void *ud;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "glang_new", argc, 3);

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gestlang");
    gl = ud;

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gest");
    gest = ud;

    filename = lil_to_string(argv[2]);

    rc = gestlang_load(gl, gest, filename);

    SKLIL_ERROR_CHECK(lil, rc != 0, "gestlang failed");

    return NULL;
}

void sklil_load_gestlang(lil_t lil)
{
    lil_register(lil, "glang_new", glang_new);
    lil_register(lil, "glang_eval", glang_eval);
    lil_register(lil, "glang_load", glang_load);
}
