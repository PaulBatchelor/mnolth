#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

int mno_lua_main(int argc, char **argv,
                 void (*load)(lua_State*),
                 void (*clean)(lua_State*));

int main(int argc, char *argv[])
{
    return mno_lua_main(argc, argv, NULL, NULL);
}
