#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define A(i, j) a[i + 4][j + 4]
char a[256][256];
int n;

bool match(const char* m, int f, int i, int j, int di, int dj)
{
    for (int k = 0; k < strlen(m); k++) {
        int s = i + di * k;
        int t = j + dj * k;
        if (f & 1) s = n - s - 1;
        if (f & 2) t = n - t - 1;
        if (f & 4) { int u = s; s = t; t = u; }
        if (A(s, t) != m[k]) return false;
    }
    return true;
}

int main()
{
    FILE* file = fopen("day4.txt", "r");
    int sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%s", &A(n++, 0))) ;

    for (int f = 0; f < 8; f++)
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
        if (match("XMAS", f, i, j, 1, 0)) sum++;
        if (match("XMAS", f, i, j, 1, 1)) sum++;
        if (match("MAS", f, i, j, 1, 1) && 
            match("MAS", f, i, j + 2, 1, -1)) sum2++;
    }

    printf("%d %d\n", sum / 2, sum2 / 2);
}
