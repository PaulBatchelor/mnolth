local grid = monome_grid

m = grid.open("/dev/ttyUSB0")

print("press (0,0) to quit")
running = true
while (running) do
    events = grid.get_input_events(m)

    for _,e in pairs(events) do
        print(e[1], e[2], e[3])
        if e[3] == 1 and e[1] == 0 and e[2] == 0 then
            running = false
        end
    end

    grid.usleep(800)
end

print("bye")
grid.close(m)
