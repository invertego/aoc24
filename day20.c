#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

#define N (1 << 8)
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

void bfs(state_t start, state_t end, int* count)
{
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            V(x, y) = INT_MAX;
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
            break;
        }

        for (int d = 0; d < 4; d++) {
            state_t t = s;
            int ndist = dist + 1;
            t.x += dx[d & 3];
            t.y += dy[d & 3];
            if (!A(t.x, t.y) || A(t.x, t.y) == '#') continue;
            if (ndist < V(t.x, t.y)) {
                V(t.x, t.y) = ndist;
                push(t);
            }
        }
    }

    count[0] = 0;
    count[1] = 0;
    //int counts[100] = {0};
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            state_t s = { x, y };
            int dist = V(s.x, s.y);
            if (dist == INT_MAX) continue;
            for (int v = -20; v <= 20; v++)
            for (int u = -20; u <= 20; u++) {
                int travel = abs(u) + abs(v);
                if (travel > 20) continue;
                state_t t = s;
                t.x += u;
                t.y += v;
                int ndist = V(t.x, t.y);
                if (ndist == INT_MAX) continue;
                int savings = dist - (ndist + travel);
                if (savings > 0) {
                    //printf("cheat %d\n", savings);
                    //counts[savings]++;
                }
                if (savings >= 100) {
                    if (travel == 2) count[0]++;
                    count[1]++;
                }
            }
        }
    }
    for (int i = 50; i < 100; i++) {
        //if (counts[i]) printf("There are %d cheats that save %d picoseconds.\n", counts[i], i);
    }
}

int main()
{
    FILE* file = fopen("day20.txt", "r");

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

    int counts[2];
    bfs((state_t){ sx, sy }, (state_t){ ex, ey }, counts);

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            //printf("%c", A(x, y));
        }
        //printf("\n");
    }

    printf("%d %d\n", counts[0], counts[1]);
}
