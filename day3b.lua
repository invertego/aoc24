#!/usr/bin/env lua

text = io.open("day3.txt"):read("a")
sum = { 0, 0 }
pat = { "don't()", "do()" }
k = 1
l = 1
while true do
    i, j = text:find(pat[k], l, true)
    for x, y in text:sub(l, i or -1):gmatch("mul%((%d+),(%d+)%)") do
        sum[k] = sum[k] + tonumber(x) * tonumber(y)
    end
    if not i then break end
    l = j + 1
    k = k % 2 + 1
end
print(sum[1] + sum[2])
print(sum[1])
