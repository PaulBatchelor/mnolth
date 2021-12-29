#line 44 "kuf.org"
#ifndef KUF_H
#define KUF_H
#line 317 "kuf.org"
enum {
    KUF_NORTH,
    KUF_SOUTH,
    KUF_EAST,
    KUF_WEST
};
#line 396 "kuf.org"
enum {
    KUF_NORTHWEST,
    KUF_NORTHEAST,
    KUF_SOUTHWEST,
    KUF_SOUTHEAST
};
#line 705 "kuf.org"
enum {
    KUF_QUAD_A,
    KUF_QUAD_B,
    KUF_QUAD_C,
    KUF_QUAD_D,
    KUF_QUAD_AB,
    KUF_QUAD_AC,
    KUF_QUAD_BD,
    KUF_QUAD_CD,
    KUF_QUAD_ABCD
};
#line 2332 "kuf.org"
#line 2435 "kuf.org"
#define KUF_HLINE_0 0x0033
#define KUF_HLINE_1 0x00cc
#define KUF_HLINE_2 0x3300
#define KUF_HLINE_3 0xcc00
#line 2445 "kuf.org"
#define KUF_VLINE_0 0x0505
#define KUF_VLINE_1 0x0a0a
#define KUF_VLINE_2 0x5050
#define KUF_VLINE_3 0xa0a0
#line 2460 "kuf.org"
#define KUF_HLINE_HALF_WEST_0 0x0003
#define KUF_HLINE_HALF_EAST_0 0x0030
#define KUF_HLINE_HALF_WEST_1 0x000c
#define KUF_HLINE_HALF_EAST_1 0x00c0
#define KUF_HLINE_HALF_WEST_2 0x0300
#define KUF_HLINE_HALF_EAST_2 0x3000
#define KUF_HLINE_HALF_WEST_3 0x0c00
#define KUF_HLINE_HALF_EAST_3 0xc000
#line 2475 "kuf.org"
#define KUF_VLINE_HALF_NORTH_0 0x0005
#define KUF_VLINE_HALF_SOUTH_0 0x0500
#define KUF_VLINE_HALF_NORTH_1 0x000a
#define KUF_VLINE_HALF_SOUTH_1 0x0a00
#define KUF_VLINE_HALF_NORTH_2 0x0050
#define KUF_VLINE_HALF_SOUTH_2 0x5000
#define KUF_VLINE_HALF_NORTH_3 0x00a0
#define KUF_VLINE_HALF_SOUTH_3 0xa000
#line 2491 "kuf.org"
#define KUF_HPARALLEL_0 (KUF_HLINE_0 | KUF_HLINE_2)
#define KUF_HPARALLEL_1 (KUF_HLINE_1 | KUF_HLINE_3)
#define KUF_VPARALLEL_0 (KUF_VLINE_0 | KUF_VLINE_2)
#define KUF_VPARALLEL_1 (KUF_VLINE_1 | KUF_VLINE_3)
#line 2501 "kuf.org"
#define KUF_CORNER_NORTHWEST \
    (KUF_HLINE_0 | \
    KUF_VLINE_0 | \
    KUF_HLINE_HALF_EAST_2 | \
    KUF_VLINE_HALF_SOUTH_2)
#define KUF_CORNER_NORTHEAST \
    (KUF_HLINE_0 | \
    KUF_VLINE_3 | \
    KUF_HLINE_HALF_WEST_2 | \
    KUF_VLINE_HALF_SOUTH_1)
#define KUF_CORNER_SOUTHWEST \
    (KUF_HLINE_3 | \
    KUF_VLINE_0 | \
    KUF_HLINE_HALF_EAST_1 | \
    KUF_VLINE_HALF_NORTH_2)
#define KUF_CORNER_SOUTHEAST \
    (KUF_HLINE_3 | \
    KUF_VLINE_3 | \
    KUF_HLINE_HALF_WEST_1 | \
    KUF_VLINE_HALF_NORTH_1)
#line 2525 "kuf.org"
#define KUF_DIAGONAL_0 0x0660
#define KUF_DIAGONAL_1 0x9009
#define KUF_CROSSHATCH_0 0x4664
#define KUF_CROSSHATCH_1 ~KUF_CROSSHATCH_0
#line 2332 "kuf.org"
#line 44 "kuf.org"
#line 111 "kuf.org"
int kuf_quad_check(uint8_t quad);
#line 134 "kuf.org"
uint32_t kuf_rand(uint32_t rng);
#line 151 "kuf.org"
uint32_t kuf_rng(uint32_t *rng);
#line 192 "kuf.org"
uint8_t kuf_gen_quad(uint32_t *rng);
#line 214 "kuf.org"
uint8_t kuf_gen_tile(uint32_t *rng);
#line 233 "kuf.org"
uint8_t kuf_gen_pair(uint32_t *rng);
#line 251 "kuf.org"
uint8_t kuf_quad_side_set(uint8_t quad,
                          uint8_t side,
                          uint8_t pair);
uint8_t kuf_quad_side_get(uint8_t quad, uint8_t side);
#line 332 "kuf.org"
uint8_t kuf_quad_tile_get(uint8_t quad, uint8_t corner);
uint8_t kuf_quad_tile_set(uint8_t quad,
                          uint8_t corner,
                          uint8_t tile);
#line 413 "kuf.org"
uint8_t kuf_square_quad_get(uint16_t s, uint8_t pos);
uint16_t kuf_square_quad_set(uint16_t s,
                             uint8_t pos,
                             uint8_t quad);
#line 724 "kuf.org"
uint16_t kuf_gen_square(uint32_t *rng);
#line 978 "kuf.org"
uint16_t kuf_glue_square(uint8_t a,
                         uint8_t b,
                         uint8_t c,
                         uint8_t d);
#line 1008 "kuf.org"
void kuf_break_square(uint16_t s,
                      uint8_t *a,
                      uint8_t *b,
                      uint8_t *c,
                      uint8_t *d);
#line 1040 "kuf.org"
void kuf_gen_block(uint32_t *rng,
                   uint16_t *pw,
                   uint16_t *px,
                   uint16_t *py,
                   uint16_t *pz);
#line 1128 "kuf.org"
void kuf_solve_wall_east(uint32_t *rng,
                         uint8_t a, uint8_t c,
                         uint8_t *pb, uint8_t *pd);
#line 1337 "kuf.org"
void kuf_solve_wall_south(uint32_t *rng,
                          uint8_t a, uint8_t b,
                          uint8_t *pc, uint8_t *pd);
#line 1547 "kuf.org"
uint8_t kuf_solve_quad_northeast(uint32_t *rng,
                                 uint8_t a,
                                 uint8_t c,
                                 uint8_t d);
#line 1695 "kuf.org"
uint8_t kuf_solve_quad_southeast(uint32_t *rng,
                                 uint8_t a,
                                 uint8_t b,
                                 uint8_t c);
#line 2011 "kuf.org"
void kuf_print_tile(uint8_t tile);
#line 2024 "kuf.org"
void kuf_print_quad(uint8_t quad);
#line 2046 "kuf.org"
void kuf_print_side(uint8_t side);
#line 2060 "kuf.org"
void kuf_print_square(uint16_t square);
#line 2094 "kuf.org"
void kuf_print_block(uint16_t w, uint16_t x, uint16_t y, uint16_t z);
#line 2201 "kuf.org"
void kuf_write_block_pbm(const char *filename,
                         uint16_t w, uint16_t x,
                         uint16_t y, uint16_t z);
#line 2321 "kuf.org"
void kuf_write_pbm(const char *filename,
                   int n, int m,
                   uint16_t *squares);
#line 2535 "kuf.org"
void kuf_set_block(int width,
                   uint16_t *squares,
                   int xpos, int ypos,
                   uint16_t w, uint16_t x,
                   uint16_t y, uint16_t z);
#line 2565 "kuf.org"
void kuf_set_square(int width,
                    uint16_t *squares,
                    int xpos, int ypos,
                    uint16_t s);
#line 2589 "kuf.org"
uint16_t kuf_correct_square(uint16_t s);
#line 2715 "kuf.org"
uint8_t kuf_correct_side(uint8_t q, int side);
#line 2817 "kuf.org"
uint8_t kuf_correct_tile(uint8_t q, int pos);
#line 2984 "kuf.org"
void kuf_correct(int n, int m, uint16_t *squares, uint8_t *bm);
#line 3309 "kuf.org"
int kuf_bitmask_nbytes(int n, int m);
#line 3334 "kuf.org"
void kuf_bitmask_set(uint8_t *bm, int n, int x, int y, int s);
#line 3363 "kuf.org"
int kuf_bitmask_get(uint8_t *bm, int n, int x, int y);
#line 48 "kuf.org"
#endif
