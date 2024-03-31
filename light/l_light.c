#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "lil/lil.h"
#include "graforge.h"
#include "core.h"
#include "sklil.h"

#include "light.h"

int sk_node_light(sk_core *core);

static void light_dtor(void *ptr)
{
     mno_light *li;
     li = (mno_light *)ptr;
     mno_light_close(li);
     free(li);
}

static lil_value_t mklight(lil_t lil, size_t argc, lil_value_t *argv)
{
    const char *portname;
    const char *key;
    mno_light *li;
    int rc;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "light", argc, 2);

    key = lil_to_string(argv[0]);
    portname = lil_to_string(argv[1]);

    li = malloc(sizeof(mno_light));
    rc = mno_light_init(li, portname);

    SKLIL_ERROR_CHECK(lil, rc, "mklight could not initialize light.");

    core = lil_get_data(lil);
    rc = sk_core_append(core, key, strlen(key), li, light_dtor);
    SKLIL_ERROR_CHECK(lil, rc, "mklight: couldn't append to dictionary.");

    printf("mklight: sleeping for 3 seconds to wait for arduino\n");
    sleep(3);

    return NULL;
}

static lil_value_t light(lil_t lil, size_t argc, lil_value_t *argv)
{
    sk_core *core;
    int rc;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "light", argc, 2);

    /* arg 0 is light data */

    rc = sklil_param(core, argv[1]);
    SKLIL_PARAM_CHECK(lil, rc, "light");

    rc = sk_node_light(core);

    SKLIL_ERROR_CHECK(lil, rc, "light did not work out.");

    return NULL;
}

void sklil_load_light(lil_t lil)
{
    lil_register(lil, "light", light);
    lil_register(lil, "mklight", mklight);
}
