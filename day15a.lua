#!/usr/bin/env lua

local a = {}
local w, h = 0, 0

local path = ""

local dstr = "^v<>"
local di = {-1, 1, 0, 0 }
local dj = { 0, 0,-1, 1 }

local dot = ('.'):byte()
local hash = ('#'):byte()
local box = ('O'):byte()
local lbox = ('['):byte()
local rbox = (']'):byte()
local start = ('@'):byte()

local function push(i, j, d, apply)
    local c = a[i][j]
    if c == dot then return true end
    if c == hash then return false end

    local adj, ac
    if c ~= box and d <= 2 then
        if c == lbox then adj = 1 end
        if c == rbox then adj = -1 end
        ac = a[i][j + adj]
    end

    local ni, nj = i + di[d], j + dj[d]
    if not push(ni, nj, d, apply) then return false end
    if adj and not push(ni, nj + adj, d, apply) then return false end
    if apply then
        a[i][j] = dot
        a[ni][nj] = c
        if adj then
            a[i][j + adj] = dot
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
            if a[i][j] == start then
                a[i][j] = dot
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
        if nc == hash then
        elseif nc == dot then
            ci, cj = ni, nj
        elseif nc == box or nc == lbox or nc == rbox then
            if push(ni, nj, d, false) then
                push(ni, nj, d, true)
                ci, cj = ni, nj
            end
        else assert(false) end
    end

    local sum = 0
    for i = 1, h do
        for j = 1, w do
            if a[i][j] == box or a[i][j] == lbox then
                sum = sum + 100 * (i - 1) + (j - 1)
            end
            --io.write(string.char((i == ci and j == cj) and start or a[i][j]))
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
            if c == start then
                wide = "@."
            elseif c == dot then
                wide = ".."
            elseif c == hash then
                wide = "##"
            elseif c == box then
                wide = "[]"
            else assert(false) end
            a[i][j * 2 - 1 + 0] = wide:byte(1)
            a[i][j * 2 - 1 + 1] = wide:byte(2)
            end
    end
    w = w * 2
end

local file = io.open("day15.txt")
local sum = { 0, 0 }

for line in file:lines() do
    if not line:match("^[#.O@]+$") then break end
    table.insert(a, { line:byte(1, #line) })
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
