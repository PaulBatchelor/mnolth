function mkrule(name, command, description)
    return {name=name, command=command, description=description}
end

function mkbuild(outputs, rule, inputs, vars)
    return {
        outputs = outputs,
        rule = rule,
        inputs = inputs,
        vars = vars
    }
end

rules = {}
build = {}

objects = {}

tangled = {}

cflags = {"-Wall", "-pedantic", "-O3"}

-- mac hacks
table.insert(cflags, "-I/opt/homebrew/include")

function add_object(obj)
    obj_o = obj .. ".o"
    table.insert(build,
        mkbuild(obj_o,
            "c89",
            obj .. ".c"))

    table.insert(objects, obj_o)
end

function add_c99object(obj)
    obj_o = obj .. ".o"
    table.insert(build,
        mkbuild(obj .. ".o",
            "c99",
            obj .. ".c"))
    table.insert(objects, obj_o)
end

function add_objects(objs)
    for _,o in pairs(objs) do
        add_object(o)
    end
end

function add_c99objects(objs)
    for _,o in pairs(objs) do
        add_c99object(o)
    end
end

function add_cflags(flags)
    for _,f in pairs(flags) do
        table.insert(cflags, f)
    end
end

function add_tangled_object(obj, header)
    if header == nil then header = true end

    tangled_c = obj .. ".c"
    tangled_files = tangled_c

    if header == true then
        tangled_files = {tangled_c, obj .. ".h"}
    end

    table.insert(build,
        mkbuild(tangled_files, "worgle", obj .. ".org"))

    table.insert(tangled, tangled_c)

    add_object(obj)
end

function add_tangled_objects(obj)
    for _,v in pairs(obj) do
        if (type(v) == "string") then
            add_tangled_object(v)
        else
            add_tangled_object(v[1], v[2])
        end
    end
end

table.insert(rules,
    mkrule("worgle", "util/worgle/worglite -Werror -g $in"))
table.insert(rules,
    mkrule("c89",
        "gcc -std=c89 -c $cflags $in -o $out",
        "c89 $in"))
table.insert(rules,
    mkrule("c99",
        "gcc -std=c99 -c $cflags $in -o $out",
        "c99 $in"))
table.insert(rules,
    mkrule("link",
        "gcc $cflags $in -o $out $ldflags $libs",
        "creating $out"))
table.insert(rules,
    mkrule("ar", "ar rcs $out $in", "creating $out"))

libs = {
    "-lm",

    -- used by SQLite
    "-lpthread",

    "-ldl",

    -- "-lreadline",
}

-- table.insert(libs, "-lx264")

add_cflags({"-Ilib", "-Icore"})

function generate_ninja()
    fp = io.open("build.ninja", "w")
    objstr = table.concat(objects, " ")

    -- mnolth
    table.insert(build,
        mkbuild("core/lil_main.o", "c89", "core/lil_main.c"))
    table.insert(build,
        mkbuild("mnolth",
            "link",
             objstr .. " core/lil_main.o"))

    -- mnotil
    table.insert(build,
        mkbuild("util/mnotil.o", "c89", "util/mnotil.c"))
    table.insert(build,
        mkbuild("mnotil",
            "link",
             objstr .. " util/mnotil.o"))

    -- mnolua
    table.insert(build,
        mkbuild("core/lua_main.o", "c89", "core/lua_main.c"))
    table.insert(build,
        mkbuild("mnolua",
            "link",
             objstr .. " core/lua_main.o"))

    -- mnoscm
    table.insert(build,
        mkbuild("core/scm_main.o", "c89", "core/scm_main.c"))
    table.insert(build,
        mkbuild("mnoscm",
            "link",
             objstr .. " core/scm_main.o"))

    -- libmnolth
    table.insert(build,
        mkbuild("libmnolth.a", "ar", objstr))

    mnort_items = {
          "util/mnort/server",
		  "util/mnort/client",
		  "util/mnort/rt",
		  "util/mnort/mnort"
    }

    mnort_objs = {}

    for _,o in pairs(mnort_items) do
        table.insert(build,
            mkbuild(o .. ".o", "c89", o .. ".c"))
        table.insert(mnort_objs, o .. ".o")
    end

    table.insert(build,
        mkbuild("mnort",
            "link",
             objstr ..
             " " ..
             table.concat(mnort_objs, " "),
             {"libs = -ljack"}))


    table.insert(build,
        mkbuild("util/worgle/worglite",
            "link",
             "util/worgle/worgle.c util/worgle/parg.c",
             {"cflags = -O3 -DWORGLITE"}))

    table.insert(build, mkbuild("worglite",
        "phony", "util/worgle/worglite"))

    table.insert(build, mkbuild("tangle", "phony", tangled))

    fp:write("cflags = " .. table.concat(cflags, " ") .."\n")
    fp:write("libs = " .. table.concat(libs, " ") .."\n")

    -- mac hack
    fp:write("ldflags = -L/opt/homebrew/lib\n")

    for _, v in pairs(rules) do
        fp:write("rule " .. v.name .. "\n")
        fp:write("    command = " .. v.command .. "\n")

        if v.description ~= nil then
            fp:write("    description = "
                .. v.description.. "\n")
        end
    end

    function process_files(f)
        if type(f) == "string" then
            return f
        end

        return table.concat(f, " ")
    end

    for _, v in pairs(build) do
        fp:write("build " ..
            process_files(v.outputs) ..
            ": " ..
            v.rule ..
            " " ..
            process_files(v.inputs) .. "\n")
        if v.vars ~= nil then
            for _, var in pairs(v.vars) do
                fp:write("    " .. var .. "\n")
            end
        end
    end

    fp:write("default mnoscm mnolth mnotil mnolua libmnolth.a\n")

    fp:close()
end

require("core/config")
require("lib/btprnt/config")
require("lib/sndkit/config")
require("lib/mathc/config")
require("lib/minimp4/config")
require("lib/gest/config")
require("lib/lodepng/config")
require("lib/kuf/config")
require("lib/cdb/config")
require("lib/sqlite3/config")
require("lib/miniz/config")
require("lib/bitlang/config")
require("lib/lpeg/config")
require("lib/lua/config")
require("lib/gestvm/config")
require("util/config")
require("util/samurai/config")
require("lib/lsqlite3/config")

generate_ninja()
