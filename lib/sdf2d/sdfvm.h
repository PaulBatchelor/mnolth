#ifndef SDF2D_SDFVM_H
#define SDF2D_SDFVM_H

typedef struct sdfvm sdfvm;
typedef struct sdfvm_stacklet sdfvm_stacklet;

#ifdef SDF2D_SDFVM_PRIV
#define SDFVM_STACKSIZE 16
#define SDFVM_NREGISTERS 16
enum {
    SDFVM_NONE,
    SDFVM_SCALAR,
    SDFVM_VEC2,
    SDFVM_VEC3
};

struct sdfvm_stacklet {
    int type;
    union {
        float s;
        struct vec2 v2;
        struct vec3 v3;
    } data;
};

struct sdfvm {
    sdfvm_stacklet stack[SDFVM_STACKSIZE];
    struct vec2 p;
    struct vec3 color;
    int stackpos;
    sdfvm_stacklet *uniforms;
    int nuniforms;
    sdfvm_stacklet registers[SDFVM_NREGISTERS];
    int pos;
    int lastop;
};

enum {
    SDFVM_OK,
    SDFVM_NOT_OK,
    SDFVM_STACK_UNDERFLOW,
    SDFVM_STACK_OVERFLOW,
    SDFVM_WRONG_TYPE,
    SDFVM_OUT_OF_BOUNDS,
    SDFVM_UNKNOWN,
    SDFVM_NOTHING
};

enum {
    SDF_OP_NONE,
    SDF_OP_SWAP,
    SDF_OP_POINT,
    SDF_OP_COLOR,
    SDF_OP_SCALAR,
    SDF_OP_VEC2,
    SDF_OP_VEC3,
    SDF_OP_UNIFORM,
    SDF_OP_REGSET,
    SDF_OP_REGGET,
    SDF_OP_CIRCLE,
    SDF_OP_POLY4,
    SDF_OP_ROUNDNESS,
    SDF_OP_FEATHER,
    SDF_OP_LERP3,
    SDF_OP_MUL,
    SDF_OP_MUL2,
    SDF_OP_ADD,
    SDF_OP_ADD2,
    SDF_OP_LERP,
    SDF_OP_GTZ,
    SDF_OP_NORMALIZE,
    SDF_OP_ONION,
    SDF_OP_UNION,
    SDF_OP_UNION_SMOOTH,
    SDF_OP_SUBTRACT,
    SDF_OP_ELLIPSE,
    SDF_OP_STACKPOS,
    SDF_OP_END
};
#endif

size_t sdfvm_sizeof(void);
void sdfvm_init(sdfvm *vm);
int sdfvm_push_scalar(sdfvm *vm, float s);
int sdfvm_push_vec2(sdfvm *vm, struct vec2 v);
int sdfvm_push_vec3(sdfvm *vm, struct vec3 v);
void sdfvm_point_set(sdfvm *vm, struct vec2 p);
struct vec2 sdfvm_point_get(sdfvm *vm);
void sdfvm_color_set(sdfvm *vm, struct vec3 color);
struct vec3 sdfvm_color_get(sdfvm *vm);

int sdfvm_pop_scalar(sdfvm *vm, float *s);
int sdfvm_pop_vec2(sdfvm *vm, struct vec2 *v);
int sdfvm_pop_vec3(sdfvm *vm, struct vec3 *v);
int sdfvm_swap(sdfvm *vm);

void sdfvm_uniforms(sdfvm *vm, sdfvm_stacklet *reg, int nreg);
int sdfvm_uniset(sdfvm *vm, int pos, sdfvm_stacklet reg);
int sdfvm_uniset_scalar(sdfvm *vm, int pos, float s);
int sdfvm_uniset_vec2(sdfvm *vm, int pos, struct vec2 v);
int sdfvm_uniset_vec3(sdfvm *vm, int pos, struct vec3 v);
int sdfvm_uniget(sdfvm *vm, int pos, sdfvm_stacklet *out);
int sdfvm_register_set(sdfvm *vm, int pos, sdfvm_stacklet val);
int sdfvm_register_get(sdfvm *vm, int pos, sdfvm_stacklet *out);
int sdfvm_regset(sdfvm *vm);
int sdfvm_regget(sdfvm *vm);
int sdfvm_uniform(sdfvm *vm);

int sdfvm_circle(sdfvm *vm);
int sdfvm_poly4(sdfvm *vm);
int sdfvm_roundness(sdfvm *vm);
int sdfvm_feather(sdfvm *vm);
int sdfvm_lerp3(sdfvm *vm);
int sdfvm_mul(sdfvm *vm);
int sdfvm_mul2(sdfvm *vm);
int sdfvm_add(sdfvm *vm);
int sdfvm_lerp(sdfvm *vm);
int sdfvm_gtz(sdfvm *vm);
int sdfvm_normalize(sdfvm *vm);
int sdfvm_onion(sdfvm *vm);
int sdfvm_union(sdfvm *vm);
int sdfvm_union_smooth(sdfvm *vm);
int sdfvm_ellipse(sdfvm *vm);

int sdfvm_execute(sdfvm *vm,
                  const uint8_t *program,
                  size_t sz);

int sdfvm_dump(const uint8_t *program,
               size_t sz);

void sdfvm_print_lookup_table(FILE *fp);
#endif
