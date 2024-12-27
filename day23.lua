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

local function solve(root, set, nxt, tpre)
    table.insert(set, nxt)
    if nxt:match("^t") then tpre = true end
    if tpre and #set == 3 then
        tcount = tcount + 1
    end
    if #set > #maxset then
        maxset = { table.unpack(set) }
    end
    for a in pairs(edges[root]) do
        if a > nxt and connected(a, set) then
            solve(root, set, a, tpre)
        end
    end
    table.remove(set)
end

for a in pairs(edges) do
    solve(a, {}, a)
end

print(tcount)
print(table.concat(maxset, ","))
