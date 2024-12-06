#!/usr/bin/env lua

local file = io.open("day5.txt")
local sum = { 0, 0 }
local order = {}
setmetatable(order, { __index = function(t, k) t[k] = {} return t[k] end })

for line in file:lines() do
    local x, y = line:match("(%d+)|(%d+)")
    if not x then break end
    order[tonumber(x)][tonumber(y)] = true
end

for line in file:lines() do
    local pages = {}
    for x in line:gmatch("%d+") do
        table.insert(pages, tonumber(x))
    end

    local sorted = true
    for i = 1, #pages do
        for j = i + 1, #pages do
            if order[pages[j]][pages[i]] then
                pages[i], pages[j] = pages[j], pages[i]
                sorted = false
            end
        end
    end

    local idx = sorted and 1 or 2
    sum[idx] = sum[idx] + pages[#pages // 2 + 1]
end

print(sum[1], sum[2])
