#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int list[2][1000];
int cnt;

int cmp(const void* va, const void* vb)
{
    const int* a = va;
    const int* b = vb;
    return *a - *b;
}

int main()
{
    FILE* file = fopen("day1.txt", "r");
    int sum = 0, sum2 = 0;

    for (int x, y; 2 == fscanf(file, "%d %d", &x, &y); ) {
        list[0][cnt] = x;
        list[1][cnt] = y;
        cnt++;
    }

    qsort(list[0], cnt, sizeof(list[0][0]), cmp);
    qsort(list[1], cnt, sizeof(list[1][0]), cmp);

    for (int i = 0; i < cnt; i++) {
        sum += abs(list[0][i] - list[1][i]);

        for (int j = 0; j < cnt; j++) {
            if (list[0][i] == list[1][j]) {
                sum2 += list[0][i];
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
