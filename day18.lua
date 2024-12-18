#!/usr/bin/env lua

local a = {}
local n = 0

local dx = { 1, 0,-1, 0 }
local dy = { 0, 1, 0,-1 }

local function key(self)
    return self.x | self.y << 7
end

local function state(x, y)
    return { x = x, y = y, v = key }
end

local function push(self, v)
    local w = self.w or 1
    self[w] = v
    self.w = w + 1
end

local function pop(self)
    assert(self.r ~= self.w)
    local r = self.r or 1
    local v = self[r]
    self[r] = nil
    self.r = r + 1
    return v
end

local function empty(self)
    return self.r == self.w
end

local function bfs(start, ends)
    local visit = setmetatable({}, { __index = function() return math.maxinteger end })
    visit[start:v()] = 0
    local open = {}
    push(open, start)

    while not empty(open) do
        local s = pop(open)
        local dist = visit[s:v()]
        --a[s.y][s.x] = 'x'
        if s.x == ends.x and s.y == ends.y then
            return dist
        end

        for d = 1, 4 do
            local t = state(s.x, s.y)
            t.x = s.x + dx[d]
            t.y = s.y + dy[d]
            local ndist = dist + 1
            local c = a[t.y] and a[t.y][t.x]
            if c and c ~= '#' and ndist < visit[t:v()] then
                visit[t:v()] = ndist
                push(open, t)
            end
        end
    end
end

local file = io.open("day18.txt")
--local limit = 12
--n = 6
local limit = 1024
n = 70

for y = 0, n do
    a[y] = {}
    for x = 0, n do
        a[y][x] = '.'
    end
end

local coords = {}
for line in file:lines() do
    local xs, ys = line:match("(%d+),(%d+)")
    local x, y = tonumber(xs), tonumber(ys)
    a[y][x] = '#'
    coords[#coords + 1] = state(x, y)

    if #coords == limit then
        local dist = bfs(state(0, 0), state(n, n))
        print(dist)
    end
end

for i = #coords, 1, -1 do
    local s = coords[i]
    a[s.y][s.x] = '.'
    local dist = bfs(state(0, 0), state(n, n))
    if dist then
        print(s.x .. "," .. s.y)
        break
    end
end

for y = 0, n do
    for x = 0, n do
        --io.write(a[y][x])
    end
    --print()
end
