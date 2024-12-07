#!/usr/bin/env lua

local function nextpow10(x)
    local p = 10
    while p <= x do p = p * 10 end
    return p
end

local function solve(terms, test, base)
    local sum = 0
    local p = base^#terms - 1
    for i = 0, p do
        local accum = terms[1]
        local quot = i
        for j = 2, #terms do
            local op = quot % base
            quot = quot // base
            if op == 0 then accum = accum * terms[j]
            elseif op == 1 then accum = accum + terms[j]
            elseif op == 2 then accum = accum * nextpow10(terms[j]) + terms[j] end
            if accum > test then break end
        end
        if accum == test then
            sum = sum + test
            break
        end
    end
    return sum
end

local file = io.open("day7.txt")
local sum = { 0, 0 }

for line in file:lines() do
    local test = tonumber(line:match("^(%d+):"))
    local terms = {}
    for term in line:gmatch(" (%d+)") do
        table.insert(terms, tonumber(term))
    end
    sum[1] = sum[1] + solve(terms, test, 2)
    sum[2] = sum[2] + solve(terms, test, 3)
end

print(sum[1], sum[2])
