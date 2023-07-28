#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <monome.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

#define GFXBUF_PRIV
#include "gfxbuf/gfxbuf.h"
#include "mathc/mathc.h"
#include "sdf2d/sdf.h"

#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/nodes/sknodes.h"
#include "hotswap.h"

int mno_rt_new(sk_core *core,
               mno_hotswap *hs,
               const char *key, int len);

int sk_node_val(sk_core *core, SKFLT *val);
int sk_node_butlp(sk_core *core);
int sk_node_buthp(sk_core *core);
int sk_node_crossfade(sk_core *core);
typedef struct {
    float radius;
    int draw;
    int x, y;
} ripple;

#define MAX_RIPPLES 8

typedef struct {
    sk_core *core;
    mno_hotswap *hs;
    SKFLT nn;
    SKFLT gate;
    SKFLT ypos;
    SKFLT xpos;
} sound_data;

int output(sk_core *core, mno_hotswap *hs)
{
    sk_param in;
    int rc;
    gf_patch *patch;
    gf_node *node;

    patch = sk_core_patch(core);
    rc = sk_param_get(core, &in);
    SK_ERROR_CHECK(rc);
    rc = gf_patch_new_node(patch, &node);
    SK_ERROR_CHECK(rc);
    mno_hotswap_out(node, hs);
    sk_param_set(core, node, &in, 0);
    mno_hotswap_please(hs);

    return 0;
}

void paramscale(sk_core *core, SKFLT *val, SKFLT mn, SKFLT mx)
{
    sk_node_val(core, val);
    sk_core_constant(core, mn);
    sk_core_constant(core, mx);
    sk_node_scale(core);
}

int patch(sound_data *sd)
{
    sk_core *core;
    mno_hotswap *hs;

    hs = sd->hs;
    core = sd->core;

    sk_node_val(core, &sd->gate);
    sk_core_constant(core, 0.5);
    sk_core_constant(core, 2);
    /* TODO: create 'changed' node */
    sk_node_thresh(core);
    paramscale(core, &sd->xpos, 0.1, 1.0);
    sk_node_tgate(core);
    paramscale(core, &sd->xpos, 0.001, 0.5);
    paramscale(core, &sd->xpos, 0.1, 8.0);
    sk_node_envar(core);

    sk_node_val(core, &sd->nn);

    paramscale(core, &sd->ypos, 0.005, 0.05);
    sk_node_smoother(core);

    sk_node_mtof(core);
    sk_node_blsaw(core);
    sk_core_constant(core, 0.2);
    sk_node_mul(core);
    paramscale(core, &sd->xpos, 2000, 4000);
    sk_core_constant(core, 0.01);
    sk_node_smoother(core);
    sk_node_butlp(core);

    sk_node_mul(core);

    sk_core_dup(core);


    sk_core_constant(core, 0.95);
    sk_core_constant(core, 0.7);
    sk_core_constant(core, 1.0);
    sk_node_vardelay(core);
    sk_core_constant(core, 1000);
    sk_node_butlp(core);
    sk_core_constant(core, 500);
    sk_node_buthp(core);
    sk_core_constant(core, 0.8);

    sk_node_mul(core);

    paramscale(core, &sd->xpos, 0.1, 1.5);
    sk_core_constant(core, 2.0);
    sk_node_smoother(core);
    sk_node_mul(core);

    sk_node_add(core);


    sk_core_dup(core);
    sk_core_constant(core, 700);
    sk_node_buthp(core);
    sk_core_dup(core);
    sk_core_constant(core, 0.97);
    sk_core_constant(core, 10000.);
    sk_node_bigverb(core);
    sk_core_drop(core);
    sk_node_dcblocker(core);

    sk_core_dup(core);
    sk_core_constant(core, 7000);
    sk_node_butlp(core);
    sk_core_constant(core, 7);
    sk_node_softclip(core);
    sk_core_constant(core, 0.3);
    sk_node_mul(core);
    sk_core_constant(core, 0.7);
    sk_node_crossfade(core);

    sk_core_constant(core, 150);
    sk_core_constant(core, 0.1);
    sk_core_constant(core, 0.5);
    sk_node_lowshelf(core);


    paramscale(core, &sd->xpos, 0.1, 1.0);
    sk_core_constant(core, 4.1);
    sk_node_smoother(core);
    sk_node_mul(core);

    sk_node_add(core);

    sk_core_constant(core, 100);
    sk_core_constant(core, 30);
    sk_core_constant(core, 3.5);
    sk_node_peakeq(core);

    sk_core_constant(core, 0.9);
    sk_node_mul(core);
    output(core, hs);
    return 0;
}

void sound_data_init(sound_data *sd)
{
    sd->core = sk_core_new(44100);
    sd->hs = malloc(mno_hotswap_sizeof());
    sd->nn = 60;
    sd->gate = 1.0;
    sd->ypos = 0.0;
    mno_hotswap_init(sd->hs, sk_core_patch(sd->core));
    mno_rt_new(sd->core, sd->hs, "rt", 2);

    patch(sd);
}

void sound_data_clean(sound_data *sd)
{
    mno_hotswap_del(sd->hs);
    free(sd->hs);
    sk_core_del(sd->core);
}

typedef struct {
    ripple rstack[MAX_RIPPLES];
    int nripples;
    sound_data *sd;
} draw_data;

void data_init(draw_data *dd)
{
    int i;
    for (i = 0; i < MAX_RIPPLES; i++) {
        ripple *rip;
        rip = &dd->rstack[i];
        rip->radius = 0;
        rip->draw = 0;
        rip->x = 0;
        rip->y = 0;
    }
    dd->nripples = 0;
}

void add_ripple(draw_data *dd, int x, int y) {
    if (dd->nripples > MAX_RIPPLES) {
        printf("out of ripples\n");
        return;
    }
    ripple *rip;
    int i;

    for (i = 0; i < MAX_RIPPLES; i++) {
        rip = &dd->rstack[i];
        if (rip->draw == 0) break;
    }
    rip->radius = 0;
    rip->draw = 1;
    rip->x = x;
    rip->y = y;
    dd->nripples++;
}

void data_update(draw_data *dd, float delta)
{
    int i;
    float scale;
    float expamt;
    float se;

    scale = 1.0/15.0;
    expamt = -1.0;
    se = 1.0 / (1.0 - exp(expamt));

    for (i = 0; i < MAX_RIPPLES; i++) {
        ripple *rip;
        rip = &dd->rstack[i];
        if (rip->draw) {
            float amt;
            amt = (1.0 - rip->y*scale);
            amt = (1.0 - exp(expamt*amt))*se;
            rip->radius += delta * (amt*3.0 + 0.2);
            if (rip->radius > 2.5) {
                dd->nripples--;
                rip->draw = 0;
                rip->radius = 0;
            }
        }
    }
}

void draw_ripple(struct vec2 *p, float radius, int x, int y, struct vec3 *oclr)
{
    struct vec3 clr[2];
    float d;
    float d2;
    float pos;
    const float scale16 = 1.0 / 16.0;
    struct vec2 poff;

    poff = *p;

    poff.x += ((16 - x)*scale16*2) - 1.0;
    poff.y += (y*scale16*2) - 1.0;
    d = sdf_circle(poff, radius);
    if (radius >= 0.05) {
        d2 = sdf_circle(poff, radius-0.05);
    } else {
        d2 = sdf_circle(poff, 0);
    }
    d = sdf_max(-d2, d);

    clr[0] = svec3(0.0, 0.0, 0.0);
    clr[1] = svec3(1.0, 1.0, 1.0);

    pos = d > 0.0 ? 1.0 : 0.0;
    pos *= 1.0 - exp(-7 * fabs(d));
    *oclr = svec3_zero();

    psvec3_lerp(oclr, &clr[1], &clr[0], pos);
}

void draw(gfxbuf_state *st)
{
    gfxbuf_pixel *out;

    struct vec2 iResolution;
    struct vec2 fragCoord;

	struct vec2 p;
	struct vec3 outclr;
	struct vec3 tmp;
	draw_data *dd;
	int r;

    dd = st->info->ud;
    fragCoord.x = st->x;
    fragCoord.y = st->info->rh - st->y;
    iResolution.x = st->info->rw;
    iResolution.y = st->info->rh;
    p = svec2_multiply_f(fragCoord, 2.0);
    psvec2_subtract(&p, &p, &iResolution);
    psvec2_divide_f(&p, &p, iResolution.y);

    outclr = svec3_zero();
    /* needs to be a FIFO, but using brute force instead */
    for (r = 0; r  < MAX_RIPPLES; r++) {
        ripple *rip;
        rip = &dd->rstack[r];
        if (rip->draw) {
            draw_ripple(&p, rip->radius, rip->x, rip->y, &tmp);
            psvec3_add(&outclr, &outclr, &tmp);
        }
    }

    if (outclr.x < 0) outclr.x = 0;
    if (outclr.x > 1) outclr.x = 1;
    if (outclr.y < 0) outclr.y = 0;
    if (outclr.y > 1) outclr.y = 1;
    if (outclr.z < 0) outclr.z = 0;
    if (outclr.z > 1) outclr.z = 1;

    out = st->out;
    out->r = outclr.x;
    out->g = outclr.y;
    out->b = outclr.z;
}

#define WIDTH 16
#define HEIGHT 16

static double now_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec*1e-6;
}

const int melodic_scale[] = {
    -15, -12, -10, -8,
    -7, -5, -3, -1,
    0, 2, 4, 7,
    11, 12, 14, 16
};

void handle_press(const monome_event_t *e, void *data) {
    unsigned int x, y;
    draw_data *dd;
    sound_data *sd;
    int base;

    dd = (draw_data *)data;

    x = e->grid.x;
    y = e->grid.y;

    add_ripple(dd, x, y);

    sd = dd->sd;
    base = 57;
    if (y >= 11) base -= 12;
    sd->nn = base + melodic_scale[15-y];
    sd->ypos = y / 15.0;
    sd->xpos = x / 15.0;
    if (sd->gate > 0) {
        sd->gate = 0;
    } else {
        sd->gate = 1;
    }
}

static volatile int is_running = 0;
static void please_close(int sig)
{
    is_running = 0;
}

int main(int argc, char *argv[])
{
    gfxbuf *gfx;
    gfxbuf_pixel *buf;
    int x, y;
    uint8_t map[4][64];
    monome_t *monome;
    draw_data dd;
    double now, then;
	double timer;
	sound_data sd;

    if (!(monome = monome_open("/dev/ttyACM0", "8000"))) {
        printf("monome_open fail\n");
        return -1;
    }

    signal(SIGINT, please_close);
    gfx = gfxbuf_new(WIDTH, HEIGHT);
    monome_register_handler(monome, MONOME_BUTTON_DOWN, handle_press, &dd);

    /* gfxbuf_ppm(gfx, "out.ppm"); */

    buf = gfx->pix->buf;
    data_init(&dd);
    sound_data_init(&sd);
    dd.sd = &sd;
    timer = 0;

    now = then = now_sec();

    is_running = 1;
    while (is_running) {
        while(monome_event_handle_next(monome));
        now = now_sec();
        if ((now - then) < 0.02) {
            usleep(80);
            continue;
        }
#if 0
        if (timer <= 0) {
            add_ripple(&dd, 12, 12);
            timer = 0.5;
        }
#endif
        gfxbuf_draw(gfx, 0, 0, WIDTH, HEIGHT, draw, &dd);
        data_update(&dd, now - then);
        timer -= now - then;
        then = now;
        for (y = 0; y < HEIGHT; y++) {
            for (x = 0; x < WIDTH; x++) {
                float s;
                int bpos;
                uint8_t b;
                uint8_t val;
                uint8_t *mbuf;
                int xp, yp;

                xp = x;
                yp = y;

                if (yp < 8) {
                    if (xp < 8) {
                        mbuf = map[0];
                    } else {
                        xp -= 8;
                        mbuf = map[1];
                    }
                } else {
                    yp -= 8;
                    if (xp < 8) {
                        mbuf = map[2];
                    } else {
                        xp -= 8;
                        mbuf = map[3];
                    }
                }

                s = buf[y*WIDTH + x].r;
                bpos = yp*8 + xp;
                b = mbuf[bpos];
                val = (uint8_t)(s*0xf);
                val &= 0xf;
                b = val;
                mbuf[bpos] = b;
            }
        }

        monome_led_level_map(monome, 0, 0, map[0]);
        monome_led_level_map(monome, 8, 0, map[1]);
        monome_led_level_map(monome, 0, 8, map[2]);
        monome_led_level_map(monome, 8, 8, map[3]);
        usleep(8000);
    }

    printf("bye.\n");
    monome_led_all(monome, 0);
    monome_close(monome);
    gfxbuf_del(gfx);
    sound_data_clean(&sd);
    return 0;
}
