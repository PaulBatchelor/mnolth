OUTPUT=lua

if [ "$#" -gt 0 ]
then
    OUTPUT=$1
fi

cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lapi.c -o lapi.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lcode.c -o lcode.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lctype.c -o lctype.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ldebug.c -o ldebug.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ldo.c -o ldo.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ldump.c -o ldump.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lfunc.c -o lfunc.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lgc.c -o lgc.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c llex.c -o llex.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lmem.c -o lmem.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lobject.c -o lobject.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lopcodes.c -o lopcodes.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lparser.c -o lparser.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lstate.c -o lstate.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lstring.c -o lstring.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ltable.c -o ltable.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ltm.c -o ltm.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lundump.c -o lundump.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lvm.c -o lvm.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lzio.c -o lzio.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lauxlib.c -o lauxlib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lbaselib.c -o lbaselib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lbitlib.c -o lbitlib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lcorolib.c -o lcorolib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ldblib.c -o ldblib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c liolib.c -o liolib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lmathlib.c -o lmathlib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c loslib.c -o loslib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lstrlib.c -o lstrlib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c ltablib.c -o ltablib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lutf8lib.c -o lutf8lib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c loadlib.c -o loadlib.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c linit.c -o linit.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua -c lua.c -o lua.o
cc -DLUA_USE_C89 -DLUA_COMPAT_5_2 -DLUA_MAIN -Ilib/lua lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o lauxlib.o lbaselib.o lbitlib.o lcorolib.o ldblib.o liolib.o lmathlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o loadlib.o linit.o lua.o -o $OUTPUT -lm
