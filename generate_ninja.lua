
function mkrule(name, command)
    return {name=name, command=command}
end

function mkbuild(outputs, rule, inputs)
    return {outputs=outputs, rule=rule, inputs=inputs}
end

rules = {}
build = {}
cflags = {"-Wall", "-pedantic", "-O3"}

function add_object(obj)
    table.insert(build,
        mkbuild(obj .. ".o",
            "c89",
            obj .. ".c"))
end

function add_c99object(obj)
    table.insert(build,
        mkbuild(obj .. ".o",
            "c99",
            obj .. ".c"))
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

table.insert(rules,
    mkrule("tangle", "worgle -Werror -g $in"))
table.insert(rules,
    mkrule("c89", "gcc -std=c89 -c $cflags $in -o $out"))
table.insert(rules,
    mkrule("c99", "gcc -std=c99 -c $cflags $in -o $out"))

add_cflags({"-Ilib", "-Icore"})

require("lib/btprnt/config")
require("lib/sndkit/config")

fp = io.open("build.ninja", "w")
fp:write("cflags = " .. table.concat(cflags, " ") .."\n")

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

fp:close()
