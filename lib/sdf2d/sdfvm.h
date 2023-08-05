#ifndef SDF2D_SDFVM_H
#define SDF2D_SDFVM_H

typedef struct sdfvm sdfvm;
typedef struct sdfvm_stacklet sdfvm_stacklet;

#ifdef SDF2D_SDFVM_PRIV
#define SDFVM_STACKSIZE 16
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
    int stackpos;
};
#endif

size_t sdfvm_sizeof(void);
void sdfvm_init(sdfvm *vm);
int sdfvm_push_scalar(sdfvm *vm, float s);
int sdfvm_push_vec2(sdfvm *vm, struct vec2 v);
int sdfvm_push_vec3(sdfvm *vm, struct vec3 v);

int sdfvm_pop_scalar(sdfvm *vm, float *s);
int sdfvm_pop_vec2(sdfvm *vm, struct vec2 *v);
int sdfvm_pop_vec3(sdfvm *vm, struct vec3 *v);

int sdfvm_circle(sdfvm *vm);
int sdfvm_poly4(sdfvm *vm);
int sdfvm_roundness(sdfvm *vm);
int sdfvm_feather(sdfvm *vm);
int sdfvm_lerp3(sdfvm *vm);
int sdfvm_mul(sdfvm *vm);
int sdfvm_lerp(sdfvm *vm);
int sdfvm_gtz(sdfvm *vm);
int sdfvm_normalize(sdfvm *vm);

#endif
