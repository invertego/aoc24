#include <stdio.h>
#include <string.h>

char pats[512][8 + 1];
int n;
long long memo[60 + 1];

long long solve(const char* target, int i)
{
    if (i == 0) memset(memo, -1, sizeof(memo));
    if (target[i] == 0) return 1;
    if (memo[i] >= 0) return memo[i];

    long long count = 0;
    for (int p = 0; p < n; p++) {
        int len = strlen(pats[p]);
        if (!strncmp(&target[i], pats[p], len)) {
            count += solve(target, i + len);
        }
    }

    return memo[i] = count;
}

int main()
{
    FILE* file = fopen("day19.txt", "r");
    long long sum = 0, sum2 = 0;

    while (1 == fscanf(file, "%[a-z],%*[ ]", pats[n])) n++;

    char target[60 + 1];
    while (1 == fscanf(file, "%s", target)) {
        long long count = solve(target, 0);
        sum += count != 0;
        sum2 += count;
    }

    printf("%lld %lld\n", sum, sum2);
}
