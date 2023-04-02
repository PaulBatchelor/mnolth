#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include "graforge.h"
#include "libxm/xm.h"
#include "libxm/state.h"

static int create_context_from_file(xm_context_t** ctx, uint32_t rate, const char* filename) {
    int xmfiledes;
    off_t size;
    int rc;
    char* data;

    rc = 1;
    xmfiledes = open(filename, O_RDONLY);
    if (xmfiledes == -1) {
        /* DEBUG_ERR("Could not open input file"); */
        *ctx = NULL;
        return 0;
    }

    size = lseek(xmfiledes, 0, SEEK_END);
    if (size == -1) {
        close(xmfiledes);
        /* DEBUG_ERR("lseek() failed"); */
        *ctx = NULL;
        return 0;
    }

    /* NB: using a VLA here was a bad idea, as the size of the
     * module file has no upper bound, whereas the stack has a
     * very finite (and usually small) size. Using mmap bypasses
     * the issue (at the cost of portability…). */
    data = mmap(NULL, size, PROT_READ, MAP_SHARED, xmfiledes, (off_t)0);
    if(data == MAP_FAILED) return 0;

    switch(xm_create_context_safe(ctx, data, size, rate)) {

    case 0:
        break;

    case 1:
        fprintf(stderr, "could not create context: module is not sane\n");
        *ctx = NULL;
        rc = 0;
        break;

    case 2:
        fprintf(stderr, "could not create context: malloc failed\n");
        rc = 0;
        break;

    default:
        fprintf(stderr, "could not create context: unknown error\n");
        rc = 0;
        break;

    }

    munmap(data, size);
    close(xmfiledes);

    return rc;
}

static void compute(gf_node *node)
{
    xm_context_t *xm;
    int blksize;
    int s;
    gf_cable *out;
    gf_cable *clk;
    GFFLT smp;

    blksize = gf_node_blksize(node);
    gf_node_get_cable(node, 0, &clk);
    gf_node_get_cable(node, 1, &out);
    xm = gf_node_get_data(node);

    for(s = 0; s < blksize; s++) {
        if(gf_cable_get(clk, s) != 0) xm_please_tick(xm);
        smp = xm_compute_sample(xm);
        gf_cable_set(out, s, smp);
    }
}

static void destroy(gf_node *node)
{
    xm_context_t *xm;

    xm = gf_node_get_data(node);
    gf_node_cables_free(node);
    xm_free_context(xm);
}

int node_xmplay(gf_node *n, const char *filename)
{
    xm_context_t *xm;
    gf_patch *patch;

    gf_node_get_patch(n, &patch);

    if (!create_context_from_file(&xm, gf_patch_srate_get(patch), filename)) {
        return 0;
    }

    gf_node_cables_alloc(n, 2);
    gf_node_set_block(n, 1);
    gf_node_set_compute(n, compute);
    gf_node_set_destroy(n, destroy);
    gf_node_set_data(n, xm);

    /* xm_please_tick(xm); */
    return 1;
}

#if 0
static runt_int rproc_xmplay(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    const char *filename;
    runt_stacklet *s;
    gf_node *n;
    gf_patch *patch;
    rgf_param clock;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    filename = runt_to_string(s->p);

    rc = rgf_get_param(vm, &clock);
    RUNT_ERROR_CHECK(rc);

    patch = rgf_get_patch(p);
    gf_patch_new_node(patch, &n);
    rc = node_xmplay(n, filename);
    if(!rc) {
        runt_print(vm, "Could not load %s\n", filename);
        return RUNT_NOT_OK;
    }

    rgf_set_param(vm, n, &clock, 0);

    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    rgf_push_output(vm, n, s, 1);
    return RUNT_OK;
}
#endif

typedef struct {
    int s;
} xmlayer_event;

typedef struct {
    xm_context_t *xm;
    gf_patch *patch;
    xm_state *state;
    xmlayer_event evt[8];
    int nevents;
    int init;
} xmlayer_d;

static void xmlayer_compute(gf_node *n)
{
    int s;
    int blksize;
    xmlayer_d *xml;
    gf_cable *clk;
    xmlayer_event *e;

    blksize = gf_node_blksize(n);
    xml = gf_node_get_data(n);


    gf_node_get_cable(n, 0, &clk);
    xml->nevents = 0;
    xm_state_save(xml->xm, xml->state);
    for(s = 0; s < blksize; s++) {
        if(gf_cable_get(clk, s) != 0 && xml->nevents < 8) {
            if(!xml->init) {
                e = &xml->evt[xml->nevents];
                e->s = s;
                xml->nevents++;
            } else {
                xml->init = 0;
            }
        }
    }
}

static void xmlayer_destroy(gf_node *n)
{
    xmlayer_d *xml;
    gf_node_cables_free(n);
    xml = gf_node_get_data(n);
    xm_free_context(xml->xm);
    xm_state_free(&xml->state);
    free(xml);
}

int node_xmlayer(gf_node *n, const char *filename, xmlayer_d **pxml)
{
    gf_patch *patch;
    int rc;
    xmlayer_d *xml;
    gf_node_cables_alloc(n, 1);

    rc = gf_node_get_patch(n, &patch);


    if(rc != GF_OK) return GF_NOT_OK;

    xml = calloc(1, sizeof(xmlayer_d));
    *pxml = xml;

    xml->patch= patch;

    xm_state_new(&xml->state);

    if(!create_context_from_file(&xml->xm,
                                 gf_patch_srate_get(patch),
                                 filename)) {
        return GF_NOT_OK;
    }

    gf_node_set_compute(n, xmlayer_compute);
    gf_node_set_destroy(n, xmlayer_destroy);
    gf_node_set_data(n, xml);
    xm_please_tick(xml->xm);
    xml->init = 1;

    return GF_OK;
}

#if 0
static runt_int rproc_xmlayer(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *s;
    gf_patch *patch;
    gf_node *n;
    const char *filename;
    rgf_param clk;
    xmlayer_d *xml;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    filename = runt_to_string(s->p);

    rc = rgf_get_param(vm, &clk);
    RUNT_ERROR_CHECK(rc);

    rc = runt_ppush(vm, &s);
    RUNT_ERROR_CHECK(rc);

    patch = rgf_get_patch(p);

    rc = gf_patch_new_node(patch, &n);
    GF_RUNT_ERROR_CHECK(rc);

    xml = NULL;
    rc = node_xmlayer(n, filename, &xml);
    GF_RUNT_ERROR_CHECK(rc);

    rgf_set_param(vm, n, &clk, 0);

    s->p = runt_mk_cptr(vm, xml);
    return RUNT_OK;
}
#endif

typedef struct {
    xmlayer_d *xml;
    int chan;
} xmlout_d;

static void xmlout_compute(gf_node *n)
{
    int s;
    int blksize;
    xmlout_d *xmlout;
    gf_cable *out;
    xmlayer_d *xml;
    GFFLT smp;
    int nevents;
    int evpos;
    xmlayer_event *ev;

    xmlout = gf_node_get_data(n);
    xml = xmlout->xml;
    blksize = gf_node_blksize(n);

    gf_node_get_cable(n, 0, &out);

    evpos = 0;
    nevents = xml->nevents;
    xm_state_restore(xml->xm, xml->state);
    for(s = 0; s < blksize; s++) {
        smp = 0;
        if(nevents > 0) {
            ev = &xml->evt[evpos];
            if(s == ev->s) {
                xm_please_tick_channel(xml->xm, xmlout->chan);
                nevents--;
                evpos++;
            }
        }
        smp = xm_compute_channel(xml->xm, xmlout->chan);
        gf_cable_set(out, s, smp);
    }
}

static void xmlout_destroy(gf_node *n)
{
    xmlout_d *xmlout;
    xmlout = gf_node_get_data(n);
    free(xmlout);
    gf_node_cables_free(n);
}

int node_xmlout(gf_node *n, xmlayer_d *xml, int chan)
{
    xmlout_d *xmlout;

    /* fprintf(stderr, "Creating xmlout channel %d\n", chan); */
    xmlout = calloc(1, sizeof(xmlout_d));
    xmlout->xml = xml;
    xmlout->chan = chan;
    gf_node_cables_alloc(n, 1);
    gf_node_set_block(n, 0);
    gf_node_set_compute(n, xmlout_compute);
    gf_node_set_destroy(n, xmlout_destroy);
    gf_node_set_data(n, xmlout);
    return GF_OK;
}

#if 0
static runt_int rproc_xmlout(runt_vm *vm, runt_ptr p)
{
    runt_int rc;
    runt_stacklet *out;
    runt_stacklet *s;
    runt_int chan;
    gf_patch *patch;
    gf_node *n;
    xmlayer_d *xml;

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    xml = runt_to_cptr(s->p);

    rc = runt_ppop(vm, &s);
    RUNT_ERROR_CHECK(rc);
    chan = floor(s->f);

    rc = runt_ppush(vm, &out);
    RUNT_ERROR_CHECK(rc);

    patch = rgf_get_patch(p);


    if(chan >= xm_get_number_of_channels(xml->xm) || chan < 0) {
        runt_print(vm, "Channel %d out of range\n", chan);
        return RUNT_NOT_OK;
    }

    rc = gf_patch_new_node(patch, &n);
    GF_RUNT_ERROR_CHECK(rc);

    rc = node_xmlout(n, xml, chan);
    GF_RUNT_ERROR_CHECK(rc);

    rgf_push_output(vm, n, out, 0);

    return RUNT_OK;
}

runt_int load_xm(runt_vm *vm, runt_ptr pw)
{
    runt_cell *c;
    runt_keyword_define(vm, "xmplay", 6, rproc_xmplay, &c);
    runt_cell_data(vm, c, pw);
    runt_keyword_define(vm, "xmlayer", 7, rproc_xmlayer, &c);
    runt_cell_data(vm, c, pw);
    runt_keyword_define(vm, "xmlout", 6, rproc_xmlout, &c);
    runt_cell_data(vm, c, pw);
    return runt_is_alive(vm);
}

runt_int rplug_xm(runt_vm *vm)
{
    runt_ptr p;
    runt_int rc;

    rc = rgf_plugin_data(vm, &p);

    if(rc != RUNT_OK) {
        runt_print(vm, "Could not load graforge.\n");
    }

    return load_xm(vm, p);
}
#endif
