#!/usr/bin/env lua

local input = io.open("day17.txt"):read("a")
local lines = {}
local function emit(...) lines[#lines + 1] = string.format(...) end

emit("return function(A, B, C)")
emit("local out = {}")

local regs = {}
for r, x in input:gmatch("(%u): (%d+)") do
    regs[r] = tonumber(x)
end

local adv, bxl, bst, jnz, bxc, out, bdv, cdv = 0, 1, 2, 3, 4, 5, 6, 7
local ip = 0
local prg = {}
local function combo(opr) return opr < 4 and opr or ({"A", "B", "C"})[opr - 3] end

for x, y in input:gmatch("(%d),(%d)") do
    emit("::L%d::", ip)
    ip = ip + 2
    local opc, opr = tonumber(x), tonumber(y)
    prg[#prg + 1] = opc
    prg[#prg + 1] = opr
    if     opc == adv then emit("A = A >> %s", combo(opr))
    elseif opc == bxl then emit("B = B ~ %s", opr)
    elseif opc == bst then emit("B = %s & 7", combo(opr))
    elseif opc == jnz then emit("if A ~= 0 then goto L%d end", opr)
    elseif opc == bxc then emit("B = B ~ C")
    elseif opc == out then emit("out[#out + 1] = %s & 7", combo(opr))
    elseif opc == bdv then emit("B = A >> %s", combo(opr))
    elseif opc == cdv then emit("C = A >> %s", combo(opr))
    end
end

emit("return out")
emit("end")

local code = table.concat(lines, "\n")
--print(code)

local run = load(code)()
print(table.concat(run(regs.A, regs.B, regs.C), ","))

local function solve(accum, pos)
    if pos < 1 then return accum end
    for low = 0, 7 do
        local a = (accum << 3) | low
        local x = run(a, regs.B, regs.C)[1]
        if x == prg[pos] then
            local result = solve(a, pos - 1)
            if result then return result end
        end
    end
end

print(solve(0, #prg))
