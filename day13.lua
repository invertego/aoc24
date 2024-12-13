#!/usr/bin/env lua

local function solve(game)
    local a, c = table.unpack(game[1])
    local b, d = table.unpack(game[2])
    local x, y = table.unpack(game[3])
    local z = d * x - b * y
    local w = a * y - c * x
    local det = a * d - b * c
    if z % det ~= 0 or w % det ~= 0 then return 0 end
    return (z * 3 + w) // det
end

local file = io.open("day13.txt")
local sum = { 0, 0 }

repeat
    local game = {
        { file:read():match("Button A: X%+(%d+), Y%+(%d+)") },
        { file:read():match("Button B: X%+(%d+), Y%+(%d+)") },
        { file:read():match("Prize: X=(%d+), Y=(%d+)") },
    }
    for i = 1, #game do
        for j = 1, #game[i] do
            game[i][j] = tonumber(game[i][j])
        end
    end

    sum[1] = sum[1] + solve(game)
    game[3][1] = game[3][1] + 10000000000000
    game[3][2] = game[3][2] + 10000000000000
    sum[2] = sum[2] + solve(game)
until not file:read()

print(sum[1], sum[2])
