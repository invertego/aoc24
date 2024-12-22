#!/usr/bin/env lua

local function mix(a, b)
    return a ~ b
end

local function prune(a)
    return a & 0xFFFFFF
end

local function generate(a)
    a = prune(mix(a << 6, a))
    a = prune(mix(a >> 5, a))
    a = prune(mix(a << 11, a))
    return a
end

local file = io.open("day22.txt")
local sum = { 0, 0 }
local sums = {}

for line in file:lines() do
    local s = tonumber(line)
    local p = s % 10
    local k = 0
    local observed = {}
    for i = 1, 2000 do
        s = generate(s)
        local q = s % 10
        local d = q - p
        k = ((k << 5) | (d + 9)) & ((1 << 20) - 1)
        if not observed[k] then
            observed[k] = true
            sums[k] = (sums[k] or 0) + q
        end
        p = q
    end
    sum[1] = sum[1] + s
end

local _, max = next(sums)
for _, v in pairs(sums) do
    if v > max then
        max = v
    end
end
sum[2] = max

print(sum[1], sum[2])
