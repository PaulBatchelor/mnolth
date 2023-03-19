local grid = monome_grid

m = grid.open("/dev/ttyUSB0")

print("press (0,0) to quit")
running = true

quadL = {0, 0, 0, 0, 0, 0, 0, 0}
quadR = {0, 0, 0, 0, 0, 0, 0, 0}

grid.update(m, quadL, quadR)

function set_led(x, y, s)
    local q = nil

    y = y + 1

    if (x >= 8) then
        q = quadR
        x = x - 8
    else
        q = quadL
    end

    if s == 1 then
        q[y] = q[y] | 1 << x
    else
        q[y] = q[y] & ~(1 << x)
    end
end

while (running) do
    events = grid.get_input_events(m)
    local redraw = false
    for _,e in pairs(events) do
        if e[3] == 1 and e[1] == 0 and e[2] == 0 then
            running = false
        end

        set_led(e[1], e[2], e[3])
        redraw = true
    end

    if (redraw) then
        grid.update(m, quadL, quadR)
    end

    grid.usleep(80)
end

print("bye")
grid.close(m)
