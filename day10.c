#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N 64
#define A(i, j) a[(i) & (N - 1)][(j) & (N - 1)]
#define V(i, j) v[(i) & (N - 1)][(j) & (N - 1)]
char a[N][N];
bool v[N][N];
int n;

const int di[] = {-1, 0, 1, 0 };
const int dj[] = { 0, 1, 0,-1 };

int dfs(int i, int j, char target, bool track)
{
    if (A(i, j) != target) return 0;
    if (track) {
        if (V(i, j)) return 0;
        V(i, j) = true;
    }
    if (target == '9') return 1;

    int sum = 0;
    for (int d = 0; d < 4; d++) {
        sum += dfs(i + di[d], j + dj[d], target + 1, track);
    }
    return sum;
}

int main()
{
    FILE* file = fopen("day10.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &A(n, 0))) n++;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            memset(v, 0, sizeof(v));
            sum += dfs(i, j, '0', true);
            sum2 += dfs(i, j, '0', false);
        }
    }

    printf("%d %d", sum, sum2);
}
