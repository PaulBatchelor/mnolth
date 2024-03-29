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

/* gfxcirc gfx cx cy r clr
 * a test circle, made using SDFs
 * cx, cy: center point.
 * r: radius
 * clr: color index from graphics buffer
 */


static lil_value_t l_gfxcirc(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    double cx, cy;
    double r;
    int pos;

    SKLIL_ARITY_CHECK(lil, "gfxcirc", argc, 4);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    cx = lil_to_double(argv[0]);
    cy = lil_to_double(argv[1]);
    r = lil_to_double(argv[2]);
    pos = lil_to_integer(argv[3]);

    gfxdrw_circ_filled(gfx, cx, cy, r, pos);

    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not push gfxbuf instance.");
    return NULL;
}

static lil_value_t l_gfxheart(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    double cx, cy, r;
    int pos;

    SKLIL_ARITY_CHECK(lil, "gfxheart", argc, 4);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    cx = lil_to_double(argv[0]);
    cy = lil_to_double(argv[1]);
    r = lil_to_double(argv[2]);
    pos = lil_to_integer(argv[3]);

    gfxdrw_heart(gfx, cx, cy, r, pos);

    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not push gfxbuf instance.");
    return NULL;
}

static lil_value_t l_gfxboxround(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    double x, y, w, h, r;
    double scale;
    int pos;

    SKLIL_ARITY_CHECK(lil, "gfxbuf", argc, 6);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    x = lil_to_double(argv[0]);
    y = lil_to_double(argv[1]);
    w = lil_to_double(argv[2]);
    h = lil_to_double(argv[3]);
    r = lil_to_double(argv[4]);
    pos = lil_to_integer(argv[5]);
    scale = 1.0;

    if (argc >= 7) {
        scale = lil_to_double(argv[6]);
    }

    gfxdrw_rounded_box(gfx, x, y, w, h, r, scale, pos);

    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not push gfxbuf instance.");
    return NULL;
}

void lil_load_draw(lil_t lil)
{
    lil_register(lil, "gfxrectf", gfxrectf);
    lil_register(lil, "gfxcirc", l_gfxcirc);
    lil_register(lil, "gfxheart", l_gfxheart);
    lil_register(lil, "gfxboxround", l_gfxboxround);
}
