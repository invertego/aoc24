#!/usr/bin/env lua

local a = {}

local dx = { 1, 0,-1, 0 }
local dy = { 0, 1, 0,-1 }

local state = {}
local statemt = { __index = state }

function state.new(x, y, d)
    return setmetatable({
        x = x,
        y = y,
        d = d,
    }, statemt)
end

function state:key()
    return self.x | self.y << 8 | self.d << 16
end

function state:clone()
    return state.new(self.x, self.y, self.d)
end

local queue = { r = 1, w = 1 }
local queuemt = { __index = queue }

function queue.new()
    return setmetatable({}, queuemt)
end

function queue:push(x)
    local w = self.w
    self[w] = x
    self.w = w + 1
end

function queue:pop()
    local r = self.r
    assert(r ~= self.w)
    local x = self[r]
    self[r] = nil
    self.r = r + 1
    return x
end

function queue:empty()
    return self.r == self.w
end

local set = {}
local setmt = { __index = set }

function set.new()
    return setmetatable({}, setmt)
end

function set:get(s)
    return self[s:key()] or math.maxinteger
end

function set:set(s, d)
    self[s:key()] = d
end

local function bfs(start, ends)
    local visit = set.new()
    local open = queue.new()
    local shortest = math.maxinteger
    visit:set(start, 0)
    open:push(start)

    while not open:empty() do
        local s = open:pop()
        local dist = visit:get(s)
        --a[s.y][s.x] = 'x'
        if s.x == ends.x and s.y == ends.y then
            if dist < shortest then shortest = dist end
        end

        for dd = -1, 1 do
            local t = s:clone()
            local ndist = dist
            if dd == 0 then
                t.x = s.x + dx[s.d]
                t.y = s.y + dy[s.d]
                ndist = ndist + 1
            else
                t.d = ((s.d - 1 + dd + 4) % 4) + 1
                ndist = ndist + 1000
            end
            if a[t.y][t.x] ~= '#' and ndist < visit:get(t) then
                visit:set(t, ndist)
                open:push(t)
            end
        end
    end

    local count = 0
    for d = 1, 4 do
        local t = ends:clone()
        t.d = d
        if visit:get(t) == shortest then
            open:push(t)
        end
    end
    while not open:empty() do
        local t = open:pop()
        local dist = visit:get(t)
        if a[t.y][t.x] ~= 'O' then
            a[t.y][t.x] = 'O'
            count = count + 1
        end

        for dd = -1, 1 do
            local s = t:clone()
            local ndist = dist
            if dd == 0 then
                s.x = t.x - dx[t.d]
                s.y = t.y - dy[t.d]
                ndist = ndist - 1
            else
                s.d = ((t.d - 1 + dd + 4) % 4) + 1
                ndist = ndist - 1000
            end
            if visit:get(s) == ndist then
                open:push(s)
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

local sum, sum2 = bfs(state.new(sx, sy, 1), state.new(ex, ey, 1))

for y = 1, #a do
    for x = 1, #a[y] do
        --io.write(a[y][x])
    end
    --print()
end

print(sum, sum2)
