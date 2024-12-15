#!/usr/bin/env lua

local a = {}
local w, h = 0, 0

local path = ""

local dstr = "^v<>"
local di = {-1, 1, 0, 0 }
local dj = { 0, 0,-1, 1 }

local function push(i, j, d, apply)
    local c = a[i][j]
    if c == '.' then return true end
    if c == '#' then return false end

    local adj, ac
    if c ~= 'O' and d <= 2 then
        if c == '[' then adj = 1 end
        if c == ']' then adj = -1 end
        ac = a[i][j + adj]
    end

    local ni, nj = i + di[d], j + dj[d]
    if not push(ni, nj, d, apply) then return false end
    if adj and not push(ni, nj + adj, d, apply) then return false end
    if apply then
        a[i][j] = '.'
        a[ni][nj] = c
        if adj then
            a[i][j + adj] = '.'
            a[ni][nj + adj] = ac
        end
    end
    return true
end

local function run()
    local b = {}
    for i = 1, h do b[i] = { table.unpack(a[i]) } end

    local si, sj
    for i = 1, h do
        for j = 1, w do
            if a[i][j] == '@' then
                a[i][j] = '.'
                si, sj = i, j
                break
            end
        end
    end

    local ci, cj = si, sj
    local npath = #path
    for p = 1, npath do
        local d = dstr:find(path:sub(p, p))
        local ni = ci + di[d]
        local nj = cj + dj[d]
        local nc = a[ni][nj]
        if nc == '#' then
        elseif nc == '.' then
            ci, cj = ni, nj
        elseif nc == 'O' or nc == '[' or nc == ']' then
            if push(ni, nj, d, false) then
                push(ni, nj, d, true)
                ci, cj = ni, nj
            end
        else assert(false) end
    end

    local sum = 0
    for i = 1, h do
        for j = 1, w do
            if a[i][j] == 'O' or a[i][j] == '[' then
                sum = sum + 100 * (i - 1) + (j - 1)
            end
            --io.write((i == ci and j == cj) and '@' or a[i][j])
        end
        --print()
    end

    a = b

    return sum
end

local function widen()
    for i = 1, h do
        for j = w, 1, -1 do
            local wide
            local c = a[i][j]
            if c == ('@') then
                wide = "@."
            elseif c == ('.') then
                wide = ".."
            elseif c == ('#') then
                wide = "##"
            elseif c == ('O') then
                wide = "[]"
            else assert(false) end
            a[i][j * 2 - 1 + 0] = wide:sub(1, 1)
            a[i][j * 2 - 1 + 1] = wide:sub(2, 2)
            end
    end
    w = w * 2
end

local file = io.open("day15.txt")
local sum = { 0, 0 }

for line in file:lines() do
    if not line:match("^[#.O@]+$") then break end
    local row = {}
    for i = 1, #line do
        row[i] = line:sub(i, i)
    end
    table.insert(a, row)
end
h = #a
w = h

for line in file:lines() do
    if not line:match("^[<>^v]+$") then break end
    path = path .. line
end

sum[1] = run()
widen()
sum[2] = run()

print(sum[1], sum[2])
