#!/usr/bin/env lua

local adv <const> = 0
local bxl <const> = 1
local bst <const> = 2
local jnz <const> = 3
local bxc <const> = 4
local out <const> = 5
local bdv <const> = 6
local cdv <const> = 7

local A <const> = 1
local B <const> = 2
local C <const> = 3

local reg = {}
local prg = {}

local function combo(o)
    if o < 4 then return o end
    if o < 7 then return reg[o - 3] end
    assert(false)
end

local function run(limit)
    local output = {}
    local ip = 0
    local ipn
    limit = limit or math.maxinteger
    while ip >= 0 and ip < #prg and #output < limit do
        assert(ip % 2 == 0)
        local opc, o = prg[ip + 1], prg[ip + 2]
        ipn = ip + 2
        if     opc == adv then reg[A] = reg[A] >> combo(o)
        elseif opc == bxl then reg[B] = reg[B] ~ o
        elseif opc == bst then reg[B] = combo(o) % 8
        elseif opc == jnz then if reg[A] ~= 0 then ipn = o end
        elseif opc == bxc then reg[B] = reg[B] ~ reg[C]
        elseif opc == out then output[#output + 1] = combo(o) % 8
        elseif opc == bdv then reg[B] = reg[A] >> combo(o)
        elseif opc == cdv then reg[C] = reg[A] >> combo(o)
        end
        ip = ipn
    end
    return output
end

local function solve(accum, pos)
    if pos < 1 then return accum end
    for low = 0, 7 do
        local a = (accum << 3) | low
        reg[A] = a
        local x = run(1)[1]
        if x == prg[pos] then
            local result = solve(a, pos - 1)
            if result then return result end
        end
    end
end

local file = io.open("day17.txt")

for line in file:lines() do
    local rs, xs = line:match("Register (%u): (%d+)")
    if not rs then break end
    local r, x = rs:byte(), tonumber(xs)
    --print(string.format("Register %c: %d", r, x))
    reg[r - ("A"):byte() + 1] = x
end

for xs in file:read():gmatch("%d") do
    local x = tonumber(xs)
    --print(string.format("%d,", x))
    prg[#prg + 1] = x
end

local output = run()
print(table.concat(output, ","))

print(solve(0, #prg))
