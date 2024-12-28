#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    X = -1,
    R = 0,
    D = 1,
    L = 2,
    U = 3,
    A = 10,

    N = 3,
};

const int dx[] = { 1, 0,-1, 0 };
const int dy[] = { 0, 1, 0,-1 };
const char dstr[] = ">v<^";

typedef struct {
    int pos[N][2];
    int num;
} state_t;

#define BITS ((2 + 1) * (N - 1) + 2 + 2 + 2)
typedef unsigned key_t;

key_t encode(state_t* s)
{
    int k = 0;
    for (int i = 0; i < N - 1; i++) {
        k <<= 2;
        k |= s->pos[i][0] & 3;
        k <<= 1;
        k |= s->pos[i][1] & 1;
    }
    k <<= 2;
    k |= s->pos[N - 1][0] & 3;
    k <<= 2;
    k |= s->pos[N - 1][1] & 3;
    k <<= 2;
    k |= s->num;
    return k;
}

void decode(key_t k, state_t* s)
{
    s->num = k & 3;
    k >>= 2;
    s->pos[N - 1][1] = k & 3;
    k >>= 2;
    s->pos[N - 1][0] = k & 3;
    k >>= 2;
    for (int i = N - 2; i >= 0; i--) {
        s->pos[i][1] = k & 1;
        k >>= 1;
        s->pos[i][0] = k & 3;
        k >>= 2;
    }
}

typedef struct {
    int dist;
    int prevd;
    key_t prevk;
} visit_t;

visit_t visit[1 << BITS];

void clear()
{
    for (int i = 0; i < COUNT(visit); i++) {
        visit[i] = (visit_t){ .dist = INT_MAX };
    }
}

visit_t* lookup(key_t k)
{
    return &visit[k];
}

typedef key_t open_t;

open_t open[1 << 14];
int openrd, openwr;

void push(open_t s)
{
    open[openwr++] = s;
    openwr &= COUNT(open) - 1;
    assert(openrd != openwr);
}

open_t pop()
{
    assert(openrd != openwr);
    open_t s = open[openrd++];
    openrd &= COUNT(open) - 1;
    return s;
}

bool dirvalid(int x, int y)
{
    return (y == 0 && x >= 1 && x <= 2)
        || (y == 1 && x >= 0 && x <= 2);
}

bool numvalid(int x, int y)
{
    return (y >= 0 && y <= 2 && x >= 0 && x <= 2)
        || (y == 3 && x >= 1 && x <= 2);
}

const int dirpad[2][3] = {
    { X, U, A },
    { L, D, R },
};

const int numpad[4][3] = {
    { 7, 8, 9 },
    { 4, 5, 6 },
    { 1, 2, 3 },
    { X, 0, A },
};

bool track = false;
char paths[N][512];

bool press(state_t s, int level, int d, const int* code, state_t* tout)
{
    //assert(d >= A);
    if (level == N) {
        assert((d >= 0 && d <= 9) || d == A);
        int target = code[s.num];
        if (d != target) return false;
        state_t t = s;
        t.num += 1;
        *tout = t;
        return true;
    }

    if (track) {
        char buf[2] = { d == A ? 'A' : dstr[d], 0 };
        strcat(paths[level], buf);
    }

    if (d == A) {
        int b;
        int* pos = s.pos[level];
        if (level < N - 1) {
            b = dirpad[pos[1]][pos[0]];
        } else {
            assert (level == N - 1);
            b = numpad[pos[1]][pos[0]];
        }
        return press(s, level + 1, b, code, tout);
    } else {
        assert(d >= 0 && d <= 3);
        int* pos = s.pos[level];
        int x = pos[0], y = pos[1];
        x += dx[d];
        y += dy[d];
        if (level < N - 1 && !dirvalid(x, y)) return false;
        if (level == N - 1 && !numvalid(x, y)) return false;
        state_t t = s;
        pos = t.pos[level];
        pos[0] = x;
        pos[1] = y;
        *tout = t;
        return true;
    }
}

void execpath(state_t start, char* path, const int* code)
{
    track = true;
    memset(paths, 0, sizeof(paths));
    state_t s = start, t;
    for (int i = 0; i < strlen(path); i++) {
        int d = path[i] == 'A' ? A : strchr(dstr, path[i]) - dstr;
        press(s, 0, d, code, &t);
        s = t;
    }
    for (int i = 0; i < N; i++) {
        printf("path[%d] %s\n", i, paths[i]);
    }
    track = false;
}

int bfs(state_t start, state_t end, const int* code)
{
    clear();
    openrd = openwr = 0;
    int shortest = INT_MAX;
    key_t startk = encode(&start), endk = encode(&end);
    lookup(startk)->dist = 0;
    push(startk);

    while (openwr != openrd) {
        key_t sk = pop();
        state_t s;
        decode(sk, &s);
        int dist = lookup(sk)->dist;
        if (!memcmp(&sk, &endk, sizeof(sk))) {
            if (dist < shortest) {
                shortest = dist;
            }
            break;
        }

        for (int d = -1; d < 4; d++) {
            state_t t = s;
            int ndist = dist + 1;
            int b = d < 0 ? A : d;
            if (!press(s, 0, b, code, &t)) continue;
            key_t tk = encode(&t);
            visit_t* tv = lookup(tk);
            if (ndist < tv->dist) {
                tv->dist = ndist;
                tv->prevk = sk;
                tv->prevd = b;
                push(tk);
            }
        }
    }

    if (shortest < 512 - 2) {
        char pathbuf[512];
        char* path = pathbuf + sizeof(pathbuf);
        *--path = 0;
        *--path = 'A';
        key_t sk = endk;
        while (memcmp(&sk, &startk, sizeof(sk))) {
            visit_t* sv = lookup(sk);
            int d = sv->prevd;
            if (d == A) *--path = 'A';
            else *--path = dstr[d];
            sk = sv->prevk;
        }
        printf("path %s\n", path);

        execpath(start, path, code);
    }

    return shortest + 1;
}

int main()
{
    FILE* file = fopen("day21.txt", "r");
    long long sum = 0;

    for (int x; 1 == fscanf(file, "%dA", &x); ) {
        printf("%03dA\n", x);
        int code[4] = {
            (x / 100) % 10,
            (x /  10) % 10,
            (x /   1) % 10,
            A,
        };

        state_t s = { 0 };
        for (int i = 0; i < N - 1; i++) {
            s.pos[i][0] = 2;
            s.pos[i][1] = 0;
        }
        s.pos[N - 1][0] = 2;
        s.pos[N - 1][1] = 3;
        s.num = 0;
        state_t e = s;
        e.num = 3;
        long long d = bfs(s, e, code);
        printf("shortest %lld\n", d);
        sum += d * x;
    }

    printf("%lld\n", sum);
}
