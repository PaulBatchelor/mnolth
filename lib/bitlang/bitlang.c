#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BITLANG_PRIV
#include "bitlang.h"
enum {
BITLANG_NOP,
BITLANG_ADD,
BITLANG_SUB,
BITLANG_MUL,
BITLANG_DIV,
BITLANG_GET,
BITLANG_MOD,
BITLANG_EQ,
BITLANG_LSHIFT,
BITLANG_RSHIFT,
BITLANG_LOR,
BITLANG_BOR,
BITLANG_BAND,
BITLANG_XOR,
BITLANG_BNOT,
BITLANG_LNOT,
BITLANG_ABS,
BITLANG_NEG,
BITLANG_END
};
void bitlang_regset(bitlang *vm, int pos, int val)
{
    vm->reg[pos] = val;
}
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
int bitlang_push(bitlang *vm, int x)
{
    if (vm->stkpos >= 8) return 1;

    vm->stkpos++;
    vm->stk[vm->stkpos] = x;

    return 0;
}
int bitlang_num(bitlang_state *st, int num)
{
    num &= 0x7f;

    if (st->len >= st->sz) return 1;

    st->bytes[st->len] = 0x80 | num;
    st->len++;
    return 0;
}
int bitlang_add(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_ADD;
    st->len++;
    return 0;
}
int bitlang_sub(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_SUB;
    st->len++;
    return 0;
}
int bitlang_mul(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_MUL;
    st->len++;
    return 0;
}
int bitlang_div(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_DIV;
    st->len++;
    return 0;
}
int bitlang_get(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_GET;
    st->len++;
    return 0;
}
int bitlang_mod(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_MOD;
    st->len++;
    return 0;
}
int bitlang_eq(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_EQ;
    st->len++;
    return 0;
}
int bitlang_lshift(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LSHIFT;
    st->len++;
    return 0;
}
int bitlang_rshift(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_RSHIFT;
    st->len++;
    return 0;
}
int bitlang_lor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LOR;
    st->len++;
    return 0;
}
int bitlang_bor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BOR;
    st->len++;
    return 0;
}
int bitlang_band(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BAND;
    st->len++;
    return 0;
}
int bitlang_xor(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_XOR;
    st->len++;
    return 0;
}
int bitlang_bnot(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_BNOT;
    st->len++;
    return 0;
}
int bitlang_lnot(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_LNOT;
    st->len++;
    return 0;
}
int bitlang_abs(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_ABS;
    st->len++;
    return 0;
}
int bitlang_neg(bitlang_state *st)
{
    if (st->len >= st->sz) return 1;
    st->bytes[st->len] = BITLANG_NEG;
    st->len++;
    return 0;
}
void bitlang_reset(bitlang *vm)
{
    vm->stkpos = -1;
}
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
case BITLANG_BNOT: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, ~x);
    if (rc) return rc;
    pos++;
    break;
}
case BITLANG_LNOT: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, !x);
    if (rc) return rc;
    pos++;
    break;
}
case BITLANG_ABS: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, abs(x));
    if (rc) return rc;
    pos++;
    break;
}
case BITLANG_NEG: {
    int x;
    rc = bitlang_pop(vm, &x);
    if (rc) return rc;
    rc = bitlang_push(vm, -x);
    if (rc) return rc;
    pos++;
    break;
}
            default:
                pos++;
                break;
        }
    }
    return 0;
}
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
else if (match(str, len, "+", 1)) {
    return bitlang_add(st);
}
else if (match(str, len, "-", 1)) {
    return bitlang_sub(st);
}
else if (match(str, len, "*", 1)) {
    return bitlang_mul(st);
}
else if (match(str, len, "/", 1)) {
    return bitlang_div(st);
}
else if (match(str, len, "get", 3)) {
    return bitlang_get(st);
}
else if (match(str, len, "x", 1)) {
    int rc;
    rc = bitlang_num(st, 0);
    if (rc) return rc;
    return bitlang_get(st);
}
else if (match(str, len, "y", 1)) {
    int rc;
    rc = bitlang_num(st, 1);
    if (rc) return rc;
    return bitlang_get(st);
}
else if (match(str, len, "w", 1)) {
    int rc;
    rc = bitlang_num(st, 2);
    if (rc) return rc;
    return bitlang_get(st);
}
else if (match(str, len, "h", 1)) {
    int rc;
    rc = bitlang_num(st, 3);
    if (rc) return rc;
    return bitlang_get(st);
}
else if (match(str, len, "t", 1)) {
    int rc;
    rc = bitlang_num(st, 4);
    if (rc) return rc;
    return bitlang_get(st);
}
else if (match(str, len, "%", 1)) {
    return bitlang_mod(st);
}
else if (match(str, len, "=", 1)) {
    return bitlang_eq(st);
}
else if (match(str, len, "<<", 2)) {
    return bitlang_lshift(st);
}
else if (match(str, len, ">>", 2)) {
    return bitlang_rshift(st);
}
else if (match(str, len, "||", 2)) {
    return bitlang_lor(st);
}
else if (match(str, len, "|", 1)) {
    return bitlang_bor(st);
}
else if (match(str, len, "&", 1)) {
    return bitlang_band(st);
}
else if (match(str, len, "^", 1)) {
    return bitlang_xor(st);
}
else if (match(str, len, "~", 1)) {
    return bitlang_bnot(st);
}
else if (match(str, len, "!", 1)) {
    return bitlang_lnot(st);
}
else if (match(str, len, "abs", 3)) {
    return bitlang_abs(st);
}
else if (match(str, len, "neg", 3)) {
    return bitlang_neg(st);
}

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
