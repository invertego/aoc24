#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

enum {
    adv,
    bxl,
    bst,
    jnz,
    bxc,
    out,
    bdv,
    cdv,
};

#define A reg[0]
#define B reg[1]
#define C reg[2]

typedef long long reg_t;

reg_t reg[3];
int prg[16];
int n;

reg_t combo(int o)
{
    if (o < 4) return o;
    if (o < 7) return reg[o - 4];
    assert(false); return 0;
}

int run(int* output, int limit)
{
    int noutput = 0;
    for (int ip = 0, ipn; ip >= 0 && ip < n && noutput < limit; ip = ipn) {
        assert(ip % 2 == 0);
        int opc = prg[ip], o = prg[ip + 1];
        ipn = ip + 2;
        switch (opc) {
        case adv: A = A >> combo(o); break;
        case bxl: B = B ^ o; break;
        case bst: B = combo(o) % 8; break;
        case jnz: if (A) { ipn = o; } break;
        case bxc: B = B ^ C; break;
        case out: output[noutput++] = combo(o) % 8; break;
        case bdv: B = A >> combo(o); break;
        case cdv: C = A >> combo(o); break;
        }
    }
    return noutput;
}

reg_t solve(reg_t accum, int pos)
{
    if (pos < 0) return accum;
    for (int low = 0; low < 8; low++) {
        reg_t a = (accum << 3) | low;
        A = a;
        int x;
        if (run(&x, 1) && x == prg[pos]) {
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
    int noutput = run(output, COUNT(output));
    assert(noutput < COUNT(output));
    for (int i = 0; i < noutput; i++) {
        if (i > 0) printf(",");
        printf("%d", output[i]);
    }
    printf("\n");

    printf("%lld\n", solve(0, n - 1));
}
