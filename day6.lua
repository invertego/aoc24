#!/usr/bin/env lua

local mt = { __index = function(t, k) t[k] = {} return t[k] end }

local byteDot = string.byte '.'
local byteX = string.byte 'X'
local byteHash = string.byte '#'
local byteCaret = string.byte '^'

local di = {-1, 0, 1, 0 }
local dj = { 0, 1, 0,-1 }

local function walk(a, si, sj)
    local i, j = si, sj
    local d = 1
    local steps = 0
    local b = {}
    setmetatable(b, mt)
    while a[i][j] do
        if not b[i][j] then
            b[i][j] = byteX + d
            steps = steps + 1
        elseif b[i][j] == byteX + d then
            steps = -steps
            break
        end
        local ni, nj = i + di[d], j + dj[d]
        if a[ni][nj] == byteHash then
            d = (d & 3) + 1
        else
            i, j = ni, nj
        end
    end
    return steps
end

local file = io.open("day6.txt")
local sum, sum2 = 0, 0
local a = {}
setmetatable(a, mt)

for line in file:lines() do
    table.insert(a, { line:byte(1, #line) })
end

local si, sj
for i = 1, #a do
    for j = 1, #a do
        if a[i][j] == byteCaret then
            a[i][j] = byteDot
            si, sj = i, j
            break
        end
    end
end

sum = walk(a, si, sj)

for i = 1, #a do
    for j = 1, #a do
        if a[i][j] == byteDot then
            a[i][j] = byteHash
            local steps = walk(a, si, sj)
            if steps < 0 then sum2 = sum2 + 1 end
            a[i][j] = byteDot
        end
    end
end

print(sum, sum2)
