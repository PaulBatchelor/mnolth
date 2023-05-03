config = {}

for _,a in pairs(arg) do
    if (a == "live") then
        config.mnort = true
    elseif (a == "mnodes") then
        config.mnodes = true
    elseif (a == "monome") then
        config.monome = true
    elseif (a == "x264") then
        config.x264 = true
    elseif (a == "all") then
        config.monome = true
        config.mnodes = true
        config.mnort = true
        config.x264 = true
    end
end


function mkrule(name, command, description)
    return {name=name, command=command, description=description}
end

function mkbuild(outputs, rule, inputs, vars, deps)
    return {
        outputs = outputs,
        rule = rule,
        inputs = inputs,
        vars = vars,
        deps = deps
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
        mkbuild(tangled_files, "worgle", obj .. ".org", nil, "worglite"))

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

if config.x264 == true then
    table.insert(libs, "-lx264")
    add_cflags{"-DMNOLTH_X264"}
end

add_cflags({"-Ilib", "-Icore"})

if config.mnort then
    add_cflags{"-DBUILD_MNORT"}

    add_objects {
      "util/mnort/server",
      "util/mnort/client",
      "util/mnort/rt",
    }

    table.insert(libs, "-ljack")
end

function add_libs(p)
    for _,v in pairs(p) do
        table.insert(libs, "-l" .. v)
    end
end

function generate_ninja()
    fp = io.open("build.ninja", "w")
    objstr = table.concat(objects, " ")

    -- mnolth
    table.insert(build,
        mkbuild("main.o", "c89", "main.c"))
    table.insert(build,
        mkbuild("mnolth",
            "link",
             objstr .. " main.o"))

    -- libmnolth
    table.insert(build,
        mkbuild("libmnolth.a", "ar", objstr))

    -- mnort_items = {
    --       "util/mnort/server",
	-- 	  "util/mnort/client",
	-- 	  "util/mnort/rt",
	-- 	  "util/mnort/mnort"
    -- }

    -- mnort_objs = {}

    -- for _,o in pairs(mnort_items) do
    --     table.insert(build,
    --         mkbuild(o .. ".o", "c89", o .. ".c"))
    --     table.insert(mnort_objs, o .. ".o")
    -- end

    -- table.insert(build,
    --     mkbuild("mnort",
    --         "link",
    --          objstr ..
    --          " " ..
    --          table.concat(mnort_objs, " "),
    --          {"libs = -ljack"}))


    table.insert(build,
        mkbuild("util/worgle/worglite",
            "link",
             "util/worgle/worgle.c util/worgle/parg.c",
             {"cflags = -O3 -DWORGLITE"}))

    table.insert(build, mkbuild("worglite",
        "phony", "util/worgle/worglite"))

    table.insert(build, mkbuild("tangle", "phony", tangled, nil, "worglite"))

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

    function process_deps(deps)
        if deps ~= nil then
            -- process_files has the same logic we need
            -- for implicit deps
            return " || " .. process_files(deps)
        end
        return ""
    end

    for _, v in pairs(build) do
        fp:write("build " ..
            process_files(v.outputs) ..
            ": " ..
            v.rule ..
            " " ..
            process_files(v.inputs) ..
            process_deps(v.deps) ..
            "\n")
        if v.vars ~= nil then
            for _, var in pairs(v.vars) do
                fp:write("    " .. var .. "\n")
            end
        end
    end

    fp:write("default mnolth libmnolth.a\n")

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
require("lib/sdf2d/config")
require("lib/libxm/config")

if config.mnodes then
    require("mnodes/config")
end

if config.monome then
    require("monome/config")
end


generate_ninja()
