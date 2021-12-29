#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"
#include "kuf.h"
#include "btprnt/btprnt.h"

static lil_value_t kuf_hi(lil_t lil,
                          size_t argc,
                          lil_value_t *argv)
{
    printf("hello there!\n");
    return NULL;
}

static void cleanbuf(void *p)
{
    free(p);
}

static lil_value_t kuf_new(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int sz;
    const char *key;
    int rc;
    uint16_t *buf;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "kufnew", argc, 2);

    core = lil_get_data(lil);

    key = lil_to_string(argv[0]);
    sz = lil_to_integer(argv[1]);

    buf = calloc(1, sizeof(uint16_t) * sz);

    rc = sk_core_append(core, key, strlen(key), buf, cleanbuf);

    SKLIL_ERROR_CHECK(lil, rc, "Could not append to dictionary.");

    return NULL;
}

static int get_squares(lil_t lil,
                       lil_value_t val,
                       uint16_t **sqrs)
{
    void *ud;
    sk_core *core;
    int rc;

    core = lil_get_data(lil);

    ud = NULL;

    rc = sk_core_generic_pop(core, &ud);

    SK_ERROR_CHECK(rc);

    *sqrs = (uint16_t *)ud;

    return rc;
}

static lil_value_t kuf_set(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int i;
    int val;
    uint16_t *sqrs;
    int rc;

    sqrs = NULL;
    rc = get_squares(lil, argv[0], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");
    i = lil_to_integer(argv[1]);
    val = lil_to_integer(argv[2]);
    sqrs[i] = val;
    return NULL;
}

static lil_value_t kuf_pbm(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    const char *filename;
    int w, h;
    uint16_t *sqrs;
    int rc;

    sqrs = NULL;
    filename = lil_to_string(argv[0]);
    w = lil_to_integer(argv[1]);
    h = lil_to_integer(argv[2]);
    rc = get_squares(lil, argv[3], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");

    kuf_write_pbm(filename, w, h, sqrs);
    return NULL;
}

static lil_value_t kuf_hex(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int out;
    const char *in;
    int len;
    int i;

    in = lil_to_string(argv[0]);

    len = strlen(in);

    out = 0;
    for(i = 0; i < len; i++) {
        char c;
        c = in[i];

        if (c >= '0' && c <= '9') {
            out <<= 4;
            out += (c - '0');
        } else if ((c >= 'A' && c <= 'F')) {
            out <<= 4;
            out += (c - 'A') + 0xA;
        }
    }
    return lil_alloc_integer(out);
}

static lil_value_t kuf_setsqr(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int w;
    uint16_t *sqrs;
    int xp, yp;
    int s;
    int rc;

    SKLIL_ARITY_CHECK(lil, "kufsetsqr", argc, 5);

    w = lil_to_integer(argv[0]);
    sqrs = NULL;
    rc = get_squares(lil, argv[1], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");
    xp = lil_to_integer(argv[2]);
    yp = lil_to_integer(argv[3]);
    s = lil_to_integer(argv[4]);

    kuf_set_square(w, sqrs, xp, yp, s);
    return NULL;
}

static lil_value_t kuf_genblk(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    uint32_t rng;
    lil_list_t list;
    uint16_t a, b, c, d;
    lil_value_t r;

    list = lil_alloc_list();

    SKLIL_ARITY_CHECK(lil, "kufgenblk", argc, 1);

    rng = lil_to_integer(argv[0]);

    a = b = c = d = 0;

    kuf_gen_block(&rng, &a, &b, &c, &d);

    lil_list_append(list, lil_alloc_integer(a));
    lil_list_append(list, lil_alloc_integer(b));
    lil_list_append(list, lil_alloc_integer(c));
    lil_list_append(list, lil_alloc_integer(d));

    r = lil_list_to_value(list, 1);
    lil_free_list(list);

    list = lil_alloc_list();

    lil_list_append(list, r);
    lil_list_append(list, lil_alloc_integer(rng));

    r = lil_list_to_value(list, 1);
    lil_free_list(list);
    return r;
}

static lil_value_t kuf_setblk(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int w;
    uint16_t *sqrs;
    int xp, yp;
    lil_list_t list;
    int rc;
    uint16_t a, b, c, d;

    SKLIL_ARITY_CHECK(lil, "kufsetsqr", argc, 5);

    w = lil_to_integer(argv[0]);
    sqrs = NULL;
    rc = get_squares(lil, argv[1], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");
    xp = lil_to_integer(argv[2]);
    yp = lil_to_integer(argv[3]);
    list = lil_subst_to_list(lil, argv[4]);

    a = lil_to_integer(lil_list_get(list, 0));
    b = lil_to_integer(lil_list_get(list, 1));
    c = lil_to_integer(lil_list_get(list, 2));
    d = lil_to_integer(lil_list_get(list, 3));
    kuf_set_block(w, sqrs, xp, yp, a, b, c, d);

    lil_free_list(list);
    return NULL;
}

static lil_value_t kuf_cor(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    int w, h;
    uint16_t *sqrs;
    int rc;

    SKLIL_ARITY_CHECK(lil, "kufcor", argc, 3);

    w = lil_to_integer(argv[0]);
    h = lil_to_integer(argv[1]);
    sqrs = NULL;
    rc = get_squares(lil, argv[2], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");

    kuf_correct(w, h, sqrs, NULL);

    return NULL;
}

/* write to btprnt region one 4-bit row (wall) at a time */

static void write_row(btprnt_region *reg,
                      int xoff, int yoff, int scale,
                      int x, int r, int y,
                      uint8_t w)
{
    int i;
    int sx, sy;

    for (i = 0; i < 4; i++) {
        /* TODO */
        int t;
        t = (w >> i) & 1;
        for (sy = 0; sy < scale; sy++)
        for (sx = 0; sx < scale; sx++) {
            btprnt_region_draw(reg,
                               xoff + x*4*scale + i*scale + sx,
                               yoff + y*4*scale + r*scale + sy,
                               t);
        }
    }
}

static lil_value_t kuf_btprnt(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    int rc;
    sk_core *core;
    uint16_t *sqrs;
    btprnt_region *reg;
    int n, m;
    int scale;
    int xoff, yoff;
    int x, y, r;
    void *ud;

    SKLIL_ARITY_CHECK(lil, "kufbp", argc, 7);

    core = lil_get_data(lil);

    rc = get_squares(lil, argv[1], &sqrs);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get squares.");

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "Couldn't get btprnt region.");

    reg = (btprnt_region *)ud;

    xoff = lil_to_integer(argv[2]);
    yoff = lil_to_integer(argv[3]);
    n = lil_to_integer(argv[4]);
    m = lil_to_integer(argv[5]);
    scale  = lil_to_integer(argv[6]);

    /* adapted from kuf_write_pbm */

    for (y = 0; y < m; y++) {
        for (r = 0; r < 4; r++) {
            for (x = 0; x < n; x++) {
                uint8_t w;
                uint16_t s;

                w = 0;
                s = sqrs[y * n + x];

                if (r < 2) {
                    /* upper quads A + B */
                    uint8_t a, b;

                    a = kuf_square_quad_get(s, KUF_QUAD_A);
                    b = kuf_square_quad_get(s, KUF_QUAD_B);

                    if (r < 1) {
                        /* North side A + B */
                        w = kuf_quad_side_get(b, KUF_NORTH);
                        w <<= 2;
                        w |= kuf_quad_side_get(a, KUF_NORTH);
                    } else {
                        /* South side A + B */
                        w = kuf_quad_side_get(b, KUF_SOUTH);
                        w <<= 2;
                        w |= kuf_quad_side_get(a, KUF_SOUTH);
                    }
                } else {
                    /* lower quads C + D */
                    uint8_t c, d;

                    c = kuf_square_quad_get(s, KUF_QUAD_C);
                    d = kuf_square_quad_get(s, KUF_QUAD_D);

                    if (r <= 2) {
                        /* North side C + D */
                        w = kuf_quad_side_get(d, KUF_NORTH);
                        w <<= 2;
                        w |= kuf_quad_side_get(c, KUF_NORTH);
                    } else {
                        /* South side C + D */
                        w = kuf_quad_side_get(d, KUF_SOUTH);
                        w <<= 2;
                        w |= kuf_quad_side_get(c, KUF_SOUTH);
                    }
                }

                write_row(reg, xoff, yoff, scale, x, r, y, w);
            }
        }
    }


    return NULL;
}

void lil_load_kuf(lil_t lil)
{
    lil_register(lil, "kufhi", kuf_hi);
    lil_register(lil, "kufnew", kuf_new);
    lil_register(lil, "kufset", kuf_set);
    lil_register(lil, "kufpbm", kuf_pbm);
    lil_register(lil, "kufhex", kuf_hex);
    lil_register(lil, "kufsetsqr", kuf_setsqr);
    lil_register(lil, "kufgenblk", kuf_genblk);
    lil_register(lil, "kufsetblk", kuf_setblk);
    lil_register(lil, "kufcor", kuf_cor);
    lil_register(lil, "kufbp", kuf_btprnt);
}
