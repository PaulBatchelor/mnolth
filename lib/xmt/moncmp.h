#ifndef MONCMP_H
#define MONCMP_H
typedef struct moncmp_d moncmp_d;
struct moncmp_d {
    uint8_t *buf;
    size_t pos;
    size_t sz;
};
void moncmp_init_getsize(cmp_ctx_t *cmp, size_t *sz);
void moncmp_init_write(moncmp_d *m,
                       cmp_ctx_t *cmp,
                       uint8_t *buf);
void moncmp_init_read(moncmp_d *m,
                      cmp_ctx_t *cmp,
                      uint8_t *buf,
                      size_t sz);
int moncmp_read(cmp_ctx_t *ctx, void *data, size_t limit);
#endif
