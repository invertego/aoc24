#!/usr/bin/env lua

local file = io.open("day9.txt")
local sum, sum2 = 0, 0

local buf = file:read("l")
local map = {}
local files = {}
local spaces = {}

local pos = 0
for i = 1, #buf do
    local l = tonumber(buf:sub(i, i))
    local id = i & 1 ~= 0 and i // 2 or -1
    for j = 1, l do
        table.insert(map, id)
    end
    table.insert(i & 1 ~= 0 and files or spaces, { pos, l })
    pos = pos + l
end

local i, j = 1, #map
while i <= j do
    if map[i] >= 0 then
        sum = sum + (i - 1) * map[i]
    else
        while j > i and map[j] < 0 do
            j = j - 1
        end
        if j <= i then break end
        sum = sum + (i - 1) * map[j]
        j = j - 1
    end
    i = i + 1
end

for i = #files, 1, -1 do
    local fpos, flen = table.unpack(files[i])
    for j = 1, #spaces do
        local spos, slen = table.unpack(spaces[j])
        if spos >= fpos then break end
        if flen <= slen then
            files[i] = { spos, flen }
            spaces[j] = { spos + flen, slen - flen }
            break
        end
    end
end

for i, file in ipairs(files) do
    local pos, len = table.unpack(file)
    for j = 0, len - 1 do
        sum2 = sum2 + (i - 1) * (pos + j)
    end
end

print(sum, sum2)
