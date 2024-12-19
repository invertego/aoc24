#!/usr/bin/env lua

local pats = {}

local function solve(target)
    local memo = { [#target + 1] = 1 }
    for i = #target, 1, -1 do
        local count = 0
        for _, pat in ipairs(pats) do
            local len = #pat
            if target:sub(i, i + len - 1) == pat then
                count = count + memo[i + len]
            end
        end
        memo[i] = count
    end
    return memo[1]
end

local file = io.open("day19.txt")
local sum = { 0, 0 }

for x in file:read():gmatch("%l+") do
    pats[#pats + 1] = x
end

for target in file:read("a"):gmatch("%l+") do
    local count = solve(target)
    if count ~= 0 then sum[1] = sum[1] + 1 end
    sum[2] = sum[2] + count
end

print(sum[1], sum[2])
