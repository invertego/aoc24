#!/usr/bin/env lua

local reg = {}
local prg = {}

local function run()
    local output = {}
    local a = reg[1]
    repeat
        local b = (a & 7) ~ 3
        output[#output + 1] = ((b ~ 5) ~ (a >> b)) & 7
        a = a >> 3
    until a == 0
    return output
end

local function solve(accum, pos)
    if pos < 1 then return accum end
    for low = 0, 7 do
        local a = (accum << 3) | low
        local b = low ~ 3
        local x = ((b ~ 5) ~ (a >> b)) & 7
        if x == prg[pos] then
            local result = solve(a, pos - 1)
            if result then return result end
        end
    end
end

local file = io.open("day17.txt")

for line in file:lines() do
    local rs, xs = line:match("Register (%u): (%d+)")
    if not rs then break end
    local r, x = rs:byte(), tonumber(xs)
    --print(string.format("Register %c: %d", r, x))
    reg[r - ("A"):byte() + 1] = x
end

for xs in file:read():gmatch("%d") do
    local x = tonumber(xs)
    --print(string.format("%d,", x))
    prg[#prg + 1] = x
end

local output = run()
print(table.concat(output, ","))

print(solve(0, #prg))
