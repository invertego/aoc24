#!/usr/bin/env lua

local file = io.open("day1.txt")

local a, b = {}, {}

for x, y in file:lines("n", "n") do
    table.insert(a, x)
    table.insert(b, y)
end

table.sort(a)
table.sort(b)

local sum, sum2 = 0, 0

for i = 1, #a do
    sum = sum + math.abs(a[i] - b[i])

    for j = 1, #a do
        if a[i] == b[j] then
            sum2 = sum2 + a[i]
        end
    end
end

print(sum, sum2)
