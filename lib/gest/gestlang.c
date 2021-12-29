#line 41 "gestlang.org"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gest.h"
#define GESTLANG_PRIV
#include "gestlang.h"
#line 213 "gestlang.org"
static void parse_word(gestlang_d *glang, gest_d *gest);
#line 362 "gestlang.org"
static int hash(const char *str, int sz);
#line 787 "gestlang.org"
static int f_comment(gest_d *g, int argc, char *argv[], void *ud);
#line 829 "gestlang.org"
static int pch2nn(const char *pch);
#line 884 "gestlang.org"
static int f_nt(gest_d *g, int argc, char *argv[], void *ud);
#line 41 "gestlang.org"
#line 81 "gestlang.org"
size_t gestlang_sizeof(void)
{
    return sizeof(gestlang_d);
}
#line 96 "gestlang.org"
void gestlang_init(gestlang_d *glang)
{
#line 104 "gestlang.org"
{
    int i;
    glang->ud = NULL;
    glang->err = 0;
    glang->wsz = 0;
    glang->nargs = 0;
    glang->psz = 0;
    glang->argpos = -1;

    for (i = 0; i < 64; i++) {
        glang->word[i] = 0;
    }

    for (i = 0; i < 256; i++) {
        glang->pool[i] = 0;
    }

    for (i = 0; i < 8; i++) {
        glang->args[i] = NULL;
    }
}
#line 353 "gestlang.org"
{
    int i;

    for (i = 0; i < 16; i++) glang->ent[i] = NULL;
}
#line 779 "gestlang.org"
glang->comment = 0;
#line 99 "gestlang.org"
}
#line 134 "gestlang.org"
void gestlang_bye(gestlang_d *glang)
{
#line 439 "gestlang.org"
{
    int i;

    for (i = 0; i < 16; i++) {
        gestlang_entry *ent, *nxt;;

        ent = glang->ent[i];

        while (ent != NULL) {
            nxt = ent->nxt;

            if (ent->del != NULL) {
                ent->del(ent->ud);
            }

            free(ent);
            ent = nxt;
        }
    }
}
#line 137 "gestlang.org"
}
#line 161 "gestlang.org"
int gestlang_parse(gestlang_d *glang,
                   gest_d *gest,
                   const char *buf,
                   int sz)
{
    gestlang_parse_buffer(glang, gest, buf, sz);
    if (glang->wsz > 0) {
        glang->word[glang->wsz] = '\0';
        parse_word(glang, gest);
        glang->wsz = 0;
    }
    return 0;
}
#line 178 "gestlang.org"
int gestlang_parse_buffer(gestlang_d *glang,
                          gest_d *gest,
                          const char *buf,
                          int sz)
{
#line 190 "gestlang.org"
int i;

for (i = 0; i < sz; i++) {
    if (glang->err) return glang->err;
#line 814 "gestlang.org"
if (glang->comment) {
    if (buf[i] == '\n') {
        glang->comment = 0;
        glang->wsz = 0;
    }
    continue;
}
#line 195 "gestlang.org"
    if (glang->wsz >= 64) {
        glang->word[63] = '\0';
        parse_word(glang, gest);
        glang->wsz = 0;
    } else if (buf[i] == ' ' || buf[i] == '\n') {
        glang->word[glang->wsz] = '\0';
        parse_word(glang, gest);
        glang->wsz = 0;
    } else {
        glang->word[glang->wsz] = buf[i];
        glang->wsz++;
    }

}
#line 184 "gestlang.org"
    return 0;
}
#line 213 "gestlang.org"
#line 265 "gestlang.org"
static int find_word(gestlang_d *glang)
{
    int rc;
    gestlang_entry *ent;

    rc = gestlang_find(glang, glang->word, glang->wsz, &ent);

    if (rc) return rc;

    glang->fun = ent->fun;
    glang->nargs = ent->nargs;
    glang->ud = ent->ud;

    return 0;
}
#line 219 "gestlang.org"
static void parse_word(gestlang_d *glang, gest_d *gest)
{
    if (glang->wsz <= 0) return;
    if (glang->argpos < 0) {
        glang->argpos = 0;

        if (find_word(glang)) {
            fprintf(stderr, "could not find function %s\n", glang->word);
            glang->err = 2;
            return;
        }

        if (glang->nargs == 0) {
            glang->fun(gest, glang->nargs, glang->args, glang->ud);
            glang->psz = 0;
            glang->argpos = -1;
        }
    } else {
        int c;

        for (c = 0; c < glang->wsz; c++) {
            if (glang->psz >= 256) {
                glang->psz = 0;
                glang->err = 1;
                return;
            }
            glang->pool[glang->psz + c] = glang->word[c];
        }
        glang->args[glang->argpos] = &glang->pool[glang->psz];
        glang->psz += glang->wsz;
        glang->pool[glang->psz] = '\0';
        glang->psz++;

        glang->argpos++;

        if (glang->argpos == glang->nargs) {
            glang->fun(gest, glang->nargs, glang->args, glang->ud);
            glang->psz = 0;
            glang->argpos = -1;
        }
    }
}
#line 291 "gestlang.org"
int gestlang_load(gestlang_d *glang,
                  gest_d *gest,
                  const char *filename)
{
    char *buf;
    FILE *fp;
    int rc;

    fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return 1;
    }

    buf = calloc(1, 128);

    rc = 0;

    while (!feof(fp)) {
        int sz;
        sz = fread(buf, 1, 128, fp);
        rc = gestlang_parse_buffer(glang, gest, buf, sz);
        if (rc) break;
    }

    free(buf);
    fclose(fp);
    return rc;
}
#line 367 "gestlang.org"
static int hash(const char *str, int sz)
{
    unsigned long h;
    int i;

    h = 5381;
    i = 0;

    for(i = 0; i < sz; i++) {
        h = ((h << 5) + h) ^ str[i];
        h %= 0x7FFFFFFF;
    }

    return h % 16;
}
#line 397 "gestlang.org"
void gestlang_add(gestlang_d *glang,
                  const char *key,
                  int sz,
                  int nargs,
                  int (*fun)(gest_d *,int,char **,void *),
                  void *ud,
                  void (*del)(void *))
{
    int pos;
    gestlang_entry *ent;
    int n;

    if (sz >= 16) return;

    ent = calloc(1, sizeof(gestlang_entry));

    for (n = 0; n < sz; n++) {
        ent->word[n] = key[n];
    }

    ent->word[sz] = '\0';
    ent->sz = sz;

    ent->fun = fun;
    ent->del = del;
    ent->nargs = nargs;
    ent->ud = ud;
    ent->nxt = NULL;

    pos = hash(key, sz);

    if (glang->ent[pos] == NULL) {
        glang->ent[pos] = ent;
    } else {
        ent->nxt = glang->ent[pos];
        glang->ent[pos] = ent;
    }
}
#line 471 "gestlang.org"
int gestlang_find(gestlang_d *glang,
                  const char *key,
                  int sz,
                  gestlang_entry **pent)
{
    int pos;
    gestlang_entry *ent;

    pos = hash(key, sz);

    ent = glang->ent[pos];

    while(ent != NULL) {
        if (ent->sz == sz && !strcmp(key, ent->word)) {
            *pent = ent;
            return 0;
        }
        ent = ent->nxt;
    }

    return 1;
}
#line 498 "gestlang.org"
#line 516 "gestlang.org"
static int f_beg(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_begin(g, atoi(argv[0]), atoi(argv[1]));
    return 0;
}
#line 534 "gestlang.org"
static int f_tar(gest_d *g, int argc, char *argv[], void *ud)
{
    return gest_addtarget(g, strtod(argv[0], 0));
}
#line 551 "gestlang.org"
static int f_stp(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_behavior_step(g);
    return 0;
}
#line 569 "gestlang.org"
static int f_end(gest_d *g, int argc, char *argv[], void *ud)
{
    return gest_end(g);
}
#line 586 "gestlang.org"
static int f_loop(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_loopit(g);
    return 0;
}
#line 604 "gestlang.org"
static int f_fin(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_finish(g);
    return 0;
}
#line 622 "gestlang.org"
static int f_mr(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_monoramp(g, atoi(argv[0]));
    return 0;
}
#line 640 "gestlang.org"
static int f_pr(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_polyramp(g, atoi(argv[0]));
    return 0;
}
#line 659 "gestlang.org"
static int f_lin(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_behavior_linear(g);
    return 0;
}
#line 678 "gestlang.org"
static int f_sg(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_behavior_smallgliss(g);
    return 0;
}
#line 697 "gestlang.org"
static int f_mg(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_behavior_mediumgliss(g);
    return 0;
}
#line 716 "gestlang.org"
static int f_gl(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_behavior_gliss(g);
    return 0;
}
#line 754 "gestlang.org"
static int f_rmp(gest_d *g, int argc, char *argv[], void *ud)
{
    gest_ramp(g);
    return 0;
}
#line 504 "gestlang.org"
void gestlang_loader(gestlang_d *glang)
{
#line 525 "gestlang.org"
gestlang_add(glang, "beg", 3, 2, f_beg, NULL, NULL);
#line 542 "gestlang.org"
gestlang_add(glang, "t", 1, 1, f_tar, NULL, NULL);
#line 560 "gestlang.org"
gestlang_add(glang, "stp", 3, 0, f_stp, NULL, NULL);
#line 577 "gestlang.org"
gestlang_add(glang, "end", 3, 0, f_end, NULL, NULL);
#line 595 "gestlang.org"
gestlang_add(glang, "loop", 4, 0, f_loop, NULL, NULL);
#line 613 "gestlang.org"
gestlang_add(glang, "fin", 3, 0, f_fin, NULL, NULL);
#line 631 "gestlang.org"
gestlang_add(glang, "mr", 2, 1, f_mr, NULL, NULL);
#line 649 "gestlang.org"
gestlang_add(glang, "pr", 2, 1, f_pr, NULL, NULL);
#line 668 "gestlang.org"
gestlang_add(glang, "lin", 3, 0, f_lin, NULL, NULL);
#line 687 "gestlang.org"
gestlang_add(glang, "sg", 2, 0, f_sg, NULL, NULL);
#line 706 "gestlang.org"
gestlang_add(glang, "mg", 2, 0, f_mg, NULL, NULL);
#line 725 "gestlang.org"
gestlang_add(glang, "gl", 2, 0, f_gl, NULL, NULL);
#line 763 "gestlang.org"
gestlang_add(glang, "rmp", 3, 0, f_rmp, NULL, NULL);
#line 806 "gestlang.org"
gestlang_add(glang, "#", 1, 0, f_comment, glang, NULL);
#line 897 "gestlang.org"
gestlang_add(glang, "nt", 2, 1, f_nt, NULL, NULL);
#line 507 "gestlang.org"
}
#line 792 "gestlang.org"
static int f_comment(gest_d *g, int argc, char *argv[], void *ud)
{
    gestlang_d *glang;

    glang = ud;

    glang->comment = 1;

    return 0;
}
#line 834 "gestlang.org"
static int pch2nn(const char *pch)
{
    int nn;

    nn = 0;

    if (pch[0] == 0) return 0;

    switch (pch[0]) {
        case 'C':
            nn = 0;
            break;
        case 'D':
            nn = 2;
            break;
        case 'E':
            nn = 4;
            break;
        case 'F':
            nn = 5;
            break;
        case 'G':
            nn = 7;
            break;
        case 'A':
            nn = 9;
            break;
        case 'B':
            nn = 11;
            break;
        default:
            break;
    }

    if (pch[1] == 0) return nn;


    if (pch[1] == '#') nn++;
    else if (pch[1] == 'b') nn--;

    if (pch[2] == 0) return nn;

    nn += 12 * (1 + (pch[2] - '0'));

    return nn;
}
#line 889 "gestlang.org"
static int f_nt(gest_d *g, int argc, char *argv[], void *ud)
{
    return gest_addtarget(g, pch2nn(argv[0]));
}
#line 41 "gestlang.org"
