#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define A(i, j) a[(i) & 255][(j) & 255]
char a[256][256], b[256][256];
int n;

const int di[] = {-1, 0, 1, 0 };
const int dj[] = { 0, 1, 0,-1 };

int walk(int si, int sj)
{
    int i = si, j = sj;
    int d = 0;
    int steps = 0;
    memcpy(b, a, sizeof(a));
    while (A(i, j)) {
        if (A(i, j) == '.') {
            A(i, j) = 'X' + d;
            steps++;
        } else if (A(i, j) == 'X' + d) {
            steps = -steps;
            break;
        }
        int ni = i + di[d], nj = j + dj[d];
        if (A(ni, nj) == '#') {
            d = (d + 1) & 3;
        } else {
            i = ni, j = nj;
        }
    }
    memcpy(a, b, sizeof(a));
    return steps;
}

int main()
{
    FILE* file = fopen("day6.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &A(n, 0))) n++;

    int si, sj;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A(i, j) == '^') {
                A(i, j) = '.';
                si = i, sj = j;
                break;
            }
        }
    }

    sum = walk(si, sj);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (A(i, j) == '.') {
                A(i, j) = '#';
                int steps = walk(si, sj);
                if (steps < 0) sum2++;
                A(i, j) = '.';
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
