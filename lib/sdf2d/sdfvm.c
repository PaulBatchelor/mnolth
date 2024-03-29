#include <string.h>
#include <stdio.h>
#include "mathc/mathc.h"
#include "sdf.h"
#define SDF2D_SDFVM_PRIV
#include "sdfvm.h"

size_t sdfvm_sizeof(void)
{
    return sizeof(sdfvm);
}

static void zero_out_stacklet(sdfvm_stacklet *s)
{
    s->type = SDFVM_NONE;

    /* zeroing out union with brute-force */
    s->data.s = 0;
    s->data.v2 = svec2_zero();
    s->data.v3 = svec3_zero();
}

void sdfvm_init(sdfvm *vm)
{
    int i;

    vm->stackpos = 0;

    for (i = 0; i < SDFVM_STACKSIZE; i++) {
        sdfvm_stacklet *s;
        s = &vm->stack[i];
        zero_out_stacklet(s);
    }

    for (i = 0; i < SDFVM_NREGISTERS; i++) {
        sdfvm_stacklet *s;
        s = &vm->registers[i];
        zero_out_stacklet(s);
    }

    vm->p = svec2_zero();
    vm->color = svec3_zero();
    vm->uniforms = NULL;
    vm->nuniforms = 0;
    vm->pos = 0;
    vm->lastop = -1;
}

static int get_stacklet(sdfvm *vm, sdfvm_stacklet **sp)
{
    sdfvm_stacklet *s;

    if (vm->stackpos >= SDFVM_STACKSIZE) return SDFVM_STACK_OVERFLOW;
    s = &vm->stack[vm->stackpos];

    vm->stackpos++;
    *sp = s;
    return 0;
}

int sdfvm_push_scalar(sdfvm *vm, float s)
{
    sdfvm_stacklet *stk;
    int rc;

    rc = get_stacklet(vm, &stk);
    if (rc) return SDFVM_STACK_OVERFLOW;

    stk->type = SDFVM_SCALAR;
    stk->data.s = s;
    return 0;
}

int sdfvm_push_vec2(sdfvm *vm, struct vec2 v)
{
    sdfvm_stacklet *stk;
    int rc;

    rc = get_stacklet(vm, &stk);
    if (rc) return rc;

    stk->type = SDFVM_VEC2;
    stk->data.v2 = v;
    return 0;
}

int sdfvm_push_vec3(sdfvm *vm, struct vec3 v)
{
    sdfvm_stacklet *stk;
    int rc;

    rc = get_stacklet(vm, &stk);
    if (rc) return rc;

    stk->type = SDFVM_VEC3;
    stk->data.v3 = v;
    return 0;
}

int sdfvm_pop_scalar(sdfvm *vm, float *s)
{
    sdfvm_stacklet *stk;

    if (vm->stackpos <= 0) return SDFVM_STACK_UNDERFLOW;
    stk = &vm->stack[vm->stackpos - 1];

    if (stk->type != SDFVM_SCALAR) return SDFVM_WRONG_TYPE;

    *s = stk->data.s;
    vm->stackpos--;

    return 0;
}

int sdfvm_pop_vec2(sdfvm *vm, struct vec2 *v)
{
    sdfvm_stacklet *stk;

    if (vm->stackpos <= 0) return 1;
    stk = &vm->stack[vm->stackpos - 1];

    if (stk->type != SDFVM_VEC2) return 2;

    *v = stk->data.v2;
    vm->stackpos--;
    return 0;
}

int sdfvm_pop_vec3(sdfvm *vm, struct vec3 *v)
{
    sdfvm_stacklet *stk;

    if (vm->stackpos <= 0) return 1;
    stk = &vm->stack[vm->stackpos - 1];

    if (stk->type != SDFVM_VEC3) return 2;

    *v = stk->data.v3;
    vm->stackpos--;
    return 0;
}

int sdfvm_swap(sdfvm *vm)
{
    sdfvm_stacklet a, b;
    if (vm->stackpos < 2) return 1;
    a = vm->stack[vm->stackpos - 1];
    b = vm->stack[vm->stackpos - 2];

    vm->stack[vm->stackpos - 1] = b;
    vm->stack[vm->stackpos - 2] = a;
    return 0;
}

int sdfvm_circle(sdfvm *vm) 
{
    int rc;
    struct vec2 p;
    float d;
    float r;

    rc = 0;

    rc = sdfvm_pop_scalar(vm, &r);
    if (rc) return rc;
    rc = sdfvm_pop_vec2(vm, &p);
    if (rc) return rc;

    d = sdf_circle(p, r);

    rc = sdfvm_push_scalar(vm, d);

    return rc;
}

int sdfvm_poly4(sdfvm *vm)
{
    int rc;
    struct vec2 p;
    struct vec2 points[4];
    float d;
    int i;

    rc = 0;

    for (i = 3; i >= 0; i--) {
        rc = sdfvm_pop_vec2(vm, &points[i]);
        if (rc) return rc;
    }

    rc = sdfvm_pop_vec2(vm, &p);
    if (rc) return rc;

    d = sdf_polygon(points, 4, p);

    rc = sdfvm_push_scalar(vm, d);

    return rc;
}

static int print_stackpos(sdfvm *vm)
{
    printf("stackpos: %d\n", vm->stackpos);
    return 0;
}

int sdfvm_roundness(sdfvm *vm)
{
    float d, r;
    int rc;
    rc = sdfvm_pop_scalar(vm, &r);
    if (rc) return rc;

    rc = sdfvm_pop_scalar(vm, &d);
    if (rc) return rc;

    rc = sdfvm_push_scalar(vm, d - r);
    if (rc) return rc;

    return 0;
}

static float smoothstep(float e0, float e1, float x)
{
    float t;
    t = clampf((x - e0) / (e1 - e0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}


static float feather(float d, float amt)
{
    float alpha;
    alpha = 0;
    alpha = sdf_sign(d) > 0;
    alpha += smoothstep(amt, 0.0, fabs(d));
    alpha = clampf(alpha, 0, 1);
    return alpha;
}

int sdfvm_feather(sdfvm *vm) 
{
    float d, amt, f;
    int rc;
    rc = sdfvm_pop_scalar(vm, &amt);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d);
    if (rc) return rc;

    f = feather(d, amt);
    rc = sdfvm_push_scalar(vm, f);
    if (rc) return rc;

    return 0;
}

int sdfvm_lerp3(sdfvm *vm) 
{
    float alpha;
    struct vec3 v[2];
    struct vec3 out;

    int rc;

    rc = sdfvm_pop_vec3(vm, &v[1]);
    if (rc) return rc;
    rc = sdfvm_pop_vec3(vm, &v[0]);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &alpha);
    if (rc) return rc;

    out = svec3_lerp(v[0], v[1], alpha);

    rc = sdfvm_push_vec3(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_mul(sdfvm *vm) 
{
    float x, y;
    int rc;

    rc = sdfvm_pop_scalar(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &x);
    if (rc) return rc;

    rc = sdfvm_push_scalar(vm, x * y);
    if (rc) return rc;

    return 0;
}

int sdfvm_mul2(sdfvm *vm) 
{
    struct vec2 x, y, out;
    int rc;

    rc = sdfvm_pop_vec2(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_vec2(vm, &x);
    if (rc) return rc;

    out = svec2_multiply(x, y);
    rc = sdfvm_push_vec2(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_add2(sdfvm *vm) 
{
    struct vec2 x, y, out;
    int rc;

    rc = sdfvm_pop_vec2(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_vec2(vm, &x);
    if (rc) return rc;

    out = svec2_add(x, y);
    rc = sdfvm_push_vec2(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_add(sdfvm *vm) 
{
    float x, y;
    int rc;

    rc = sdfvm_pop_scalar(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &x);
    if (rc) return rc;

    rc = sdfvm_push_scalar(vm, x * y);
    if (rc) return rc;

    return 0;
}

int sdfvm_lerp(sdfvm *vm) 
{
    float x, y, a, out;
    int rc;

    rc = sdfvm_pop_scalar(vm, &a);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &x);
    if (rc) return rc;

    out = a*y + (1 - a)*x;
    rc = sdfvm_push_scalar(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_gtz(sdfvm *vm)
{
    float d;
    int rc;

    rc = sdfvm_pop_scalar(vm, &d);
    if (rc) return rc;
    rc = sdfvm_push_scalar(vm, d > 0.0);
    if (rc) return rc;

    return 0;
}

int sdfvm_normalize(sdfvm *vm)
{
    struct vec2 x, y;
    struct vec2 out;
    int rc;

    rc = sdfvm_pop_vec2(vm, &y);
    if (rc) return rc;
    rc = sdfvm_pop_vec2(vm, &x);
    if (rc) return rc;

    out = sdf_normalize(x, y);

    rc = sdfvm_push_vec2(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_onion(sdfvm *vm)
{
    int rc;
    float r, d, out;

    rc = sdfvm_pop_scalar(vm, &r);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d);
    if (rc) return rc;

    out = sdf_onion(d, r);
    rc = sdfvm_push_scalar(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_union(sdfvm *vm)
{
    int rc;
    float d1, d2, out;

    rc = sdfvm_pop_scalar(vm, &d2);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d1);
    if (rc) return rc;

    out = sdf_union(d1, d2);
    rc = sdfvm_push_scalar(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_union_smooth(sdfvm *vm)
{
    int rc;
    float d1, d2, k, out;

    rc = sdfvm_pop_scalar(vm, &k);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d2);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d1);
    if (rc) return rc;

    out = sdf_union_smooth(d1, d2, k);
    rc = sdfvm_push_scalar(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_subtract(sdfvm *vm)
{
    int rc;
    float d1, d2, out;

    rc = sdfvm_pop_scalar(vm, &d2);
    if (rc) return rc;
    rc = sdfvm_pop_scalar(vm, &d1);
    if (rc) return rc;

    out = sdf_subtract(d1, d2);
    rc = sdfvm_push_scalar(vm, out);
    if (rc) return rc;

    return 0;
}

int sdfvm_ellipse(sdfvm *vm)
{
    int rc;
    struct vec2 p, ab;
    float d;

    rc = 0;

    rc = sdfvm_pop_vec2(vm, &ab);
    if (rc) return rc;
    rc = sdfvm_pop_vec2(vm, &p);
    if (rc) return rc;

    d = sdf_ellipse(p, ab);

    rc = sdfvm_push_scalar(vm, d);

    return rc;
}

void sdfvm_point_set(sdfvm *vm, struct vec2 p)
{
    vm->p = p;
}

struct vec2 sdfvm_point_get(sdfvm *vm)
{
    return vm->p;
}

void sdfvm_color_set(sdfvm *vm, struct vec3 color)
{
    vm->color = color;
}

struct vec3 sdfvm_color_get(sdfvm *vm)
{
    return vm->color;
}

void sdfvm_uniforms(sdfvm *vm, sdfvm_stacklet *reg, int nreg)
{
    vm->uniforms = reg;
    vm->nuniforms = nreg;
}

int sdfvm_uniget(sdfvm *vm, int pos, sdfvm_stacklet *out)
{
    if (pos < 0 || pos >= vm->nuniforms) return SDFVM_OUT_OF_BOUNDS;
    *out = vm->uniforms[pos];
    return 0;
}

int sdfvm_uniset(sdfvm *vm, int pos, sdfvm_stacklet reg)
{
    if (pos < 0 || pos >= vm->nuniforms) return 1;
    vm->uniforms[pos] = reg;
    return 0;
}

int sdfvm_uniset_scalar(sdfvm *vm, int pos, float s)
{
    sdfvm_stacklet stk;

    stk.type = SDFVM_SCALAR;
    stk.data.s = s;

    return sdfvm_uniset(vm, pos, stk);
}

int sdfvm_uniset_vec2(sdfvm *vm, int pos, struct vec2 v)
{
    sdfvm_stacklet stk;

    stk.type = SDFVM_VEC2;
    stk.data.v2 = v;

    return sdfvm_uniset(vm, pos, stk);
}

int sdfvm_uniset_vec3(sdfvm *vm, int pos, struct vec3 v)
{
    sdfvm_stacklet stk;

    stk.type = SDFVM_VEC3;
    stk.data.v3 = v;

    return sdfvm_uniset(vm, pos, stk);
}

int sdfvm_uniform(sdfvm *vm)
{
    float fpos;
    int pos;
    sdfvm_stacklet *stk;
    int rc;

    pos = fpos = 0;
    rc = sdfvm_pop_scalar(vm, &fpos);
    if (rc) return rc;
    pos = (int)fpos;

    rc = get_stacklet(vm, &stk);
    if (rc) return rc;
    rc = sdfvm_uniget(vm, pos, stk);
    if (rc) return rc;

    return 0;
}

int sdfvm_register_get(sdfvm *vm, int pos, sdfvm_stacklet *out)
{
    if (pos < 0 || pos >= SDFVM_NREGISTERS) return 1;
    *out = vm->registers[pos];
    return 0;
}

int sdfvm_regget(sdfvm *vm)
{
    float fpos;
    int pos;
    sdfvm_stacklet *stk;
    int rc;

    pos = fpos = 0;
    rc = sdfvm_pop_scalar(vm, &fpos);
    if (rc) return rc;
    pos = (int)fpos;

    rc = get_stacklet(vm, &stk);
    if (rc) return rc;
    rc = sdfvm_register_get(vm, pos, stk);
    if (rc) return rc;

    return 0;
}

int sdfvm_register_set(sdfvm *vm, int pos, sdfvm_stacklet val)
{
    if (pos < 0 || pos >= SDFVM_NREGISTERS) return 1;
    vm->registers[pos] = val;
    return 0;
}

int sdfvm_regset(sdfvm *vm)
{
    float fpos;
    int pos;
    sdfvm_stacklet *stk;
    int rc;

    pos = fpos = 0;
    rc = sdfvm_pop_scalar(vm, &fpos);
    if (rc) return rc;
    pos = (int)fpos;
    if (vm->stackpos <= 0) return 1;
    stk = &vm->stack[vm->stackpos - 1];
    vm->stackpos--;

    rc = sdfvm_register_set(vm, pos, *stk);
    if (rc) return rc;

    return 0;
}

static int get_float(const uint8_t *program,
                     size_t sz,
                     size_t *n,
                     float *out)
{
    uint8_t tmp[4];
    uint8_t pos;
    float *f;
    int i;

    pos = *n;
    if ((sz - pos) < 4) return SDFVM_OUT_OF_BOUNDS;

    for (i = 0; i < 4; i++) {
        tmp[i] = program[pos + i];
    }

    f = (float *)tmp;

    *n = pos + 4;
    *out = *f;
    return 0;
}

int sdfvm_execute(sdfvm *vm,
                  const uint8_t *program,
                  size_t sz)
{
    size_t n;
    float f[3];

    if (sz <= 0) return 2;

    n = 0;
    f[0] = f[1] = f[2] = 0;
    vm->pos = 0;
    vm->lastop = -1;

    while (n < sz) {
        uint8_t c;
        int rc;

        c = program[n];
        vm->lastop = c;
        vm->pos++;
        switch(c) {
            case SDF_OP_POINT:
                n++;
                rc = sdfvm_push_vec2(vm, sdfvm_point_get(vm));
                if (rc) return rc;
                break;
            case SDF_OP_SWAP:
                n++;
                rc = sdfvm_swap(vm);
                if (rc) return rc;
                break;
            case SDF_OP_UNIFORM:
                n++;
                rc = sdfvm_uniform(vm);
                if (rc) return rc;
                break;
            case SDF_OP_REGGET:
                n++;
                rc = sdfvm_regget(vm);
                if (rc) return rc;
                break;
            case SDF_OP_REGSET:
                n++;
                rc = sdfvm_regset(vm);
                if (rc) return rc;
                break;
            case SDF_OP_COLOR:
                n++;
                rc = sdfvm_push_vec3(vm, sdfvm_color_get(vm));
                if (rc) return rc;
                break;
            case SDF_OP_SCALAR:
                n++;
                rc = get_float(program, sz, &n, &f[0]);
                if (rc) return rc;
                rc = sdfvm_push_scalar(vm, f[0]);
                if (rc) return rc;
                break;
            case SDF_OP_VEC2:
                n++;
                rc = get_float(program, sz, &n, &f[0]);
                if (rc) return rc;
                rc = get_float(program, sz, &n, &f[1]);
                if (rc) return rc;
                rc = sdfvm_push_vec2(vm, svec2(f[0], f[1]));
                if (rc) return rc;
                break;
            case SDF_OP_VEC3:
                n++;
                rc = get_float(program, sz, &n, &f[0]);
                if (rc) return rc;
                rc = get_float(program, sz, &n, &f[1]);
                if (rc) return rc;
                rc = get_float(program, sz, &n, &f[2]);
                if (rc) return rc;
                rc = sdfvm_push_vec3(vm, svec3(f[0], f[1], f[2]));
                if (rc) return rc;
                break;
            case SDF_OP_CIRCLE:
                n++;
                rc = sdfvm_circle(vm);
                if (rc) return rc;
                break;
            case SDF_OP_POLY4:
                n++;
                rc = sdfvm_poly4(vm);
                if (rc) return rc;
                break;
            case SDF_OP_ROUNDNESS:
                n++;
                rc = sdfvm_roundness(vm);
                if (rc) return rc;
                break;
            case SDF_OP_FEATHER:
                n++;
                rc = sdfvm_feather(vm);
                if (rc) return rc;
                break;
            case SDF_OP_LERP3:
                n++;
                rc = sdfvm_lerp3(vm);
                if (rc) return rc;
                break;
            case SDF_OP_MUL:
                n++;
                rc = sdfvm_mul(vm);
                if (rc) return rc;
                break;
            case SDF_OP_MUL2:
                n++;
                rc = sdfvm_mul2(vm);
                if (rc) return rc;
                break;
            case SDF_OP_ADD:
                n++;
                rc = sdfvm_add(vm);
                if (rc) return rc;
                break;
            case SDF_OP_ADD2:
                n++;
                rc = sdfvm_add2(vm);
                if (rc) return rc;
                break;
            case SDF_OP_LERP:
                n++;
                rc = sdfvm_lerp(vm);
                if (rc) return rc;
                break;
            case SDF_OP_GTZ:
                n++;
                rc = sdfvm_gtz(vm);
                if (rc) return rc;
                break;
            case SDF_OP_NORMALIZE:
                n++;
                rc = sdfvm_normalize(vm);
                if (rc) return rc;
                break;
            case SDF_OP_ONION:
                n++;
                rc = sdfvm_onion(vm);
                if (rc) return rc;
                break;
            case SDF_OP_UNION:
                n++;
                rc = sdfvm_union(vm);
                if (rc) return rc;
                break;
            case SDF_OP_UNION_SMOOTH:
                n++;
                rc = sdfvm_union_smooth(vm);
                if (rc) return rc;
                break;
            case SDF_OP_SUBTRACT:
                n++;
                rc = sdfvm_subtract(vm);
                if (rc) return rc;
                break;
            case SDF_OP_ELLIPSE:
                n++;
                rc = sdfvm_ellipse(vm);
                if (rc) return rc;
                break;
            case SDF_OP_STACKPOS:
                n++;
                rc = print_stackpos(vm);
                if (rc) return rc;
                break;
            default:
                return SDFVM_UNKNOWN;
        }
    }

    return 0;
}

const char *sdfvm_errors[] = {
    /* SDFVM_OK, */
    "okay!",
    /* SDFVM_NOT_OK, */
    "not okay",
    /* SDFVM_STACK_UNDERFLOW, */
    "stack underflow",
    /* SDFVM_STACK_OVERFLOW, */
    "stack overflow",
    /* SDFVM_WRONG_TYPE, */
    "wrong type",
    /* SDFVM_OUT_OF_BOUNDS, */
    "wrong type",
    /* SDFVM_UNKNOWN, */
    "unknown opcode",
    /* SDFVM_NOTHING */
    "this error shouldn't happen",
};

void sdfvm_print_lookup_table(FILE *fp)
{
    if (fp == NULL) fp = stdout;
    fprintf(fp, "{\n");
    fprintf(fp, "    \"none\": %d,\n", SDF_OP_NONE);
    fprintf(fp, "    \"point\": %d,\n", SDF_OP_POINT);
    fprintf(fp, "    \"color\": %d,\n", SDF_OP_COLOR);
    fprintf(fp, "    \"scalar\": %d,\n", SDF_OP_SCALAR);
    fprintf(fp, "    \"vec2\": %d,\n", SDF_OP_VEC2);
    fprintf(fp, "    \"vec3\": %d,\n", SDF_OP_VEC3);
    fprintf(fp, "    \"uniform\": %d,\n", SDF_OP_UNIFORM);
    fprintf(fp, "    \"circle\": %d,\n", SDF_OP_CIRCLE);
    fprintf(fp, "    \"poly4\": %d,\n", SDF_OP_POLY4);
    fprintf(fp, "    \"roundness\": %d,\n", SDF_OP_ROUNDNESS);
    fprintf(fp, "    \"feather\": %d,\n", SDF_OP_FEATHER);
    fprintf(fp, "    \"lerp3\": %d,\n", SDF_OP_LERP3);
    fprintf(fp, "    \"mul\": %d,\n", SDF_OP_MUL);
    fprintf(fp, "    \"mul2\": %d,\n", SDF_OP_MUL2);
    fprintf(fp, "    \"add\": %d,\n", SDF_OP_ADD);
    fprintf(fp, "    \"add2\": %d,\n", SDF_OP_ADD2);
    fprintf(fp, "    \"lerp\": %d,\n", SDF_OP_LERP);
    fprintf(fp, "    \"gtz\": %d,\n", SDF_OP_GTZ);
    fprintf(fp, "    \"normalize\": %d,\n", SDF_OP_NORMALIZE);
    fprintf(fp, "    \"onion\": %d,\n", SDF_OP_ONION);
    fprintf(fp, "    \"union\": %d,\n", SDF_OP_UNION);
    fprintf(fp, "    \"union_smooth\": %d,\n", SDF_OP_UNION_SMOOTH);
    fprintf(fp, "    \"subtract\": %d,\n", SDF_OP_SUBTRACT);
    fprintf(fp, "    \"swap\": %d,\n", SDF_OP_SWAP);
    fprintf(fp, "    \"regget\": %d,\n", SDF_OP_REGGET);
    fprintf(fp, "    \"regset\": %d,\n", SDF_OP_REGSET);
    fprintf(fp, "    \"ellipse\": %d,\n", SDF_OP_ELLIPSE);
    fprintf(fp, "    \"stackpos\": %d,\n", SDF_OP_STACKPOS);
    fprintf(fp, "    \"end\": %d\n", SDF_OP_END);
    fprintf(fp, "}\n");
}
