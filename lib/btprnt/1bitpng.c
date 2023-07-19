/* this was a PNG writer I wrote for btprnt, optimized for 1-bit
 * images. I've modified it a little bit to use miniz instead
 * of zlib, and to make it work within the mnolth ecosystem.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "btprnt/btprnt.h"
#include "miniz/miniz.h"

static unsigned long crc_table[256];

static int crc_table_computed = 0;

static void make_crc_table(void)
{
    unsigned long c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1) {
                c = 0xedb88320L ^ (c >> 1);
            } else {
                c = c >> 1;
            }
        }
        crc_table[n] = c;
    }

    crc_table_computed = 1;
}

static unsigned long update_crc(unsigned long crc, unsigned char *buf,
                         int len)
{
    unsigned long c;
    int n;

    c = crc;

    if (!crc_table_computed) {
        make_crc_table();
    }

    for (n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }

    return c;
}

static void write_long(FILE *fp, unsigned long x)
{
    uint8_t b[4];
    int i;

    for (i = 3; i >= 0; i--) {
        b[i] = x & 0xff;
        x >>= 8;
    }

    fwrite(b, 1, 4, fp);
}

static void write_chunk(FILE *fp,
                 unsigned long len,
                 const char *type,
                 unsigned char *data)
{
    unsigned long x;

    /* part 1: length */
    write_long(fp, len);
    /* part 2: type */
    fwrite(type, 1, 4, fp);
    /* part 3: data */
    fwrite(data, 1, len, fp);
    /* part 4: CRC */

    x = 0xffffffffL;
    x = update_crc(x, (unsigned char *)type, 4);
    x = update_crc(x, data, len);
    x ^= 0xffffffffL;

    write_long(fp, x);
}

static void write_header(FILE *fp, uint32_t w, uint32_t h)
{
    uint8_t data[13];
    uint8_t b[4];
    uint32_t x;
    int pos, i;

    pos = 0;

    /* width (4 bytes) */

    x = w;
    for (i = 3; i >= 0; i--) {
        b[i] = x & 0xff;
        x >>= 8;
    }
    for (i = 0; i < 4; i++) {
        data[pos] = b[i];
        pos++;
    }

    /* height (4 bytes) */

    x = h;
    for (i = 3; i >= 0; i--) {
        b[i] = x & 0xff;
        x >>= 8;
    }

    for (i = 0; i < 4; i++) {
        data[pos] = b[i];
        pos++;
    }

    /* bit depth 1 bytes, set to be 1 bit */
    b[0] = 1;
    data[pos] = b[0];
    pos++;

    /* color type: grayscale (0) */
    b[0] = 0;
    data[pos] = b[0];
    pos++;

    /* compression method: default DEFLATE (0) */
    b[0] = 0;
    data[pos] = b[0];
    pos++;

    /* filter method: 0 (default) */
    b[0] = 0;
    data[pos] = b[0];
    pos++;

    /* interlace: no interlace (0) */
    b[0] = 0;
    data[pos] = b[0];
    pos++;

    write_chunk(fp, 13, "IHDR", data);
}

static void write_data(FILE *fp, btprnt_buf *buf)
{
    uint8_t *data;
    uint8_t *zdata;
    uint32_t sz;
    long unsigned int zsz;
    int x, y;
    int w, h;
    int stride;
    uint32_t pos;
    int bpos;
    z_stream defstream;

    h = btprnt_buf_height(buf);
    w = btprnt_buf_width(buf);

    stride = w / 8;

    if ((stride * 8) < w) stride++;

    sz = (stride + 1) * h;

    zdata = calloc(1, sz);
    data = calloc(1, sz);
    zsz = sz;

    pos = 0;

    for (y = 0; y < h; y++) {
        uint8_t byte;
        uint8_t bit;

        bpos = -1;
        byte = 0;
        bit = 0;

        data[pos] = 0x00;
        pos++;

        for (x = 0; x < w; x++) {
            bit = btprnt_buf_read(buf, x, y);
            bpos++;

            if (!bit) {
                byte |= 1 << (7 - bpos);
            }

            if (bpos == 7) {
                data[pos] = byte;
                pos++;
                byte = 0;
                bpos = -1;
            }
        }

        if (bpos >= 0) {
            data[pos] = byte;
            pos++;
            byte = 0;
            bpos = -1;
        }
    }

    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.avail_in = sz;
    defstream.next_in = data;
    defstream.avail_out = sz;
    defstream.next_out = zdata;

    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);

    /* compress(zdata, &zsz, data, sz); */
    zsz = sz - defstream.avail_out;

    write_chunk(fp, zsz, "IDAT", zdata);

    deflateEnd(&defstream);
    free(zdata);
    free(data);
}

static void write_end(FILE *fp)
{
    write_chunk(fp, 0, "IEND", NULL);
}

void btprnt_png(btprnt *bp, const char *out)
{
    FILE *fp;
    unsigned char signature[] = {137, 80, 78, 71, 13, 10, 26, 10};
    btprnt_buf *buf;
    int w, h;

    if (out == NULL) {
        fp = stdout;
    } else {
        fp = fopen(out, "wb");
    }

    buf = btprnt_buf_get(bp);

    fwrite(signature, 1, 8, fp);

    w = btprnt_buf_width(buf);
    h = btprnt_buf_height(buf);

    write_header(fp, w, h);

    write_data(fp, buf);

    write_end(fp);

    if (out != NULL) fclose(fp);
}
