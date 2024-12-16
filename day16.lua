#!/usr/bin/env lua

local a = {}

local dx = { 1, 0,-1, 0 }
local dy = { 0, 1, 0,-1 }

local function key(self)
    return self.x | self.y << 8 | self.d << 16
end

local function state(x, y, d)
    return {
        x = x,
        y = y,
        d = d,
        v = key,
    }
end

local open = {}
local openrd, openwr = 1, 1

local function push(s)
    open[openwr] = s
    openwr = openwr + 1
end

local function pop()
    assert(openrd ~= openwr)
    local s = open[openrd]
    open[openrd] = nil
    openrd = openrd + 1
    return s
end

local function bfs(start, ends)
    local visit = setmetatable({}, { __index = function() return math.maxinteger end })
    local shortest = math.maxinteger
    visit[start:v()] = 0
    push(start)

    while openrd ~= openwr do
        local s = pop()
        local dist = visit[s:v()]
        --a[s.y][s.x] = 'x'
        if s.x == ends.x and s.y == ends.y then
            if dist < shortest then shortest = dist end
        end

        for dd = -1, 1 do
            local t = state(s.x, s.y, s.d)
            local ndist = dist
            if dd == 0 then
                t.x = s.x + dx[s.d]
                t.y = s.y + dy[s.d]
                ndist = ndist + 1
            else
                t.d = ((s.d - 1 + dd + 4) % 4) + 1
                ndist = ndist + 1000
            end
            if a[t.y][t.x] ~= '#' and ndist < visit[t:v()] then
                visit[t:v()] = ndist
                push(t)
            end
        end
    end

    local count = 0
    for d = 1, 4 do
        local t = state(ends.x, ends.y, d)
        if visit[t:v()] == shortest then
            push(t)
        end
    end
    while openrd ~= openwr do
        local t = pop()
        local dist = visit[t:v()]
        if a[t.y][t.x] ~= 'O' then
            a[t.y][t.x] = 'O'
            count = count + 1
        end

        for dd = -1, 1 do
            local s = state(t.x, t.y, t.d)
            local ndist = dist
            if dd == 0 then
                s.x = t.x - dx[t.d]
                s.y = t.y - dy[t.d]
                ndist = ndist - 1
            else
                s.d = ((t.d - 1 + dd + 4) % 4) + 1
                ndist = ndist - 1000
            end
            if visit[s:v()] == ndist then
                push(s)
            end
        end
    end

    return shortest, count
end

local file = io.open("day16.txt")

for line in file:lines() do
    local row = {}
    for i = 1, #line do row[i] = line:sub(i, i) end
    table.insert(a, row)
end

local sx, sy, ex, ey
for y = 1, #a do
    for x = 1, #a[y] do
        if a[y][x] == 'S' then
            a[y][x] = '.'
            sx, sy = x, y
        elseif a[y][x] == 'E' then
            a[y][x] = '.'
            ex, ey = x, y
        end
    end
end
--print("n", #a, "sx", sx, "sy", sy, "ex", ex, "ey", ey)

local sum, sum2 = bfs(state(sx, sy, 1), state(ex, ey, 1))

for y = 1, #a do
    for x = 1, #a[y] do
        --io.write(a[y][x])
    end
    --print()
end

print(sum, sum2)
