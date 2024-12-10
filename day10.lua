#!/usr/bin/env lua

local mt = { __index = function(t, k) t[k] = {} return t[k] end }

local a = setmetatable({}, mt)
local v = setmetatable({}, mt)

local di = {-1, 0, 1, 0 }
local dj = { 0, 1, 0,-1 }

local zero = string.byte('0')
local nine = string.byte('9')

local function dfs(i, j, target, track)
    if a[i][j] ~= target then return 0 end
    if track then
        if v[i][j] then return 0 end
        v[i][j] = true
    end
    if target == nine then return 1 end

    local sum = 0
    for d = 1, 4 do
        sum = sum + dfs(i + di[d], j + dj[d], target + 1, track)
    end
    return sum
end

local file = io.open("day10.txt")
local sum = { 0, 0 }

for line in file:lines() do
    table.insert(a, { line:byte(1, #line) })
end

for i = 1, #a do
    for j = 1, #a do
        v = setmetatable({}, mt)
        sum[1] = sum[1] + dfs(i, j, zero, true)
        sum[2] = sum[2] + dfs(i, j, zero, false)
    end
end

print(sum[1], sum[2])
