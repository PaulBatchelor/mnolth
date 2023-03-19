local arc = monome_arc

m = arc.open("/dev/ttyUSB1")

print("turn rightmost knob counter-clockwise to quit")
running = true

function pos2map(pos)
    local lower = 0
    local upper = 0
    while pos < 0 do
        pos = pos + 64
    end

    if pos > 63 then
        pos = pos % 63
    end

    if pos < 32 then
        lower = 1 << pos
    else
        pos = pos - 32
        upper = 1 << pos
    end

    return lower, upper

end

function progress2map(prog)
    local lower = 0
    local upper = 0
    if prog < 0 then
        prog = 0
    end

    if prog > 63 then
        prog = 63
    end

    if prog <= 32 then
        upper = 0
        if prog == 32 then
            lower = 0xFFFFFFFF
        else
            lower = (1 << prog) - 1
        end
    else
        lower = 0xFFFFFFFF
        prog = prog - 32
        if prog == 31 then
            upper = 0xFFFFFFFF
        else
            upper = (1 << prog) - 1
        end
    end

    return lower, upper
end

knobpos = 0
low, up = pos2map(knobpos)
arc.update(m, 0, low, up)

progress = 0
low, up = progress2map(progress)
arc.update(m, 1, low, up)

while (running) do
    events = arc.get_input_events(m)

    for _,e in pairs(events) do
        number = e[1]
        delta = e[2]

        if number == 3 and delta < 0 then
            running = false
        end

        if number == 0 then
            knobpos = knobpos + delta
            low, up = pos2map(knobpos)
            arc.update(m, 0, low, up)
        end

        if number == 1 then
            progress = progress + delta
            if progress < 0 then
                progress = 0
            elseif progress > 63 then
                progress = 63 
            end
            low, up = progress2map(progress)
            arc.update(m, 1, low, up)
        end
    end

    arc.usleep(800)
end

print("bye")
arc.close(m)
