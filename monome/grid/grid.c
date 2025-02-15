#define _BSD_SOURCE /* for usleep() */
#include <monome.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

typedef struct {
	monome_t *monome;
	lua_State *L;
    uint8_t quadL[8];
    uint8_t quadR[8];

    uint8_t map[4][64];
} monome_grid_data;

static void pos2lua(lua_State *L, int x, int y, int s)
{
    int pos;

    lua_len(L, -1);
    pos = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushinteger(L, pos + 1);

    lua_createtable(L, 3, 0);
    lua_pushinteger(L, 1);
    lua_pushinteger(L, x);
    lua_settable(L, -3);

    lua_pushinteger(L, 2);
    lua_pushinteger(L, y);
    lua_settable(L, -3);

    lua_pushinteger(L, 3);
    lua_pushinteger(L, s);
    lua_settable(L, -3);

    lua_settable(L, -3);
}

static void handle_press_up(const monome_event_t *e, void *data)
{
	unsigned int x, y;
    lua_State *L;
    monome_grid_data *md;

    md = data;
	x = e->grid.x;
	y = e->grid.y;
	L = md->L;

    pos2lua(L, x, y, 0);
}

static void handle_press_down(const monome_event_t *e, void *data)
{
	unsigned int x, y;
    lua_State *L;
    monome_grid_data *md;

    md = data;
	x = e->grid.x;
	y = e->grid.y;
	L = md->L;

    pos2lua(L, x, y, 1);
}

static int grid_open(lua_State *L)
{
    const char *path;
    monome_grid_data *md;

    path = lua_tostring(L, 1);
    printf("opening %s\n", path);

    md = lua_newuserdata(L, sizeof(monome_grid_data));

	if(!(md->monome = monome_open(path, "8000"))) {
        luaL_error(L, "Could not open grid at %s", path);
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

    return 1;
}

static int grid_close(lua_State *L)
{
    monome_grid_data *md;
    md = lua_touserdata(L, 1);

	monome_led_all(md->monome, 0);
	monome_close(md->monome);
    return 0;
}

static int grid_get_input_events(lua_State *L)
{
    monome_grid_data *md;
    md = lua_touserdata(L, 1);

    lua_newtable(L);

    while(monome_event_handle_next(md->monome));

    return 1;
}

static int grid_usleep(lua_State *L)
{
    unsigned int usecs;
    usecs = lua_tointeger(L, 1);

    usleep(usecs);
    return 0;
}

static int grid_update(lua_State *L)
{
    monome_grid_data *md;
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
    return 0;
}

/* like grid_udpate, only for the zero. renders to bottom
 * quads.
 */
static int grid_update_zero(lua_State *L)
{
    monome_grid_data *md;
    int i, k;
    uint8_t *left, *right;
    md = lua_touserdata(L, 1);

    left = md->map[2];
    right = md->map[3];

    for (i = 0; i < 8; i++) {
        int val;
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 2);
        val = lua_tointeger(L, -1);
        lua_pop(L, 1);

        for (k = 0; k < 8; k++) {
            uint8_t b;
            b = (val & (1<<k)) > 0;
            b *= 0xF;
            left[i*8 + k] = b;
        }

        lua_pushinteger(L, i + 1);
        lua_gettable(L, 3);
        val = lua_tointeger(L, -1);
        lua_pop(L, 1);

        for (k = 0; k < 8; k++) {
            uint8_t b;
            b = (val & (1<<k)) > 0;
            b *= 0xF;
            right[i*8 + k] = b;
        }
    }

    monome_led_level_map(md->monome, 0, 8, left);
    monome_led_level_map(md->monome, 8, 8, right);
    return 0;
}

static int grid_level_map(lua_State *L)
{
    monome_grid_data *md;
    int i;
    md = lua_touserdata(L, 1);

    for (i = 0; i < 256; i++) {
        int val, x, y;
        uint8_t *map;
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 2);
        val = lua_tointeger(L, -1);
        lua_pop(L, 1);

        x = i % 16;
        y = i / 16;

        if (y < 8) {
            if (x < 8) {
                map = md->map[0];
            } else {
                x -= 8;
                map = md->map[1];
            }
        } else {
            y -= 8;
            if (x < 8) {
                map = md->map[2];
            } else {
                x -= 8;
                map = md->map[3];
            }
        }

        map[y*8 + x] = val & 0xF;
    }

    monome_led_level_map(md->monome, 0, 0, md->map[0]);
    monome_led_level_map(md->monome, 8, 0, md->map[1]);
    monome_led_level_map(md->monome, 0, 8, md->map[2]);
    monome_led_level_map(md->monome, 8, 8, md->map[3]);
    return 0;
}

static double now_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec*1e-6;
}

static int grid_now(lua_State *L)
{
    double t;

    t = now_sec();

    lua_pushnumber(L, t);
    return 1;
}

static int grid_all(lua_State *L)
{
    int s;
    monome_grid_data *md;

    md = lua_touserdata(L, 1);
    s = lua_tointeger(L, 2);

    monome_led_all(md->monome, s);

    return 0;
}

static const luaL_Reg grid_lib[] = {
    {"open", grid_open},
    {"close", grid_close},
    {"get_input_events", grid_get_input_events},
    {"usleep", grid_usleep},
    {"update", grid_update},
    {"update_zero", grid_update_zero},
    {"level_map", grid_level_map},
    {"now", grid_now},
    {"all", grid_all},
    {NULL, NULL}
};

int luaopen_monome_grid(lua_State *L)
{
    luaL_newlib(L, grid_lib);
    return 1;
}
