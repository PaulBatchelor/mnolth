#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"
#include "btprnt/btprnt.h"
#include "gfxbuf.h"
#include "font.xbm"

#include "sndkit/extra/verify/md5.h"

#define BP_REGPOOL_PRIV
#include "regpool.h"

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

void btprnt_png(btprnt *bp, const char *out);

struct lil_btprnt {
    btprnt *bp;
    btprnt_regpool pool;
};

struct lil_bpfont {
    btprnt_buf *buf;
    int w, h;
    int free;
};

btprnt_buf * lil_bpfont_buf(struct lil_bpfont *fnt)
{
    return fnt->buf;
}

int lil_bpfont_width(struct lil_bpfont *fnt)
{
    return fnt->w;
}

int lil_bpfont_height(struct lil_bpfont *fnt)
{
    return fnt->h;
}

static lil_value_t l_btphi(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    printf("hello there!\n");
    return NULL;
}

static void delbp(void *ptr)
{
    struct lil_btprnt *lbp;

    lbp = ptr;

    btprnt_del(&lbp->bp);
    free(lbp);
}

static lil_value_t l_bpnew(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    sk_core *core;
    int rc;
    const char *key;
    int w, h;

    SKLIL_ARITY_CHECK(lil, "bpnew", argc, 3);

    core = lil_get_data(lil);

    lbp = malloc(sizeof(struct lil_btprnt));

    key = lil_to_string(argv[0]);
    w = lil_to_integer(argv[1]);
    h = lil_to_integer(argv[2]);

    lbp->bp = btprnt_new(w, h);

    btprnt_regpool_init(lbp->bp, &lbp->pool);

    rc = sk_core_append(core, key, strlen(key), lbp, delbp);

    SKLIL_ERROR_CHECK(lil, rc, "could not append bpnew key");

    return NULL;
}

static int getlbp(lil_t lil, sk_core *core, struct lil_btprnt **lbp)
{

    void *ud;
    int rc;

    rc = sk_core_generic_pop(core, &ud);

    if (rc) {
        lil_set_error(lil, "Could not get btprnt data");
        return 1;
    }

    *lbp = (struct lil_btprnt *)ud;
    return 0;
}

static int getreg(lil_t lil, sk_core *core, btprnt_region **reg)
{

    void *ud;
    int rc;

    rc = sk_core_generic_pop(core, &ud);

    if (rc) {
        lil_set_error(lil, "Could not get btprnt region");
        return 1;
    }

    *reg= (btprnt_region *)ud;
    return 0;
}

int lil_btprnt_region(lil_t lil, sk_core *core, btprnt_region **reg)
{
    return getreg(lil, core, reg);
}

static int getfont(lil_t lil, sk_core *core, struct lil_bpfont **fnt)
{

    void *ud;
    int rc;

    rc = sk_core_generic_pop(core, &ud);

    if (rc) {
        lil_set_error(lil, "Could not get btprnt region");
        return 1;
    }

    *fnt = (struct lil_bpfont *)ud;
    return 0;
}

static lil_value_t l_bpset(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int r;
    int x, y, w, h;
    int rc;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "bpset", argc, 6);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    r = lil_to_integer(argv[1]);
    x = lil_to_integer(argv[2]);
    y = lil_to_integer(argv[3]);
    w = lil_to_integer(argv[4]);
    h = lil_to_integer(argv[5]);

    btprnt_regpool_set(&lbp->pool, r, x, y, w, h);

    return NULL;
}

static lil_value_t l_bpget(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int r;
    int rc;
    sk_core *core;
    btprnt_region *reg;

    SKLIL_ARITY_CHECK(lil, "bpget", argc, 2);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    r = lil_to_integer(argv[1]);

    reg = btprnt_regpool_get(&lbp->pool, r);

    if (reg == NULL) {
        lil_set_error(lil, "could not find region");
    }

    rc = sk_core_generic_push(core, reg);

    SKLIL_ERROR_CHECK(lil, rc, "Could not push region");

    return NULL;
}

static lil_value_t l_bpmrk(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int r;
    int rc;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "bpmrk", argc, 2);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    r = lil_to_integer(argv[1]);

    btprnt_regpool_mark(&lbp->pool, r);

    return NULL;
}

static lil_value_t l_bpclr(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int r;
    int rc;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "bpclr", argc, 2);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    r = lil_to_integer(argv[1]);

    btprnt_regpool_clear(&lbp->pool, r);

    return NULL;
}

static lil_value_t l_bpnxt(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int r;
    int rc;
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "bpnxt", argc, 1);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    r = btprnt_regpool_next(&lbp->pool);

    SKLIL_ERROR_CHECK(lil, (r < 0), "Could not find free register");

    return lil_alloc_integer(r);
}

static lil_value_t l_bppbm(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int rc;
    sk_core *core;
    const char *filename;

    SKLIL_ARITY_CHECK(lil, "bppbm", argc, 1);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    if (argc >= 2) {
        filename = lil_to_string(argv[1]);
    } else {
        filename = NULL;
    }

    btprnt_buf_pbm(btprnt_buf_get(lbp->bp), filename);

    return NULL;
}

static lil_value_t l_bprect(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int x, y, w, h, clr;

    SKLIL_ARITY_CHECK(lil, "bprect", argc, 6);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    x = lil_to_integer(argv[1]);
    y = lil_to_integer(argv[2]);
    w = lil_to_integer(argv[3]);
    h = lil_to_integer(argv[4]);
    clr = lil_to_integer(argv[5]);

    btprnt_draw_rect(reg, x, y, w, h, clr);

    return NULL;
}

static lil_value_t l_bprectf(lil_t lil,
                             size_t argc,
                             lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int x, y, w, h, clr;

    SKLIL_ARITY_CHECK(lil, "bprectf", argc, 6);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    x = lil_to_integer(argv[1]);
    y = lil_to_integer(argv[2]);
    w = lil_to_integer(argv[3]);
    h = lil_to_integer(argv[4]);
    clr = lil_to_integer(argv[5]);

    btprnt_draw_rect_filled(reg, x, y, w, h, clr);

    return NULL;
}

static void btprnt_transfer(gfxbuf *gfx,
                            btprnt *b,
                            int xpos, int ypos,
                            int w, int h,
                            int offx, int offy,
                            gfxbuf_pixel p)
{
    int x, y;
    int bpw, bph;
    btprnt_buf *buf;

    buf = btprnt_buf_get(b);

    bpw = btprnt_buf_width(buf);
    bph = btprnt_buf_height(buf);

    if ((w + offx) > bpw) {
        w -= (w + offx) - bpw;
    }

    if ((h + offy) > bph) {
        h -= (h + offy) - bph;
    }

    if (w <= 0 || h <= 0) return;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (btprnt_buf_read(buf, x + offx, y + offy)) {
                gfxbuf_set(gfx,xpos + x, ypos + y, p);
            }
        }
    }

}

static lil_value_t l_bptr(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int rc;
    sk_core *core;
    int x, y, w, h;
    int xoff, yoff;
    int clr;
    void *ud;
    gfxbuf *gfx;
    gfxbuf_pixel p;

    SKLIL_ARITY_CHECK(lil, "bptr", argc, 8);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    rc = sk_core_generic_pop(core, &ud);
    SKLIL_ERROR_CHECK(lil, rc, "could not get gfxbuf");
    gfx = ud;

    x = lil_to_integer(argv[1]);
    y = lil_to_integer(argv[2]);
    w = lil_to_integer(argv[3]);
    h = lil_to_integer(argv[4]);
    xoff = lil_to_integer(argv[5]);
    yoff = lil_to_integer(argv[6]);
    clr = lil_to_integer(argv[7]);

    p = gfxbuf_clrget(gfx, clr);

    btprnt_transfer(gfx, lbp->bp, x, y, w, h, xoff, yoff, p);

    return NULL;
}

extern unsigned char font_bits[];

static void delfont(void *ptr)
{
    struct lil_bpfont *fnt;
    fnt = ptr;
    if (fnt->free) btprnt_buf_free(&fnt->buf);
    free(fnt);
}

int btprnt_font_append(sk_core *core,
                       const char *key,
                       int sz,
                       btprnt_buf *buf,
                       int w, int h,
                       int free)
{
    struct lil_bpfont *fnt;
    fnt = calloc(1, sizeof(struct lil_bpfont));
    fnt->buf = buf;
    fnt->w = 8;
    fnt->h = 8;
    fnt->free = 1;
    return sk_core_append(core, key, sz, fnt, delfont);
}

static lil_value_t l_bpfnt_default(lil_t lil,
                                   size_t argc,
                                   lil_value_t *argv)
{
    int rc;
    const char *key;
    /* struct lil_bpfont *fnt; */
    sk_core *core;

    SKLIL_ARITY_CHECK(lil, "bpfnt_default", argc, 1);

    core = lil_get_data(lil);

    key = lil_to_string(argv[0]);

    /* fnt = calloc(1, sizeof(struct lil_bpfont)); */

    /* fnt->buf = btprnt_buf_extmem(256, 32, font_bits); */
    /* fnt->w = 8; */
    /* fnt->h = 8; */
    /* fnt->free = 1; */

    /* rc = sk_core_append(core, key, strlen(key), fnt, delfont); */

    rc = btprnt_font_append(core,
                            key, strlen(key),
                            btprnt_buf_extmem(256, 32, font_bits),
                            8, 8, 1);

    SKLIL_ERROR_CHECK(lil, rc, "Could not create key for font.");

    return NULL;
}

static lil_value_t l_bptxtbox(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    btprnt_region *reg;
    struct lil_bpfont *fnt;
    int rc;
    sk_core *core;
    int x, y;
    const char *str;

    SKLIL_ARITY_CHECK(lil, "txtbox", argc, 5);
    core = lil_get_data(lil);

    rc = getfont(lil, core, &fnt);
    if (rc) return NULL;
    rc = getreg(lil, core, &reg);
    if (rc) return NULL;

    x = lil_to_integer(argv[2]);
    y = lil_to_integer(argv[3]);
    str = lil_to_string(argv[4]);

    btprnt_draw_wraptext(reg, fnt->buf,
                        x, y,
                        fnt->w, fnt->h,
                        str);

    return NULL;
}

static lil_value_t l_bpchar(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    btprnt_region *reg;
    struct lil_bpfont *fnt;
    int rc;
    sk_core *core;
    int x, y;
    const char *str;
    int scale;
    int clr;

    SKLIL_ARITY_CHECK(lil, "bpchar", argc, 7);
    core = lil_get_data(lil);

    rc = getfont(lil, core, &fnt);
    if (rc) return NULL;
    rc = getreg(lil, core, &reg);
    if (rc) return NULL;

    x = lil_to_integer(argv[2]);
    y = lil_to_integer(argv[3]);
    str = lil_to_string(argv[4]);
    scale = lil_to_integer(argv[5]);
    clr = lil_to_integer(argv[6]);

    btprnt_draw_char(reg, fnt->buf,
                     x, y,
                     fnt->w, fnt->h,
                     str[0], scale, clr);

    return NULL;
}

static lil_value_t l_bpfill(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int clr;

    SKLIL_ARITY_CHECK(lil, "bpfill", argc, 2);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    clr = lil_to_integer(argv[1]);

    btprnt_fill(reg, clr);

    return NULL;
}

static lil_value_t l_bpline(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int x0, y0;
    int x1, y1;
    int clr;

    SKLIL_ARITY_CHECK(lil, "bptline", argc, 6);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    x0 = lil_to_integer(argv[1]);
    y0 = lil_to_integer(argv[2]);
    x1 = lil_to_integer(argv[3]);
    y1 = lil_to_integer(argv[4]);
    clr = lil_to_integer(argv[5]);

    btprnt_draw_line(reg, x0, y0, x1, y1, clr);

    return NULL;
}

static lil_value_t l_bpcirc(lil_t lil,
                            size_t argc,
                            lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int x, y, r, clr;

    SKLIL_ARITY_CHECK(lil, "bpcirc", argc, 5);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    x = lil_to_integer(argv[1]);
    y = lil_to_integer(argv[2]);
    r = lil_to_integer(argv[3]);
    clr = lil_to_integer(argv[4]);

    btprnt_draw_circ(reg, x, y, r, clr);

    return NULL;
}

static lil_value_t l_bpoutline(lil_t lil,
                               size_t argc,
                               lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int clr;

    SKLIL_ARITY_CHECK(lil, "bpoutline", argc, 2);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    clr  = lil_to_integer(argv[1]);

    btprnt_layout_outline(reg, clr);

    return NULL;
}

static lil_value_t l_bproundrect(lil_t lil,
                                 size_t argc,
                                 lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int x, y, w, h, r, clr;

    SKLIL_ARITY_CHECK(lil, "bproundrect", argc, 7);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    x = lil_to_integer(argv[1]);
    y = lil_to_integer(argv[2]);
    w = lil_to_integer(argv[3]);
    h = lil_to_integer(argv[4]);
    r = lil_to_integer(argv[5]);
    clr = lil_to_integer(argv[6]);

    btprnt_draw_roundrect(reg, x, y, w, h, r, clr);

    return NULL;
}

static char nibble(int x)
{
    char out;

    out = '0';
    x &= 0xf;

    if (x >= 0x0 && x <= 0x9) {
        out = '0' + x;
    } else if (x >= 0xa && x <= 0xf) {
        out = 'a' + (x - 0xa);
    }

    return out;
}

static lil_value_t l_bpverify(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int rc;
    sk_core *core;
    md5_state_t state;
    md5_byte_t digest[16];
    char out[33];
    btprnt_buf *buf;
    int n;
    size_t buflen;
    const char *cmp;

    SKLIL_ARITY_CHECK(lil, "bpverify", argc, 1);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    md5_init(&state);

    buf = btprnt_buf_get(lbp->bp);
    buflen = btprnt_buf_size(buf);

    md5_append(&state,
               (const md5_byte_t *)btprnt_buf_data(buf),
               buflen);
    md5_finish(&state, digest);

    for (n = 0; n < 16; n++) {
        out[2*n + 1] = nibble(digest[n]);
        out[2*n] = nibble(digest[n]>>4);
    }

    out[32] = '\0';

    if (argc == 1) {
        printf("%s\n", out);
        return NULL;
    }

    cmp = lil_to_string(argv[1]);

    if (strcmp(cmp, out)) {
        /* technically not secure. try not to stack smash this. */
        char str[128];
        sprintf(str, "expected %s, got %s", cmp, out);
        lil_set_error(lil, str);
        lil_set_errcode(1);
        return NULL;
    }

    return NULL;
}

static lil_value_t l_bppng(lil_t lil,
                           size_t argc,
                           lil_value_t *argv)
{
    struct lil_btprnt *lbp;
    int rc;
    sk_core *core;
    const char *filename;

    SKLIL_ARITY_CHECK(lil, "bppng", argc, 1);
    core = lil_get_data(lil);

    rc = getlbp(lil, core, &lbp);

    if (rc) return NULL;

    if (argc >= 2) {
        filename = lil_to_string(argv[1]);
    } else {
        filename = NULL;
    }

    btprnt_png(lbp->bp, filename);

    return NULL;
}

static lil_value_t l_bpinvert(lil_t lil,
                              size_t argc,
                              lil_value_t *argv)
{
    btprnt_region *reg;
    int rc;
    sk_core *core;
    int xoff, yoff, w, h;

    SKLIL_ARITY_CHECK(lil, "bpinvert", argc, 5);
    core = lil_get_data(lil);

    rc = getreg(lil, core, &reg);

    if (rc) return NULL;

    xoff = lil_to_integer(argv[1]);
    yoff = lil_to_integer(argv[2]);
    w = lil_to_integer(argv[3]);
    h = lil_to_integer(argv[4]);

    btprnt_invert(reg, xoff, yoff, w, h);

    return NULL;
}

void lil_load_btprnt(lil_t lil)
{
    lil_register(lil, "bpnew", l_bpnew);
    lil_register(lil, "bpset", l_bpset);
    lil_register(lil, "bpget", l_bpget);
    lil_register(lil, "bpmrk", l_bpmrk);
    lil_register(lil, "bpclr", l_bpclr);
    lil_register(lil, "bpnxt", l_bpnxt);
    lil_register(lil, "bppbm", l_bppbm);
    lil_register(lil, "bprect", l_bprect);
    lil_register(lil, "bprectf", l_bprectf);
    lil_register(lil, "bptr", l_bptr);
    lil_register(lil, "bpfnt_default", l_bpfnt_default);
    lil_register(lil, "bptxtbox", l_bptxtbox);
    lil_register(lil, "bpchar", l_bpchar);
    lil_register(lil, "bpfill", l_bpfill);
    lil_register(lil, "bpline", l_bpline);
    lil_register(lil, "bpcirc", l_bpcirc);
    lil_register(lil, "bpoutline", l_bpoutline);
    lil_register(lil, "bproundrect", l_bproundrect);
    lil_register(lil, "bpverify", l_bpverify);
    lil_register(lil, "bppng", l_bppng);
    lil_register(lil, "bpinvert", l_bpinvert);

    /* TODO */
    lil_register(lil, "bpfnt_cherry", l_btphi);
    lil_register(lil, "bpcircf", l_btphi);
    lil_register(lil, "bphline", l_btphi);
    lil_register(lil, "bpvline", l_btphi);
    lil_register(lil, "bpcenterbox", l_btphi);
    lil_register(lil, "bpborder", l_btphi);
    lil_register(lil, "bpscrtxt", l_btphi);
}

static int lua_draw(lua_State *L)
{
    btprnt_region *reg;
    int x, y, s;
    reg = lua_touserdata(L, 1);
    x = lua_tointeger(L, 2);
    y = lua_tointeger(L, 3);
    s = lua_tointeger(L, 4);

    btprnt_region_draw(reg, x, y, s);
    return 0;
}

static const luaL_Reg btprnt_lib[] = {
    {"draw", lua_draw},
    {NULL, NULL}
};

int luaopen_btprnt(lua_State *L)
{
    luaL_newlib(L, btprnt_lib);
    return 1;
}
