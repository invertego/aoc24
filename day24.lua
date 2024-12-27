#!/usr/bin/env lua

local input = io.open("day24.txt"):read("a")

local gates = {}
local list = {}
local cache = {}

for name, value in input:gmatch("(%w+): (%d)") do
    local gate = { name = name, op = "SET", out = tonumber(value) }
    gates[name] = gate
end
for a, op, b, name in input:gmatch("(%w+) (%w+) (%w+) %-> (%w+)") do
    local gate = { name = name, op = op, inp = { a, b } }
    gates[name] = gate
    table.insert(list, name)
end
table.sort(list)

local panic = false
local function eval(gate, depth)
    depth = (depth or 0) + 1
    if depth > 100 then
        --print("panic")
        panic = true
        return 0
    end
    --print(gate.name)
    local value = gate.out or cache[gate]
    if not value then
        local a = eval(gates[gate.inp[1]], depth)
        local b = eval(gates[gate.inp[2]], depth)
        if gate.op == "AND" then value = a & b end
        if gate.op == "OR"  then value = a | b end
        if gate.op == "XOR" then value = a ~ b end
        cache[gate] = value
    end
    return value
end

local xyzname = { x = {}, y = {}, z = {} }
local function makename(prefix, index)
    local name = xyzname[prefix][index]
    if not name then
        name = string.format("%s%02d", prefix, index - 1)
        xyzname[prefix][index] = name
    end
    return name
end

local function collect(prefix)
    panic = false
    cache = {}
    local results = {}
    for i = 1, math.huge do
        local gate = gates[makename(prefix, i)]
        if not gate then break end
        results[i] = eval(gate)
    end
    return results
end

local function tonum(results)
    return tonumber(table.concat(results):reverse(), 2)
end

local function tobin(value)
    local bits = {}
    for i = 1, math.huge do
        bits[i] = value & 1
        value = value >> 1
        if value == 0 then break end
    end
    return bits
end

local function setnum(prefix, n, value)
    local b = tobin(value)
    for i = 1, n do
        gates[makename(prefix, i)].out = b[i] or 0
    end
end

local function swap(a, b)
    gates[a], gates[b] = gates[b], gates[a]
end

local function finderror(nz, nx, any)
    local lowest
    for trial = 1, 100 do
        local xnum = math.random((1 << nx) - 1)
        local ynum = math.random((1 << nx) - 1)
        setnum("x", nx, xnum)
        setnum("y", nx, ynum)
        local z = collect("z")
        local w = tobin(xnum + ynum)
        if panic then
            lowest = 0
        else
            for i = 1, nz do
                if z[i] ~= (w[i] or 0) then
                    lowest = math.min(i, lowest or math.huge)
                    break
                end
            end
        end
        if any and lowest then break end
    end
    return lowest
end

local x = collect("x")
--local y = collect("y")
local z = collect("z")
--local w = tobin(tonum(x) + tonum(y))

--print(tonum(x))
--print(tonum(y))
print(tonum(z))
--print(tonum(w))

local solution = {}

while true do
    local bit = finderror(#z, #x, false)
    if not bit then break end
    --print("bit", bit - 1)

    local pair = nil
    for i = 1, #list do
        for j = i + 1, #list do
            swap(list[i], list[j])
            if not finderror(bit, #x, true) then
                --print("swap", list[i], list[j])
                assert(not pair)
                pair = { list[i], list[j] }
            end
            swap(list[i], list[j])
        end
    end
    if not pair then
        print("failed")
        break
    end
    swap(pair[1], pair[2])
    table.insert(solution, pair[1])
    table.insert(solution, pair[2])
end

table.sort(solution)
print(table.concat(solution, ","))
