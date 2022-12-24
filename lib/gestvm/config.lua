add_objects {
    "lib/gestvm/gestvm",
	"lib/gestvm/l_gestvm",
	"lib/gestvm/uxn/uxn",
	"lib/gestvm/uxn/uxnasm",
}

-- Extra objects (not part of standard gestvm)

add_tangled_objects {
    {"lib/gestvm/memops", false},
	"lib/gestvm/gestlive",
}
