add_cflags {
    "-DLUA_USE_C89",
    "-DLUA_COMPAT_5_2",
    "-Ilib/lua",
}

add_objects {
    "lib/lua/lua",

    -- lua core
    "lib/lua/lapi",
    "lib/lua/lcode",
    "lib/lua/lctype",
    "lib/lua/ldebug",
    "lib/lua/ldo",
    "lib/lua/ldump",
    "lib/lua/lfunc",
    "lib/lua/lgc",
    "lib/lua/llex",
    "lib/lua/lmem",
    "lib/lua/lobject",
    "lib/lua/lopcodes",
    "lib/lua/lparser",
    "lib/lua/lstate",
    "lib/lua/lstring",
    "lib/lua/ltable",
    "lib/lua/ltm",
    "lib/lua/lundump",
    "lib/lua/lvm",
    "lib/lua/lzio",

    -- lua lib
    "lib/lua/lauxlib",
    "lib/lua/lbaselib",
    "lib/lua/lbitlib",
    "lib/lua/lcorolib",
    "lib/lua/ldblib",
    "lib/lua/liolib", 
    "lib/lua/lmathlib",
    "lib/lua/loslib",
    "lib/lua/lstrlib",
    "lib/lua/ltablib",
    "lib/lua/lutf8lib",
    "lib/lua/loadlib",
    "lib/lua/linit",
}
