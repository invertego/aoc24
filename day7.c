#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int nextpow10(int x)
{
    int p = 10;
    while (p <= x) p *= 10;
    return p;
}

long long solve(const int* terms, int n, long long test, int base)
{
    long long sum = 0;
    int p = pow(base, n);
    for (int i = 0; i < p; i++) {
        long long accum = terms[0];
        int quot = i;
        for (int j = 1; j < n; j++) {
            int op = quot % base;
            quot /= base;
            switch (op) {
            case 0: accum *= terms[j]; break;
            case 1: accum += terms[j]; break;
            case 2: accum = accum * nextpow10(terms[j]) + terms[j]; break;
            }
            if (accum > test) break;
        }
        if (accum == test) {
            sum += test;
            break;
        }
    }
    return sum;
}

int main()
{
    FILE* file = fopen("day7.txt", "r");
    long long sum = 0, sum2 = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        long long test;
        char* t = strtok(line, ":");
        sscanf(t, "%lld", &test);

        int terms[16], n = 0;
        while ((t = strtok(NULL, " \n"))) {
            sscanf(t, "%d", &terms[n++]);
        }

        sum += solve(terms, n, test, 2);
        sum2 += solve(terms, n, test, 3);
    }

    printf("%lld %lld\n", sum, sum2);
}
