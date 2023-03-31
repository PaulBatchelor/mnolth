#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "gfxbuf.h"

int minimp4_main(int argc, const char *argv[]);

static void delgfx(void *ud)
{
    gfxbuf *gfx;
    gfx = ud;
    gfxbuf_del(gfx);
}

static lil_value_t l_gfxnew(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    const char *key;
    int w, h;
    int rc;
    gfxbuf *gfx;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gfxbufnew", argc, 3);

    key = lil_to_string(argv[0]);
    w = lil_to_integer(argv[1]);
    h = lil_to_integer(argv[2]);

    gfx = gfxbuf_new(w, h);

    rc = sk_core_append(core, key, strlen(key), gfx, delgfx);

    SKLIL_ERROR_CHECK(lil, rc, "Couldn't create key.");

    return NULL;
}

/* gfxnewz: similar to gfxnew, but with a zoom amount */

static lil_value_t l_gfxnewz(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    const char *key;
    int w, h, z;
    int rc;
    gfxbuf *gfx;

    core = lil_get_data(lil);

    SKLIL_ARITY_CHECK(lil, "gfxnewz", argc, 4);

    key = lil_to_string(argv[0]);
    w = lil_to_integer(argv[1]);
    h = lil_to_integer(argv[2]);
    z = lil_to_integer(argv[3]);

    gfx = gfxbuf_new_zoom(w, h, z);

    rc = sk_core_append(core, key, strlen(key), gfx, delgfx);

    SKLIL_ERROR_CHECK(lil, rc, "Couldn't create key.");

    return NULL;
}

static lil_value_t l_gfxtransfer(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    gfxbuf_transfer(gfx);

    return NULL;
}

static lil_value_t l_gfxtransferz(lil_t lil,
                                  size_t argc,
                                  lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    gfxbuf_transferz(gfx);

    return NULL;
}

static lil_value_t l_gfxappend(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    gfxbuf_append(gfx);

    return NULL;
}

static lil_value_t l_gfxppm(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    const char *ppm;

    SKLIL_ARITY_CHECK(lil, "gfxppm", argc, 1);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    ppm = lil_to_string(argv[0]);

    gfxbuf_ppm(gfx, ppm);
    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not pushgfxbuf instance.");

    return NULL;
}

static void fill(gfxbuf *gfx, gfxbuf_pixel clr)
{
    int x, y;
    int h, w;

    h = gfxbuf_height(gfx);
    w = gfxbuf_width(gfx);

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            gfxbuf_set(gfx, x, y, clr);
        }
    }
}

static lil_value_t l_gfxfill(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    int clr;
    gfxbuf_pixel pix;

    SKLIL_ARITY_CHECK(lil, "gfxfill", argc, 1);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    clr = lil_to_double(argv[0]);
    pix = gfxbuf_clrget(gfx, clr);

    fill(gfx, pix);
    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not push gfxbuf instance.");

    return NULL;
}

static lil_value_t l_gfxopen(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    const char *h264;

    SKLIL_ARITY_CHECK(lil, "gfxopen", argc, 1);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    h264 = lil_to_string(argv[0]);

    gfxbuf_open(gfx, h264);

    return NULL;
}

static lil_value_t l_gfxclose(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    gfxbuf_close(gfx);

    return NULL;
}

static lil_value_t l_gfxmp4(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    const char *minimp4_argv[3];

    SKLIL_ARITY_CHECK(lil, "gfxmp4", argc, 2);

    minimp4_argv[0] = "minimp4";
    minimp4_argv[1] = lil_to_string(argv[0]);
    minimp4_argv[2] = lil_to_string(argv[1]);

    minimp4_main(3, minimp4_argv);

    return NULL;
}

static lil_value_t l_gfxclrset(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    int pos;
    double r, g, b;

    SKLIL_ARITY_CHECK(lil, "gfxclrset", argc, 4);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    pos = lil_to_integer(argv[0]);
    r = lil_to_double(argv[1]);
    g = lil_to_double(argv[2]);
    b = lil_to_double(argv[3]);

    gfxbuf_clrset(gfx, pos, gfxbuf_rgb(r, g, b));

    rc = sk_core_generic_push(core, ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not push gfxbuf instance.");
    return NULL;
}

static lil_value_t l_gfxclrrgb(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;
    int pos;
    int r, g, b;

    SKLIL_ARITY_CHECK(lil, "gfxclrrgb", argc, 4);

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    pos = lil_to_integer(argv[0]);
    r = lil_to_integer(argv[1]);
    g = lil_to_integer(argv[2]);
    b = lil_to_integer(argv[3]);

    gfxbuf_clrset(gfx, pos, gfxbuf_rgb24(r, g, b));
    return NULL;
}

static lil_value_t l_gfxzoomit(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    sk_core *core;
    gfxbuf *gfx;
    void *ptr;
    int rc;

    core = lil_get_data(lil);

    rc = sk_core_generic_pop(core, &ptr);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf instance.");

    gfx = (gfxbuf *)ptr;

    gfxbuf_zoomit(gfx);

    return NULL;
}

void lil_load_gfxbuf(lil_t lil)
{
    lil_register(lil, "gfxnew", l_gfxnew);
    lil_register(lil, "gfxnewz", l_gfxnewz);
    lil_register(lil, "gfxtransfer", l_gfxtransfer);
    lil_register(lil, "gfxtransferz", l_gfxtransferz);
    lil_register(lil, "gfxappend", l_gfxappend);
    lil_register(lil, "gfxppm", l_gfxppm);
    lil_register(lil, "gfxfill", l_gfxfill);
    lil_register(lil, "gfxopen", l_gfxopen);
    lil_register(lil, "gfxclose", l_gfxclose);
    lil_register(lil, "gfxclrset", l_gfxclrset);
    lil_register(lil, "gfxclrrgb", l_gfxclrrgb);
    lil_register(lil, "gfxmp4", l_gfxmp4);
    lil_register(lil, "gfxzoomit", l_gfxzoomit);
}
