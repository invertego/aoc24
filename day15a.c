#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define A(i, j) a[i][j]
char a[64][64];
int n;

char path[1 << 16];

const char dstr[] = "^v<>";
const int di[] = {-1, 1, 0, 0 };
const int dj[] = { 0, 0,-1, 1 };

int main()
{
    FILE* file = fopen("day15.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%[#.O@]\n", &A(n, 0))) n++;

    while (1 == fscanf(file, "%[<>^v]\n", path + strlen(path))) ;


    int si, sj;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A(i, j) == '@') {
                A(i, j) = '.';
                si = i, sj = j;
                break;
            }
        }
    }

    int ci = si, cj = sj;
    int npath = strlen(path);
    for (int p = 0; p < npath; p++) {
        int d = strchr(dstr, path[p]) - dstr;
        int ni = ci + di[d];
        int nj = cj + dj[d];
        char nc = A(ni, nj);
        if (nc == '#') {
        } else if (nc == '.') {
            ci = ni, cj = nj;
        } else if (nc == 'O') {
            int mi = ni, mj = nj;
            while (1) {
                mi += di[d];
                mj += dj[d];
                char mc = A(mi, mj);
                if (mc == 'O') {
                    continue;
                } else if (mc == '.') {
                    A(mi, mj) = 'O';
                    A(ni, nj) = '.';
                    ci = ni, cj = nj;
                    break;
                } else if (mc == '#') {
                    break;
                } else assert(false);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A(i, j) == 'O') sum += 100 * i + j;
            //printf("%c", (i == ci && j == cj) ? '@' : A(i, j));
        }
        //printf("\n");
    }

    printf("%d %d\n", sum, sum2);
}
