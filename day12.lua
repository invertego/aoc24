#!/usr/bin/env lua

local mt = { __index = function(t, k) t[k] = {} return t[k] end }

local a = setmetatable({}, mt)
local r = setmetatable({}, mt)
local n = 0

local di = {-1, 0, 1, 0 }
local dj = { 0, 1, 0,-1 }

local function dfs(i, j, x)
    r[i][j] = x
    local c = a[i][j]
    for d = 1, 4 do
        local ni = i + di[d]
        local nj = j + dj[d]
        if a[ni][nj] == c and not r[ni][nj] then
            dfs(ni, nj, x)
        end
    end
end

local function calc(x)
    local area = 0
    local perim = 0
    local fences = 0
    local f = setmetatable({}, mt)

    for i = 1, n do
        for j = 1, n do
            f[i][j] = {}
            if r[i][j] == x then
                area = area + 1
                for d = 1, 4 do
                    local ni = i + di[d]
                    local nj = j + dj[d]
                    if r[ni][nj] ~= x then
                        perim = perim + 1
                        f[i][j][d] = true
                    end
                end
            end
        end
    end

    for d = 1, 4 do
        for i = 1, n do
            local j = 1
            while j <= n do
                local k = j
                local len = 0
                while k <= n and (d & 1 == 0 and f[k][i] or f[i][k])[d] do
                    k = k + 1
                    len = len + 1
                end
                if len ~= 0 then
                    --print("fence", len)
                    fences = fences + 1
                    j = k - 1
                end
                j = j + 1
            end
        end
    end

    --print("area", area, "perim", perim, "fences", fences)
    return area, perim, fences
end

local file = io.open("day12.txt")
local sum = { 0, 0 }

for line in file:lines() do
    table.insert(a, { line:byte(1, #line) })
    n = n + 1
end

local x = 0
for i = 1, n do
    for j = 1, n do
        if not r[i][j] then
            x = x + 1
            dfs(i, j, x)
        end
    end
end

for i = 1, x do
    local area, perim, fences = calc(i)
    sum[1] = sum[1] + area * perim
    sum[2] = sum[2] + area * fences
end

print(sum[1], sum[2])
