#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sndkit/lil/lil.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sklil.h"
#include "gfxbuf.h"
#include "mathc/mathc.h"

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

/* gfxcirc gfx cx cy r clr
 * a test circle, made using SDFs
 * cx, cy: center point.
 * r: radius
 * clr: color index from graphics buffer
 */


static float sdfcirc(struct vec2 p, float r)
{
    return svec2_length(p) - r;
}

struct vec2 normalize(struct vec2 pos, struct vec2 res)
{
    struct vec2 p;
    p = svec2_multiply_f(pos, 2.0);
    p = svec2_subtract(p, res);
    p = svec2_divide_f(p, res.y);
    return p;
}

static float smoothstep(float e0, float e1, float x)
{
    float t;
    t = clampf((x - e0) / (e1 - e0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

static float sign(float x)
{
    if (x < 0) return -1;
    else if (x > 0) return 1;
    return 0;
}

static void circ(gfxbuf_state *st)
{
    /* float a; */
    /* struct gdraw_data *gd; */

    /* gd = st->info->ud; */

    /* a = (float)st->y / st->info->rh; */

    /* st->out->r = a * gd->clr2->r + (1 - a) * gd->clr1->r; */
    /* st->out->g = a * gd->clr2->g + (1 - a) * gd->clr1->g; */
    /* st->out->b = a * gd->clr2->b + (1 - a) * gd->clr1->b; */
    float d;
    struct vec3 col;
    float alpha;
    struct vec2 p;
    struct vec2 res;
    gfxbuf_pixel *pix;
    struct vec3 bg;


    res = svec2(st->info->rw, st->info->rh);

    pix = st->info->ud;

    p = normalize(svec2(st->x, st->y), res);
    d = -sdfcirc(p, 0.9);

    alpha = 0;
    alpha = sign(d) > 0;

    alpha += smoothstep(3.0 / st->info->rw, 0.0, fabs(d));
    alpha = clampf(alpha, 0, 1);

    bg.x = st->out->r;
    bg.y = st->out->g;
    bg.z = st->out->b;

    bg = svec3(st->out->r, st->out->g, st->out->b);
    col = svec3(pix->r, pix->g, pix->b);
    col = svec3_lerp(bg, col, alpha);

    st->out->r = col.x;
    st->out->g = col.y;
    st->out->b = col.z;
}


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
    int x, y;
    int w, h;
    gfxbuf_pixel clr;
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

    w = 2 * r;
    h = w;

    x = cx - r;
    y = cy - r;

    clr = gfxbuf_clrget(gfx, pos);

    gfxbuf_draw(gfx, x, y, w, h, circ, &clr);
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
    lil_register(lil, "gfxcirc", l_gfxcirc);
    lil_register(lil, "gfxclrset", l_gfxclrset);
    lil_register(lil, "gfxclrrgb", l_gfxclrrgb);
    lil_register(lil, "gfxmp4", l_gfxmp4);
    lil_register(lil, "gfxzoomit", l_gfxzoomit);
}
