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
void load_gestlive(lil_t lil);
void gestvm_memops_lil(lil_t lil);
void lil_load_uf2(lil_t lil);
void lil_load_bpsdf(lil_t lil);
void lil_load_mnobuf(lil_t lil);

#ifdef MNODES
void sklil_load_mnodes(lil_t lil);
#endif

#ifdef BUILD_MNORT
void lil_load_hotswap(lil_t lil);
#endif

#ifdef MNOLIGHT
void sklil_load_light(lil_t lil);
#endif

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
    load_gestlive(lil);
    gestvm_memops_lil(lil);
    lil_load_uf2(lil);
    lil_load_bpsdf(lil);
    lil_load_mnobuf(lil);

#ifdef MNODES
    sklil_load_mnodes(lil);
#endif

#ifdef BUILD_MNORT
    lil_load_hotswap(lil);
    lil_load_rt(lil);
#endif

#ifdef MNOLIGHT
    sklil_load_light(lil);
#endif

}

void mno_clean(lil_t lil)
{
    sklil_clean(lil);
}
