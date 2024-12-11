#!/usr/bin/env lua

local memo = {}
for i = 0, 75 do memo[i] = {} end

local function blink(x, n)
    if memo[n][x] then return memo[n][x] end
    local y
    if n == 0 then y = 1
    elseif x == 0 then y = blink(1, n - 1)
    else
        local buf = tostring(x)
        local len = #buf
        if len % 2 == 0 then
            y = blink(tonumber(buf:sub(len // 2 + 1)), n - 1)
            y = y + blink(tonumber(buf:sub(1, len // 2)), n - 1)
        else
            y = blink(x * 2024, n - 1)
        end
    end
    memo[n][x] = y
    return y
end

local file = io.open("day11.txt")
local sum = { 0, 0 }

for t in file:read():gmatch("%d+") do
    local x = tonumber(t)
    sum[1] = sum[1] + blink(x, 25)
    sum[2] = sum[2] + blink(x, 75)
end

print(sum[1], sum[2])
