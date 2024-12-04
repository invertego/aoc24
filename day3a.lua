#!/usr/bin/env lua

function mull(text)
    local sum = 0
    for x, y in string.gmatch(text, "mul%((%d+),(%d+)%)") do
        sum = sum + tonumber(x) * tonumber(y)
    end
    print(sum)
end

text = io.open("day3.txt"):read("a")
mull(text)
text = string.gsub(text, "don't%(%).-do%(%)", "")
text = string.gsub(text, "don't%(%).*", "")
mull(text)
