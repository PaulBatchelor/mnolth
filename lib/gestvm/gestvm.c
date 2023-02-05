#line 177 "gestvm.org"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "uxn/uxn.h"
#define GESTVM_PRIV
#include "gestvm.h"

#line 486 "gestvm.org"
static SKFLT skewer_tick(gestvm *gvm, SKFLT phs);
#line 559 "gestvm.org"
static gestvm_behavior find_skewer(int id);
#line 607 "gestvm.org"
static SKFLT s_passthru(gestvm *gvm, SKFLT a);
static SKFLT s_exp_pos(gestvm *gvm, SKFLT a);
static SKFLT s_exp_neg(gestvm *gvm, SKFLT a);
#line 649 "gestvm.org"
static SKFLT rephasor_tick(gestvm *gvm, SKFLT phs);
#line 994 "gestvm.org"
static Uint8 nil_dei(Device *d, Uint8 port);
static void nil_deo(Device *d, Uint8 port);
#line 1028 "gestvm.org"
static void console_deo(Device *d, Uint8 port);
#line 1053 "gestvm.org"
static void gestvm_deo(Device *d, Uint8 port);
#line 1202 "gestvm.org"
static void vm_tick(gestvm *gvm, SKFLT phs);
#line 1339 "gestvm.org"
static SKFLT b_linear(gestvm *gvm, SKFLT a);
static SKFLT b_step(gestvm *gvm, SKFLT a);
static SKFLT b_gliss_medium(gestvm *gvm, SKFLT a);
static SKFLT b_gliss(gestvm *gvm, SKFLT a);
#line 1391 "gestvm.org"
static SKFLT interpolate(gestvm *gvm, SKFLT phs);
#line 1410 "gestvm.org"
static gestvm_behavior find_behavior(int id);
#line 1661 "gestvm.org"
static void uxn_mass(gestvm *gvm, unsigned char val);
#line 1683 "gestvm.org"
static void uxn_inertia(gestvm *gvm, unsigned char val);
#line 1720 "gestvm.org"
static SKFLT b_gate_rel_25(gestvm *gvm, SKFLT a);
static SKFLT b_gate_rel_50(gestvm *gvm, SKFLT a);
static SKFLT b_gate_rel_125(gestvm *gvm, SKFLT a);
#line 177 "gestvm.org"
#line 711 "gestvm.org"
int uxn_halt(Uxn *u, Uint8 error, char *name, int id)
{
    /* doing nothing for now */
	return 0;
}
#line 177 "gestvm.org"
#line 231 "gestvm.org"
void gestvm_init(gestvm *gvm, gestvm_uxn *u)
{
#line 518 "gestvm.org"
gestvm_rephasor_init(&gvm->skew);
gestvm_rephasor_scale(&gvm->skew, 1.0);
#line 535 "gestvm.org"
gvm->skewdur = 1;
#line 551 "gestvm.org"
gvm->update_skewer = 0;
#line 598 "gestvm.org"
gvm->skewer = s_passthru;
#line 688 "gestvm.org"
gestvm_rephasor_init(&gvm->rephasor);
gestvm_rephasor_scale(&gvm->rephasor, 1.0);
gvm->num = 1;
gvm->den = 1;
gvm->update_rephasor = 0;
#line 806 "gestvm.org"
gvm->u = u;
#line 968 "gestvm.org"
gvm->ptr = 0;
#line 1197 "gestvm.org"
gvm->lphs = 999;
#line 1246 "gestvm.org"
gvm->empty_value = 1;
#line 1299 "gestvm.org"
gvm->cur = 0;
gvm->nxt = 0;
#line 1317 "gestvm.org"
gvm->behavior = b_linear;
#line 1332 "gestvm.org"
gvm->a = 0;
#line 1510 "gestvm.org"
gvm->mass = 0;
gvm->inertia = 0;
#line 1757 "gestvm.org"
gvm->interp = 1;
#line 234 "gestvm.org"
}
#line 247 "gestvm.org"
size_t gestvm_sizeof(void)
{
    return sizeof(gestvm);
}
#line 268 "gestvm.org"
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
#line 347 "gestvm.org"
void gestvm_rephasor_scale(gestvm_rephasor *rp, SKFLT scale)
{
    if (scale != rp->s) {
        rp->s = scale;
        rp->si = 1.0 / scale;
    }
}
#line 365 "gestvm.org"
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
#line 419 "gestvm.org"
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
#line 491 "gestvm.org"
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
#line 564 "gestvm.org"
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
#line 614 "gestvm.org"
static SKFLT s_passthru(gestvm *gvm, SKFLT a)
{
    return a;
}
#line 622 "gestvm.org"
static SKFLT s_exp_pos(gestvm *gvm, SKFLT a)
{
    return (1.0 - exp(1.5*a)) / (1.0 - exp(1.5));
}
#line 630 "gestvm.org"
static SKFLT s_exp_neg(gestvm *gvm, SKFLT a)
{
    return (1.0 - exp(-1.5*a)) / (1.0 - exp(-1.5));
}
#line 657 "gestvm.org"
static SKFLT rephasor_tick(gestvm *gvm, SKFLT phs)
{
    SKFLT out;

    if (gvm->update_rephasor) {
        SKFLT scale;
        gvm->update_rephasor = 0;

        if (gvm->den != 0) {
            scale =
                (SKFLT)(gvm->num * gvm->skewdur) /
                (SKFLT)gvm->den;
        } else scale = 1.0;

        gestvm_rephasor_scale(&gvm->rephasor, scale);
    }

    out = gestvm_rephasor_tick(&gvm->rephasor, phs);
    return out;
}
#line 753 "gestvm.org"
void gestvm_uxn_init(gestvm_uxn *u)
{
    uxn_boot(&u->u);
    u->gvm = NULL;

#line 1013 "gestvm.org"
{
    int i;

    for (i = 0x0; i <= 0xf; i++) {
        uxn_port(&u->u, i, nil_dei, nil_deo);
    }
}
#line 753 "gestvm.org"
#line 1044 "gestvm.org"
uxn_port(&u->u, 0x1, nil_dei, console_deo);
#line 753 "gestvm.org"
#line 1080 "gestvm.org"
uxn_port(&u->u, 0x2, nil_dei, gestvm_deo);
#line 761 "gestvm.org"
}
#line 771 "gestvm.org"
size_t gestvm_uxn_sizeof(void)
{
    return sizeof(gestvm_uxn);
}
#line 785 "gestvm.org"
void gestvm_uxn_set(gestvm_uxn *gu, gestvm *gvm)
{
    gu->gvm = gvm;
}

gestvm *gestvm_uxn_get(gestvm_uxn *gu)
{
    return gu->gvm;
}
#line 826 "gestvm.org"
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
#line 892 "gestvm.org"
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
#line 980 "gestvm.org"
void gestvm_pointer(gestvm *gvm, unsigned int ptr)
{
    gvm->ptr = ptr;
}
#line 1000 "gestvm.org"
static void nil_deo(Device *d, Uint8 port)
{
	if(port == 0x1) d->vector = peek16(d->dat, 0x0);
}

static Uint8 nil_dei(Device *d, Uint8 port)
{
	return d->dat[port];
}
#line 1033 "gestvm.org"
static void console_deo(Device *d, Uint8 port)
{
	if(port == 0x1)
		d->vector = peek16(d->dat, 0x0);
	if(port > 0x7)
		write(port - 0x7, (char *)&d->dat[port], 1);
}
#line 1062 "gestvm.org"
static void gestvm_deo(Device *d, Uint8 port)
{
    gestvm_uxn *gu;
    gestvm *gvm;

    gu = (gestvm_uxn *)d->u;
    gvm = gu->gvm;

    switch (port) {
#line 1087 "gestvm.org"
case 0:
    uxn_mass(gvm, d->dat[port]);
    break;
#line 1094 "gestvm.org"
case 1:
    uxn_inertia(gvm, d->dat[port]);
    break;
#line 1101 "gestvm.org"
case 2:
    gvm->skewer = find_skewer(d->dat[port]);
    break;
#line 1108 "gestvm.org"
case 3: {
    int skewdur = d->dat[port];

    if (skewdur > 0) {
        gvm->skewdur = skewdur;
        gvm->update_skewer = 1;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1122 "gestvm.org"
case 4: {
    int num;

    num = d->dat[port];

    if (num > 0 && num != gvm->num) {
        gvm->num = num;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1137 "gestvm.org"
case 5: {
    int den;

    den = d->dat[port];

    if (den > 0 && den != gvm->den) {
        gvm->den = den;
        gvm->update_rephasor = 1;
    }
    break;
}
#line 1152 "gestvm.org"
case 6:
    gvm->nxt = (SKFLT) d->dat[port];
    break;
#line 1162 "gestvm.org"
case 7:
    gvm->behavior = find_behavior(d->dat[port]);
    break;
#line 1169 "gestvm.org"
case 10:
    gvm->interp = d->dat[port];
    break;
#line 1072 "gestvm.org"
        default:
            break;
    }
}
#line 1207 "gestvm.org"
static void vm_tick(gestvm *gvm, SKFLT phs)
{
    if (phs < gvm->lphs) {
        gvm->u->gvm = gvm;
        gvm->cur = gvm->nxt;
        uxn_eval(&gvm->u->u, gvm->ptr);
        gvm->ptr = gvm->u->u.ram.ptr;

#line 1261 "gestvm.org"
if (gvm->empty_value) {
    gvm->empty_value = 0;
    gvm->cur = gvm->nxt;
    uxn_eval(&gvm->u->u, gvm->ptr);
    gvm->ptr = gvm->u->u.ram.ptr;
}
#line 1216 "gestvm.org"
    }

    gvm->lphs = phs;
}
#line 1282 "gestvm.org"
void gestvm_eval(gestvm_uxn *gu, unsigned int addr)
{
    uxn_eval(&gu->u, addr);
}
#line 1347 "gestvm.org"
static SKFLT b_linear(gestvm *gvm, SKFLT a)
{
    return a;
}

static SKFLT b_step(gestvm *gvm, SKFLT a)
{
    return 0;
}

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
#line 1396 "gestvm.org"
static SKFLT interpolate(gestvm *gvm, SKFLT phs)
{
    SKFLT a;
    a = gvm->behavior(gvm, phs);
    /* cache interpolation value */
    gvm->a = a;
    if (gvm->interp)
        return (1.0 - a)*gvm->cur + a*gvm->nxt;
    return a;
}
#line 1415 "gestvm.org"
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
        default:
            break;
    }

    return b;
}
#line 1474 "gestvm.org"
void gestvm_get_last_values(gestvm *gvm,
                            SKFLT *x,
                            SKFLT *y,
                            SKFLT *a)
{
    *x = gvm->cur;
    *y = gvm->nxt;
    *a = gvm->a;
}
#line 1549 "gestvm.org"
size_t gestvm_weight_sizeof(void)
{
    return sizeof(gestvm_weight);
}
#line 1562 "gestvm.org"
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
#line 1589 "gestvm.org"
void gestvm_weight_amppos(gestvm_weight *gw, SKFLT amp)
{
    gw->amppos = amp;
}

void gestvm_weight_ampneg(gestvm_weight *gw, SKFLT amp)
{
    gw->ampneg = amp;
}
#line 1609 "gestvm.org"
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
#line 1666 "gestvm.org"
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
#line 1688 "gestvm.org"
static void uxn_inertia(gestvm *gvm, unsigned char val)
{
    SKFLT i;
    val &= 0xFF;

    i = (SKFLT)val / 0xFF;
    i *= 0.3;

    gvm->inertia = i;
}
#line 1727 "gestvm.org"
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
#line 177 "gestvm.org"
