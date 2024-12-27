#!/usr/bin/env lua

local edges = {}
local tcount = 0
local maxset = {}

local function edge(a, b)
    edges[a] = edges[a] or {}
    edges[a][b] = true
end

local input = io.open("day23.txt"):read("a")
for a, b in input:gmatch("(%l%l)-(%l%l)") do
    edge(a, b)
    edge(b, a)
end

local function connected(a, set)
    for _, b in ipairs(set) do
        if not edges[a][b] then
            return false
        end
    end
    return true
end

local function solve(verts, set, nxt, idx, tpre)
    table.insert(set, nxt)
    if nxt:match("^t") then tpre = true end
    if tpre and #set == 3 then
        tcount = tcount + 1
    end
    if #set > #maxset then
        maxset = { table.unpack(set) }
    end
    for i = idx, #verts do
        local a = verts[i]
        if connected(a, set) then
            solve(verts, set, a, i + 1, tpre)
        end
    end
    table.remove(set)
end

for a in pairs(edges) do
    local verts = {}
    for b in pairs(edges[a]) do
        if b > a then
            table.insert(verts, b)
        end
    end
    table.sort(verts)
    solve(verts, {}, a, 1)
end

print(tcount)
print(table.concat(maxset, ","))
