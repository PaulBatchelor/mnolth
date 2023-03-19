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

    if (y < 1 or y > 8) then return end
    if (x < 0 or x > 15) then return end

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

ripples = {}
nripples = 0


function new_ripple(x, y)
    return {
        size = 0,
        x = x,
        y = y,
        speed = 5 + 15 * math.random(),
        running = true
    }
end

function reset_ripple(pos, x, y)
    r = ripples[pos]
    r.x = x
    r.y = y
    r.running = true
    r.size = 0
    r.speed = 5 + 15 * math.random()
end

function find_free_ripple()
    for i,r in pairs(ripples) do
        if r.running == false then
            return i
        end
    end
end

speedmul = 1.0 / 60

function rect(x, y, w, h)
    for i = 0,w do
        set_led(x + i, y, 1)
        set_led(x + i, y + h, 1)
    end

    for i = 0,h-1 do
        set_led(x, y + i, 1)
        set_led(x + w, y + i, 1)
    end
end

function clear()
    for i =1,8 do
        quadL[i] = 0
        quadR[i] = 0
    end
end

function draw_ripple_and_update(r)
    if r.running == false then return end

    sz = math.floor(r.size)


    if sz == 0 then
        set_led(r.x, r.y, 1)
    else
        rect(r.x - sz, r.y - sz, 2*sz, 2*sz)
    end

    r.size = r.size + r.speed * speedmul

    if r.size > 10 then
        r.running = false
    end
end

thn = grid.now()
while (running) do
    events = grid.get_input_events(m)
    local redraw = false
    for _,e in pairs(events) do
        if e[3] == 1 and e[1] == 0 and e[2] == 0 then
            running = false
            break
        end

        if e[3] == 0 then
            if nripples < 10 then
                table.insert(ripples, new_ripple(e[1], e[2]))
                nripples = nripples + 1
            else
                local r = find_free_ripple()
                if r ~= nil then
                    reset_ripple(r, e[1], e[2])
                end
            end
        end

        -- set_led(e[1], e[2], e[3])
        -- redraw = true
    end

    local now = grid.now()

    if ((now - thn)) > 0.016 then
        thn = now
        clear()
        for _,r in pairs(ripples) do
            draw_ripple_and_update(r)
        end
        grid.update(m, quadL, quadR)
    end

    -- if (redraw) then
    --     grid.update(m, quadL, quadR)
    -- end

    grid.usleep(80)
end

print("bye")
grid.close(m)
