add_objects {
    "lib/libxm/context",
    "lib/libxm/load",
    "lib/libxm/play",
    "lib/libxm/state",
    "lib/libxm/n_xm"
}

add_c99objects {
    "lib/libxm/xmtowav"
}

add_cflags {
   "-DXM_LINEAR_INTERPOLATION=1",
   "-DXM_DEBUG=0"
}
