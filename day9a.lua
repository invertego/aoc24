#!/usr/bin/env lua

local file = io.open("day9.txt")
local sum, sum2 = 0, 0

local buf = file:read("l")
local map = {}

for i = 1, #buf do
    local l = tonumber(buf:sub(i, i))
    local id = i & 1 ~= 0 and i // 2 or -1
    for j = 1, l do
        table.insert(map, id)
    end
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

local sbeg = #map
while sbeg >= 1 do
    while sbeg >= 1 and map[sbeg] < 0 do
        sbeg = sbeg - 1
    end
    if sbeg < 1 then break end
    local id = map[sbeg]
    local send = sbeg - 1
    while send >= 1 and map[send] == id do
        send = send - 1
    end
    local slen = sbeg - send
    local dlen = 0
    local dbeg = 1
    while dbeg <= send do
        while dbeg <= send and map[dbeg] >= 0 do
            dbeg = dbeg + 1
        end
        if dbeg > send then break end
        local dend = dbeg + 1
        while dend <= #map and map[dend] < 0 do
            dend = dend + 1
        end
        dlen = dend - dbeg
        if dlen >= slen then break end
        dbeg = dend
        dlen = 0
    end
    if dlen >= slen then
        for i = 0, slen - 1 do
            map[dbeg + i] = id
            map[send + 1 + i] = -1
        end
    end
    sbeg = send
end

for i = 1, #map do
    if map[i] >= 0 then
        sum2 = sum2 + (i - 1) * map[i]
    end
end

print(sum, sum2)
