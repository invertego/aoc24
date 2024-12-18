#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

#define N (1 << 7)
#define A(x, y) a[(y) & (N - 1)][(x) & (N - 1)]
char a[N][N];
int n;

#define V(x, y) visit[(y) & (N - 1)][(x) & (N - 1)]
int visit[N][N];

const int dx[] = { 1, 0,-1, 0 };
const int dy[] = { 0, 1, 0,-1 };

typedef struct {
    int x, y;
} state_t;

state_t open[1 << 8];
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

int bfs(state_t start, state_t end)
{
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            V(x,  y) = INT_MAX;
        }
    }
    V(start.x, start.y) = 0;
    openrd = openwr = 0;
    push(start);

    while (openwr != openrd) {
        state_t s = pop();
        int dist = V(s.x, s.y);
        //A(s.x, s.y) = 'x';
        if (s.x == end.x && s.y == end.y) {
            return dist;
        }

        for (int d = 0; d < 4; d++) {
            state_t t = s;
            t.x += dx[d];
            t.y += dy[d];
            if (!A(t.x, t.y) || A(t.x, t.y) == '#') continue;
            int ndist = dist + 1;
            if (ndist < V(t.x, t.y)) {
                V(t.x, t.y) = ndist;
                push(t);
            }
        }
    }

    return INT_MAX;
}

int main()
{
    FILE* file = fopen("day18.txt", "r");
    //int limit = 12;
    //n = 6 + 1;
    int limit = 1024;
    n = 70 + 1;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            A(x, y) = '.';
        }
    }

    int cnt = 0;
    for (int x, y; 2 == fscanf(file, "%d,%d\n", &x, &y); ) {
        A(x, y) = '#';
        ++cnt;

        int dist = bfs((state_t){ 0, 0 }, (state_t){ n - 1, n - 1 });
        if (cnt == limit) {
            printf("%d\n", dist);
        }
        if (dist == INT_MAX) {
            printf("%d,%d\n", x, y);
            break;
        }
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            //printf("%c", A(x, y));
        }
        //printf("\n");
    }
}
