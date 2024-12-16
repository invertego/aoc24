#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

#define WS 8
#define HS 8
#define W (1 << WS)
#define H (1 << HS)
#define A(x, y) a[y][x]
char a[H][W];
int n;

const int dx[] = { 1, 0,-1, 0 };
const int dy[] = { 0, 1, 0,-1 };

typedef union {
    struct {
        unsigned x : WS;
        unsigned y : HS;
        unsigned d : 2;
    };
    unsigned v;
} state_t;

int visit[1 << (WS + HS + 2)];

state_t open[1 << 12];
int openrd, openwr;

void push(state_t s)
{
    open[openwr++] = s;
    openwr &= COUNT(open) - 1;
    assert(openrd != openwr);
}

state_t pop()
{
    assert(openrd != openwr);
    state_t s = open[openrd++];
    openrd &= COUNT(open) - 1;
    return s;
}

int bfs(state_t start, state_t end, int* count)
{
    for (int i = 0; i < COUNT(visit); i++) {
        visit[i] = INT_MAX;
    }
    int shortest = INT_MAX;
    visit[start.v] = 0;
    push(start);

    while (openwr != openrd) {
        state_t s = pop();
        int dist = visit[s.v];
        //A(s.x, s.y) = 'x';
        if (s.x == end.x && s.y == end.y) {
            if (dist < shortest) shortest = dist;
        }

        for (int dd = -1; dd <= 1; dd++) {
            state_t t = s;
            int ndist = dist;
            if (dd == 0) {
                t.x += dx[s.d];
                t.y += dy[s.d];
                if (A(t.x, t.y) == '#') continue;
                ndist += 1;
            } else {
                t.d = (s.d + dd + 4) % 4;
                ndist += 1000;
            }
            if (ndist < visit[t.v]) {
                visit[t.v] = ndist;
                push(t);
            }
        }
    }

    *count = 0;
    for (int d = 0; d < 4; d++) {
        state_t t = end;
        t.d = d;
        if (visit[t.v] == shortest) {
            push(t);
        }
    }
    while (openwr != openrd) {
        state_t t = pop();
        int dist = visit[t.v];
        if (A(t.x, t.y) != 'O') {
            A(t.x, t.y) = 'O';
            (*count)++;
        }

        for (int dd = -1; dd <= 1; dd++) {
            state_t s = t;
            int ndist = dist;
            if (dd == 0) {
                s.x -= dx[t.d];
                s.y -= dy[t.d];
                ndist -= 1;
            } else {
                s.d = (t.d + dd + 4) % 4;
                ndist -= 1000;
            }
            if (visit[s.v] == ndist) {
                push(s);
            }
        }
    }

    return shortest;
}

int main()
{
    FILE* file = fopen("day16.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &A(0, n))) n++;

    int sx, sy, ex, ey;
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (A(x, y) == 'S') {
                A(x, y) = '.';
                sx = x, sy = y;
            } else if (A(x, y) == 'E') {
                A(x, y) = '.';
                ex = x, ey = y;
            }
        }
    }
    //printf("n %d sx %d sy %d ex %d ey %d\n", n, sx, sy, ex, ey);

    sum = bfs((state_t){{ sx, sy, 0 }}, (state_t){{ ex, ey, 0 }}, &sum2);

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            //printf("%c", A(x, y));
        }
        //printf("\n");
    }

    printf("%d %d\n", sum, sum2);
}
