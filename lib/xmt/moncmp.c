#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "cmp/cmp.h"
#include "moncmp.h"

static size_t get_size(cmp_ctx_t *ctx,
                       const void *ud,
                       size_t count)
{
    size_t *size;
    size = (size_t *)ctx->buf;
    *size += count;
    return count;
}

static size_t memwrite(cmp_ctx_t *ctx, const void *ud, size_t count)
{
    size_t n;
    moncmp_d *m;
    const uint8_t *bytes;

    m = ctx->buf;
    bytes = ud;
    for(n = 0; n < count; n++) {
        m->buf[m->pos + n] = bytes[n];
    }
    m->pos += count;
    return count;
}

static bool memread(cmp_ctx_t *ctx, void *data, size_t limit)
{
    size_t n;
    moncmp_d *m;
    uint8_t *bytes;

    m = ctx->buf;
    bytes = data;

    if (m->pos + limit > m->sz) {
        fprintf(stderr,
                "Read %ld out of range of size %ld\n",
                m->pos + limit, m->sz);
        return false;
    }

    for (n = 0; n < limit; n++) {
        bytes[n] = m->buf[m->pos + n];
    }
    m->pos += limit;
    return true;
}

int moncmp_read(cmp_ctx_t *ctx, void *data, size_t limit)
{
    bool rc;
    rc = memread(ctx, data, limit);

    if (rc == false) return 0;

    return 1;
}

void moncmp_init_getsize(cmp_ctx_t *cmp, size_t *sz)
{
    cmp_init(cmp, sz, NULL, NULL, get_size);
}

void moncmp_init_write(moncmp_d *m,
                       cmp_ctx_t *cmp,
                       uint8_t *buf)
{
    m->buf = buf;
    m->pos = 0;
    cmp_init(cmp, m, NULL, NULL, memwrite);
}

void moncmp_init_read(moncmp_d *m,
                      cmp_ctx_t *cmp,
                      uint8_t *buf,
                      size_t sz)
{
    m->pos = 0;
    m->buf = buf;
    m->sz = sz;
    cmp_init(cmp, m, memread, NULL, NULL);
}
