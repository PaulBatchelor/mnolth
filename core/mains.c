#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dynload.h"
#include "scheme.h"
#include "scheme-private.h"
#include "init.scm.h"
#include "sndkit/graforge/graforge.h"
#include "sndkit/core.h"
#include "sndkit/lil/lil.h"
#include "sndkit/nodes/sklil.h"
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

void mno_load(lil_t lil);
void mno_clean(lil_t lil);

#ifdef MNODES
void mnodes_lua_funcs(lua_State *L);
#endif

static pointer scm_lil(scheme *sc, pointer args)
{
    const char *str;
    lil_value_t val;
    lil_t lil;
    const char *err;
    size_t pos;

    lil = (lil_t)scheme_get_external_data(sc);
    str = string_value(pair_car(args));
    val = lil_parse(lil, str, strlen(str), 0);
    lil_free_value(val);

    if (lil_error(lil, &err, &pos)) {
        return mk_string(sc, err);
    }

    return sc->T;
}

static pointer scm_pop(scheme *sc, pointer args)
{
    lil_t lil;
    sk_core *core;
    int rc;
    sk_stack *stk;
    sk_stacklet *s;

    lil = (lil_t)scheme_get_external_data(sc);

    core = lil_get_data(lil);

    stk = sk_core_stack(core);

    rc = sk_stack_pop(stk, &s);

    if (rc) {
        return sc->NIL;
    }

    if (sk_stacklet_isconstant(s)) {
        return mk_real(sc, sk_stacklet_constant(s));
    }

    return sc->NIL;
}

static pointer scm_cmd(scheme *sc, pointer args)
{
    char buf[128];
    int pos;
    int n;
    memset(buf, 0, 128);
    pos = 0;
    while (args != sc->NIL && pos < 127) {
        const char *str;
        int len;
        str = string_value(pair_car(args));
        len = strlen(str);

        if (pos != 0) {
            buf[pos] = ' ';
            pos++;
        }

        for (n = 0; n < len; n++) {
            if (pos >= 127) return sc->NIL;
            buf[pos] = str[n];
            pos++;
        }
        args = pair_cdr(args);
    }

    return mk_string(sc, buf);
}

static pointer scm_mnotop(scheme *sc, pointer args)
{
    const char *str;

    str = getenv("MNOLTH_TOP");

    if (str != NULL) return mk_string(sc, str);

    return mk_string(sc, "/usr/local/share/mnolth");
}

static pointer scm_getenv(scheme *sc, pointer args)
{
    const char *str;
    const char *name;

    name = string_value(pair_car(args));
    str = getenv(name);

    if (str != NULL) return mk_string(sc, str);

    return sc->NIL;
}

static pointer scm_chdir(scheme *sc, pointer args)
{
    const char *path;

    path = string_value(pair_car(args));

    if (chdir(path)) {
        return sc->F;
    }

    return sc->T;
}

scheme * mno_scm_new(lil_t lil)
{
    scheme *sc;
    sk_core *core;

    sc = malloc(sizeof(scheme));
    memset(sc, 0, sizeof(scheme));
    scheme_init(sc);

    core = lil_get_data(lil);

    sk_core_append(core, "scm", 3, sc, NULL);

    scheme_set_external_data(sc, lil);

    scheme_set_input_port_file(sc, stdin);
    scheme_set_output_port_file(sc, stdout);
    scheme_eval_string(sc, init_scm);

#if USE_DL
    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "load-extension"),
        mk_foreign_func(sc, scm_load_ext));
#endif

    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "lil"),
        mk_foreign_func(sc, scm_lil));

    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "pop"),
        mk_foreign_func(sc, scm_pop));

    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "cmd"),
        mk_foreign_func(sc, scm_cmd));

    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "mnotop"),
        mk_foreign_func(sc, scm_mnotop));

    scheme_define(sc, sc->global_env,
        mk_symbol(sc,"getenv"),
        mk_foreign_func(sc, scm_getenv));

    scheme_define(sc, sc->global_env,
        mk_symbol(sc, "chdir"),
        mk_foreign_func(sc, scm_chdir));

    return sc;
}

int mno_scm_main(int argc, char *argv[],
                 void (*load)(lil_t),
                 void (*clean)(lil_t))
{
    scheme *sc;
    FILE *fp;
    lil_t lil;

    if (load == NULL) load = mno_load;
    if (clean == NULL) clean = mno_clean;

    lil = lil_new();
    load(lil);

    sc = mno_scm_new(lil);

    if(argc == 1) {
        scheme_load_named_file(sc, stdin, 0);
    } else {
        fp = fopen(argv[1], "r");
        scheme_load_named_file(sc, fp, argv[1]);
        fclose(fp);
    }

    clean(lil);
    lil_free(lil);
    scheme_deinit(sc);
    free(sc);

    return 0;
}

int mno_scm_main_noloader(int argc, char *argv[])
{
    return mno_scm_main(argc, argv, NULL, NULL);
}

int mno_lil_main(int argc, char *argv[],
                 void (*load)(lil_t),
                 void (*clean)(lil_t))
{
    if (load == NULL) load = mno_load;
    if (clean == NULL) clean = mno_clean;
    return lil_main(argc, argv, load, clean);
}

int mno_lil_main_noloader(int argc, char *argv[])
{
    return mno_lil_main(argc, argv, NULL, NULL);
}

static int lvler(lua_State *L)
{
    lil_t lil;
    const char *str;
    lil_value_t val;
    const char *err;
    size_t pos;

    lua_getglobal(L, "__lil");
    lil = lua_touserdata(L, -1);

    str = lua_tostring(L, 1);

    val = lil_parse(lil, str, strlen(str), 0);
    lil_free_value(val);

    if (lil_error(lil, &err, &pos)) {
        luaL_error(L, "lil error: %s\n", err);
    }

    return 0;
}

static int lilpop(lua_State *L)
{
    lil_t lil;
    sk_core *core;
    sk_stacklet *s;
    int rc;
    sk_stack *stk;

    lua_getglobal(L, "__lil");
    lil = lua_touserdata(L, -1);

    core = lil_get_data(lil);

    /* rc = sk_param_get_constant(core, &out); */
    stk = sk_core_stack(core);
    rc = sk_stack_pop(stk, &s);

    if (rc) {
        luaL_error(L, "could not pop value\n");
    }

    if (sk_stacklet_isconstant(s)) {
        float out;
        out = sk_stacklet_constant(s);
        lua_pushnumber(L, out);
        return 1;
    } else if (sk_stacklet_isgeneric(s) || sk_stacklet_istable(s)) {
        void *ud;
        ud = sk_stacklet_userdata(s);
        lua_pushlightuserdata(L, ud);
        return 1;
    }

    luaL_error(L, "Unknown data type\n");
    return 0;
}

static int mnoreset(lua_State *L)
{
    lil_t lil;

    lua_getglobal(L, "__lil");
    lil = lua_touserdata(L, -1);

    mno_clean(lil);
    lil_free(lil);

    lil = lil_new();
    mno_load(lil);

    lua_pushlightuserdata(L, lil);
    lua_setglobal(L, "__lil");
    return 0;
}

static int mnorealloc(lua_State *L)
{
    lil_t lil;
    sk_core *core;
    gf_patch *patch;
    int blksize;
    int nbuffers;
    int stack_size;

    nbuffers = lua_tointeger(L, 1);
    stack_size = lua_tointeger(L, 2);

    lua_getglobal(L, "__lil");
    lil = lua_touserdata(L, -1);

    core = lil_get_data(lil);

    patch = sk_core_patch(core);
    blksize = gf_patch_blksize(patch);
    gf_patch_realloc(patch, nbuffers, stack_size, blksize);
    return 0;
}

void gestvm_memops_lua(lua_State *L);
int luaopen_lpeg(lua_State *L);
int luaopen_lsqlite3(lua_State *L);
int luaopen_mnobuf(lua_State *L);
int luaopen_btprnt(lua_State *L);
int luaopen_sdfvm(lua_State *L);

#ifdef MONOME_GRID
int luaopen_monome_grid(lua_State *L);
#endif

#ifdef MONOME_ARC
int luaopen_monome_arc(lua_State *L);
#endif

static void load_lua_funcs(lua_State *L, lil_t lil)
{
    lua_pushlightuserdata(L, lil);
    lua_setglobal(L, "__lil");
    lua_register(L, "lil", lvler);
    lua_register(L, "pop", lilpop);
    lua_register(L, "mnoreset", mnoreset);
    lua_register(L, "mnorealloc", mnorealloc);
    gestvm_memops_lua(L);
    luaL_requiref(L, "lpeg", luaopen_lpeg, 1);
    luaL_requiref(L, "sqlite3", luaopen_lsqlite3, 1);
    luaL_requiref(L, "sdfvm", luaopen_sdfvm, 1);
#ifdef MONOME_GRID
    luaL_requiref(L, "monome_grid", luaopen_monome_grid, 1);
#endif

#ifdef MONOME_ARC
    luaL_requiref(L, "monome_arc", luaopen_monome_arc, 1);
#endif

#ifdef MNODES
    mnodes_lua_funcs(L);
#endif

    luaL_requiref(L, "mnobuf", luaopen_mnobuf, 1);
    luaL_requiref(L, "btprnt", luaopen_btprnt, 1);
}

int lua_main (int argc, char **argv,
              void (*loader)(lua_State*),
              void (*clean)(lua_State*));

void mno_lua_load(lua_State *L)
{
    lil_t lil;
    sk_core *core;

    lil = lil_new();
    mno_load(lil);
    load_lua_funcs(L, lil);

    core = lil_get_data(lil);

    sk_core_append(core, "lua", 3, L, NULL);
}

void mno_lua_clean(lua_State *L)
{
    lil_t lil;

    lua_getglobal(L, "__lil");

    lil = lua_touserdata(L, -1);

    mno_clean(lil);
    lil_free(lil);
}


int mno_lua_main(int argc, char **argv,
                 void (*load)(lua_State*),
                 void (*clean)(lua_State*))
{
    if (load == NULL) load = mno_lua_load;
    if (clean == NULL) clean = mno_lua_clean;

    return lua_main(argc, argv, load, clean);
}

int mno_lua_main_noloader(int argc, char *argv[])
{
    return mno_lua_main(argc, argv, NULL, NULL);
}
