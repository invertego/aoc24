#!/usr/bin/env lua

local X = -1
local R = 0
local D = 1
local L = 2
local U = 3
local A = 10

local dstr = { ">", "v", "<", "^" }

local function coord(o)
    return { x = o[1], y = o[2] }
end

local function state(o)
    local s = { pos = {}, num = 0 }
    if o then
        table.move(o.pos, 1, #o.pos, 1, s.pos)
        s.num = o.num
    end
    return s
end

local visit = {}

local function clear()
    visit = setmetatable({}, {
        __index = function(t, k)
            t[k] = {
                dist = math.maxinteger,
                nexts = state(),
                path = {},
            }
            return t[k]
        end
    })
end

local function lookup(s, level, d)
    local x = s.pos[level].x - 1
    local y = s.pos[level].y - 1
    local k = x | (y << 2) | (d << 3) | (level << 7)
    return visit[k]
end

local function dirvalid(x, y)
    return (y == 1 and x >= 2 and x <= 3)
        or (y == 2 and x >= 1 and x <= 3)
end

local function numvalid(x, y)
    return (y >= 1 and y <= 3 and x >= 1 and x <= 3)
        or (y == 4 and x >= 2 and x <= 3)
end

local dirpad = {
    { X, U, A },
    { L, D, R },
}

local invdirpad = {}
for y = 1, #dirpad do
    for x = 1, #dirpad[y] do
        local d = dirpad[y][x]
        if d ~= X then
            invdirpad[d] = coord{ x, y }
        end
    end
end

local numpad = {
    { 7, 8, 9 },
    { 4, 5, 6 },
    { 1, 2, 3 },
    { X, 0, A },
}

local invnumpad = {}
for y = 1, #numpad do
    for x = 1, #numpad[y] do
        local d = numpad[y][x]
        if d ~= X then
            invnumpad[d] = coord{ x, y }
        end
    end
end

local function sign(x)
    if x < 0 then return -1 end
    if x > 0 then return 1 end
    return 0
end

local nlevel
local code = {}

local solvelevel
local dirs = { { L, R }, { U, D } }

local function solvemove(s, level, tpos, firstaxis, path)
    local dist = 0
    local pos = s.pos[level]
    local posv = { pos.x, pos.y }
    local tposv = { tpos.x, tpos.y }
    local sdirv = { sign(tpos.x - pos.x), sign(tpos.y - pos.y) }
    for i = 1, 2 do
        local axis = firstaxis == i and 1 or 2
        while posv[axis] ~= tposv[axis] do
            posv[axis] = posv[axis] + sdirv[axis]
            if (level == nlevel and not numvalid(posv[1], posv[2]))
            or (level ~= nlevel and not dirvalid(posv[1], posv[2])) then
                return math.maxinteger
            end
            dist = dist + solvelevel(s, level - 1, sdirv[axis] < 0 and dirs[axis][1] or dirs[axis][2], path)
        end
    end
    s.pos[level] = tpos
    local adist = solvelevel(s, level - 1, A, path)
    dist = dist + adist
    return dist
end

solvelevel = function(s, level, d, path)
    if level < 1 then
        --[[
        if nlevel <= 3 then
            local buf = d == A and 'A' or dstr[d + 1]
            table.insert(path, buf)
        end
        --]]
        return 1
    end
    local v = nil
    if level < nlevel then
        v = lookup(s, level, d)
        if v.dist ~= math.maxinteger then
            table.move(v.nexts.pos, 1, level, 1, s.pos)
            table.move(v.path, 1, #v.path, #path + 1, path)
            return v.dist
        end
    end

    local tpos = (level == nlevel and invnumpad or invdirpad)[d]
    local nexts = { state(s), state(s) }
    local dists = { math.maxinteger, math.maxinteger };
    local paths = { {}, {} }
    for axis = 1, 2 do
        dists[axis] = solvemove(nexts[axis], level, tpos, axis, paths[axis])
    end

    assert(dists[1] < math.maxinteger or dists[2] < math.maxinteger)
    local min = dists[1] < dists[2] and 1 or 2
    table.move(nexts[min].pos, 1, level, 1, s.pos)
    table.move(paths[min], 1, #paths[min], #path + 1, path)

    if v then
        v.dist = dists[min]
        table.move(s.pos, 1, level, 1, v.nexts.pos)
        table.move(paths[min], 1, #paths[min], 1, v.path)
    end
    return dists[min]
end

local function solve(nbots)
    nlevel = nbots
    clear()

    local dist = 0
    local path = {}
    local s = state()
    for i = 1, nlevel - 1 do
        s.pos[i] = coord{ 3, 1 }
    end
    s.pos[nlevel] = coord{ 3, 4 }
    s.num = 1

    while s.num <= 4 do
        local target = code[s.num]
        dist = dist + solvelevel(s, nlevel, target, path)
        s.num = s.num + 1
    end
    if #path > 0 then print("path", table.concat(path)) end
    return dist
end

local input = io.open("day21.txt"):read("a")
local sum = { 0, 0 }

for xs in input:gmatch("(%d+)A") do
    local x = tonumber(xs)
    --print(string.format("%03dA", x))
    code[1] = (x // 100) % 10
    code[2] = (x //  10) % 10
    code[3] = (x //   1) % 10
    code[4] = A
    local d1 = solve(2 + 1)
    local d2 = solve(25 + 1)
    --print(d1, d2)
    sum[1] = sum[1] + d1 * x
    sum[2] = sum[2] + d2 * x
end

print(sum[1], sum[2])
