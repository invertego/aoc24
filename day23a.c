#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

typedef struct {
    int edges[16];
    int nedges;
} vert_t;

vert_t verts[1 << 10];
bool edges[COUNT(verts)][COUNT(verts)];
int tcount = 0;
int maxset[16];
int nmaxset;

int encode(const char* s)
{
    return ((s[0] - 'a') << 5) | (s[1] - 'a');
}

char* decode(int a, char* s)
{
    s[0] = (a >> 5) + 'a';
    s[1] = (a & 0x1f) + 'a';
    return s;
}

bool prefix(int a, char c)
{
    char s[2];
    return decode(a, s)[0] == c;
}

int cmpint(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

void edge(int a, int b)
{
    edges[a][b] = true;
    vert_t* v = &verts[a];
    assert(v->nedges < COUNT(v->edges));
    v->edges[v->nedges++] = b;
}

bool connected(int a, int* set, int nset)
{
    for (int i = 0; i < nset; i++) {
        int b = set[i];
        if (!edges[a][b]) {
            return false;
        }
    }
    return true;
}

void solve(int root, int* set, int nset, int next, int idx, bool tpre)
{
    assert(nset < COUNT(maxset));
    set[nset++] = next;
    if (prefix(next, 't')) tpre = true;
    if (tpre && nset == 3) tcount++;
    if (nset > nmaxset) {
        memcpy(maxset, set, nset * sizeof(set[0]));
        nmaxset = nset;
    }
    vert_t* v = &verts[root];
    for (int i = idx; i < v->nedges; i++) {
        int a = v->edges[i];
        if (connected(a, set, nset)) {
            solve(root, set, nset, a, i + 1, tpre);
        }
    }
}

int main()
{
    FILE* file = fopen("day23.txt", "r");
    for (char as[3], bs[3]; 2 == fscanf(file, "%[a-z]-%[a-z]\n", as, bs); ) {
        int a = encode(as), b = encode(bs);
        edge(a, b);
        edge(b, a);
    }

    for (int a = 0; a < COUNT(verts); a++) {
        vert_t* v = &verts[a];
        int idx = v->nedges;
        qsort(v->edges, v->nedges, sizeof(v->edges[0]), cmpint);
        for (int ib = 0; ib < v->nedges; ib++) {
            int b = v->edges[ib];
            if (b > a) {
                idx = ib;
                break;
            }
        }
        int set[16];
        solve(a, set, 0, a, idx, false);
    }

    printf("%d\n", tcount);

    for (int i = 0; i < nmaxset; i++) {
        if (i > 0) printf(",");
        char buf[3] = { 0 };
        printf("%s", decode(maxset[i], buf));
    }
}
