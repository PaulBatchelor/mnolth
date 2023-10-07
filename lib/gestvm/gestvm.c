#line 177 "gestvm.org"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"

#include "uxn/uxn.h"
#define GESTVM_PRIV
#include "gestvm.h"

#line 489 "gestvm.org"
static SKFLT skewer_tick(gestvm *gvm, SKFLT phs);
#line 562 "gestvm.org"
static gestvm_behavior find_skewer(int id);
#line 610 "gestvm.org"
static SKFLT s_passthru(gestvm *gvm, SKFLT a);
static SKFLT s_exp_pos(gestvm *gvm, SKFLT a);
static SKFLT s_exp_neg(gestvm *gvm, SKFLT a);
#line 652 "gestvm.org"
static SKFLT rephasor_tick(gestvm *gvm, SKFLT phs);
#line 1056 "gestvm.org"
static Uint8 nil_dei(Device *d, Uint8 port);
static void nil_deo(Device *d, Uint8 port);
#line 1090 "gestvm.org"
static void console_deo(Device *d, Uint8 port);
#line 1115 "gestvm.org"
static void gestvm_deo(Device *d, Uint8 port);
#line 1285 "gestvm.org"
static void vm_tick(gestvm *gvm, SKFLT phs);
#line 1452 "gestvm.org"
static SKFLT b_linear(gestvm *gvm, SKFLT a);
#line 1467 "gestvm.org"
static SKFLT b_step(gestvm *gvm, SKFLT a);
#line 1486 "gestvm.org"
static SKFLT b_gliss_medium(gestvm *gvm, SKFLT a);
static SKFLT b_gliss(gestvm *gvm, SKFLT a);
#line 1525 "gestvm.org"
static SKFLT b_gliss_parametric(gestvm *gvm, SKFLT a);
#line 1565 "gestvm.org"
static SKFLT expmap(SKFLT in, SKFLT slope);
static SKFLT b_expon_convex_high(gestvm *gvm, SKFLT a);
static SKFLT b_expon_convex_low(gestvm *gvm, SKFLT a);
static SKFLT b_expon_concave_high(gestvm *gvm, SKFLT a);
static SKFLT b_expon_concave_low(gestvm *gvm, SKFLT a);
#line 1605 "gestvm.org"
static SKFLT interpolate(gestvm *gvm, SKFLT phs);
#line 1629 "gestvm.org"
static gestvm_behavior find_behavior(int id);
#line 1739 "gestvm.org"
static SKFLT scale255(int x);
#line 1927 "gestvm.org"
static void uxn_mass(gestvm *gvm, unsigned char val);
#line 1949 "gestvm.org"
static void uxn_inertia(gestvm *gvm, unsigned char val);
#line 1986 "gestvm.org"
static SKFLT b_gate_rel_25(gestvm *gvm, SKFLT a);
static SKFLT b_gate_rel_50(gestvm *gvm, SKFLT a);
static SKFLT b_gate_rel_125(gestvm *gvm, SKFLT a);
#line 2054 "gestvm.org"
static SKFLT b_tabread(gestvm *gvm, SKFLT a);
#line 177 "gestvm.org"
#line 771 "gestvm.org"
int uxn_halt(Uxn *u, Uint8 error, char *name, int id)
{
    /* doing nothing for now */
    fprintf(stderr, "uxn halt %d, %x:  %s\n", error, id, name);
	return 0;
}
#line 177 "gestvm.org"
#line 234 "gestvm.org"
void gestvm_init(gestvm *gvm, gestvm_uxn *u)
{
#line 521 "gestvm.org"
gestvm_rephasor_init(&gvm->skew);
gestvm_rephasor_scale(&gvm->skew, 1.0);
#line 538 "gestvm.org"
gvm->skewdur = 1;
#line 554 "gestvm.org"
gvm->update_skewer = 0;
#line 601 "gestvm.org"
gvm->skewer = s_passthru;
#line 731 "gestvm.org"
gestvm_rephasor_init(&gvm->rephasor);
gestvm_rephasor_scale(&gvm->rephasor, 1.0);
gvm->num = 1;
gvm->den = 1;
gvm->update_rephasor = 0;
gvm->extscale = gvm->pextscale = 1.0;
#line 868 "gestvm.org"
gvm->u = u;
#line 1030 "gestvm.org"
gvm->ptr = 0;
#line 1280 "gestvm.org"
gvm->lphs = 999;
#line 1330 "gestvm.org"
gvm->empty_value = 1;
#line 1393 "gestvm.org"
gvm->cur = 0;
gvm->nxt = 0;
#line 1411 "gestvm.org"
gvm->behavior = b_linear;
#line 1426 "gestvm.org"
gvm->a = 0;
#line 1436 "gestvm.org"
gvm->cnd = 0;
#line 1730 "gestvm.org"
{
    int i;
    for (i = 0; i < 4; i++) gvm->params[i] = 0;
    gvm->pselect = 0;
}
#line 1776 "gestvm.org"
gvm->mass = 0;
gvm->inertia = 0;
#line 2023 "gestvm.org"
gvm->interp = 1;
#line 2034 "gestvm.org"
gvm->tablist = NULL;
gvm->ntables = 0;
#line 2098 "gestvm.org"
gvm->adder = 0;
#line 2108 "gestvm.org"
memset(&gvm->wst, 0, sizeof(Stack));
memset(&gvm->rst, 0, sizeof(Stack));
#line 237 "gestvm.org"
}
#line 250 "gestvm.org"
size_t gestvm_sizeof(void)
{
    return sizeof(gestvm);
}
#line 271 "gestvm.org"
SKFLT gestvm_tick(gestvm *gvm, SKFLT cnd)
{
    SKFLT out;

    out = 0.0;

    /* Skew! */

    cnd = skewer_tick(gvm, cnd);

    /* Main Rephasor! */

    cnd = rephasor_tick(gvm, cnd);

    /* VM process! */

    vm_tick(gvm, cnd);

    /* Interpolate! */

    out = interpolate(gvm, cnd);

    return out;
}
#line 350 "gestvm.org"
void gestvm_rephasor_scale(gestvm_rephasor *rp, SKFLT scale)
{
    if (scale != rp->s) {
        rp->s = scale;
        rp->si = 1.0 / scale;
    }
}
#line 368 "gestvm.org"
void gestvm_rephasor_init(gestvm_rephasor *rp)
{
    rp->pr = 0;
    rp->pc[0] = 0;
    rp->pc[1] = 0;
    rp->pe[0] = 0;
    rp->pe[1] = 0;
    rp->c = 1.0;
    rp->s = 1.0;
    rp->si = 1.0;

    rp->ir = 0.0;
    rp->ic = 0.0;
}
#line 422 "gestvm.org"
static SKFLT phasor(SKFLT phs, SKFLT inc)
{
    phs += inc;

    if (phs > 1.0) return 0;

    return phs;
}

SKFLT gestvm_rephasor_tick(gestvm_rephasor *rp, SKFLT ext)
{
    SKFLT pr, pc;
    SKFLT out;


    /* delta function of \theta_e */
    if (ext > rp->pe[0]) {
        rp->ir = ext - rp->pe[0];
    }

    /* compute main rephasor \theta_r */
    pr = phasor(rp->pr, rp->s * rp->ir * rp->c);

    /* delta function of \theta_r */
    if (pr > rp->pr) {
        rp->ic = pr - rp->pr;
    }

    /* compute rephasor \theta_c */
    pc = phasor(rp->pc[0], rp->si * rp->ic);

    /* compute correction coefficient */
    if (rp->pc[1] != 0) {
        rp->c = rp->pe[1] / rp->pc[1];
    }

    /* clamping the correction */
    if (rp->c > 2.0) rp->c = 2.0;
    if (rp->c < 0.5) rp->c = 0.5;

    out = pr;

    /* update state */

    rp->pr = pr;

    rp->pc[1] = rp->pc[0];
    rp->pc[0] = pc;

    rp->pe[1] = rp->pe[0];
    rp->pe[0] = ext;

    return out;
}
#line 494 "gestvm.org"
static SKFLT skewer_tick(gestvm *gvm, SKFLT phs)
{
    SKFLT out;

    if (gvm->update_skewer) {
        gvm->update_skewer = 0;

        if (gvm->skewdur > 0) {
            SKFLT scale;
            scale = 1.0 / gvm->skewdur;
            gestvm_rephasor_scale(&gvm->skew, scale);
        }
    }

    out = gestvm_rephasor_tick(&gvm->skew, phs);

    out = gvm->skewer(gvm, out);

    return out;
}
#line 567 "gestvm.org"
static gestvm_behavior find_skewer(int id)
{
    gestvm_behavior s;

    s = s_passthru;

    switch (id) {
        case 0:
            s = s_passthru;
            break;
        case 1:
            s = s_exp_pos;
            break;
        case 2:
            s = s_exp_neg;
            break;
        default:
            break;
    }

    return s;
}
#line 617 "gestvm.org"
static SKFLT s_passthru(gestvm *gvm, SKFLT a)
{
    return a;
}
#line 625 "gestvm.org"
static SKFLT s_exp_pos(gestvm *gvm, SKFLT a)
{
    return (1.0 - exp(1.5*a)) / (1.0 - exp(1.5));
}
#line 633 "gestvm.org"
static SKFLT s_exp_neg(gestvm *gvm, SKFLT a)
{
    return (1.0 - exp(-1.5*a)) / (1.0 - exp(-1.5));
}
#line 660 "gestvm.org"
static SKFLT rephasor_tick(gestvm *gvm, SKFLT phs)
{
    SKFLT out;

    if (gvm->extscale != gvm->pextscale) {
        gvm->pextscale = gvm->extscale;
        gvm->update_rephasor = 1;
    }

    if (gvm->update_rephasor) {
        SKFLT scale;
        gvm->update_rephasor = 0;

        if (gvm->den != 0) {
            scale =
                (SKFLT)(gvm->num * gvm->skewdur) /
                (SKFLT)gvm->den;
        } else scale = 1.0;

        scale *= gvm->extscale;

        gestvm_rephasor_scale(&gvm->rephasor, scale);
    }

    out = gestvm_rephasor_tick(&gvm->rephasor, phs);
    gvm->cnd = out;
    return out;
}
#line 697 "gestvm.org"
SKFLT gestvm_last_conductor(gestvm *gvm)
{
    return gvm->cnd;
}
#line 749 "gestvm.org"
void gestvm_extscale(gestvm *gvm, SKFLT extscale)
{
    gvm->extscale = extscale;
}
#line 814 "gestvm.org"
void gestvm_uxn_init(gestvm_uxn *u)
{
    uxn_boot(&u->u);
    u->gvm = NULL;

#line 1075 "gestvm.org"
{
    int i;

    for (i = 0x0; i <= 0xf; i++) {
        uxn_port(&u->u, i, nil_dei, nil_deo);
    }
}
#line 814 "gestvm.org"
#line 1106 "gestvm.org"
uxn_port(&u->u, 0x1, nil_dei, console_deo);
#line 814 "gestvm.org"
#line 1142 "gestvm.org"
uxn_port(&u->u, 0x2, nil_dei, gestvm_deo);
#line 822 "gestvm.org"

}
#line 833 "gestvm.org"
size_t gestvm_uxn_sizeof(void)
{
    return sizeof(gestvm_uxn);
}
#line 847 "gestvm.org"
void gestvm_uxn_set(gestvm_uxn *gu, gestvm *gvm)
{
    gu->gvm = gvm;
}

gestvm *gestvm_uxn_get(gestvm_uxn *gu)
{
    return gu->gvm;
}
#line 888 "gestvm.org"
int gestvm_load(gestvm_uxn *gu, const char *rom)
{
	FILE *f;
	int r;
    Uxn *u;
    char sym[3];

	if (!(f = fopen(rom, "rb"))) return 1;

    sym[0] = sym[1] = sym[2] = 0;

    fread(sym, 1, 3, f);

    if (sym[0] == 'S' && sym[1] == 'Y' && sym[2] == 'M') {
        unsigned char b[2];
        unsigned short sz;
        b[0] = b[1] = 0;
        fread(b, 1, 2, f);
        sz = b[0] | (b[1] << 8);
        fseek(f, sz, SEEK_CUR);

    } else fseek(f, 0L, SEEK_SET);


    u = &gu->u;

	r = fread(u->ram.dat + PAGE_PROGRAM,
              1, sizeof(u->ram.dat) - PAGE_PROGRAM, f);
	fclose(f);
	if(r < 1) return 1;
	return 0;
}
#line 954 "gestvm.org"
unsigned int gestvm_lookup(const char *rom, const char *sym)
{
    unsigned char symlen;
    unsigned short sz;
    FILE *fp;
    unsigned char buf[64];
    unsigned int addr;

    symlen = strlen(sym);
    addr = 0;

    fp = fopen(rom, "r");

    if (fp == NULL) {
        return 0;
    }

    memset(buf, 0, 64);

    fread(buf, 1, 3, fp);

    if (buf[0] != 'S' || buf[1] != 'Y' || buf[2] != 'M') {
        return 0;
    }

    sz = 0;
    fread(buf, 1, 2, fp);

    sz = buf[0] + (buf[1] << 8);

    while (sz) {
        unsigned char len;
        fread(&len, 1, 1, fp);

        if (len == symlen) {
            int i;
            int match;
            fread(buf, 1, len, fp);
            match = 1;
            for (i = 0; i < len; i++) {
                if (buf[i] != sym[i]) {
                    match = 0;
                    break;
                }
            }

            if (match) {
                fread(buf, 1, 2, fp);
                addr = buf[0] + (buf[1] << 8);
                break;
            } else {
                fseek(fp, 2, SEEK_CUR);
            }
        } else {
            fseek(fp, len + 2, SEEK_CUR);
        }

        sz -= (len + 2 + 1);
    }

    fclose(fp);

    return addr;
}
#line 1042 "gestvm.org"
void gestvm_pointer(gestvm *gvm, unsigned int ptr)
{
    gvm->ptr = ptr;
}
#line 1062 "gestvm.org"
static void nil_deo(Device *d, Uint8 port)
{
	if(port == 0x1) d->vector = peek16(d->dat, 0x0);
}

static Uint8 nil_dei(Device *d, Uint8 port)
{
	return d->dat[port];
}
#line 1095 "gestvm.org"
static void console_deo(Device *d, Uint8 port)
{
	if(port == 0x1)
		d->vector = peek16(d->dat, 0x0);
	if(port > 0x7)
		write(port - 0x7, (char *)&d->dat[port], 1);
}
#line 1124 "gestvm.org"
static void gestvm_deo(Device *d, Uint8 port)
{
    gestvm_uxn *gu;
    gestvm *gvm;

    gu = (gestvm_uxn *)d->u;
    gvm = gu->gvm;

    switch (port) {
#line 1149 "gestvm.org"
case 0:
    uxn_mass(gvm, d->dat[port]);
    break;
#line 1156 "gestvm.org"
case 1:
    uxn_inertia(gvm, d->dat[port]);
    break;
#line 1163 "gestvm.org"
case 2:
    gvm->skewer = find_skewer(d->dat[port]);
    break;
#line 1170 "gestvm.org"
case 3: {
    int skewdur = d->dat[port];

    if (skewdur > 0) {
        gvm->skewdur = skewdur;
        gvm->update_skewer = 1;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1184 "gestvm.org"
case 4: {
    int num;

    num = d->dat[port];

    if (num > 0 && num != gvm->num) {
        gvm->num = num;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1199 "gestvm.org"
case 5: {
    int den;

    den = d->dat[port];

    if (den > 0 && den != gvm->den) {
        gvm->den = den;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1214 "gestvm.org"
case 6:
    gvm->nxt = (SKFLT) d->dat[port];
    break;
#line 1224 "gestvm.org"
case 7:
    gvm->behavior = find_behavior(d->dat[port]);
    break;
#line 1231 "gestvm.org"
case 10:
    gvm->interp = d->dat[port];
    break;
#line 1238 "gestvm.org"
case 11:
    gvm->cur = (SKFLT) d->dat[port];
    break;
#line 1245 "gestvm.org"
case 12:
    gvm->pselect = d->dat[port];
    break;
#line 1252 "gestvm.org"
case 13:
    gvm->params[gvm->pselect] = d->dat[port];
    break;
#line 1134 "gestvm.org"
        default:
            break;
    }
}
#line 1290 "gestvm.org"
static void vm_tick(gestvm *gvm, SKFLT phs)
{
    if (phs < gvm->lphs) {
        gvm->u->gvm = gvm;
        gvm->cur = gvm->nxt;
        /* uxn_eval(&gvm->u->u, gvm->ptr); */
        gestvm_eval(gvm->u, gvm, gvm->ptr);
        gvm->ptr = gvm->u->u.ram.ptr;

#line 1345 "gestvm.org"
if (gvm->empty_value) {
    gvm->empty_value = 0;
    gvm->cur = gvm->nxt;
    /* uxn_eval(&gvm->u->u, gvm->ptr); */
    gestvm_eval(gvm->u, gvm, gvm->ptr);
    gvm->ptr = gvm->u->u.ram.ptr;
}
#line 1300 "gestvm.org"
    }

    gvm->lphs = phs;
}
#line 1367 "gestvm.org"
void gestvm_eval(gestvm_uxn *gu, gestvm *gvm, unsigned int addr)
{
    Stack *l_rst, *l_wst;
    l_rst = gu->u.rst;
    l_wst = gu->u.wst;
    if (gvm != NULL) {
        gu->u.rst = &gvm->rst;
        gu->u.wst = &gvm->wst;
    }
    uxn_eval(&gu->u, addr);
    gu->u.rst = l_rst;
    gu->u.wst = l_wst;
}
#line 1457 "gestvm.org"
static SKFLT b_linear(gestvm *gvm, SKFLT a)
{
    return a;
}
#line 1472 "gestvm.org"
static SKFLT b_step(gestvm *gvm, SKFLT a)
{
    return 0;
}
#line 1492 "gestvm.org"
static SKFLT b_gliss_medium(gestvm *gvm, SKFLT a)
{
    if (a < 0.75) {
        a = 0;
    } else {
        a -= 0.75;
        if (a < 0) a = 0;
        a /= 0.25;
        a = a * a * a;
    }
    return a;
}

static SKFLT b_gliss(gestvm *gvm, SKFLT a)
{
    if (a < 0.25) {
        a = 0;
    } else {
        a -= 0.25;
        if (a < 0) a = 0;
        a /= 0.75;
        a = a * a * a;
    }

    return a;
}
#line 1530 "gestvm.org"
static SKFLT b_gliss_parametric(gestvm *gvm, SKFLT a)
{
    SKFLT pos;

    pos = scale255(gvm->params[0]);

    if (pos == 1.0) pos = 0.0;
    if (a < pos) {
        a = 0;
    } else {
        a -= pos;
        if (a < 0) a = 0;
        a /= (1.0 - pos);
        a = a * a * a;
    }
    return a;
}
#line 1574 "gestvm.org"
static SKFLT expmap(SKFLT in, SKFLT slope)
{
    return (1 - exp(in*slope)) / (1 - exp(slope));
}

static SKFLT b_expon_convex_high(gestvm *gvm, SKFLT a)
{
    return expmap(a, -4);
}

static SKFLT b_expon_convex_low(gestvm *gvm, SKFLT a)
{

    return expmap(a, -1);
}

static SKFLT b_expon_concave_high(gestvm *gvm, SKFLT a)
{

    return expmap(a, 4);
}

static SKFLT b_expon_concave_low(gestvm *gvm, SKFLT a)
{
    return expmap(a, 1);
}
#line 1610 "gestvm.org"
static SKFLT interpolate(gestvm *gvm, SKFLT phs)
{
    SKFLT a;
    a = gvm->behavior(gvm, phs);
    /* cache interpolation value */
    gvm->a = a;
    if (gvm->interp) {
        if (gvm->adder) {
            gvm->adder = 0;
            return gvm->cur + a*gvm->nxt;
        }
        return (1.0 - a)*gvm->cur + a*gvm->nxt;
    }
    return a;
}
#line 1634 "gestvm.org"
static gestvm_behavior find_behavior(int id)
{
    gestvm_behavior b;

    b = b_linear;

    switch (id) {
        case 0:
            b = b_linear;
            break;
        case 1:
            b = b_step;
            break;
        case 2:
            b = b_gliss_medium;
            break;
        case 3:
            b = b_gliss;
            break;
        case 4:
            b = b_gate_rel_125;
            break;
        case 5:
            b = b_gate_rel_25;
            break;
        case 6:
            b = b_gate_rel_50;
            break;
        case 7:
            b = b_expon_convex_low;
            break;
        case 8:
            b = b_expon_convex_high;
            break;
        case 9:
            b = b_expon_concave_low;
            break;
        case 10:
            b = b_expon_concave_high;
            break;
        case 11:
            b = b_gliss_parametric;
            break;
        case 12:
            b = b_tabread;
            break;
        default:
            break;
    }

    return b;
}
#line 1711 "gestvm.org"
void gestvm_get_last_values(gestvm *gvm,
                            SKFLT *x,
                            SKFLT *y,
                            SKFLT *a)
{
    *x = gvm->cur;
    *y = gvm->nxt;
    *a = gvm->a;
}
#line 1744 "gestvm.org"
static const SKFLT oned255 = 1.0 / 255;
static SKFLT scale255(int x)
{
    return (SKFLT)x*oned255;
}
#line 1815 "gestvm.org"
size_t gestvm_weight_sizeof(void)
{
    return sizeof(gestvm_weight);
}
#line 1828 "gestvm.org"
void gestvm_weight_init(gestvm_weight *gw, gestvm *gvm, int sr)
{
    gw->sr = sr;
    gw->gvm = gvm;
    gw->prev = 0;
    gw->a = 0.0;
    gw->scale = 1.0 / 120;
    gestvm_weight_amppos(gw, 20);
    gestvm_weight_ampneg(gw, 20);
}
#line 1855 "gestvm.org"
void gestvm_weight_amppos(gestvm_weight *gw, SKFLT amp)
{
    gw->amppos = amp;
}

void gestvm_weight_ampneg(gestvm_weight *gw, SKFLT amp)
{
    gw->ampneg = amp;
}
#line 1875 "gestvm.org"
SKFLT gestvm_weight_tick(gestvm_weight *gw)
{
    SKFLT i;
    SKFLT mass;
    gestvm *gvm;
    SKFLT out;

    gvm = gw->gvm;

    mass = gvm->mass;
    i = gvm->inertia;

    if (i != gw->inertia) {
        gw->inertia = i;
        if (i == 0) {
            gw->a = 0;
            gw->prev = 0;
        } else {
            gw->a = exp(-1.0 / (gw->sr * i));
        }
    }


    out =
        (1 - gw->a) * mass * gw->scale +
        gw->a * gw->prev;

    if (out > 1) out = 1;
    if (out < -1) out = -1;

    gw->prev = out;

    if (out >= 0) {
        out *= gw->amppos;
    } else {
        out *= gw->ampneg;
    }

    return out;
}
#line 1932 "gestvm.org"
static void uxn_mass(gestvm *gvm, unsigned char val)
{
    SKFLT m;
    val &= 0xFF;

    m = (SKFLT)(val - 0x7F) / 0x7F;
    m *= 120.0;

    if (m < -120) m = -120;
    if (m > 120) m = 120;

    gvm->mass = m;
}
#line 1954 "gestvm.org"
static void uxn_inertia(gestvm *gvm, unsigned char val)
{
    SKFLT i;
    val &= 0xFF;

    i = (SKFLT)val / 0xFF;
    i *= 0.3;

    gvm->inertia = i;
}
#line 1993 "gestvm.org"
static SKFLT b_gate_rel_25(gestvm *gvm, SKFLT a)
{
    return a <= 0.25;
}

static SKFLT b_gate_rel_50(gestvm *gvm, SKFLT a)
{
    return a <= 0.5;
}

static SKFLT b_gate_rel_125(gestvm *gvm, SKFLT a)
{
    return a <= 0.125;
}
#line 2045 "gestvm.org"
void gestvm_tablist(gestvm *gvm, sk_table **tablist, int ntables)
{
    gvm->tablist = tablist;
    gvm->ntables = ntables;
}
#line 2059 "gestvm.org"
static SKFLT b_tabread(gestvm *gvm, SKFLT a)
{
    SKFLT *data;
    int sz;
    sk_table *tab;
    int ipos;
    SKFLT fpos;

    if (gvm->params[0] < 0 || gvm->params[0] >= gvm->ntables) {
        return 0;
    }

    gvm->adder = 1;

    tab = gvm->tablist[gvm->params[0]];

    data = sk_table_data(tab);
    sz = sk_table_size(tab);

    if (a < 0) a = 0;
    if (a > 1) a = 1;

    fpos = a * (sz - 2);
    ipos = (int)fpos;
    fpos -= ipos;

    a = (1.0 - fpos)*data[ipos] + fpos*data[ipos + 1];

    return a;
}
#line 177 "gestvm.org"
