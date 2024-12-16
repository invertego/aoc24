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
state_t prev[1 << (WS + HS + 2)][3];

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

int dfs(state_t t)
{
    int sum = 0;
    if (A(t.x, t.y) != 'O') {
        A(t.x, t.y) = 'O';
        sum++;
    }
    for (int p = 0; p < 3; p++) {
        state_t s = prev[t.v][p];
        if (s.v) {
            prev[t.v][p].v = 0;
            sum += dfs(s);
        }
    }
    return sum;
}

void insert(state_t s, state_t t, int ndist)
{
    if (ndist > visit[t.v]) return;
    if (ndist < visit[t.v]) memset(prev[t.v], 0, sizeof(prev[t.v]));
    for (int p = 0; p < 3; p++) {
        if (prev[t.v][p].v == s.v) return;
        if (!prev[t.v][p].v) {
            prev[t.v][p] = s;
            break;
        }
    }
    visit[t.v] = ndist;
    push(t);
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

        int nx = (int)s.x + dx[s.d];
        int ny = (int)s.y + dy[s.d];
        if (A(nx, ny) != '#') {
            state_t t = s;
            t.x = nx;
            t.y = ny;
            insert(s, t, dist + 1);
        }

        for (int dd = -1; dd <= 1; dd += 2) {
            state_t t = s;
            t.d = (s.d + dd + 4) % 4;
            insert(s, t, dist + 1000);
        }
    }

    *count = 0;
    for (int d = 0; d < 4; d++) {
        state_t t = end;
        t.d = d;
        if (visit[t.v] == shortest) {
            *count += dfs(t);
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
