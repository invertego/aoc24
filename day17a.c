#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

typedef long long reg_t;

reg_t reg[3];
int prg[16];
int n;

int run(int* output)
{
    int noutput = 0;
    reg_t a = reg[0];
    do {
        int b = (a & 7) ^ 3;
        output[noutput++] = ((b ^ 5) ^ (a >> b)) & 7;
        a = a >> 3;
    } while (a);
    return noutput;
}

reg_t solve(reg_t accum, int pos)
{
    if (pos < 0) return accum;
    for (int low = 0; low < 8; low++) {
        reg_t a = (accum << 3) | low;
        int b = low ^ 3;
        int x = ((b ^ 5) ^ (a >> b)) & 7;
        if (x == prg[pos]) {
            reg_t result = solve(a, pos - 1);
            if (result >= 0) return result;
        }
    }
    return -1;
}

int main()
{
    FILE* file = fopen("day17.txt", "r");

    char r;
    for (int x; 2 == fscanf(file, "Register %c: %d\n", &r, &x); ) {
        //printf("Register %c: %d\n", r, x);
        reg[r - 'A'] = x;
    }

    fscanf(file, "Program: ");

    for (int x, y; 2 == fscanf(file, "%d,%d,", &x, &y); ) {
        //printf("%d,%d,\n", x, y);
        prg[n++] = x;
        prg[n++] = y;
    }

    int output[16];
    int noutput = run(output);
    assert(noutput <= COUNT(output));
    for (int i = 0; i < noutput; i++) {
        if (i > 0) printf(",");
        printf("%d", output[i]);
    }
    printf("\n");

    printf("%lld\n", solve(0, n - 1));
}
