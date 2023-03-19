#define _BSD_SOURCE /* for usleep() */
#include <monome.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

typedef struct {
    /* TODO impleemnt */
	monome_t *monome;
} monome_arc_data;

static int arc_open(lua_State *L)
{
    /* TODO: implement */
#if 0
    const char *path;
    monome_arc_data *md;

    path = lua_tostring(L, 1);
    printf("opening %s\n", path);

    md = lua_newuserdata(L, sizeof(monome_arc_data));

	if(!(md->monome = monome_open(path, "8000"))) {
        luaL_error(L, "Could not open arc at %s", path);
    }

	monome_led_all(md->monome, 0);

	monome_register_handler(md->monome,
	                        MONOME_BUTTON_DOWN,
	                        handle_press_down,
	                        md);

	monome_register_handler(md->monome,
	                        MONOME_BUTTON_UP,
	                        handle_press_up,
	                        md);

    md->L = L;
#endif
    return 1;
}

static int arc_close(lua_State *L)
{
    /* TODO: implement */
#if 0
    monome_arc_data *md;
    md = lua_touserdata(L, 1);

	monome_led_all(md->monome, 0);
	monome_close(md->monome);
#endif
    return 0;
}

static int arc_get_input_events(lua_State *L)
{
    /* TODO: implement */
#if 0
    monome_arc_data *md;
    md = lua_touserdata(L, 1);

    lua_newtable(L);

    while(monome_event_handle_next(md->monome));
#endif
    return 1;
}

static int arc_usleep(lua_State *L)
{
    unsigned int usecs;
    usecs = lua_tointeger(L, 1);

    usleep(usecs);
    return 0;
}

static int arc_update(lua_State *L)
{
    /* TODO implement */
#if 0
    monome_arc_data *md;
    int i;
    md = lua_touserdata(L, 1);

    for (i = 0; i < 8; i++) {
        int val;
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 2);
        val = lua_tointeger(L, -1);
        lua_pop(L, 1);
        md->quadL[i] = val;
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 3);
        val = lua_tointeger(L, -1);
        lua_pop(L, 1);
        md->quadR[i] = val;
    }

    monome_led_map(md->monome, 0, 0, md->quadL);
    monome_led_map(md->monome, 255, 0, md->quadR);
#endif
    return 0;
}

static double now_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec*1e-6;
}

static int arc_now(lua_State *L)
{
    double t;

    t = now_sec();

    lua_pushnumber(L, t);
    return 1;
}

static const luaL_Reg arc_lib[] = {
    {"open", arc_open},
    {"close", arc_close},
    {"get_input_events", arc_get_input_events},
    {"usleep", arc_usleep},
    {"update", arc_update},
    {"now", arc_now},
    {NULL, NULL}
};

int luaopen_monome_arc(lua_State *L)
{
    luaL_newlib(L, arc_lib);
    return 1;
}
