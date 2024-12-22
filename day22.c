#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N (1 << 20)

bool observed[N];
int sums[N];

unsigned mix(unsigned a, unsigned b)
{
    return a ^ b;
}

unsigned prune(unsigned a)
{
    return a & 0xFFFFFF;
}

unsigned next(unsigned a)
{
    a = prune(mix(a << 6, a));
    a = prune(mix(a >> 5, a));
    a = prune(mix(a << 11, a));
    return a;
}

int main()
{
    FILE* file = fopen("day22.txt", "r");
    long long sum = 0, sum2 = 0;

    for (unsigned s; 1 == fscanf(file, "%u", &s); ) {
        int p = s % 10;
        int k = 0;
        memset(observed, 0, sizeof(observed));
        for (int i = 0; i < 2000; i++) {
            s = next(s);
            int q = s % 10;
            int d = q - p;
            k = ((k << 5) | (d + 9)) & (N - 1);
            if (!observed[k]) {
                observed[k] = true;
                sums[k] += q;
            }
            p = q;
        }
        sum += s;
    }

    int kmax = 0;
    for (int k = 0; k < N; k++) {
        if (sums[k] > sums[kmax]) {
            kmax = k;
        }
    }
    sum2 = sums[kmax];

    printf("%lld %lld\n", sum, sum2);
}
