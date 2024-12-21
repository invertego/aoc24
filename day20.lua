#!/usr/bin/env lua

local a = {}

local dx = { 1, 0,-1, 0 }
local dy = { 0, 1, 0,-1 }

local function key(self)
    return self.x | self.y << 8
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
            break
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

    local count = { 0, 0 }
    --local counts = {}
    for y = 1, #a do
        for x = 1, #a[y] do
            local s = state(x, y)
            local dist = visit[s:v()]
            if dist ~= math.maxinteger then
                for v = -20, 20 do
                    for u = -20, 20 do
                        local travel = math.abs(u) + math.abs(v)
                        if travel <= 20 then
                            local t = state(s.x, s.y)
                            t.x = t.x + u
                            t.y = t.y + v
                            local ndist = visit[t:v()]
                            if ndist ~= math.maxinteger then
                                local savings = dist - (ndist + travel)
                                --[[if savings > 0 then
                                    counts[savings] = (counts[savings] or 0) + 1
                                end--]]
                                if savings >= 100 then
                                    if travel == 2 then count[1] = count[1] + 1 end
                                    count[2] = count[2] + 1
                                end
                            end
                        end
                    end
                end
            end
        end
    end
    --[[for i = 50, 100 do
        if (counts[i] or 0) ~= 0 then
            print(string.format("There are %d cheats that save %d picoseconds.", counts[i], i))
        end
    end--]]

    return count
end

local file = io.open("day20.txt")

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

local sum = bfs(state(sx, sy), state(ex, ey))

for y = 1, #a do
    for x = 1, #a[y] do
        --io.write(a[y][x])
    end
    --print()
end

print(sum[1], sum[2])
