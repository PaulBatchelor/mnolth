-- <@>
lil([[
hsnew hs
rtnew [grab hs] rt

func out {} {
    hsout [grab hs]
    hsswp [grab hs]
}

func playtog {} {
    hstog [grab hs]
}
]])
-- </@>

-- <@>
function setup_light(portname)
    print("setting up light")
    local setup_code = {
        "mklight li " .. portname
    }

    lil(table.concat(setup_code, "\n"))
end
-- </@>

-- <@>
setup_light("/dev/cu.usbmodem1101")
-- </@>

-- <@>
function patch()
    print("patch")
    lil("phasor " .. 1/60 .. " 0")
    lil("flipper zz")
    lil("expmap zz 10")
    lil("scale zz 0.1 1.1")
    lil("sine zz 1")
    lil("biscale zz 0 1")
    lil("hold zz")
    lil("regset zz 0")

    lil("add [rline -0.01 0.01 0.2] [expr 36]")
    lil("blsaw [ji zz 0]")
    lil("mul zz [dblin -3]")

    lil("add [rline -0.01 0.01 0.3] [expr 36 + 12]")
    lil("blsaw [ji zz 0]")
    lil("mul zz [dblin 0]")

    lil("add zz zz")
    lil("add [rline -0.01 0.01 0.3] [expr 36 - 12]")
    lil("blsaw [ji zz 0]")
    lil("mul zz [dblin 1]")

    lil("add zz zz")

    lil("regget 0")
    lil("scale zz 0 0.3")
    lil("mul zz zz")
    lil("regget 0")
    lil("scale zz 100 4000")
    lil("butlp zz zz")

    lil("rline -0.1 0.1 0.5")
    lil("tseq [genvals [tabnew 1] \"4 2 5 4 7 2 8 7\"] [metro 0.1] [param 0]")
    lil("add zz 72")
    lil("tsmoother [tick] zz 0.5")
    lil("add zz zz")
    lil("sine 6 0.2")
    lil("sine 0.052 1")
    lil("biscale zz 0 1")
    lil("mul zz zz")
    lil("add zz zz")
    lil("ji zz 0")
    lil("blsaw zz")
    lil("mul zz [dblin -6]")
    lil("param 2000")
    lil("butlp zz zz")
    lil("buthp zz 800")
    lil("add zz zz")

    lil("grab li")
    lil("regget 0")
    lil("scale zz 0.0 1")
    lil("expmap zz 10")
    lil("dup")
    lil("thresh zz 0.01 2")
    lil("tog zz")
    lil("envar zz 0.25 0.01")
    lil("mul zz zz")
    lil("light zz zz")

    lil("peakeq zz 100 100 2.5")
    lil("mul zz [dblin -5]")
    lil("dup")

    lil("add [rline -0.01 0.01 0.3] [expr 36 + 24 + 1]")
    lil("blsaw [ji zz 0]")
    lil("mul zz [dblin -3]")
    lil("butlp zz 2000")
    lil("buthp zz 800")
    lil("phasor " .. 1 / 45 .. " 0")
    lil("flipper zz")
    lil("expmap zz 10")
    lil("mul zz zz")
    lil("add zz zz")

    lil("buthp zz 300")
    lil("vardelay zz 0 0.12 0.2")
    lil("dup")
    lil("vardelay zz 0.9 0.6 3.0")
    lil("mul zz [dblin -3]")
    lil("butlp zz 2000")
    lil("add zz zz")

    lil("dup")
    lil("bigverb zz zz 0.98 10000")
    lil("add zz zz")
    lil("dcblocker zz")
    lil("mul zz [dblin -15]")
    lil("swap; mul zz [dblin -2]");
    lil("add zz zz")
    lil("limit zz -1 1")

    lil("unhold [regget 0]")
    lil("out")
end
-- </@>

-- <@>
patch()
-- </@>

-- <@>
lil("stop")
-- </@>
