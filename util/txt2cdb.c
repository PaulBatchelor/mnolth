/*
 * txt2cdb: in.txt out.db
 *
 * A silly small little thing that takes in text file with
 * key/value pairs like this:
 *
 * @foo
 * bar
 * @hello
 * world
 * @lines
 * aaa
 * bbb
 * ccc
 *
 * and turns it into a constant database file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "cdb/cdb.h"

struct parser {
    int mode;
    int keysz;
    int valsz;
    int pos;
    char buf[2048];
    struct cdb_make cdbm;
};

static void parser_init(struct parser *p)
{
    int i;
    p->mode = 0;
    p->pos = 0;
    p->keysz = 0;
    p->valsz = 0;

    for (i = 0; i < 2048; i++) p->buf[i] = 0;
}

static void print_keyval(struct parser *p,
                         const char *key,
                         const char *val)
{
    if (p->keysz <= 0) return;
    cdb_make_add(&p->cdbm, key, p->keysz, val, p->valsz);
}

static void finish(struct parser *p)
{
    if (p->mode == 3 || p->mode == 2) {
        p->pos--;
        if (p->pos < 0) return;
        p->valsz = p->pos - p->keysz - 1;
        p->buf[p->pos] = 0;
        print_keyval(p, p->buf, &p->buf[p->keysz + 1]);
    }
}

static int parse(struct parser *p, const char *buf, int sz)
{
    int i;

    int mode;

    mode = p->mode;
    for (i = 0; i < sz; i++) {
        char c;

        if (p->pos >= 2048) {
            p->mode = -1;
            return 3;
        }

        c = buf[i];

        switch (mode) {
            case 0: /* init: search for '@' */
                if (c == '@') {
                    mode = 1;
                    p->keysz = 0;
                    p->pos = 0;
                    p->valsz = 0;
                } else {
                    p->mode = -1;
                    return 2;
                }
                break;
            case 1: /* parse keyword up to newline */
                if (c == '\n') {
                    p->keysz = p->pos;
                    p->buf[p->keysz] = 0;
                    p->pos++;
                    mode = 3;
                    break;
                }
                p->buf[p->pos] = c;
                p->pos++;
                break;
            case 2: /* newline in value, check for @ */
                if (c == '@') {
                    p->pos--;
                    if (p->pos < 0) return 4;
                    p->valsz = p->pos - p->keysz - 1;
                    p->buf[p->pos] = 0;
                    print_keyval(p, p->buf, &p->buf[p->keysz + 1]);
                    mode = 1;
                    p->pos = 0;
                    p->keysz = 0;
                    p->valsz = 0;
                } else if (c == '\n') {/* newline, try again */
                    p->buf[p->pos] = c;
                    p->pos++;
                    mode = 2;
                } else {
                    p->buf[p->pos] = c;
                    p->pos++;
                    mode = 3;
                }
                break;
            case 3: /* read line */
                if (c == '\n') {
                    p->buf[p->pos] = c;
                    p->pos++;
                    mode = 2;
                } else {
                    p->buf[p->pos] = c;
                    p->pos++;
                }
                break;
            default: /* error */
                return 1;
        }
    }

    p->mode = mode;
    return 0;
}

int txt2cdb_main(int argc, char *argv[])
{
    FILE *fp;
    char *buf;
    struct parser *p;
    int fd;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s in.txt out.db\n", argv[0]);
        return 1;
    }

    fp = fopen(argv[1], "r");

    fd = open(argv[2], O_RDWR | O_CREAT, 0644);

    if (fp == NULL) {
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 1;
    }

    buf = calloc(1, 256);
    p = malloc(sizeof(struct parser));

    parser_init(p);

    cdb_make_start(&p->cdbm, fd);

    while (!feof(fp)) {
        int sz, rc;
        sz = fread(buf, 1, 256, fp);
        rc = parse(p, buf, sz);
        if (rc) {
            fprintf(stderr, "error: %d\n", rc);
            break;
        }
    }

    finish(p);
    cdb_make_finish(&p->cdbm);
    close(fd);
    free(buf);
    free(p);
    fclose(fp);
    return 0;
}
