#include <stdio.h>
#include <stdlib.h>

#define M (1 << 20)
long long memo[M][75 + 1];

long long blink(long long x, int n)
{
    if (x < M && memo[x][n]) return memo[x][n];
    long long y;
    if (n == 0) y = 1;
    else if (x == 0) y = blink(1, n - 1);
    else {
        char buf[32];
        int len = sprintf(buf, "%lld", x);
        if (!(len % 2)) {
            y = blink(atoll(buf + len / 2), n - 1);
            buf[len / 2] = 0;
            y += blink(atoll(buf), n - 1);
        } else {
            y = blink(x * 2024, n - 1);
        }
    }
    if (x < M) memo[x][n] = y;
    return y;
}

int main()
{
    FILE* file = fopen("day11.txt", "r");
    long long sum = 0, sum2 = 0;

    for (int x; 1 == fscanf(file, "%d", &x); ) {
        sum += blink(x, 25);
        sum2 += blink(x, 75);
    }

    printf("%lld %lld", sum, sum2);
}
