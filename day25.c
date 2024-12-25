#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N 500
#define W 5
int locks[N][W];
int nlocks = 0;
int keys[N][W];
int nkeys = 0;

bool fits(int* key, int* lock)
{
    for (int i = 0; i < W; i++) {
        if (lock[i] > key[i]) return false;
    }
    return true;
}

int main()
{
    FILE* file = fopen("day25.txt", "r");

    for (char line[W + 2]; fgets(line, sizeof(line), file); ) {
        int levels[W] = { 0 };
        char kind = 0;
        for (int i = 0; strlen(line) >= W; i++) {
            for (int j = 0; j < W; j++) {
                char cell = line[j];
                if (!kind) kind = cell;
                if (cell == kind) levels[j] = i;
            }
            if (!fgets(line, sizeof(line), file)) break;
        }
        memcpy(kind == '#' ? locks[nlocks++] : keys[nkeys++],
            levels, W * sizeof(levels[0]));
    }

    int count = 0;
    for (int i = 0; i < nkeys; i++) {
        for (int j = 0; j < nlocks; j++) {
            if (fits(keys[i], locks[j])) count++;
        }
    }
    printf("%d\n", count);
}
