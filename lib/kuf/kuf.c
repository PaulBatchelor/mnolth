#line 34 "kuf.org"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "kuf.h"
#line 167 "kuf.org"
static uint8_t set_bit(uint8_t quad,
                       uint8_t pos,
                       uint8_t bit);
#line 2848 "kuf.org"
static uint16_t correct_dquad(uint16_t s);
#line 2892 "kuf.org"
static uint16_t correct_cquad(uint16_t s);
#line 2925 "kuf.org"
static uint16_t correct_eastwall(uint16_t s);
#line 34 "kuf.org"
#line 116 "kuf.org"
int kuf_quad_check(uint8_t quad)
{
    quad &= 0xf;

    return
        quad != 0 &&
        quad != 6 &&
        quad != 9 &&
        quad != 0xf;
}
#line 139 "kuf.org"
uint32_t kuf_rand(uint32_t rng)
{
    rng = (1103515245 * rng + 12345) % 2147483648;
    return rng;
}
#line 156 "kuf.org"
uint32_t kuf_rng(uint32_t *rng)
{
    *rng = kuf_rand(*rng);
    return *rng;
}
#line 174 "kuf.org"
static uint8_t set_bit(uint8_t quad,
                       uint8_t pos,
                       uint8_t bit)
{
    if (bit) quad |= 1 << pos;
    else quad &= ~(1 << pos);

    return quad;
}

#line 197 "kuf.org"
uint8_t kuf_gen_quad(uint32_t *rng)
{
    uint8_t quad;

    quad = (kuf_rng(rng) >> 8) & 0xf;

    while (!kuf_quad_check(quad)) quad = (kuf_rng(rng) >> 8) & 0xf;

    return quad;
}
#line 221 "kuf.org"
uint8_t kuf_gen_tile(uint32_t *rng)
{
    return (kuf_rng(rng) >> 8) & 1;
}
#line 238 "kuf.org"
uint8_t kuf_gen_pair(uint32_t *rng)
{
    return (kuf_rng(rng) >> 8) & 0x3;
}
#line 259 "kuf.org"
uint8_t kuf_quad_side_set(uint8_t quad,
                          uint8_t side,
                          uint8_t pair)
{
    pair &= 0x3;

    switch (side) {
        case KUF_NORTH:
            quad = set_bit(quad, 0, pair & 1);
            quad = set_bit(quad, 1, (pair >> 1) & 1);
            break;
        case KUF_SOUTH:
            quad = set_bit(quad, 2, pair & 1);
            quad = set_bit(quad, 3, (pair >> 1) & 1);
            break;
        case KUF_EAST:
            quad = set_bit(quad, 1, pair & 1);
            quad = set_bit(quad, 3, (pair >> 1) & 1);
            break;
        case KUF_WEST:
            quad = set_bit(quad, 0, pair & 1);
            quad = set_bit(quad, 2, (pair >> 1) & 1);
            break;
    }

    return quad;
}
#line 290 "kuf.org"
uint8_t kuf_quad_side_get(uint8_t quad, uint8_t side)
{
    uint8_t s;

    s = 0;

    switch (side) {
        case KUF_NORTH:
            s = quad & 3;
            break;
        case KUF_SOUTH:
            s = (quad >> 2) & 3;
            break;
        case KUF_EAST:
            s = ((quad >> 1) & 1) | (((quad >> 3) & 1) << 1);
            break;
        case KUF_WEST:
            s = (quad & 1) | (((quad >> 2) & 1) << 1);
            break;
    }

    return s;
}
#line 340 "kuf.org"
uint8_t kuf_quad_tile_get(uint8_t quad, uint8_t corner)
{
    int pos;

    pos = 0;

    switch (corner) {
        case KUF_NORTHWEST:
            pos = 0;
            break;
        case KUF_NORTHEAST:
            pos = 1;
            break;
        case KUF_SOUTHWEST:
            pos = 2;
            break;
        case KUF_SOUTHEAST:
            pos = 3;
            break;
    }

    return (quad >> pos) & 1;
}
#line 367 "kuf.org"
uint8_t kuf_quad_tile_set(uint8_t quad,
                          uint8_t corner,
                          uint8_t tile)
{
    int pos;

    pos = 0;

    switch (corner) {
        case KUF_NORTHWEST:
            pos = 0;
            break;
        case KUF_NORTHEAST:
            pos = 1;
            break;
        case KUF_SOUTHWEST:
            pos = 2;
            break;
        case KUF_SOUTHEAST:
            pos = 3;
            break;
    }

    return set_bit(quad, pos, tile & 1);
}
#line 421 "kuf.org"
uint8_t kuf_square_quad_get(uint16_t s, uint8_t pos)
{
    uint8_t quad;

    quad = 0;

    switch (pos) {
#line 455 "kuf.org"
case KUF_QUAD_A:
    quad = s & 0xf;
    break;
case KUF_QUAD_B:
    quad = (s >> 4) & 0xf;
    break;
case KUF_QUAD_C:
    quad = (s >> 8) & 0xf;
    break;
case KUF_QUAD_D:
    quad = (s >> 12) & 0xf;
    break;
#line 421 "kuf.org"
#line 496 "kuf.org"
case KUF_QUAD_AB: {
    uint8_t tmp;

    /* east side of quad A */
    tmp = kuf_quad_side_get(s & 0xf, KUF_EAST);
    quad = kuf_quad_side_set(quad, KUF_WEST, tmp);

    /* west side of quad B */
    tmp = kuf_quad_side_get((s >> 4) & 0xf, KUF_WEST);
    quad = kuf_quad_side_set(quad, KUF_EAST, tmp);
    break;
}
#line 533 "kuf.org"
case KUF_QUAD_AC: {
    uint8_t tmp;

    /* south side of quad A */
    tmp = kuf_quad_side_get(s & 0xf, KUF_SOUTH);
    quad = kuf_quad_side_set(quad, KUF_NORTH, tmp);

    /* north side of quad C */
    tmp = kuf_quad_side_get((s >> 8) & 0xf, KUF_NORTH);
    quad = kuf_quad_side_set(quad, KUF_SOUTH, tmp);
    break;
}
#line 570 "kuf.org"
case KUF_QUAD_BD: {
    uint8_t tmp;

    /* south side of quad B */
    tmp = kuf_quad_side_get((s >> 4) & 0xf, KUF_SOUTH);
    quad = kuf_quad_side_set(quad, KUF_NORTH, tmp);

    /* north side of quad D */
    tmp = kuf_quad_side_get((s >> 12) & 0xf, KUF_NORTH);
    quad = kuf_quad_side_set(quad, KUF_SOUTH, tmp);
    break;
}
#line 607 "kuf.org"
case KUF_QUAD_CD: {
    uint8_t tmp;

    /* east side of quad C */
    tmp = kuf_quad_side_get((s >> 8) & 0xf, KUF_EAST);
    quad = kuf_quad_side_set(quad, KUF_WEST, tmp);

    /* west side of quad D */
    tmp = kuf_quad_side_get((s >> 12) & 0xf, KUF_WEST);
    quad = kuf_quad_side_set(quad, KUF_EAST, tmp);
    break;
}
#line 421 "kuf.org"
#line 646 "kuf.org"
case KUF_QUAD_ABCD: {
    uint8_t tmp;

    /* A(SE) -> NW */
    tmp = kuf_quad_tile_get(s & 0xf, KUF_SOUTHEAST);
    quad = kuf_quad_tile_set(quad, KUF_NORTHWEST, tmp);

    /* B(SW) -> NE */
    tmp = kuf_quad_tile_get(s >> 4 & 0xf, KUF_SOUTHWEST);
    quad = kuf_quad_tile_set(quad, KUF_NORTHEAST, tmp);

    /* C(NE) -> SW */
    tmp = kuf_quad_tile_get(s >> 8 & 0xf, KUF_NORTHEAST);
    quad = kuf_quad_tile_set(quad, KUF_SOUTHWEST, tmp);

    /* D(NW) -> SE */
    tmp = kuf_quad_tile_get(s >> 12 & 0xf, KUF_NORTHWEST);
    quad = kuf_quad_tile_set(quad, KUF_SOUTHEAST, tmp);

    break;
}
#line 431 "kuf.org"
    }

    return quad;
}

uint16_t kuf_square_quad_set(uint16_t s,
                             uint8_t pos,
                             uint8_t quad)
{
    switch (pos) {
#line 471 "kuf.org"
case KUF_QUAD_A:
    s = (s & ~0xf) | quad;
    break;
case KUF_QUAD_B:
    s = (s & ~(0xf << 4)) | (quad << 4);
    break;
case KUF_QUAD_C:
    s = (s & ~(0xf << 8)) | (quad << 8);
    break;
case KUF_QUAD_D:
    s = (s & ~(0xf << 12)) | (quad << 12);
    break;
#line 431 "kuf.org"
#line 512 "kuf.org"
case KUF_QUAD_AB: {
    uint8_t tmp;

    tmp = kuf_quad_side_set(s & 0xf,
                            KUF_EAST,
                            kuf_quad_side_get(quad, KUF_WEST));
    s = (s & ~0xf) | tmp;

    tmp = kuf_quad_side_set((s >> 4) & 0xf,
                            KUF_WEST,
                            kuf_quad_side_get(quad, KUF_EAST));
    s = (s & ~(0xf << 4)) | (tmp << 4);
    break;
}
#line 549 "kuf.org"
case KUF_QUAD_AC: {
    uint8_t tmp;

    tmp = kuf_quad_side_set(s & 0xf,
                            KUF_SOUTH,
                            kuf_quad_side_get(quad, KUF_NORTH));
    s = (s & ~0xf) | tmp;

    tmp = kuf_quad_side_set((s >> 8) & 0xf,
                            KUF_NORTH,
                            kuf_quad_side_get(quad, KUF_SOUTH));
    s = (s & ~(0xf << 8)) | (tmp << 8);
    break;
}
#line 586 "kuf.org"
case KUF_QUAD_BD: {
    uint8_t tmp;

    tmp = kuf_quad_side_set((s >> 4) & 0xf,
                            KUF_SOUTH,
                            kuf_quad_side_get(quad, KUF_NORTH));
    s = (s & ~(0xf << 4)) | (tmp << 4);

    tmp = kuf_quad_side_set((s >> 12) & 0xf,
                            KUF_NORTH,
                            kuf_quad_side_get(quad, KUF_SOUTH));
    s = (s & ~(0xf << 12)) | (tmp << 12);
    break;
}
#line 623 "kuf.org"
case KUF_QUAD_CD: {
    uint8_t tmp;

    tmp = kuf_quad_side_set((s >> 8) & 0xf,
                            KUF_EAST,
                            kuf_quad_side_get(quad, KUF_WEST));
    s = (s & ~(0xf << 8)) | (tmp << 8);

    tmp = kuf_quad_side_set((s >> 12) & 0xf,
                            KUF_WEST,
                            kuf_quad_side_get(quad, KUF_EAST));
    s = (s & ~(0xf << 12)) | (tmp << 12);
    break;
}
#line 431 "kuf.org"
#line 671 "kuf.org"
case KUF_QUAD_ABCD: {
    uint8_t tmp;

    /* Q(NW) -> A(SE) */
    tmp = kuf_quad_tile_set(s & 0xf,
                            KUF_SOUTHEAST,
                            kuf_quad_tile_get(quad, KUF_NORTHWEST));
    s = (s & ~0xf) | tmp;

    /* Q(NE) -> B(SW) */
    tmp = kuf_quad_tile_set((s >> 4) & 0xf,
                            KUF_SOUTHWEST,
                            kuf_quad_tile_get(quad, KUF_NORTHEAST));

    /* Q(SW) -> C(NE) */
    s = (s & ~(0xf << 4)) | (tmp << 4);

    tmp = kuf_quad_tile_set((s >> 8) & 0xf,
                            KUF_NORTHEAST,
                            kuf_quad_tile_get(quad, KUF_SOUTHWEST));
    s = (s & ~(0xf << 8)) | (tmp << 8);

    /* Q(SE) -> D(NW) */
    tmp = kuf_quad_tile_set((s >> 12) & 0xf,
                            KUF_NORTHWEST,
                            kuf_quad_tile_get(quad, KUF_SOUTHEAST));
    s = (s & ~(0xf << 12)) | (tmp << 12);

    break;
}
#line 444 "kuf.org"
    }
    return s;
}
#line 729 "kuf.org"
uint16_t kuf_gen_square(uint32_t *rng)
{
    uint16_t sq;
    sq = 0;
#line 753 "kuf.org"
sq = kuf_square_quad_set(sq, KUF_QUAD_A, kuf_gen_quad(rng));
#line 729 "kuf.org"
#line 768 "kuf.org"
{
    uint8_t p_s;
    uint8_t p_n;
    uint8_t ac;

    p_s = kuf_gen_pair(rng);
    p_n =
        kuf_quad_side_get(
            kuf_square_quad_get(sq, KUF_QUAD_A),
            KUF_SOUTH);

    ac = 0;
    ac = kuf_quad_side_set(ac, KUF_NORTH, p_n);
    ac = kuf_quad_side_set(ac, KUF_SOUTH, p_s);

    if (!kuf_quad_check(ac)) {
        int i;

        for (i = 0; i < 4; i++) {
            ac = kuf_quad_side_set(ac, KUF_SOUTH, (p_s + i) % 4);

            if (kuf_quad_check(ac)) break;
        }
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_AC, ac);
}
#line 729 "kuf.org"
#line 802 "kuf.org"
{
    uint8_t p_s;
    uint8_t p_n;
    uint8_t c;

    p_s = kuf_gen_pair(rng);
    p_n =
        kuf_quad_side_get(
            kuf_square_quad_get(sq, KUF_QUAD_AC),
            KUF_SOUTH);

    c = 0;
    c = kuf_quad_side_set(c, KUF_NORTH, p_n);
    c = kuf_quad_side_set(c, KUF_SOUTH, p_s);

    if (!kuf_quad_check(c)) {
        int i;

        for (i = 0; i < 4; i++) {
            c = kuf_quad_side_set(c, KUF_SOUTH, (p_s + i) % 4);

            if (kuf_quad_check(c)) break;
        }
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_C, c);
}
#line 729 "kuf.org"
#line 839 "kuf.org"
{
    uint8_t abcd;
    uint8_t p_e;
    abcd = 0;

    abcd = kuf_quad_tile_set(abcd,
                             KUF_NORTHWEST,
                             kuf_quad_tile_get(
                                 kuf_square_quad_get(sq, KUF_QUAD_A),
                                 KUF_SOUTHEAST));

    abcd = kuf_quad_tile_set(abcd,
                             KUF_SOUTHWEST,
                             kuf_quad_tile_get(
                                 kuf_square_quad_get(sq, KUF_QUAD_C),
                                 KUF_NORTHEAST));

    p_e = kuf_gen_pair(rng);

    abcd = kuf_quad_side_set(abcd, KUF_EAST, p_e);

    if (!kuf_quad_check(abcd)) {
        int i;
        for (i = 0; i < 4; i++) {
            abcd = kuf_quad_side_set(abcd, KUF_EAST, (p_e + i) % 4);
            if (kuf_quad_check(abcd)) break;
        }
    }


    sq = kuf_square_quad_set(sq, KUF_QUAD_ABCD, abcd);
}
#line 729 "kuf.org"
#line 880 "kuf.org"
{
    uint8_t bd;
    uint8_t abcd;
    uint8_t p_e;

    abcd = kuf_square_quad_get(sq, KUF_QUAD_ABCD);
    bd = 0;
    bd = kuf_quad_side_set(bd,
                           KUF_WEST,
                           kuf_quad_side_get(abcd, KUF_EAST));

    p_e = kuf_gen_pair(rng);

    bd = kuf_quad_side_set(bd, KUF_EAST, p_e);

    if (!kuf_quad_check(bd)) {
        int i;
        for (i = 0; i < 4; i++) {
            bd = kuf_quad_side_set(bd, KUF_EAST, (p_e + i) % 4);
            if (kuf_quad_check(bd)) break;
        }
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_BD, bd);
}
#line 729 "kuf.org"
#line 920 "kuf.org"
{
    uint8_t ab;
    uint8_t cd;
    uint8_t b;
    uint8_t d;
    uint8_t t;

    ab = kuf_square_quad_get(sq, KUF_QUAD_AB);
    sq = kuf_square_quad_set(sq, KUF_QUAD_AB, ab);

    t = kuf_gen_tile(rng);
    ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t);

    if (!kuf_quad_check(ab)) {
        ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t ? 0 : 1);
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_AB, ab);

    b = kuf_square_quad_get(sq, KUF_QUAD_B);
    t = kuf_gen_tile(rng);
    b = kuf_quad_tile_set(b, KUF_NORTHEAST, t);

    if (!kuf_quad_check(b)) {
        b = kuf_quad_tile_set(b, KUF_NORTHEAST, t ? 0 : 1);
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_B, b);

    cd = kuf_square_quad_get(sq, KUF_QUAD_CD);
    t = kuf_gen_tile(rng);
    cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(cd)) {
        cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t ? 0 : 1);
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_CD, cd);

    d = kuf_square_quad_get(sq, KUF_QUAD_D);
    t = kuf_gen_tile(rng);
    d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(d)) {
        d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t ? 0 : 1);
    }

    sq = kuf_square_quad_set(sq, KUF_QUAD_D, d);
}
#line 739 "kuf.org"
    return sq;
}
#line 986 "kuf.org"
uint16_t kuf_glue_square(uint8_t a,
                         uint8_t b,
                         uint8_t c,
                         uint8_t d)
{
    uint16_t s;
    s = 0;

    s = kuf_square_quad_set(s, KUF_QUAD_A, a);
    s = kuf_square_quad_set(s, KUF_QUAD_B, b);
    s = kuf_square_quad_set(s, KUF_QUAD_C, c);
    s = kuf_square_quad_set(s, KUF_QUAD_D, d);

    return s;
}
#line 1017 "kuf.org"
void kuf_break_square(uint16_t s,
                      uint8_t *a,
                      uint8_t *b,
                      uint8_t *c,
                      uint8_t *d)
{
    *a = kuf_square_quad_get(s, KUF_QUAD_A);
    *b = kuf_square_quad_get(s, KUF_QUAD_B);
    *c = kuf_square_quad_get(s, KUF_QUAD_C);
    *d = kuf_square_quad_get(s, KUF_QUAD_D);
}
#line 1049 "kuf.org"
void kuf_gen_block(uint32_t *rng,
                   uint16_t *pw,
                   uint16_t *px,
                   uint16_t *py,
                   uint16_t *pz)
{
    uint16_t w, x, y, z;
    uint16_t wy;
    uint16_t wxyz;
    uint16_t xz;
    uint16_t wx;
    uint16_t yz;

    w = x = y = z = 0;

#line 1844 "kuf.org"
w = kuf_gen_square(rng);
#line 1049 "kuf.org"
#line 1853 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(w, KUF_QUAD_C);
    b = kuf_square_quad_get(w, KUF_QUAD_D);

    c = d = 0;

    kuf_solve_wall_south(rng, a, b, &c, &d);

    wy = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1872 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(wy, KUF_QUAD_C);
    b = kuf_square_quad_get(wy, KUF_QUAD_D);

    c = d = 0;

    kuf_solve_wall_south(rng, a, b, &c, &d);

    y = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1893 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(w, KUF_QUAD_D);
    c = kuf_square_quad_get(y, KUF_QUAD_B);

    b = d = 0;

    kuf_solve_wall_east(rng, a, c, &b, &d);

    wxyz = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1913 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(wxyz, KUF_QUAD_B);
    c = kuf_square_quad_get(wxyz, KUF_QUAD_D);

    b = d = 0;

    kuf_solve_wall_east(rng, a, c, &b, &d);

    xz = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1934 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(w, KUF_QUAD_B);
    c = kuf_square_quad_get(w, KUF_QUAD_D);
    d = kuf_square_quad_get(wxyz, KUF_QUAD_B);

    b = kuf_solve_quad_northeast(rng, a, c, d);

    wx = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1954 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(wx, KUF_QUAD_B);
    c = kuf_square_quad_get(wx, KUF_QUAD_D);
    d = kuf_square_quad_get(xz, KUF_QUAD_B);

    b = kuf_solve_quad_northeast(rng, a, c, d);

    x = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1974 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(y, KUF_QUAD_B);
    c = kuf_square_quad_get(y, KUF_QUAD_D);
    b = kuf_square_quad_get(xz, KUF_QUAD_C);

    d = kuf_solve_quad_southeast(rng, a, b, c);

    yz = kuf_glue_square(a, b, c, d);
}
#line 1049 "kuf.org"
#line 1994 "kuf.org"
{
    uint8_t a, b, c, d;

    a = kuf_square_quad_get(yz, KUF_QUAD_B);
    c = kuf_square_quad_get(yz, KUF_QUAD_D);
    b = kuf_square_quad_get(xz, KUF_QUAD_D);

    d = kuf_solve_quad_southeast(rng, a, b, c);

    z = kuf_glue_square(a, b, c, d);
}
#line 1073 "kuf.org"

    *pw = w;
    *px = x;
    *py = y;
    *pz = z;
}
#line 1135 "kuf.org"
void kuf_solve_wall_east(uint32_t *rng,
                         uint8_t a, uint8_t c,
                         uint8_t *pb, uint8_t *pd)
{
    uint8_t b;
    uint8_t d;
    uint8_t abcd;
    uint8_t bd;
    uint8_t ab;
    uint8_t cd;

    b = 0;
    d = 0;

#line 1172 "kuf.org"
{
    uint8_t t;
    abcd = 0;

    t = kuf_quad_tile_get(a, KUF_SOUTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHWEST, t);
    t = kuf_quad_tile_get(c, KUF_NORTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_SOUTHWEST, t);

    t = kuf_gen_pair(rng);

    abcd = kuf_quad_side_set(abcd, KUF_EAST, t);

    if (!kuf_quad_check(abcd)) {
        int i;
        for (i = 0; i < 4; i++) {
            abcd = kuf_quad_side_set(abcd, KUF_EAST, (t + i) % 4);
            if (kuf_quad_check(abcd)) break;
        }
    }
}
#line 1135 "kuf.org"
#line 1201 "kuf.org"
{
    uint8_t t;
    bd = 0;

    t = kuf_quad_tile_get(abcd, KUF_NORTHEAST);
    bd = kuf_quad_tile_set(bd, KUF_NORTHWEST, t);

    t = kuf_quad_tile_get(abcd, KUF_SOUTHEAST);
    bd = kuf_quad_tile_set(bd, KUF_SOUTHWEST, t);

    t = kuf_gen_pair(rng);
    bd = kuf_quad_side_set(bd, KUF_EAST, t);

    if (!kuf_quad_check(bd)) {
        int i;
        for (i = 0; i < 4; i++) {
            bd = kuf_quad_side_set(bd, KUF_EAST, (t + i) % 4);
            if (kuf_quad_check(bd)) break;
        }
    }
}
#line 1135 "kuf.org"
#line 1231 "kuf.org"
{
    uint8_t t;

    ab = 0;

    t = kuf_quad_side_get(a, KUF_EAST);
    ab = kuf_quad_side_set(ab, KUF_WEST, t);

    t = kuf_quad_tile_get(abcd, KUF_NORTHEAST);
    ab = kuf_quad_tile_set(ab, KUF_SOUTHEAST, t);

    t = kuf_gen_tile(rng);
    ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t);

    if (!kuf_quad_check(ab)) {
        ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1135 "kuf.org"
#line 1258 "kuf.org"
{
    uint8_t t;

    b = 0;

    t = kuf_quad_side_get(ab, KUF_EAST);
    b = kuf_quad_side_set(b, KUF_WEST, t);

    t = kuf_quad_tile_get(bd, KUF_NORTHEAST);
    b = kuf_quad_tile_set(b, KUF_SOUTHEAST, t);

    t = kuf_gen_tile(rng);
    b = kuf_quad_tile_set(b, KUF_NORTHEAST, t);

    if (!kuf_quad_check(b)) {
        b = kuf_quad_tile_set(b, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1135 "kuf.org"
#line 1285 "kuf.org"
{
    uint8_t t;

    cd = 0;

    t = kuf_quad_side_get(abcd, KUF_SOUTH);
    cd = kuf_quad_side_set(cd, KUF_NORTH, t);

    t = kuf_quad_tile_get(c, KUF_SOUTHEAST);
    cd = kuf_quad_tile_set(cd, KUF_SOUTHWEST, t);

    t = kuf_gen_tile(rng);
    cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(cd)) {
        cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1135 "kuf.org"
#line 1312 "kuf.org"
{
    uint8_t t;

    d = 0;

    t = kuf_quad_side_get(cd, KUF_EAST);
    d = kuf_quad_side_set(d, KUF_WEST, t);

    t = kuf_quad_tile_get(bd, KUF_SOUTHEAST);
    d = kuf_quad_tile_set(d, KUF_NORTHEAST, t);

    t = kuf_gen_tile(rng);
    d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(d)) {
        d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1155 "kuf.org"

    *pb = b;
    *pd = d;
}
#line 1348 "kuf.org"
void kuf_solve_wall_south(uint32_t *rng,
                          uint8_t a, uint8_t b,
                          uint8_t *pc, uint8_t *pd)
{
    uint8_t c, d;
    uint8_t abcd;
    uint8_t cd;
    uint8_t ac;
    uint8_t bd;

    c = d = 0;

#line 1379 "kuf.org"
{
    uint8_t t;
    abcd = 0;

    t = kuf_quad_tile_get(a, KUF_SOUTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHWEST, t);
    t = kuf_quad_tile_get(b, KUF_SOUTHWEST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHEAST, t);

    t = kuf_gen_pair(rng);

    abcd = kuf_quad_side_set(abcd, KUF_SOUTH, t);

    if (!kuf_quad_check(abcd)) {
        int i;
        for (i = 0; i < 4; i++) {
            abcd = kuf_quad_side_set(abcd, KUF_SOUTH, (t + i) % 4);
            if (kuf_quad_check(abcd)) break;
        }
    }
}
#line 1348 "kuf.org"
#line 1409 "kuf.org"
{
    uint8_t t;
    cd = 0;

    t = kuf_quad_tile_get(abcd, KUF_SOUTHWEST);
    cd = kuf_quad_tile_set(cd, KUF_NORTHWEST, t);

    t = kuf_quad_tile_get(abcd, KUF_SOUTHEAST);
    cd = kuf_quad_tile_set(cd, KUF_NORTHEAST, t);

    t = kuf_gen_pair(rng);
    cd = kuf_quad_side_set(cd, KUF_SOUTH, t);

    if (!kuf_quad_check(cd)) {
        int i;
        for (i = 0; i < 4; i++) {
            cd = kuf_quad_side_set(cd, KUF_SOUTH, (t + i) % 4);
            if (kuf_quad_check(cd)) break;
        }
    }
}
#line 1348 "kuf.org"
#line 1439 "kuf.org"
{
    uint8_t t;

    ac = 0;

    t = kuf_quad_side_get(abcd, KUF_WEST);
    ac = kuf_quad_side_set(ac, KUF_EAST, t);

    t = kuf_quad_tile_get(a, KUF_SOUTHWEST);
    ac = kuf_quad_tile_set(ac, KUF_NORTHWEST, t);

    t = kuf_gen_tile(rng);
    ac = kuf_quad_tile_set(ac, KUF_SOUTHWEST, t);

    if (!kuf_quad_check(ac)) {
        ac = kuf_quad_tile_set(ac, KUF_SOUTHWEST, t ? 0 : 1);
    }
}
#line 1348 "kuf.org"
#line 1466 "kuf.org"
{
    uint8_t t;

    c = 0;

    t = kuf_quad_side_get(ac, KUF_SOUTH);
    c = kuf_quad_side_set(c, KUF_NORTH, t);

    t = kuf_quad_tile_get(cd, KUF_SOUTHWEST);
    c = kuf_quad_tile_set(c, KUF_SOUTHEAST, t);

    t = kuf_gen_tile(rng);
    c = kuf_quad_tile_set(c, KUF_SOUTHWEST, t);

    if (!kuf_quad_check(c)) {
        c = kuf_quad_tile_set(c, KUF_SOUTHWEST, t ? 0 : 1);
    }
}
#line 1348 "kuf.org"
#line 1493 "kuf.org"
{
    uint8_t t;

    bd = 0;

    t = kuf_quad_side_get(abcd, KUF_EAST);
    bd = kuf_quad_side_set(bd, KUF_WEST, t);

    t = kuf_quad_tile_get(b, KUF_SOUTHEAST);
    bd = kuf_quad_tile_set(bd, KUF_NORTHEAST, t);

    t = kuf_gen_tile(rng);
    bd = kuf_quad_tile_set(bd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(bd)) {
        bd = kuf_quad_tile_set(bd, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1348 "kuf.org"
#line 1520 "kuf.org"
{
    uint8_t t;

    d = 0;

    t = kuf_quad_side_get(bd, KUF_SOUTH);
    d = kuf_quad_side_set(d, KUF_NORTH, t);

    t = kuf_quad_tile_get(cd, KUF_SOUTHEAST);
    d = kuf_quad_tile_set(d, KUF_SOUTHWEST, t);

    t = kuf_gen_tile(rng);
    d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(d)) {
        d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1366 "kuf.org"

    *pc = c;
    *pd = d;
}
#line 1555 "kuf.org"
uint8_t kuf_solve_quad_northeast(uint32_t *rng,
                                 uint8_t a,
                                 uint8_t c,
                                 uint8_t d)
{
    uint8_t b;
    uint8_t abcd;
    uint8_t bd;
    uint8_t ab;
    b = 0;
#line 1585 "kuf.org"
{
    uint8_t t;

    abcd = 0;

    t = kuf_quad_tile_get(a, KUF_SOUTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHWEST, t);

    t = kuf_quad_tile_get(c, KUF_NORTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_SOUTHWEST, t);

    t = kuf_quad_tile_get(d, KUF_NORTHWEST);
    abcd = kuf_quad_tile_set(abcd, KUF_SOUTHEAST, t);

    t = kuf_gen_tile(rng);

    abcd = kuf_quad_tile_set(abcd, KUF_NORTHEAST, t);

    if (!kuf_quad_check(abcd)) {
        abcd = kuf_quad_tile_set(abcd, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1555 "kuf.org"
#line 1616 "kuf.org"
{
    uint8_t t;

    bd = 0;

    t = kuf_quad_side_get(abcd, KUF_EAST);
    bd = kuf_quad_side_set(bd, KUF_WEST, t);

    t = kuf_quad_tile_get(d, KUF_NORTHEAST);
    bd = kuf_quad_tile_set(bd, KUF_SOUTHEAST, t);

    t = kuf_gen_tile(rng);

    bd = kuf_quad_tile_set(bd, KUF_NORTHEAST, t);

    if (!kuf_quad_check(bd)) {
        bd = kuf_quad_tile_set(bd, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1555 "kuf.org"
#line 1644 "kuf.org"
{
    uint8_t t;

    ab = 0;

    t = kuf_quad_side_get(abcd, KUF_NORTH);
    ab = kuf_quad_side_set(ab, KUF_SOUTH, t);

    t = kuf_quad_tile_get(a, KUF_NORTHEAST);
    ab = kuf_quad_tile_set(ab, KUF_NORTHWEST, t);

    t = kuf_gen_tile(rng);

    ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t);

    if (!kuf_quad_check(ab)) {
        ab = kuf_quad_tile_set(ab, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1555 "kuf.org"
#line 1672 "kuf.org"
{
    uint8_t t;

    b = 0;

    t = kuf_quad_side_get(bd, KUF_NORTH);
    b = kuf_quad_side_set(b, KUF_SOUTH, t);

    t = kuf_quad_tile_get(ab, KUF_NORTHEAST);
    b = kuf_quad_tile_set(b, KUF_NORTHWEST, t);

    t = kuf_gen_tile(rng);

    b = kuf_quad_tile_set(b, KUF_NORTHEAST, t);

    if (!kuf_quad_check(b)) {
        b = kuf_quad_tile_set(b, KUF_NORTHEAST, t ? 0 : 1);
    }
}
#line 1569 "kuf.org"
    return b;
}
#line 1703 "kuf.org"
uint8_t kuf_solve_quad_southeast(uint32_t *rng,
                                 uint8_t a,
                                 uint8_t b,
                                 uint8_t c)
{
    uint8_t d;
    uint8_t abcd;
    uint8_t bd;
    uint8_t cd;

    d = 0;

#line 1735 "kuf.org"
{
    uint8_t t;

    abcd = 0;

    t = kuf_quad_tile_get(a, KUF_SOUTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHWEST, t);

    t = kuf_quad_tile_get(b, KUF_SOUTHWEST);
    abcd = kuf_quad_tile_set(abcd, KUF_NORTHEAST, t);

    t = kuf_quad_tile_get(c, KUF_NORTHEAST);
    abcd = kuf_quad_tile_set(abcd, KUF_SOUTHWEST, t);

    t = kuf_gen_tile(rng);
    abcd = kuf_quad_tile_set(abcd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(abcd)) {
        abcd = kuf_quad_tile_set(abcd, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1703 "kuf.org"
#line 1765 "kuf.org"
{
    uint8_t t;

    bd = 0;

    t = kuf_quad_side_get(abcd, KUF_EAST);
    bd = kuf_quad_side_set(bd, KUF_WEST, t);

    t = kuf_quad_tile_get(b, KUF_SOUTHEAST);
    bd = kuf_quad_tile_set(bd, KUF_NORTHEAST, t);

    t = kuf_gen_tile(rng);
    bd = kuf_quad_tile_set(bd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(bd)) {
        bd = kuf_quad_tile_set(bd, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1703 "kuf.org"
#line 1792 "kuf.org"
{
    uint8_t t;

    cd = 0;

    t = kuf_quad_side_get(abcd, KUF_SOUTH);
    cd = kuf_quad_side_set(cd, KUF_NORTH, t);

    t = kuf_quad_tile_get(c, KUF_SOUTHEAST);
    cd = kuf_quad_tile_set(cd, KUF_SOUTHWEST, t);

    t = kuf_gen_tile(rng);
    cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(cd)) {
        cd = kuf_quad_tile_set(cd, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1703 "kuf.org"
#line 1819 "kuf.org"
{
    uint8_t t;

    d = 0;

    t = kuf_quad_side_get(bd, KUF_SOUTH);
    d = kuf_quad_side_set(d, KUF_NORTH, t);

    t = kuf_quad_tile_get(cd, KUF_SOUTHEAST);
    d = kuf_quad_tile_set(d, KUF_SOUTHWEST, t);

    t = kuf_gen_tile(rng);
    d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t);

    if (!kuf_quad_check(d)) {
        d = kuf_quad_tile_set(d, KUF_SOUTHEAST, t ? 0 : 1);
    }
}
#line 1719 "kuf.org"

    return d;
}
#line 2016 "kuf.org"
void kuf_print_tile(uint8_t tile)
{
    printf("%s", tile ? "#" : ".");
}
#line 2029 "kuf.org"
void kuf_print_quad(uint8_t quad)
{
    printf("+--+\n");
    printf("|");
    kuf_print_tile(quad & 1);
    kuf_print_tile((quad & 2) >> 1);
    printf("|\n|");
    kuf_print_tile((quad & 4) >> 2);
    kuf_print_tile((quad & 8) >> 3);
    printf("|\n");
    printf("+--+\n");
}
#line 2051 "kuf.org"
void kuf_print_side(uint8_t side)
{
    kuf_print_tile(side & 1);
    kuf_print_tile(side & 2);
}
#line 2065 "kuf.org"
void kuf_print_square(uint16_t square)
{
    printf("+----+\n");
    printf("|");
    kuf_print_side(kuf_quad_side_get(square & 0xf, KUF_NORTH));
    kuf_print_side(kuf_quad_side_get((square >> 4) & 0xf, KUF_NORTH));
    printf("|\n");

    printf("|");
    kuf_print_side(kuf_quad_side_get(square & 0xf, KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get((square >> 4) & 0xf, KUF_SOUTH));
    printf("|\n");

    printf("|");
    kuf_print_side(kuf_quad_side_get((square >> 8) & 0xf, KUF_NORTH));
    kuf_print_side(kuf_quad_side_get((square >> 12) & 0xf, KUF_NORTH));
    printf("|\n");

    printf("|");
    kuf_print_side(kuf_quad_side_get((square >> 8) & 0xf, KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get((square >> 12) & 0xf, KUF_SOUTH));
    printf("|\n");

    printf("+----+\n");
}
#line 2099 "kuf.org"
void kuf_print_block(uint16_t w, uint16_t x, uint16_t y, uint16_t z)
{
    uint8_t s_a[4];
    uint8_t s_b[4];
    printf("+--------+\n");
    /* print w + x squares */

    s_a[0] = kuf_square_quad_get(w, KUF_QUAD_A);
    s_a[1] = kuf_square_quad_get(w, KUF_QUAD_B);
    s_a[2] = kuf_square_quad_get(w, KUF_QUAD_C);
    s_a[3] = kuf_square_quad_get(w, KUF_QUAD_D);

    s_b[0] = kuf_square_quad_get(x, KUF_QUAD_A);
    s_b[1] = kuf_square_quad_get(x, KUF_QUAD_B);
    s_b[2] = kuf_square_quad_get(x, KUF_QUAD_C);
    s_b[3] = kuf_square_quad_get(x, KUF_QUAD_D);


    /* row 1: W(A,N), W(B, N), X(A, N), X(B, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[0], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_a[1], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[0], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[1], KUF_NORTH));
    printf("|\n");

    /* row 2: W(A,S), W(B, S), X(A, S), X(B, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[0], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_a[1], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[0], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[1], KUF_SOUTH));
    printf("|\n");

    /* row 3: W(C,N), W(D, N), X(C, N), X(D, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[2], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_a[3], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[2], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[3], KUF_NORTH));
    printf("|\n");

    /* row 4: W(C,S), W(D, S), X(C, S), X(D, S) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[2], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_a[3], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[2], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[3], KUF_SOUTH));
    printf("|\n");

    /* print y + z squares */

    s_a[0] = kuf_square_quad_get(y, KUF_QUAD_A);
    s_a[1] = kuf_square_quad_get(y, KUF_QUAD_B);
    s_a[2] = kuf_square_quad_get(y, KUF_QUAD_C);
    s_a[3] = kuf_square_quad_get(y, KUF_QUAD_D);

    s_b[0] = kuf_square_quad_get(z, KUF_QUAD_A);
    s_b[1] = kuf_square_quad_get(z, KUF_QUAD_B);
    s_b[2] = kuf_square_quad_get(z, KUF_QUAD_C);
    s_b[3] = kuf_square_quad_get(z, KUF_QUAD_D);

    /* row 5: Y(A,N), Y(B, N), Z(A, N), Z(B, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[0], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_a[1], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[0], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[1], KUF_NORTH));
    printf("|\n");

    /* row 6: Y(A,S), Y(B, S), Z(A, S), Z(B, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[0], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_a[1], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[0], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[1], KUF_SOUTH));
    printf("|\n");

    /* row 7: Y(C,N), Y(D, N), Z(C, N), Z(D, N) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[2], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_a[3], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[2], KUF_NORTH));
    kuf_print_side(kuf_quad_side_get(s_b[3], KUF_NORTH));
    printf("|\n");

    /* row 8: Y(C,S), Y(D, S), Z(C, S), Z(D, S) */
    printf("|");
    kuf_print_side(kuf_quad_side_get(s_a[2], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_a[3], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[2], KUF_SOUTH));
    kuf_print_side(kuf_quad_side_get(s_b[3], KUF_SOUTH));
    printf("|\n");

    printf("+--------+\n");
}
#line 2208 "kuf.org"
static void print_side(FILE *fp, uint8_t side)
{
    fputc((side & 1) ? '1' : '0', fp);
    fputc(' ', fp);
    fputc((side & 2) ? '1' : '0', fp);
    fputc(' ', fp);
}

void kuf_write_block_pbm(const char *filename,
                         uint16_t w, uint16_t x,
                         uint16_t y, uint16_t z)
{
    FILE *fp;
    uint8_t s_a[4];
    uint8_t s_b[4];

    fp = fopen(filename, "w");

    fprintf(fp, "P1\n# kuf block\n8 8\n");
    /* print w + x squares */

    s_a[0] = kuf_square_quad_get(w, KUF_QUAD_A);
    s_a[1] = kuf_square_quad_get(w, KUF_QUAD_B);
    s_a[2] = kuf_square_quad_get(w, KUF_QUAD_C);
    s_a[3] = kuf_square_quad_get(w, KUF_QUAD_D);

    s_b[0] = kuf_square_quad_get(x, KUF_QUAD_A);
    s_b[1] = kuf_square_quad_get(x, KUF_QUAD_B);
    s_b[2] = kuf_square_quad_get(x, KUF_QUAD_C);
    s_b[3] = kuf_square_quad_get(x, KUF_QUAD_D);

    /* row 1: W(A,N), W(B, N), X(A, N), X(B, N) */
    print_side(fp, kuf_quad_side_get(s_a[0], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_a[1], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[0], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[1], KUF_NORTH));
    fprintf(fp, "\n");

    /* row 2: W(A,S), W(B, S), X(A, S), X(B, N) */
    print_side(fp, kuf_quad_side_get(s_a[0], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_a[1], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[0], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[1], KUF_SOUTH));
    fprintf(fp, "\n");

    /* row 3: W(C,N), W(D, N), X(C, N), X(D, N) */
    print_side(fp, kuf_quad_side_get(s_a[2], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_a[3], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[2], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[3], KUF_NORTH));
    fprintf(fp, "\n");

    /* row 4: W(C,S), W(D, S), X(C, S), X(D, S) */
    print_side(fp, kuf_quad_side_get(s_a[2], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_a[3], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[2], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[3], KUF_SOUTH));
    fprintf(fp, "\n");

    /* print y + z squares */

    s_a[0] = kuf_square_quad_get(y, KUF_QUAD_A);
    s_a[1] = kuf_square_quad_get(y, KUF_QUAD_B);
    s_a[2] = kuf_square_quad_get(y, KUF_QUAD_C);
    s_a[3] = kuf_square_quad_get(y, KUF_QUAD_D);

    s_b[0] = kuf_square_quad_get(z, KUF_QUAD_A);
    s_b[1] = kuf_square_quad_get(z, KUF_QUAD_B);
    s_b[2] = kuf_square_quad_get(z, KUF_QUAD_C);
    s_b[3] = kuf_square_quad_get(z, KUF_QUAD_D);

    /* row 5: Y(A,N), Y(B, N), Z(A, N), Z(B, N) */
    print_side(fp, kuf_quad_side_get(s_a[0], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_a[1], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[0], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[1], KUF_NORTH));
    fprintf(fp, "\n");

    /* row 6: Y(A,S), Y(B, S), Z(A, S), Z(B, N) */
    print_side(fp, kuf_quad_side_get(s_a[0], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_a[1], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[0], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[1], KUF_SOUTH));
    fprintf(fp, "\n");

    /* row 7: Y(C,N), Y(D, N), Z(C, N), Z(D, N) */
    print_side(fp, kuf_quad_side_get(s_a[2], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_a[3], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[2], KUF_NORTH));
    print_side(fp, kuf_quad_side_get(s_b[3], KUF_NORTH));
    fprintf(fp,"\n");

    /* row 8: Y(C,S), Y(D, S), Z(C, S), Z(D, S) */
    print_side(fp, kuf_quad_side_get(s_a[2], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_a[3], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[2], KUF_SOUTH));
    print_side(fp, kuf_quad_side_get(s_b[3], KUF_SOUTH));
    fprintf(fp, "\n");

    fclose(fp);
}
#line 2332 "kuf.org"
static void write_row(FILE *fp, uint8_t w)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (i > 0) fprintf(fp, " ");
        fprintf(fp, "%d", (w >> i) & 1);
    }
}

void kuf_write_pbm(const char *filename,
                   int n, int m,
                   uint16_t *squares)
{
    int x, y, r;
    FILE *fp;

    fp = fopen(filename, "w");

    fprintf(fp, "P1\n# kuf block\n%d %d\n", n*4, m*4);

    for (y = 0; y < m; y++) {
        for (r = 0; r < 4; r++) {
            for (x = 0; x < n; x++) {
                uint8_t w;
                uint16_t s;

                w = 0;
                s = squares[y * n + x];

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

                write_row(fp, w);

                if (x < (n - 1)) fprintf(fp, " ");
            }
            fprintf(fp, "\n");
        }
    }

    fclose(fp);
}
#line 2544 "kuf.org"
void kuf_set_block(int width,
                   uint16_t *squares,
                   int xpos, int ypos,
                   uint16_t w, uint16_t x,
                   uint16_t y, uint16_t z)
{
    int start;

    start = ypos * width + xpos;

    squares[start] = w;
    squares[start + 1] = x;

    start += width;
    squares[start] = y;
    squares[start + 1] = z;
}
#line 2573 "kuf.org"
void kuf_set_square(int width,
                    uint16_t *squares,
                    int xpos, int ypos,
                    uint16_t s)
{
    squares[ypos * width + xpos] = s;
}
#line 2594 "kuf.org"
uint16_t kuf_correct_square(uint16_t s)
{
    uint8_t tmp;

#line 2622 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_B);
while (!kuf_quad_check(tmp)) {
    tmp++;
    tmp &= 0xf;
}
s = kuf_square_quad_set(s, KUF_QUAD_B, tmp);
#line 2594 "kuf.org"
#line 2636 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_BD);
tmp = kuf_correct_side(tmp, KUF_SOUTH);
s = kuf_square_quad_set(s, KUF_QUAD_BD, tmp);
#line 2594 "kuf.org"
#line 2646 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_D);
tmp = kuf_correct_side(tmp, KUF_SOUTH);
s = kuf_square_quad_set(s, KUF_QUAD_D, tmp);
#line 2594 "kuf.org"
#line 2656 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_ABCD);
tmp = kuf_correct_side(tmp, KUF_WEST);
s = kuf_square_quad_set(s, KUF_QUAD_ABCD, tmp);
#line 2594 "kuf.org"
#line 2666 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_AC);
tmp = kuf_correct_side(tmp, KUF_WEST);
s = kuf_square_quad_set(s, KUF_QUAD_AC, tmp);
#line 2594 "kuf.org"
#line 2677 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_AB);
tmp = kuf_correct_tile(tmp, KUF_NORTHWEST);
s = kuf_square_quad_set(s, KUF_QUAD_AB, tmp);
#line 2594 "kuf.org"
#line 2684 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_CD);
tmp = kuf_correct_tile(tmp, KUF_SOUTHWEST);
s = kuf_square_quad_set(s, KUF_QUAD_CD, tmp);
#line 2594 "kuf.org"
#line 2697 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_A);
tmp = kuf_correct_tile(tmp, KUF_NORTHWEST);
s = kuf_square_quad_set(s, KUF_QUAD_A, tmp);
#line 2594 "kuf.org"
#line 2704 "kuf.org"
tmp = kuf_square_quad_get(s, KUF_QUAD_C);
tmp = kuf_correct_tile(tmp, KUF_SOUTHWEST);
s = kuf_square_quad_set(s, KUF_QUAD_C, tmp);
#line 2607 "kuf.org"

    return s;
}
#line 2720 "kuf.org"
uint8_t kuf_correct_side(uint8_t q, int side)
{
    if (kuf_quad_check(q)) return q;
    switch (side) {
        case KUF_NORTH:
#line 2743 "kuf.org"
{
    int i;

    for (i = 0; i < 4; i++) {
        uint8_t s;

        s = kuf_quad_side_get(q, KUF_NORTH);
        s++;
        s &= 0x3;
        q = kuf_quad_side_set(q, KUF_NORTH, s);
        if (kuf_quad_check(q)) break;
    }
}
#line 2726 "kuf.org"
            break;
        case KUF_EAST:
#line 2760 "kuf.org"
{
    int i;

    for (i = 0; i < 4; i++) {
        uint8_t s;

        s = kuf_quad_side_get(q, KUF_EAST);
        s++;
        s &= 0x3;
        q = kuf_quad_side_set(q, KUF_EAST, s);
        if (kuf_quad_check(q)) break;
    }
}
#line 2729 "kuf.org"
            break;
        case KUF_SOUTH:
#line 2794 "kuf.org"
{
    int i;

    for (i = 0; i < 4; i++) {
        uint8_t s;

        s = kuf_quad_side_get(q, KUF_SOUTH);
        s++;
        s &= 0x3;
        q = kuf_quad_side_set(q, KUF_SOUTH, s);
        if (kuf_quad_check(q)) break;
    }
}
#line 2732 "kuf.org"
            break;
        case KUF_WEST:
#line 2777 "kuf.org"
{
    int i;

    for (i = 0; i < 4; i++) {
        uint8_t s;

        s = kuf_quad_side_get(q, KUF_WEST);
        s++;
        s &= 0x3;
        q = kuf_quad_side_set(q, KUF_WEST, s);
        if (kuf_quad_check(q)) break;
    }
}
#line 2735 "kuf.org"
            break;
    }
    return q;
}
#line 2822 "kuf.org"
uint8_t kuf_correct_tile(uint8_t q, int pos)
{
    uint8_t tile;

    if (kuf_quad_check(q)) return q;

    tile = kuf_quad_tile_get(q, pos);
    tile = tile ? 0 : 1;

    q = kuf_quad_tile_set(q, pos, tile);

    if (!kuf_quad_check(q)) {
        fprintf(stderr, "Oops. Could not correct tile!\n");
    }

    return q;
}
#line 2857 "kuf.org"
static uint16_t correct_dquad(uint16_t s)
{
    uint8_t q;

    q = kuf_square_quad_get(s, KUF_QUAD_ABCD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_ABCD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_CD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_CD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_BD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_BD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_D);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_D, q);

    if (!kuf_quad_check(q)) {
        fprintf(stderr, "Oops. Could not correct D quad!\n");
    }

    return s;
}
#line 2903 "kuf.org"
static uint16_t correct_cquad(uint16_t s)
{
    uint8_t q;

    q = kuf_square_quad_get(s, KUF_QUAD_AC);
    q = kuf_correct_side(q, KUF_SOUTH);
    s = kuf_square_quad_set(s, KUF_QUAD_AC, q);

    q = kuf_square_quad_get(s, KUF_QUAD_C);
    q = kuf_correct_side(q, KUF_SOUTH);
    s = kuf_square_quad_set(s, KUF_QUAD_C, q);

    if (!kuf_quad_check(q)) {
        fprintf(stderr, "Oops. Could not correct D quad!\n");
    }

    return s;
}
#line 2936 "kuf.org"
static uint16_t correct_eastwall(uint16_t s)
{
    uint8_t q;

    q = kuf_square_quad_get(s, KUF_QUAD_AB);
    q = kuf_correct_side(q, KUF_EAST);
    s = kuf_square_quad_set(s, KUF_QUAD_AB, q);

    q = kuf_square_quad_get(s, KUF_QUAD_B);
    q = kuf_correct_side(q, KUF_EAST);
    s = kuf_square_quad_set(s, KUF_QUAD_B, q);

    q = kuf_square_quad_get(s, KUF_QUAD_ABCD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_ABCD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_BD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_BD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_CD);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_CD, q);

    q = kuf_square_quad_get(s, KUF_QUAD_D);
    q = kuf_correct_tile(q, KUF_SOUTHEAST);
    s = kuf_square_quad_set(s, KUF_QUAD_D, q);

    return s;
}
#line 2989 "kuf.org"
void kuf_correct(int n, int m, uint16_t *squares, uint8_t *bm)
{
    int x, y;
    int movesx;
    int movesy;
    uint8_t a, b, c, d;

    movesx = (n * 2) - 1;
    movesy = (m * 2) - 1;

    a = b = c = d = 0;

    for (y = 0; y < movesy; y++) {
        int yodd;
        yodd = y % 2;
#line 3023 "kuf.org"
if (yodd) {
    uint16_t q[2];

    q[0] = squares[(y/2) * n];
    q[1] = squares[((y/2) + 1) * n];

    a = kuf_square_quad_get(q[0], KUF_QUAD_C);
    b = kuf_square_quad_get(q[0], KUF_QUAD_D);
    c = kuf_square_quad_get(q[1], KUF_QUAD_A);
    d = kuf_square_quad_get(q[1], KUF_QUAD_B);
} else {
    kuf_break_square(squares[(y/2)*n], &a, &b, &c, &d);
}
#line 3005 "kuf.org"
        for (x = 0; x < movesx; x++) {
#line 3043 "kuf.org"
uint16_t cur;
int xodd;
/* find new b + d based on position */

xodd = x % 2;

#line 3137 "kuf.org"
if (xodd) {
    if (yodd) {
        uint16_t ts[2];

        ts[0] = squares[(y / 2) * n + ((x / 2) + 1)];
        ts[1] = squares[((y / 2) + 1) * n + ((x / 2) + 1)];

        b = kuf_square_quad_get(ts[0], KUF_QUAD_C);
        d = kuf_square_quad_get(ts[1], KUF_QUAD_A);
    } else {
        uint16_t ts;

        ts = squares[(y / 2) * n + ((x / 2) + 1)];

        b = kuf_square_quad_get(ts, KUF_QUAD_A);
        d = kuf_square_quad_get(ts, KUF_QUAD_C);
    }
} else {
    if (yodd) {
        uint16_t ts[2];

        ts[0] = squares[(y / 2) * n + (x / 2)];
        ts[1] = squares[((y / 2) + 1) * n + (x / 2)];

        b = kuf_square_quad_get(ts[0], KUF_QUAD_D);
        d = kuf_square_quad_get(ts[1], KUF_QUAD_B);
    } else {
        uint16_t ts;

        ts = squares[(y / 2) * n + (x / 2)];
        b = kuf_square_quad_get(ts, KUF_QUAD_B);
        d = kuf_square_quad_get(ts, KUF_QUAD_D);
    }
}
#line 3050 "kuf.org"

/* construct current square */
cur = kuf_glue_square(a, b, c, d);

#line 3092 "kuf.org"
if (!xodd && !yodd) {
    if (!kuf_bitmask_get(bm, n, x/2, y/2)) {

        if (kuf_bitmask_get(bm, n, (x/2) + 1, y/2)) {
            uint16_t *ts;
            uint8_t ta, tb, tc, td;
            uint16_t nxt;

            nxt = squares[(y/2)*n + ((x/2) + 1)];
            nxt =
                kuf_glue_square(b,
                                kuf_square_quad_get(nxt,
                                                    KUF_QUAD_A),
                                d,
                                kuf_square_quad_get(nxt,
                                                    KUF_QUAD_C));
            if (y == 0) {
                /* correct eastern wall */
                nxt = correct_eastwall(nxt);
            } else {
                /* correct just the D quad */
                nxt = correct_dquad(nxt);
            }

            /* write back into squares */
            kuf_break_square(nxt, &ta, &tb, &tc, &td);

            ts = &squares[(y/2)*n + ((x/2) + 1)];

            *ts = kuf_square_quad_set(*ts, KUF_QUAD_A, tb);
            *ts = kuf_square_quad_set(*ts, KUF_QUAD_C, td);
        }

        kuf_break_square(squares[(y/2)*n + (x/2) + 1], &a, &b, &c, &d);
        x+=1;
        continue;
    }
}
#line 3055 "kuf.org"

if (y == 0) {
    if (x == 0) {
#line 3181 "kuf.org"
cur = kuf_correct_square(cur);
#line 3059 "kuf.org"
    } else {
#line 3189 "kuf.org"
cur = correct_eastwall(cur);
#line 3061 "kuf.org"
    }
} else {
    if (x == 0) {
#line 3207 "kuf.org"
cur = correct_cquad(cur);
#line 3065 "kuf.org"
    }

#line 3198 "kuf.org"
cur = correct_dquad(cur);
#line 3068 "kuf.org"
}

#line 3220 "kuf.org"
kuf_break_square(cur, &a, &b, &c, &d);
if (xodd) {
    if (yodd) {
        /* TODO: may not need the array anymore */
        /* Re-use variable instead */
        uint16_t *ts[4];

        ts[0] = &squares[(y / 2) * n + (x / 2)];
        *ts[0] = kuf_square_quad_set(*ts[0], KUF_QUAD_D, a);

        if (kuf_bitmask_get(bm, n, (x/2) + 1, (y/2))) {
            ts[1] = &squares[(y / 2) * n + ((x / 2) + 1)];
            *ts[1] = kuf_square_quad_set(*ts[1], KUF_QUAD_C, b);
        }

        if (kuf_bitmask_get(bm, n, (x/2), (y/2) + 1)) {
            ts[2] = &squares[((y / 2) + 1) * n + (x / 2)];
            *ts[2] = kuf_square_quad_set(*ts[2], KUF_QUAD_B, c);
        }

        if (kuf_bitmask_get(bm, n, (x/2) + 1, (y/2) + 1)) {
            ts[3] = &squares[((y / 2) + 1) * n + ((x / 2) + 1)];
            *ts[3] = kuf_square_quad_set(*ts[3], KUF_QUAD_A, d);
        }
    } else {
        /* TODO: may not need the array anymore */
        /* Re-use variable instead */
        uint16_t *ts[2];

        ts[0] = &squares[(y / 2) * n + (x / 2)];
        *ts[0] = kuf_square_quad_set(*ts[0], KUF_QUAD_B, a);
        *ts[0] = kuf_square_quad_set(*ts[0], KUF_QUAD_D, c);

        if (kuf_bitmask_get(bm, n, (x/2) + 1, (y/2))) {
            ts[1] = &squares[(y / 2) * n + ((x / 2) + 1)];
            *ts[1] = kuf_square_quad_set(*ts[1], KUF_QUAD_A, b);
            *ts[1] = kuf_square_quad_set(*ts[1], KUF_QUAD_C, d);
        }
    }
} else {
    if (yodd) {
        uint16_t *ts[2];

        ts[0] = &squares[(y / 2) * n + (x / 2)];

        *ts[0] = kuf_square_quad_set(*ts[0], KUF_QUAD_C, a);
        *ts[0] = kuf_square_quad_set(*ts[0], KUF_QUAD_D, b);

        if (kuf_bitmask_get(bm, n, (x/2), (y/2) + 1)) {
            ts[1] = &squares[((y / 2) + 1) * n + (x / 2)];
            *ts[1] = kuf_square_quad_set(*ts[1], KUF_QUAD_A, c);
            *ts[1] = kuf_square_quad_set(*ts[1], KUF_QUAD_B, d);
        }
    } else {
        uint16_t *ts;
        ts = &squares[((y / 2) * n) + (x / 2)];

        *ts = kuf_square_quad_set(*ts, KUF_QUAD_A, a);
        *ts = kuf_square_quad_set(*ts, KUF_QUAD_B, b);
        *ts = kuf_square_quad_set(*ts, KUF_QUAD_C, c);
        *ts = kuf_square_quad_set(*ts, KUF_QUAD_D, d);
    }
}
#line 3068 "kuf.org"
#line 3289 "kuf.org"
a = b;
c = d;
#line 3068 "kuf.org"
#line 3007 "kuf.org"
        }
    }
}
#line 3314 "kuf.org"
int kuf_bitmask_nbytes(int n, int m)
{
    int nbits;
    int nbytes;

    nbits = n * m;

    nbytes = nbits / 8;

    if ((nbytes * 8) < nbits) nbytes++;

    return nbytes;
}
#line 3339 "kuf.org"
void kuf_bitmask_set(uint8_t *bm, int n, int x, int y, int s)
{
    int bitpos;
    int bytepos;
    int localpos;

    bitpos = y * n + x;
    bytepos = bitpos / 8;
    localpos = bitpos - (bytepos * 8);

    if (s) {
        bm[bytepos] |= (1 << localpos);
    } else {
        bm[bytepos] &= ~(1 << localpos);
    }
}
#line 3372 "kuf.org"
int kuf_bitmask_get(uint8_t *bm, int n, int x, int y)
{
    int s;
    int bitpos;
    int bytepos;
    int localpos;

    if (bm == NULL) return 1;

    s = 0;

    bitpos = y * n + x;
    bytepos = bitpos / 8;
    localpos = bitpos - (bytepos * 8);

    s = (bm[bytepos] & (1 << localpos)) > 0;

    return s;
}
#line 34 "kuf.org"
