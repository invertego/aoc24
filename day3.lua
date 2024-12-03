#!/usr/bin/env lua

local file = io.open("day3.txt")
local sum, sum2 = 0, 0
local enable = true

for line in file:lines() do
    for i = 1, #line do
        local x, y = string.match(line, "^mul%((%d+),(%d+)%)", i)
        if x and y then
            local prod = tonumber(x) * tonumber(y)
            sum = sum + prod
            if enable then sum2 = sum2 + prod end
        elseif string.match(line, "^do%(%)", i) then
            enable = true
        elseif string.match(line, "^don't%(%)", i) then
            enable = false
        end
    end
end

print(sum, sum2)
