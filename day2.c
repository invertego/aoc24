#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool safe(const int* x, int c)
{
    bool inc = true, dec = true, adj = true;
    for (int i = 0; i < c - 1; i++) {
        if (!(x[i] > x[i + 1])) dec = false;
        if (!(x[i] < x[i + 1])) inc = false;
        int d = abs(x[i] - x[i + 1]);
        if (!(d >= 1 && d <= 3)) adj = false;
    }
    return (inc || dec) && adj;
}

int main()
{
    FILE* file = fopen("day2.txt", "r");
    int sum = 0, sum2 = 0;

    char line[256];
    while (fgets(line, 256, file)) {
        int x[8];
        int c = 0;

        char* t = strtok(line, " \n");
        while (t) {
            sscanf(t, "%d", &x[c++]);
            t = strtok(NULL, " \n");
        }

        if (safe(x, c)) {
            sum++;
        }
        for (int i = 0; i < c; i++) {
            int y[8];
            for (int j = 0, k = 0; j < c - 1; j++, k++) {
                if (i == j) k++;
                y[j] = x[k];
            }
            if (safe(y, c - 1)) {
                sum2++;
                break;
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
