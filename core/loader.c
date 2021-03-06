#include <stdlib.h>
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"

void lil_load_kuf(lil_t lil);
void lil_load_gfxbuf(lil_t lil);
void lil_load_var(lil_t lil);
void sklil_load_gest(lil_t lil);
void sklil_load_gestlang(lil_t lil);
void sklil_loader_withextra(lil_t lil);
void lil_load_sqlar(lil_t lil);
void lil_load_tract(lil_t lil);
void lil_load_btprnt(lil_t lil);
void lil_load_draw(lil_t lil);
void lil_load_hotswap(lil_t lil);
void lil_load_rt(lil_t lil);
void lil_load_misc(lil_t lil);
void load_gestvm(lil_t lil);

void mno_load(lil_t lil)
{
    sklil_loader_withextra(lil);
    lil_load_kuf(lil);
    lil_load_gfxbuf(lil);
    lil_load_var(lil);
    sklil_load_gest(lil);
    sklil_load_gestlang(lil);
    lil_load_sqlar(lil);
    lil_load_tract(lil);
    lil_load_btprnt(lil);
    lil_load_draw(lil);
    lil_load_hotswap(lil);
    lil_load_misc(lil);
    load_gestvm(lil);
}

void mno_clean(lil_t lil)
{
    sklil_clean(lil);
}
