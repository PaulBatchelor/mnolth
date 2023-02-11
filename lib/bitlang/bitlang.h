#ifndef BITLANG_H
#define BITLANG_H
typedef struct bitlang bitlang;
typedef struct bitlang_state bitlang_state;

#ifdef BITLANG_PRIV
struct bitlang {
    int stk[8];
    int stkpos;
    int reg[8];
    int err;
};
struct bitlang_state {
    char *bytes;
    int sz;
    int len;
};
#endif

void bitlang_regset(bitlang *vm, int pos, int val);
void bitlang_state_init(bitlang_state *st, char *b, int sz);
void bitlang_init(bitlang *vm);
int bitlang_pop(bitlang *vm, int *x);
int bitlang_push(bitlang *vm, int x);
int bitlang_num(bitlang_state *st, int num);
int bitlang_add(bitlang_state *st);
int bitlang_sub(bitlang_state *st);
int bitlang_mul(bitlang_state *st);
int bitlang_div(bitlang_state *st);
int bitlang_get(bitlang_state *st);
int bitlang_mod(bitlang_state *st);
int bitlang_eq(bitlang_state *st);
int bitlang_lshift(bitlang_state *st);
int bitlang_rshift(bitlang_state *st);
int bitlang_lor(bitlang_state *st);
int bitlang_bor(bitlang_state *st);
int bitlang_band(bitlang_state *st);
int bitlang_xor(bitlang_state *st);
int bitlang_bnot(bitlang_state *st);
int bitlang_lnot(bitlang_state *st);
int bitlang_abs(bitlang_state *st);
int bitlang_neg(bitlang_state *st);
void bitlang_reset(bitlang *vm);
int bitlang_exec(bitlang *vm, bitlang_state *st);
int bitlang_compile(bitlang_state *st, const char *code);
#endif
