#!/usr/bin/env lua

local file = io.open("day14.txt")
local sum = { 0, 0 }

local pos = {}
local vel = {}
for line in file:lines() do
    local px, py, vx, vy = line:match("p=(%d+),(%d+) v=(-?%d+),(-?%d+)")
    table.insert(pos, { tonumber(px), tonumber(py) })
    table.insert(vel, { tonumber(vx), tonumber(vy) })
end

local w, h = 101, 103
local min = math.maxinteger
for t = 1, 10000 do
    for i = 1, #pos do
        pos[i][1] = pos[i][1] + vel[i][1]
        pos[i][2] = pos[i][2] + vel[i][2]
        pos[i][1] = (pos[i][1] + w) % w
        pos[i][2] = (pos[i][2] + h) % h
    end

    local q = { 0, 0, 0, 0 }
    for i = 1, #pos do
        if pos[i][1] < w // 2 then
            if pos[i][2] < h // 2 then q[1] = q[1] + 1 end
            if pos[i][2] > h // 2 then q[2] = q[2] + 1 end
        end
        if pos[i][1] > w // 2 then
            if pos[i][2] < h // 2 then q[3] = q[3] + 1 end
            if pos[i][2] > h // 2 then q[4] = q[4] + 1 end
        end
    end
    local prod = q[1] * q[2] * q[3] * q[4]
    if t == 100 then sum[1] = prod end
    if prod < min then
        min = prod
        sum[2] = t
    end
end

print(sum[1], sum[2])
