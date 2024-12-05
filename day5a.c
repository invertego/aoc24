#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool order[100][100];

int compare(const void* va, const void* vb)
{
    const int* a = va, * b = vb;
    if (order[*a][*b]) return -1;
    if (order[*b][*a]) return 1;
    return 0;
}

bool is_sorted(const int* pages, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (order[pages[j]][pages[i]]) return false;
        }
    }
    return true;
}

int main()
{
    FILE* file = fopen("day5.txt", "r");
    int sum[2] = {};
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        int x, y;
        if (2 != sscanf(line, "%d|%d", &x, &y)) break;
        order[x][y] = true;
    }

    while (fgets(line, sizeof(line), file)) {
        int pages[30], n = 0;
        char* t = strtok(line, ",\n");
        while (t) {
            sscanf(t, "%d", &pages[n++]);
            t = strtok(NULL, ",\n");
        }

        bool sorted = is_sorted(pages, n);
        if (!sorted) qsort(pages, n, sizeof(pages[0]), compare);
        sum[!sorted] += pages[n / 2];
    }

    printf("%d %d\n", sum[0], sum[1]);
}
