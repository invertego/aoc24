#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

#define M(i, j) map[(i) & 127][(j) & 127]
#define V(i, j) visit[(i) & 127][(j) & 127]
char map[128][128];
char visit[128][128];
int n;

typedef struct {
    int pairs[8][2];
    int n;
} pair_t;

pair_t pairs[128];

bool mark(int i , int j, int f)
{
    if (!M(i, j) || (V(i, j) & f)) return false;
    V(i, j) |= f;
    return true;
}

int sweep(int* p, int* d, int f)
{
    int cnt = 0;
    int i = p[0], j = p[1];
    while (M(i, j)) {
        if (mark(i, j, f)) cnt++;
        i += d[0];
        j += d[1];
    }
    return cnt;
}

int main()
{
    FILE* file = fopen("day8.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &M(n, 0))) n++;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (M(i, j) != '.') {
                pair_t* p = &pairs[(int)M(i, j)];
                assert(p->n < COUNT(p->pairs));
                p->pairs[p->n][0] = i;
                p->pairs[p->n][1] = j;
                p->n++;
            }
        }
    }

    for (int u = 0; u < COUNT(pairs); u++) {
        pair_t* p = &pairs[u];
        for (int v = 0; v < p->n; v++) {
            for (int w = v + 1; w < p->n; w++) {
                int* pv = p->pairs[v];
                int* pw = p->pairs[w];
                int d[2] = { pw[0] - pv[0], pw[1] - pv[1] };
                if (mark(pw[0] + d[0], pw[1] + d[1], 1)) sum++;
                if (mark(pv[0] - d[0], pv[1] - d[1], 1)) sum++;
                sum2 += sweep(pw, d, 2);
                d[0] *= -1;
                d[1] *= -1;
                sum2 += sweep(pv, d, 2);
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
