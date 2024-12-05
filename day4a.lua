#!/usr/bin/env lua

local a = {}
local n = 0
local function A(i, j)
    return i * 256 + j
end

local function match(m, f, i, j, di, dj)
    for k = 1, #m do
        local s = i + di * (k - 1)
        local t = j + dj * (k - 1)
        if f & 1 ~= 0 then s = n - s + 1 end
        if f & 2 ~= 0 then t = n - t + 1 end
        if f & 4 ~= 0 then s, t = t, s end
        if a[A(s, t)] ~= m:byte(k) then
            return false
        end
    end
    return true
end

local file = io.open("day4.txt")
local sum, sum2 = 0, 0

for line in file:lines() do
    n = n + 1
    for j = 1, #line do
        a[A(n, j)] = line:byte(j)
    end
end

for f = 0, 7 do
    for i = 1, n do
        for j = 1, n do
            if match("XMAS", f, i, j, 1, 0) then
                sum = sum + 1
            end
            if match("XMAS", f, i, j, 1, 1) then
                sum = sum + 1
            end
            if match("MAS", f, i, j, 1, 1) and
               match("MAS", f, i, j + 2, 1, -1) then
                sum2 = sum2 + 1
            end
        end
    end
end

print(sum / 2, sum2 / 2)
