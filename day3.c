#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE* file = fopen("day3.txt", "r");
    int sum = 0, sum2 = 0;
    bool enable = true;
    char line[4096];

    while (fgets(line, sizeof(line), file)) {
        for (char* s = line; *s; s++) {
            int x, y, n = 0;
            if (2 == sscanf(s, "mul(%d,%d)%n", &x, &y, &n) && n) {
                sum += x * y;
                if (enable) sum2 += x * y;
            } else if (!strncmp(s, "do()", 4)) {
                enable = true;
            } else if (!strncmp(s, "don't()", 7)) {
                enable = false;
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
