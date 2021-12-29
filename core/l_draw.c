#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"
#include "gfxbuf.h"
#include "draw.h"

static int getgfx(lil_t lil, gfxbuf **gfx)
{
    sk_core *core;
    int rc;
    void *ud;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ud);

    if (rc) {
        lil_set_error(lil, "Could not get gfxbuf");
        return 1;
    }

    *gfx = (gfxbuf *)ud;
    return 0;
}

static lil_value_t gfxrectf(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    int rc;
    int x, y;
    int w, h;
    int clr;
    gfxbuf *gfx;

    SKLIL_ARITY_CHECK(lil, "gfxrectf", argc, 5);

    rc = getgfx(lil, &gfx);

    if (rc) return NULL;

    x = lil_to_integer(argv[0]);
    y = lil_to_integer(argv[1]);
    w = lil_to_integer(argv[2]);
    h = lil_to_integer(argv[3]);
    clr = lil_to_integer(argv[4]);

    gfxdrw_rect_filled(gfx, x, y, w, h, clr);

    return NULL;
}

void lil_load_draw(lil_t lil)
{
    lil_register(lil, "gfxrectf", gfxrectf);
}
