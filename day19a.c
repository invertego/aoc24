#include <stdio.h>
#include <string.h>

char pats[512][8 + 1];
int n;

long long solve(const char* target)
{
    long long memo[60 + 1];
    memo[strlen(target)] = 1;

    for (int i = strlen(target) - 1; i >= 0; i--) {
        long long count = 0;
        for (int p = 0; p < n; p++) {
            int len = strlen(pats[p]);
            if (!strncmp(&target[i], pats[p], len)) {
                count += memo[i + len];
            }
        }
        memo[i] = count;
    }

    return memo[0];
}

int main()
{
    FILE* file = fopen("day19.txt", "r");
    long long sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%[a-z],%*[ ]", pats[n])) n++;

    char target[60 + 1];
    while (1 == fscanf(file, "%s", target)) {
        long long count = solve(target);
        sum += count != 0;
        sum2 += count;
    }

    printf("%lld %lld\n", sum, sum2);
}
