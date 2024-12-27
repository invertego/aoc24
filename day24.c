#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct gate_t {
    char name[4];
    char op[4];
    char a[4], b[4];
    struct gate_t* inp[2];
    int out;
    int cache;
} gate_t;

gate_t gates[512];
int ngates;
gate_t* xyzgates[3][64];

bool panic = false;
int eval(gate_t* gate, int depth)
{
    depth++;
    if (depth > 100) {
        //puts("panic");
        panic = true;
        return 0;
    }
    //puts(gate->name);
    int value = gate->out ? gate->out : gate->cache;
    if (!value) {
        int a = eval(gate->inp[0], depth);
        int b = eval(gate->inp[1], depth);
        if (!strcmp(gate->op, "AND")) value = a & b;
        if (!strcmp(gate->op, "OR" )) value = a | b;
        if (!strcmp(gate->op, "XOR")) value = a ^ b;
        value++;
        gate->cache = value;
    }
    return value - 1;
}

int collect(int* results, char prefix)
{
    panic = false;
    for (int i = 0; i < ngates; i++) {
        gates[i].cache = 0;
    }
    int n;
    for (n = 0; n < 64; n++) {
        gate_t* gate = xyzgates[prefix - 'x'][n];
        if (!gate) break;
        results[n] = eval(gate, 0);
    }
    return n;
}

long long tonum(int* results, int n)
{
    long long num = 0;
    for (int i = 0; i < n; i++) {
        if (results[i]) num |= 1LL << i;
    }
    return num;
}

void tobin(int* bits, int n, long long value)
{
    for (int i = 0; i < n; i++) {
        bits[i] = (value >> i) & 1;
    }
}

void setnum(char prefix, int* b, int n)
{
    for (int i = 0; i < n; i++) {
        xyzgates[prefix - 'x'][i]->out = b[i] + 1;
    }
}

void swap(gate_t* a, gate_t* b)
{
    gate_t tmp = *a;
    *a = *b;
    *b = tmp;
}

int finderror(int nz, int nx, bool any)
{
    int lowest = 0;
    for (int trial = 0; trial < 100; trial++) {
        int x[64], y[64], z[64], w[64];
        for (int i = 0; i < nx; i++) {
            x[i] = rand() % 2;
            y[i] = rand() % 2;
        }
        setnum('x', x, nx);
        setnum('y', y, nx);
        collect(z, 'z');
        tobin(w, nz, tonum(x, nx) + tonum(y, nx));
        if (panic) {
            lowest = 1;
        } else {
            for (int i = 0; i < nz; i++) {
                if (z[i] != w[i]) {
                    if (!lowest || i + 1 < lowest) lowest = i + 1;
                    break;
                }
            }
        }
        if (any && lowest) break;
    }
    return lowest;
}

int cmpstr(const void* a, const void* b) { return strcmp(a, b); }

int main()
{
    FILE* file = fopen("day24.txt", "r");

    for (char line[32]; fgets(line, sizeof(line), file); ) {
        gate_t* gate = &gates[ngates];
        if (2 == sscanf(line, "%[a-z0-9]: %d", gate->name, &gate->out)) {
            strcpy(gate->op, "SET");
            gate->out++;
        } else if (4 == sscanf(line, "%s %s %s -> %s", gate->a, gate->op, gate->b, gate->name)) {
        } else continue;
        ngates++;
        if (gate->name[0] >= 'x' && gate->name[0] <= 'z') {
            xyzgates[gate->name[0] - 'x'][atoi(gate->name + 1)] = gate;
        }
    }

    for (int i = 0; i < ngates; i++) {
        if (!gates[i].a[0]) continue;
        for (int j = 0; j < ngates; j++) {
            if (!strcmp(gates[i].a, gates[j].name)) {
                gates[i].inp[0] = &gates[j];
            } else if (!strcmp(gates[i].b, gates[j].name)) {
                gates[i].inp[1] = &gates[j];
            }
        }
    }

    int x[64];
    //int y[64];
    int z[64];
    //int w[64];
    int nx = collect(x, 'x');
    //int ny = collect(y, 'y');
    int nz = collect(z, 'z');
    //tobin(w, nz, tonum(x, nx) + tonum(y, ny));

    //printf("%lld\n", tonum(x, nx));
    //printf("%lld\n", tonum(y, ny));
    printf("%lld\n", tonum(z, nz));
    //printf("%lld\n", tonum(w, nz));

    char solution[8][4];
    int nsolution = 0;

    while (true) {
        int bit = finderror(nz, nx, false);
        if (!bit) break;
        //printf("bit %d\n", bit - 1);

        gate_t* pair[2] = { 0 };
        for (int i = 0; i < ngates; i++) {
            gate_t* a = &gates[i];
            if (!a->inp[0]) continue;
            for (int j = i + 1; j < ngates; j++) {
                gate_t* b = &gates[j];
                if (!b->inp[0]) continue;
                swap(a, b);
                if (!finderror(bit, nx, true)) {
                    //printf("swap %s %s\n", a->name, b->name);
                    assert(!pair[0]);
                    pair[0] = a;
                    pair[1] = b;
                }
                swap(a, b);
            }
        }
        if (!pair[0]) {
            puts("failed");
            break;
        }
        swap(pair[0], pair[1]);
        strcpy(solution[nsolution++], pair[0]->name);
        strcpy(solution[nsolution++], pair[1]->name);
    }

    qsort(solution, nsolution, sizeof(solution[0]), cmpstr);
    for (int i = 0; i < nsolution; i++) {
        if (i > 0) printf(",");
        printf("%s", solution[i]);
    }
    printf("\n");
}
