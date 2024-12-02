#!/usr/bin/env lua

local function safe(a)
    local inc, dec, adj = true, true, true
    for i = 1, #a - 1 do
        if not (a[i] > a[i + 1]) then dec = false end
        if not (a[i] < a[i + 1]) then inc = false end
        local d = math.abs(a[i] - a[i + 1])
        if not (d >= 1 and d <= 3) then adj = false end
    end
    return (inc or dec) and adj
end

local file = io.open("day2.txt")
local sum, sum2 = 0, 0

for line in file:lines() do
    local a = {}
    for x in string.gmatch(line, "%d+") do
        table.insert(a, tonumber(x))
    end
    if safe(a) then
        sum = sum + 1
    end
    for i = 1, #a do
        local b = { table.unpack(a) }
        table.remove(b, i)
        if safe(b) then
            sum2 = sum2 + 1
            break
        end
    end
end

print(sum, sum2)
