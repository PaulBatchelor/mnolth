#line 10 "bitlang.org"
#ifndef BITLANG_H
#define BITLANG_H
typedef struct bitlang bitlang;
typedef struct bitlang_state bitlang_state;

#ifdef BITLANG_PRIV
#line 42 "bitlang.org"
struct bitlang {
    int stk[8];
    int stkpos;
    int reg[8];
    int err;
};
#line 10 "bitlang.org"
#line 65 "bitlang.org"
struct bitlang_state {
    char *bytes;
    int sz;
    int len;
};
#line 18 "bitlang.org"
#endif

#line 52 "bitlang.org"
void bitlang_regset(bitlang *vm, int pos, int val);
#line 74 "bitlang.org"
void bitlang_state_init(bitlang_state *st, char *b, int sz);
#line 94 "bitlang.org"
void bitlang_init(bitlang *vm);
#line 119 "bitlang.org"
int bitlang_pop(bitlang *vm, int *x);
#line 139 "bitlang.org"
int bitlang_push(bitlang *vm, int x);
#line 160 "bitlang.org"
int bitlang_num(bitlang_state *st, int num);
#line 184 "bitlang.org"
int bitlang_add(bitlang_state *st);
#line 227 "bitlang.org"
int bitlang_sub(bitlang_state *st);
#line 270 "bitlang.org"
int bitlang_mul(bitlang_state *st);
#line 313 "bitlang.org"
int bitlang_div(bitlang_state *st);
#line 359 "bitlang.org"
int bitlang_get(bitlang_state *st);
#line 453 "bitlang.org"
int bitlang_mod(bitlang_state *st);
#line 497 "bitlang.org"
int bitlang_eq(bitlang_state *st);
#line 540 "bitlang.org"
int bitlang_lshift(bitlang_state *st);
#line 583 "bitlang.org"
int bitlang_rshift(bitlang_state *st);
#line 626 "bitlang.org"
int bitlang_lor(bitlang_state *st);
#line 669 "bitlang.org"
int bitlang_bor(bitlang_state *st);
#line 712 "bitlang.org"
int bitlang_band(bitlang_state *st);
#line 755 "bitlang.org"
int bitlang_xor(bitlang_state *st);
#line 798 "bitlang.org"
int bitlang_bnot(bitlang_state *st);
#line 839 "bitlang.org"
int bitlang_lnot(bitlang_state *st);
#line 880 "bitlang.org"
int bitlang_abs(bitlang_state *st);
#line 916 "bitlang.org"
void bitlang_reset(bitlang *vm);
#line 929 "bitlang.org"
int bitlang_exec(bitlang *vm, bitlang_state *st);
#line 974 "bitlang.org"
int bitlang_compile(bitlang_state *st, const char *code);
#line 21 "bitlang.org"
#endif
