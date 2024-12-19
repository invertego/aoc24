#!/usr/bin/env lua

local pats = {}
local memo = {}

local function solve(target, i)
    if i == 1 then memo = {} end
    if i > #target then return 1 end
    if memo[i] then return memo[i] end

    local count = 0
    for _, pat in ipairs(pats) do
        local len = #pat
        if target:sub(i, i + len - 1) == pat then
            count = count + solve(target, i + len)
        end
    end
    memo[i] = count
    return count
end

local file = io.open("day19.txt")
local sum = { 0, 0 }

for x in file:read():gmatch("%l+") do
    pats[#pats + 1] = x
end

for target in file:read("a"):gmatch("%l+") do
    local count = solve(target, 1)
    if count ~= 0 then sum[1] = sum[1] + 1 end
    sum[2] = sum[2] + count
end

print(sum[1], sum[2])
