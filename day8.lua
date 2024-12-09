#!/usr/bin/env lua

local map = {}
local visit = {}
local coords = {}
local mt = { __index = function(t, k) t[k] = {} return t[k] end }
setmetatable(map, mt)
setmetatable(visit, mt)
setmetatable(coords, mt)

local function mark(i , j, f)
    local v = visit[i][j] or 0
    if not map[i][j] or (v & f) ~= 0 then return false end
    visit[i][j] = v | f
    return true
end

local function sweep(p, d, f)
    local cnt = 0
    local i, j = p[1], p[2]
    while map[i][j] do
        if mark(i, j, f) then cnt = cnt + 1 end
        i = i + d[1]
        j = j + d[2]
    end
    return cnt
end

local file = io.open("day8.txt")
local sum, sum2 = 0, 0

for line in file:lines() do
    table.insert(map, { line:byte(1, #line) })
end

local dot = string.byte('.')

for i = 1, #map do
    for j = 1, #map do
        local m = map[i][j]
        if m ~= dot then
            table.insert(coords[m], { i, j })
        end
    end
end

for _, p in pairs(coords) do
    for v = 1, #p do
        for w = v + 1, #p do
            local pv = p[v]
            local pw = p[w]
            local d = { pw[1] - pv[1], pw[2] - pv[2] }
            if mark(pw[1] + d[1], pw[2] + d[2], 1) then sum = sum + 1 end
            if mark(pv[1] - d[1], pv[2] - d[2], 1) then sum = sum + 1 end
            sum2 = sum2 + sweep(pw, d, 2)
            d[1] = -d[1]
            d[2] = -d[2]
            sum2 = sum2 + sweep(pv, d, 2)
        end
    end
end

print(sum, sum2)
