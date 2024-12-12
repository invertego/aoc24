#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A(i, j) a[(i) & 255][(j) & 255]
#define R(i, j) r[(i) & 255][(j) & 255]
#define F(i, j) f[(i) & 255][(j) & 255]
char a[256][256];
int r[256][256];
bool f[256][256][4];
int n;

const int di[] = {-1, 0, 1, 0 };
const int dj[] = { 0, 1, 0,-1 };

void dfs(int i, int j, int x)
{
    R(i, j) = x;
    for (int d = 0; d < 4; d++) {
        int ni = i + di[d];
        int nj = j + dj[d];
        char c = A(ni, nj);
        if (c != A(i, j)) continue;
        if (R(ni, nj)) continue;
        dfs(ni, nj, x);
    }
}

int calc(int x, int* perim, int* fences)
{
    int area = 0;
    *perim = 0;
    *fences = 0;
    memset(f, false, sizeof(f));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (R(i, j) != x) continue;
            area++;
            for (int d = 0; d < 4; d++) {
                int ni = i + di[d];
                int nj = j + dj[d];
                if (R(ni, nj) != x) {
                    *perim += 1;
                    F(i, j)[d] = true;
                }
            }
        }
    }

    for (int d = 0; d < 4; d++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int k = j;
                int len = 0;
                while ((d & 1 ? F(k, i) : F(i, k))[d]) {
                    k++;
                    len++;
                }
                if (len) {
                    //printf("fence %d\n", len);
                    *fences += 1;
                    j = k - 1;
                }
            }
        }
    }

    //printf("area %d perim %d fences %d\n", area, *perim, *fences);
    return area;
}

int main()
{
    FILE* file = fopen("day12.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &A(n, 0))) {
        n++;
    }

    int x = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (R(i, j)) continue;
            dfs(i, j, ++x);
        }
    }

    for (int i = 1; i <= x; i++) {
        int perim, fences;
        int area = calc(i, &perim, &fences);
        sum += area * perim;
        sum2 += area * fences;
    }

    printf("%d %d", sum, sum2);
}
