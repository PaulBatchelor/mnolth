#define _BSD_SOURCE /* for usleep() */
#include <monome.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

typedef struct {
    /* TODO implement */
	monome_t *monome;
	lua_State *L;
    uint8_t map[64];

} monome_arc_data;

static void delta2lua(lua_State *L, int number, int delta)
{
    int pos;

    lua_len(L, -1);
    pos = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushinteger(L, pos + 1);

    lua_createtable(L, 2, 0);

    lua_pushinteger(L, 1);
    lua_pushinteger(L, number);
    lua_settable(L, -3);

    lua_pushinteger(L, 2);
    lua_pushinteger(L, delta);
    lua_settable(L, -3);

    lua_settable(L, -3);
}
void handle_delta(const monome_event_t *e, void *data)
{
	unsigned int number, delta;
    lua_State *L;
    monome_arc_data *md;

    md = data;
    number = e->encoder.number;
    delta = e->encoder.delta;
	L = md->L;

    delta2lua(L, number, delta);
}

static void clear_rings(monome_t *monome)
{
	int i;

	for( i = 0; i < 4; i++ )
		monome_led_ring_all(monome, i, 0);
}

static int arc_open(lua_State *L)
{
    const char *path;
    monome_arc_data *md;

    path = lua_tostring(L, 1);
    printf("opening %s\n", path);

    md = lua_newuserdata(L, sizeof(monome_arc_data));

	if(!(md->monome = monome_open(path, "8000"))) {
        luaL_error(L, "Could not open arc at %s", path);
    }

	clear_rings(md->monome);

	monome_register_handler(md->monome,
	                        MONOME_ENCODER_DELTA,
	                        handle_delta,
	                        md);
    md->L = L;
    return 1;
}

static int arc_close(lua_State *L)
{
    monome_arc_data *md;
    md = lua_touserdata(L, 1);

	clear_rings(md->monome);
	monome_close(md->monome);
    return 0;
}

static int arc_get_input_events(lua_State *L)
{
    monome_arc_data *md;
    md = lua_touserdata(L, 1);

    lua_newtable(L);

    while(monome_event_handle_next(md->monome));
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
    monome_arc_data *md;
    int i;
    int knob;
    uint32_t state[2];
    uint8_t *map;

    md = lua_touserdata(L, 1);
    map = md->map;

    knob = lua_tointeger(L, 2);
    state[0] = lua_tointeger(L, 3);
    state[1] = lua_tointeger(L, 4);

    for (i = 0; i < 32; i ++) {
        int p;
        p = i;
        map[p] = ((state[0] & (1 << i)) >> i) * 15;
        p = i + 32;
        map[p] = ((state[1] & (1 << i)) >> i) * 15;
    }

    monome_led_ring_map(md->monome, knob, map);
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
