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

    NBOTS = 25 + 1,
};

const char dstr[] = ">v<^";

typedef union {
    struct {
        int x, y;
    };
    int v[2];
} coord_t;

typedef struct {
    coord_t pos[NBOTS];
    int num;
} state_t;

#define KEYBITS (2 + 1 + 4 + 5)
typedef union {
    struct {
        unsigned x : 2;
        unsigned y : 1;
        unsigned d : 4;
        unsigned l : 5;
    };
    unsigned v;
} key_t;

typedef struct {
    long long dist;
    state_t nexts;
    char path[16];
} visit_t;

visit_t visit[1 << KEYBITS];

void clear()
{
    for (int i = 0; i < COUNT(visit); i++) {
        visit[i] = (visit_t){ .dist = LONG_LONG_MAX };
    }
}

visit_t* lookup(state_t* s, int level, int d)
{
    key_t k = {
        .x = s->pos[level].x,
        .y = s->pos[level].y,
        .d = d,
        .l = level,
    };
    return &visit[k.v];
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

coord_t invdirpad[11];

const int numpad[4][3] = {
    { 7, 8, 9 },
    { 4, 5, 6 },
    { 1, 2, 3 },
    { X, 0, A },
};

coord_t invnumpad[11];

int sign(int x)
{
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

int nlevel;
int code[4];

long long solvelevel(state_t* s, int level, int d, char* path);

long long solvemove(state_t* s, int level, coord_t tpos, int firstaxis, char* path)
{
    const int dirs[2][2] = { { L, R }, { U, D } };
    long long dist = 0;
    coord_t* pos = &s->pos[level];
    coord_t sdir = {{ sign(tpos.x - pos->x), sign(tpos.y - pos->y) }};
    for (int i = 0; i < 2; i++) {
        int axis = i ^ firstaxis;
        while (pos->v[axis] != tpos.v[axis]) {
            pos->v[axis] += sdir.v[axis];
            if ((level == nlevel && !numvalid(pos->x, pos->y)) ||
                (level != nlevel && !dirvalid(pos->x, pos->y))) {

                return LONG_LONG_MAX;
            }
            dist += solvelevel(s, level - 1, sdir.v[axis] < 0 ? dirs[axis][0] : dirs[axis][1], path);
        }
    }
    dist += solvelevel(s, level - 1, A, path);;
    return dist;
}

long long solvelevel(state_t* s, int level, int d, char* path)
{
    if (level < 0) {
        #if 0
        if (nlevel < 3) {
            char buf[2] = { d == A ? 'A' : dstr[d], 0 };
            strcat(path, buf);
        }
        #endif
        return 1;
    }
    visit_t* v = NULL;
    if (level < nlevel) {
        v = lookup(s, level, d);
        if (v->dist != LONG_LONG_MAX) {
            memcpy(s->pos, v->nexts.pos, (level + 1) * sizeof(s->pos[0]));
            strcat(path, v->path);
            return v->dist;
        }
    }

    coord_t tpos = level == nlevel ? invnumpad[d] : invdirpad[d];
    state_t nexts[2] = { *s, *s };
    long long dists[2] = { LONG_LONG_MAX, LONG_LONG_MAX };
    char paths[2][64] = { "", "" };
    for (int axis = 0; axis < 2; axis++) {
        dists[axis] = solvemove(&nexts[axis], level, tpos, axis, paths[axis]);
    }

    assert(dists[0] < LONG_LONG_MAX || dists[1] < LONG_LONG_MAX);
    int min = dists[0] < dists[1] ? 0 : 1;
    *s = nexts[min];
    strcat(path, paths[min]);

    if (v) {
        v->dist = dists[min];
        memcpy(v->nexts.pos, s->pos, (level + 1) * sizeof(s->pos[0]));
        strcpy(v->path, paths[min]);
    }
    return dists[min];
}

long long solve(int nbots)
{
    nlevel = nbots - 1;
    clear();

    long long dist = 0;
    char path[128] = "";
    state_t s = { 0 };
    for (int i = 0; i < nlevel; i++) {
        s.pos[i] = (coord_t){{ 2, 0 }};
    }
    s.pos[nlevel] = (coord_t){{ 2, 3 }};
    s.num = 0;

    while (s.num < 4) {
        int target = code[s.num];
        dist += solvelevel(&s, nlevel, target, path);
        s.num++;
    }
    if (path[0]) printf("path %s\n", path);
    return dist;
}

int main()
{
    FILE* file = fopen("day21.txt", "r");
    long long sum = 0, sum2 = 0;

    for (int y = 0; y < COUNT(numpad); y++) {
        for (int x = 0; x < COUNT(numpad[y]); x++) {
            int d = numpad[y][x];
            if (d != X) {
                invnumpad[d] = (coord_t){{ x, y }};
            }
        }
    }

    for (int y = 0; y < COUNT(dirpad); y++) {
        for (int x = 0; x < COUNT(dirpad[y]); x++) {
            int d = dirpad[y][x];
            if (d != X) {
                invdirpad[d] = (coord_t){{ x, y }};
            }
        }
    }

    for (int x = 0; 1 == fscanf(file, "%dA", &x); ) {
        //printf("%03dA\n", x);
        code[0] = (x / 100) % 10;
        code[1] = (x /  10) % 10;
        code[2] = (x /   1) % 10;
        code[3] = A;
        long long d = solve(2 + 1);
        long long d2 = solve(25 + 1);
        //printf("%lld %lld\n", d, d2);
        sum += d * x;
        sum2 += d2 * x;
    }

    printf("%lld %lld\n", sum, sum2);
}
