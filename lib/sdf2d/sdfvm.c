#include <string.h>
#include "mathc/mathc.h"
#include "sdf.h"
#define SDF2D_SDFVM_PRIV
#include "sdfvm.h"

size_t sdfvm_sizeof(void)
{
    return sizeof(sdfvm);
}

void sdfvm_init(sdfvm *vm)
{
    int i;

    vm->stackpos = 0;

    for (i = 0; i < SDFVM_STACKSIZE; i++) {
        sdfvm_stacklet *s;
        s = &vm->stack[i];
        s->type = SDFVM_NONE;

        /* zeroing out union with brute-force */
        s->data.s = 0;
        s->data.v2 = svec2_zero();
        s->data.v3 = svec3_zero();
    }
}

static int get_stacklet(sdfvm *vm, sdfvm_stacklet **sp)
{
    sdfvm_stacklet *s;

    if (vm->stackpos >= SDFVM_STACKSIZE) return 1;
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
    if (rc) return rc;

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

    if (vm->stackpos <= 0) return 1;
    stk = &vm->stack[vm->stackpos - 1];

    if (stk->type != SDFVM_SCALAR) return 2;

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
