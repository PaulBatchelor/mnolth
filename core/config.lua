add_tangled_object("core/hotswap")
add_tangled_object("core/gfxbuf")
add_object("core/l_gfxbuf")
add_tangled_object("core/draw")
add_tangled_object("core/var")
add_tangled_object("core/misc", false)
add_tangled_object("core/uf2")

add_object("core/loader")
add_object("core/l_var")
add_object("core/mains")
add_object("core/scheme")
add_c99object("core/l_sqlar")
add_object("core/l_tract")
add_object("core/l_draw")
add_object("core/l_hotswap")

-- disables main in tinyscheme
add_cflags {
    "-DSTANDALONE=0"
}
