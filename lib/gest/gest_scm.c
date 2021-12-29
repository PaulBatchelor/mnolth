#include <stdio.h>
#include "runt.h"
#include "graforge.h"
#include "runt_graforge.h"
#include "monolith.h"
#include "s9core.h"
#include "s9import.h"
#include "s9ext.h"
#include "scheme.h"

#include "gest.h"

gest_d *gest_gf_alloc(gf_patch *patch);
gest_scalar *gest_scalar_gf_alloc(gf_patch *patch);
int gf_node_gestweight(gf_node *node, gest_d *g);
int gf_node_gesticulate(gf_node *node, gest_d *g);
int gf_node_gescalar(gf_node *node, gest_scalar *s);
int gf_node_gestick(gf_node *node, gest_scalar *s);
int gf_node_conductor(gf_node *node);

static cell pp_gest_new(cell x)
{
    gest_d *g;
    monolith_d *m;
    gf_patch *patch;
    m = monolith_data_get();

    patch = monolith_graforge_get(m);
    g = gest_gf_alloc(patch);

    return s9_make_pointer(g);
}

static cell pp_gest_begin(cell x)
{

    gest_d *g;
    int div;
    int nbeats;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    nbeats = integer_value(NULL, car(x));
    x = cdr(x);
    div = integer_value(NULL, car(x));
    x = cdr(x);

    gest_begin(g, nbeats, div);
    return UNSPECIFIC;
}

static cell pp_gest_target(cell x)
{
    gest_d *g;
    GFFLT val;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    val = REAL2FLOAT(car(x));

    gest_addtarget(g, val);
    return UNSPECIFIC;
}

static cell pp_gest_step(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_behavior_step(g);
    return UNSPECIFIC;
}

static cell pp_gest_linear(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_behavior_linear(g);
    return UNSPECIFIC;
}

static cell pp_gest_end(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_end(g);
    return UNSPECIFIC;
}

static cell pp_gest_loopit(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_loopit(g);
    return UNSPECIFIC;
}

static cell pp_gest_finish(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_finish(g);
    return UNSPECIFIC;
}

static cell pp_gest_iculate(cell x)
{
    gest_d *g;
    gf_patch *patch;
    gf_node *node;
    monolith_d *m;
    int rc;
    runt_vm *vm;
    rgf_param conductor;
    runt_stacklet *out;

    g = s9_to_pointer(car(x));

    m = monolith_data_get();
    patch = monolith_graforge_get(m);

    vm = monolith_runt_vm(m);

    rc = rgf_get_param(vm, &conductor);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Gesticulate couldn't do it\n", car(x));
    }

    rc = runt_ppush(vm, &out);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Gesticulate couldn't do it\n", car(x));
    }

    rc = gf_patch_new_node(patch, &node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not allocate node\n", car(x));
    }

    rc = gf_node_gesticulate(node, g);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not init node\n", car(x));
    }

    rgf_set_param(vm, node, &conductor, 0);
    rgf_push_output(vm, node, out, 1);

    return UNSPECIFIC;
}

static cell pp_gest_weight(cell x)
{
    gest_d *g;
    gf_patch *patch;
    gf_node *node;
    monolith_d *m;
    int rc;
    runt_vm *vm;
    runt_stacklet *out;

    g = s9_to_pointer(car(x));

    m = monolith_data_get();
    patch = monolith_graforge_get(m);

    vm = monolith_runt_vm(m);

    rc = runt_ppush(vm, &out);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Gesticulate couldn't do it\n", car(x));
    }

    rc = gf_patch_new_node(patch, &node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not allocate node\n", car(x));
    }

    rc = gf_node_gestweight(node, g);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not init node\n", car(x));
    }

    rgf_push_output(vm, node, out, 0);

    return UNSPECIFIC;
}

static cell pp_gest_polyramp(cell x)
{
    gest_d *g;
    int div;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    div = integer_value(NULL, car(x));

    gest_polyramp(g, div);
    return UNSPECIFIC;
}

static cell pp_gest_monoramp(cell x)
{
    gest_d *g;
    int nbeats;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    nbeats = integer_value(NULL, car(x));

    gest_monoramp(g, nbeats);
    return UNSPECIFIC;
}

static cell pp_gest_gliss(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_behavior_gliss(g);
    return UNSPECIFIC;
}

static cell pp_gest_smallgliss(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_behavior_smallgliss(g);
    return UNSPECIFIC;
}

static cell pp_gest_exponential(cell x)
{
    gest_d *g;
    GFFLT slope;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    slope = REAL2FLOAT(car(x));

    gest_behavior_exponential(g, slope);
    return UNSPECIFIC;
}

static cell pp_gest_bezier(cell x)
{
    gest_d *g;
    GFFLT cx, cy;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    cx = REAL2FLOAT(car(x));
    x = cdr(x);
    cy = REAL2FLOAT(car(x));

    gest_behavior_bezier(g, cx, cy);
    return UNSPECIFIC;
}

static cell pp_gest_mass(cell x)
{
    gest_d *g;
    GFFLT mass;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    mass = REAL2FLOAT(car(x));

    gest_mass(g, mass);
    return UNSPECIFIC;
}

static cell pp_gest_inertia(cell x)
{
    gest_d *g;
    GFFLT mass;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    mass = REAL2FLOAT(car(x));

    gest_inertia(g, mass);
    return UNSPECIFIC;
}

static cell pp_gest_mediumgliss(cell x)
{
    gest_d *g;
    g = s9_to_pointer(car(x));
    gest_behavior_mediumgliss(g);
    return UNSPECIFIC;
}

static cell pp_gest_metatarget(cell x)
{
    gest_d *g;
    int size;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    size = integer_value(NULL, car(x));

    gest_addmetatarget(g, size);
    return UNSPECIFIC;
}

static cell pp_gest_newscalar(cell x)
{
    gest_scalar *s;
    monolith_d *m;
    gf_patch *patch;
    m = monolith_data_get();

    patch = monolith_graforge_get(m);
    s = gest_scalar_gf_alloc(patch);

    return s9_make_pointer(s);
}

static cell pp_gest_setscalar(cell x)
{
    gest_d *g;
    gest_scalar *s;
    GFFLT val;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    s = s9_to_pointer(car(x));
    x = cdr(x);

    val = REAL2FLOAT(car(x));

    gest_setscalar(g, s, val);
    return UNSPECIFIC;
}

static cell pp_gest_scalar(cell x)
{
    gest_scalar *s;
    gf_patch *patch;
    gf_node *node;
    monolith_d *m;
    int rc;
    runt_vm *vm;
    runt_stacklet *out;

    s = s9_to_pointer(car(x));

    m = monolith_data_get();
    patch = monolith_graforge_get(m);

    vm = monolith_runt_vm(m);

    rc = runt_ppush(vm, &out);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Gescalar couldn't do it\n", car(x));
    }

    rc = gf_patch_new_node(patch, &node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not allocate node\n", car(x));
    }

    rc = gf_node_gescalar(node, s);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not init node\n", car(x));
    }

    rgf_push_output(vm, node, out, 0);

    return UNSPECIFIC;
}

static cell pp_gest_tick(cell x)
{
    gest_scalar *s;
    gf_patch *patch;
    gf_node *node;
    monolith_d *m;
    int rc;
    runt_vm *vm;
    runt_stacklet *out;

    s = s9_to_pointer(car(x));

    m = monolith_data_get();
    patch = monolith_graforge_get(m);

    vm = monolith_runt_vm(m);

    rc = runt_ppush(vm, &out);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Gescalar couldn't do it\n", car(x));
    }

    rc = gf_patch_new_node(patch, &node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not allocate node\n", car(x));
    }

    rc = gf_node_gestick(node, s);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not init node\n", car(x));
    }

    rgf_push_output(vm, node, out, 0);

    return UNSPECIFIC;
}

static cell pp_gest_metabehavior(cell x)
{
    gest_d *g;
    int size;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    size = integer_value(NULL, car(x));

    gest_addmetabehavior(g, size);
    return UNSPECIFIC;
}

static cell pp_gest_metanode(cell x)
{
    gest_d *g;
    int nbeats;
    int size;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    nbeats = integer_value(NULL, car(x));
    x = cdr(x);
    size = integer_value(NULL, car(x));

    gest_addmetanode(g, nbeats, size);
    return UNSPECIFIC;
}

static cell pp_gest_metaphrase(cell x)
{
    gest_d *g;
    int size;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    size = integer_value(NULL, car(x));

    gest_addmetaphrase(g, size);
    return UNSPECIFIC;
}

static cell pp_gest_randtarget(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_randtarget(g);
    return UNSPECIFIC;
}

static cell pp_gest_randbehavior(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_randbehavior(g);
    return UNSPECIFIC;
}

static cell pp_gest_randnode(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_randnode(g);
    return UNSPECIFIC;
}

static cell pp_gest_randphrase(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_randphrase(g);
    return UNSPECIFIC;
}

static cell pp_gest_repeat(cell x)
{
    gest_d *g;
    int ntimes;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    ntimes = integer_value(NULL, car(x));

    gest_repeat(g, ntimes);
    return UNSPECIFIC;
}

static cell pp_gest_skewexp(cell x)
{
    gest_d *g;
    int dir;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    dir = integer_value(NULL, car(x));

    gest_skewexp(g, dir);
    return UNSPECIFIC;
}

static cell pp_gest_skewshuf(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_skewshuf(g);
    return UNSPECIFIC;
}

static cell pp_gest_grow(cell x)
{
    gest_d *g;
    GFFLT amt;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    amt = REAL2FLOAT(car(x));

    gest_grow(g, amt);
    return UNSPECIFIC;
}

static cell pp_gest_shrink(cell x)
{
    gest_d *g;
    GFFLT amt;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    amt = REAL2FLOAT(car(x));

    gest_shrink(g, amt);
    return UNSPECIFIC;
}

static cell pp_gest_smoothstep(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_behavior_smoothstep(g);
    return UNSPECIFIC;
}

static cell pp_gest_ramp(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_ramp(g);
    return UNSPECIFIC;
}

static cell pp_gest_invramp(cell x)
{
    gest_d *g;

    g = s9_to_pointer(car(x));

    gest_invramp(g);
    return UNSPECIFIC;
}

static cell pp_gest_biramp(cell x)
{
    gest_d *g;
    GFFLT pos;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    pos = REAL2FLOAT(car(x));

    gest_behavior_biramp(g, pos);
    return UNSPECIFIC;
}

static cell pp_gest_gate(cell x)
{
    gest_d *g;
    GFFLT amt;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    amt = REAL2FLOAT(car(x));

    gest_behavior_gate(g, amt);
    return UNSPECIFIC;
}

static cell pp_gest_squawk(cell x)
{
    gest_d *g;
    int squawk;

    g = s9_to_pointer(car(x));
    x = cdr(x);

    squawk = integer_value(NULL, car(x));

    gest_squawk(g, squawk);
    return UNSPECIFIC;
}

static cell pp_gest_tolerance(cell x)
{
    gest_d *g;
    GFFLT tolerance;

    g = s9_to_pointer(car(x));
    x = cdr(x);
    tolerance = REAL2FLOAT(car(x));

    gest_tolerance(g, tolerance);
    return UNSPECIFIC;
}

static cell pp_gest_conductor(cell x)
{
    gf_patch *patch;
    gf_node *node;
    monolith_d *m;
    int rc;
    runt_vm *vm;
    rgf_param bpm;
    runt_stacklet *out;

    m = monolith_data_get();
    patch = monolith_graforge_get(m);

    vm = monolith_runt_vm(m);

    rc = rgf_get_param(vm, &bpm);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Condutor couldn't do it\n", car(x));
    }

    rc = runt_ppush(vm, &out);

    if (rc != RUNT_OK) {
        monolith_scheme_error("Conductor couldn't do it\n", car(x));
    }

    rc = gf_patch_new_node(patch, &node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not allocate node\n", car(x));
    }

    rc = gf_node_conductor(node);

    if (rc != GF_OK) {
        monolith_scheme_error("Could not init node\n", car(x));
    }

    rgf_set_param(vm, node, &bpm, 0);
    rgf_push_output(vm, node, out, 1);

    return UNSPECIFIC;
}

static S9_PRIM gest_primitives[] = {
    {"gest:new", pp_gest_new, 0, 0, {___,___,___}},
    {"gest:iculate", pp_gest_iculate, 1, 1, {___,___,___}},
    {"gest:weight", pp_gest_weight, 1, 1, {___,___,___}},
    {"gest:begin", pp_gest_begin, 3, 3, {___,INT,INT}},
    {"gest:end", pp_gest_end, 1, 1, {___,___,___}},
    {"gest:finish", pp_gest_finish, 1, 1, {___,___,___}},
    {"gest:target", pp_gest_target, 2, 2, {___,REA,___}},
    {"gest:polyramp", pp_gest_polyramp, 2, 2, {___,INT,___}},
    {"gest:monoramp", pp_gest_monoramp, 2, 2, {___,INT,___}},
    {"gest:linear", pp_gest_linear, 1, 1, {___,___,___}},
    {"gest:step", pp_gest_step, 1, 1, {___,___,___}},
    {"gest:inertia", pp_gest_inertia, 2, 2, {___,REA,___}},
    {"gest:mass", pp_gest_mass, 2, 2, {___,REA,___}},
    {"gest:loopit", pp_gest_loopit, 1, 1, {___,___,___}},
    {"gest:gliss", pp_gest_gliss, 1, 1, {___,___,___}},
    {"gest:smallgliss", pp_gest_smallgliss, 1, 1, {___,___,___}},
    {"gest:exponential", pp_gest_exponential, 2, 2, {___,REA,___}},
    {"gest:bezier", pp_gest_bezier, 3, 3, {___,REA,REA}},
    {"gest:metatarget", pp_gest_metatarget, 2, 2, {___,INT,___}},
    {"gest:mediumgliss", pp_gest_mediumgliss, 1, 1, {___,___,___}},
    {"gest:metabehavior", pp_gest_metabehavior, 2, 2, {___,INT,___}},
    {"gest:metanode", pp_gest_metanode, 3, 3, {___,INT,INT}},
    {"gest:metaphrase", pp_gest_metaphrase, 2, 2, {___,INT,___}},
    {"gest:randtarget", pp_gest_randtarget, 1, 1, {___,___,___}},
    {"gest:randbehavior", pp_gest_randbehavior, 1, 1, {___,___,___}},
    {"gest:randnode", pp_gest_randnode, 1, 1, {___,___,___}},
    {"gest:randphrase", pp_gest_randphrase, 1, 1, {___,___,___}},
    {"gest:newscalar", pp_gest_newscalar, 0, 0, {___,___,___}},
    {"gest:setscalar", pp_gest_setscalar, 3, 3, {___,___,REA}},
    {"gest:scalar", pp_gest_scalar, 1, 1, {___,___,___}},
    {"gest:tick", pp_gest_tick, 1, 1, {___,___,___}},
    {"gest:repeat", pp_gest_repeat, 2, 2, {___,INT,___}},
    /* TODO: rseed */
    /* TODO: mark */
    /* TODO: return */
    /* TODO: skewquad */
    {"gest:skewexp", pp_gest_skewexp, 2, 2, {___,INT,___}},
    {"gest:skewshuf", pp_gest_skewshuf, 1, 1, {___,___,___}},
    {"gest:grow", pp_gest_grow, 2, 2, {___,REA,___}},
    {"gest:shrink", pp_gest_shrink, 2, 2, {___,REA,___}},
    {"gest:smoothstep", pp_gest_smoothstep, 1, 1, {___,___,___}},
    {"gest:ramp", pp_gest_ramp, 1, 1, {___,___,___}},
    {"gest:invramp", pp_gest_invramp, 1, 1, {___,___,___}},
    {"gest:biramp", pp_gest_biramp, 2, 2, {___,REA,___}},
    {"gest:gate", pp_gest_gate, 2, 2, {___,REA,___}},
    {"gest:squawk", pp_gest_squawk, 2, 2, {___,INT,___}},
    {"gest:tolerance", pp_gest_tolerance, 2, 2, {___,REA,___}},
    {"gest:conductor", pp_gest_conductor, 0, 0, {___,___,___}},
    {NULL}
};

void s9_load_gest(void)
{
    add_primitives("gest", gest_primitives);
}
