#!/usr/bin/env lua

local input = io.open("day25.txt"):read("a")
local locks = {}
local keys = {}

for grid in input:gsub("\n\n", "\0"):gmatch("[^\0]+") do
    local levels = {}
    local kind
    local i = 1
    for row in grid:gmatch("[^\n]+") do
        local j = 1
        for cell in row:gmatch(".") do
            kind = kind or cell
            if cell == kind then levels[j] = i end
            j = j + 1
        end
        i = i + 1
    end
    table.insert(kind == "#" and locks or keys, levels)
end

local function fits(key, lock)
    for i = 1, #lock do
        if lock[i] > key[i] then return false end
    end
    return true
end

local count = 0
for _, key in ipairs(keys) do
    for _, lock in ipairs(locks) do
        if fits(key, lock) then count = count + 1 end
    end
end
print(count)
