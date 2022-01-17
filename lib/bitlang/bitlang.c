#line 26 "bitlang.org"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BITLANG_PRIV
#include "bitlang.h"
enum {
BITLANG_NOP,
#line 179 "bitlang.org"
BITLANG_ADD,
#line 222 "bitlang.org"
BITLANG_SUB,
#line 265 "bitlang.org"
BITLANG_MUL,
#line 308 "bitlang.org"
BITLANG_DIV,
#line 354 "bitlang.org"
BITLANG_GET,
#line 448 "bitlang.org"
BITLANG_MOD,
#line 492 "bitlang.org"
BITLANG_EQ,
#line 535 "bitlang.org"
BITLANG_LSHIFT,
#line 578 "bitlang.org"
BITLANG_RSHIFT,
#line 621 "bitlang.org"
BITLANG_LOR,
#line 664 "bitlang.org"
BITLANG_BOR,
#line 707 "bitlang.org"
BITLANG_BAND,
#line 750 "bitlang.org"
BITLANG_XOR,
#line 793 "bitlang.org"
BITLANG_BNOT,
#line 834 "bitlang.org"
BITLANG_LNOT,
#line 875 "bitlang.org"
BITLANG_ABS,
#line 35 "bitlang.org"
BITLANG_END
};
#line 57 "bitlang.org"
void bitlang_regset(bitlang *vm, int pos, int val)
{
    vm->reg[pos] = val;
}
#line 79 "bitlang.org"
void bitlang_state_init(bitlang_state *st, char *b, int sz)
{
    int i;
    st->bytes = b;
    st->sz = sz;
    st->len = 0;

    for (i = 0; i < sz; i++) {
        st->bytes[i] = 0;
    }
}
#line 99 "bitlang.org"
void bitlang_init(bitlang *vm)
{
    int i;

    for (i = 0; i < 8; i++) {
        vm->stk[i] = 0;
    }

    vm->stkpos = -1;

    for (i = 0; i < 8; i++) {
        vm->reg[i] = 0;
    }

    vm->err = 0;
}
#line 124 "bitlang.org"
int bitlang_pop(bitlang *vm, int *x)
{
    if (vm->stkpos < 0) {
        vm->err = 1;
        return 1;
    }

    *x = vm->stk[vm->stkpos];
    vm->stkpos--;
    return 0;
}
#line 144 "bitlang.org"
int bitlang_push(bitlang *vm, int x)
{
    if (vm->stkpos >= 8) return 1;

    vm->stkpos++;
    vm->stk[vm->stkpos] = x;

    return 0;
}
#line 165 "bitlang.org"
int bitlang_num(bitlang_state *st, int num)
{
    num &= 0x7f;

    if (st->len >= st->sz) return 1;

    st->bytes[st->len] = 0x80 | num;
    st->len++;
    return 0;
}
#line 189 "bitlang.org"
int bitlang_add(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_ADD;
    st->len++;
    return 0;
}
#line 232 "bitlang.org"
int bitlang_sub(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_SUB;
    st->len++;
    return 0;
}
#line 275 "bitlang.org"
int bitlang_mul(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_MUL;
    st->len++;
    return 0;
}
#line 318 "bitlang.org"
int bitlang_div(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_DIV;
    st->len++;
    return 0;
}
#line 364 "bitlang.org"
int bitlang_get(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_GET;
    st->len++;
    return 0;
}
#line 458 "bitlang.org"
int bitlang_mod(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_MOD;
    st->len++;
    return 0;
}
#line 502 "bitlang.org"
int bitlang_eq(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_EQ;
    st->len++;
    return 0;
}
#line 545 "bitlang.org"
int bitlang_lshift(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LSHIFT;
    st->len++;
    return 0;
}
#line 588 "bitlang.org"
int bitlang_rshift(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_RSHIFT;
    st->len++;
    return 0;
}
#line 631 "bitlang.org"
int bitlang_lor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LOR;
    st->len++;
    return 0;
}
#line 674 "bitlang.org"
int bitlang_bor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BOR;
    st->len++;
    return 0;
}
#line 717 "bitlang.org"
int bitlang_band(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BAND;
    st->len++;
    return 0;
}
#line 760 "bitlang.org"
int bitlang_xor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_XOR;
    st->len++;
    return 0;
}
#line 803 "bitlang.org"
int bitlang_bnot(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BNOT;
    st->len++;
    return 0;
}
#line 844 "bitlang.org"
int bitlang_lnot(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LNOT;
    st->len++;
    return 0;
}
#line 885 "bitlang.org"
int bitlang_abs(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_ABS;
    st->len++;
    return 0;
}
#line 921 "bitlang.org"
void bitlang_reset(bitlang *vm)
{
    vm->stkpos = -1;
}
#line 934 "bitlang.org"
int bitlang_exec(bitlang *vm, bitlang_state *st)
{
    int pos;
    int sz;
    char *bytes;
    int rc;

    pos = 0;
    rc = 0;

    sz = st->sz;
    bytes = st->bytes;

    while (pos < sz) {
        char c;

        c = bytes[pos];

        if (c & 0x80) {
            rc = bitlang_push(vm, c & 0x7f);
            if (rc) return rc;
            pos++;
            continue;
        }

        switch(c) {
#line 200 "bitlang.org"
case BITLANG_ADD: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x + y);
    if (rc) return rc;
    pos++;
    break;
}
#line 243 "bitlang.org"
case BITLANG_SUB: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x - y);
    if (rc) return rc;
    pos++;
    break;
}
#line 286 "bitlang.org"
case BITLANG_MUL: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x * y);
    if (rc) return rc;
    pos++;
    break;
}
#line 329 "bitlang.org"
case BITLANG_DIV: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    if (y == 0) return 1;
    rc = bitlang_push(vm, x / y);
    if (rc) return rc;
    pos++;
    break;
}
#line 375 "bitlang.org"
case BITLANG_GET: {
    int rp;
    rc = bitlang_pop(vm, &rp);
    if (rc) return rc;
    if (rp < 0 || rp >= 8) return 1;
    rc = bitlang_push(vm, vm->reg[rp]);
    if (rc) return rc;
    pos++;
    break;
}
#line 469 "bitlang.org"
case BITLANG_MOD: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    if (y == 0) rc = bitlang_push(vm, 0);
    else rc = bitlang_push(vm, x % y);
    if (rc) return rc;
    pos++;
    break;
}
#line 513 "bitlang.org"
case BITLANG_EQ: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x == y);
    if (rc) return rc;
    pos++;
    break;
}
#line 556 "bitlang.org"
case BITLANG_LSHIFT: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x << y);
    if (rc) return rc;
    pos++;
    break;
}
#line 599 "bitlang.org"
case BITLANG_RSHIFT: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x >> y);
    if (rc) return rc;
    pos++;
    break;
}
#line 642 "bitlang.org"
case BITLANG_LOR: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x || y);
    if (rc) return rc;
    pos++;
    break;
}
#line 685 "bitlang.org"
case BITLANG_BOR: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x | y);
    if (rc) return rc;
    pos++;
    break;
}
#line 728 "bitlang.org"
case BITLANG_BAND: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x & y);
    if (rc) return rc;
    pos++;
    break;
}
#line 771 "bitlang.org"
case BITLANG_XOR: {
    int x, y;
    rc = bitlang_pop(vm, &y);
    if (rc) return rc;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, x ^ y);
    if (rc) return rc;
    pos++;
    break;
}
#line 814 "bitlang.org"
case BITLANG_BNOT: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, ~x);
    if (rc) return rc;
    pos++;
    break;
}
#line 855 "bitlang.org"
case BITLANG_LNOT: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, !x);
    if (rc) return rc;
    pos++;
    break;
}
#line 896 "bitlang.org"
case BITLANG_ABS: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, abs(x));
    if (rc) return rc;
    pos++;
    break;
}
#line 961 "bitlang.org"
            default:
                pos++;
                break;
        }
    }
    return 0;
}
#line 979 "bitlang.org"
static int match(const char *str1, int sz1,
                 const char *str2, int sz2)
{
    int n;
    if (sz1 != sz2) return 0;

    for (n = 0; n < sz1; n++)
        if (str1[n] != str2[n]) return 0;

    return 1;
}

static int isnum(const char *str) {
    char c;

    c = str[0] - '0';

    if (c >= 1 && c <= 9) return 1;

    return 0;
}

static int mknum(const char *str, int len) {
    int x;
    int i;

    x = 0;

    for (i = 0; i < len; i++) {
        int c;

        c = str[i] - '0';

        if (c >= 0 && c <= 9) {
            x *= 10;
            x += c;
        }
    }

    return x;
}

static int tokenize(bitlang_state *st,
                    const char *code,
                    int b, int e)
{
    int len;
    const char *str;

    len = (e - b) + 1;

    str = &code[b];

    if (isnum(str)) {
        return bitlang_num(st, mknum(str, len));
    }
#line 215 "bitlang.org"
else if (match(str, len, "+", 1)) {
    return bitlang_add(st);
}
#line 258 "bitlang.org"
else if (match(str, len, "-", 1)) {
    return bitlang_sub(st);
}
#line 301 "bitlang.org"
else if (match(str, len, "*", 1)) {
    return bitlang_mul(st);
}
#line 345 "bitlang.org"
else if (match(str, len, "/", 1)) {
    return bitlang_div(st);
}
#line 389 "bitlang.org"
else if (match(str, len, "get", 3)) {
    return bitlang_get(st);
}
#line 398 "bitlang.org"
else if (match(str, len, "x", 1)) {
    int rc;
    rc = bitlang_num(st, 0);
    if (rc) return rc;
    return bitlang_get(st);
}
#line 408 "bitlang.org"
else if (match(str, len, "y", 1)) {
    int rc;
    rc = bitlang_num(st, 1);
    if (rc) return rc;
    return bitlang_get(st);
}
#line 418 "bitlang.org"
else if (match(str, len, "w", 1)) {
    int rc;
    rc = bitlang_num(st, 2);
    if (rc) return rc;
    return bitlang_get(st);
}
#line 428 "bitlang.org"
else if (match(str, len, "h", 1)) {
    int rc;
    rc = bitlang_num(st, 3);
    if (rc) return rc;
    return bitlang_get(st);
}
#line 438 "bitlang.org"
else if (match(str, len, "t", 1)) {
    int rc;
    rc = bitlang_num(st, 4);
    if (rc) return rc;
    return bitlang_get(st);
}
#line 485 "bitlang.org"
else if (match(str, len, "%", 1)) {
    return bitlang_mod(st);
}
#line 528 "bitlang.org"
else if (match(str, len, "=", 1)) {
    return bitlang_eq(st);
}
#line 571 "bitlang.org"
else if (match(str, len, "<<", 2)) {
    return bitlang_lshift(st);
}
#line 614 "bitlang.org"
else if (match(str, len, ">>", 2)) {
    return bitlang_rshift(st);
}
#line 657 "bitlang.org"
else if (match(str, len, "||", 2)) {
    return bitlang_lor(st);
}
#line 700 "bitlang.org"
else if (match(str, len, "|", 1)) {
    return bitlang_bor(st);
}
#line 743 "bitlang.org"
else if (match(str, len, "&", 1)) {
    return bitlang_band(st);
}
#line 786 "bitlang.org"
else if (match(str, len, "^", 1)) {
    return bitlang_xor(st);
}
#line 827 "bitlang.org"
else if (match(str, len, "~", 1)) {
    return bitlang_bnot(st);
}
#line 868 "bitlang.org"
else if (match(str, len, "!", 1)) {
    return bitlang_lnot(st);
}
#line 909 "bitlang.org"
else if (match(str, len, "abs", 3)) {
    return bitlang_abs(st);
}
#line 1036 "bitlang.org"

    return 1;
}

int bitlang_compile(bitlang_state *st, const char *code)
{
    int b, e;
    int sz;
    int n;
    int mode;

    sz = strlen(code);

    b = 0;
    e = 0;

    mode = 0;

    n = 0;

    while (n < sz) {
        char c;

        c = code[n];
        if (mode == 0) {
            if (c == ' ') {
                n++;
            } else {
                b = n;
                mode = 1;
                n++;
            }
        } else if (mode == 1) {
            if (c == ' ') {
                e = n - 1;

                mode = 0;
                tokenize(st, code, b, e);
            }
            n++;
        } else {
            n++;
        }
    }

    if (b < sz) {
        e = sz - 1;
        tokenize(st, code, b, e);
    }
    return 0;
}
#line 35 "bitlang.org"
