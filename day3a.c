#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* scan(char* s)
{
    char* t = strstr(s, "mul(");
    char* u = strstr(s, "do()");
    char* v = strstr(s, "don't()");
    if (u && u < t) t = u;
    if (v && v < t) t = v;
    return t;
}

int main()
{
    FILE* file = fopen("day3.txt", "r");
    int sum = 0, sum2 = 0;

    char line[4096];
    bool e = true;
    while (fgets(line, sizeof(line), file)) {
        for (char* s = scan(line); s; s = scan(s)) {
            if (s[0] == 'd') {
                e = (s[2] == '(');
                s++;
            } else {
                int x, y, n = 0;
                int r = sscanf(s, "mul(%d,%d)%n", &x, &y, &n);
                //printf("%d %d %d %d\n", x, y, n, r);
                if (r == 2 && n != 0) {
                    s += n;
                    sum += x * y;
                    if (e) sum2 += x * y;
                } else {
                    s++;
                }
            }
        }
    }

    printf("%d %d\n", sum, sum2);
}
