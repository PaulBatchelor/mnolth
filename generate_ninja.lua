function mkrule(name, command)
    return {name=name, command=command}
end

function mkbuild(outputs, rule, inputs)
    return {outputs=outputs, rule=rule, inputs=inputs}
end

rules = {}
build = {}

objects = {}

cflags = {"-Wall", "-pedantic", "-O3"}

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
    tangled = tangled_c

    if header == true then
        tangled = {tangled_c, obj .. ".h"}
    end

    table.insert(build,
        mkbuild(tangled, "tangle", obj .. ".org"))


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
    mkrule("tangle", "worgle -Werror -g $in"))
table.insert(rules,
    mkrule("c89", "gcc -std=c89 -c $cflags $in -o $out"))
table.insert(rules,
    mkrule("c99", "gcc -std=c99 -c $cflags $in -o $out"))

table.insert(rules,
    mkrule("link", "gcc $in -o $out $libs"))

libs = {
    "-lm", 
    "-lx264",

    -- used by SQLite
    "-lpthread",

    "-ldl",

    -- "-lreadline",
}

add_cflags({"-Ilib", "-Icore"})

function generate_ninja()
    fp = io.open("build.ninja", "w")

    table.insert(build,
        mkbuild("core/lil_main.o", "c89", "core/lil_main.c"))
    table.insert(build,
        mkbuild("mnolth",
            "link",
            table.concat(objects, " ") .. " core/lil_main.o"))

    fp:write("cflags = " .. table.concat(cflags, " ") .."\n")
    fp:write("libs = " .. table.concat(libs, " ") .."\n")

    for _, v in pairs(rules) do
        fp:write("rule " .. v.name .. "\n")
        fp:write("    command = " .. v.command .. "\n")
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
    end

    fp:write("default mnolth\n")

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

generate_ninja()
