#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

bool edges[1 << 10][1 << 10];
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

void edge(int a, int b)
{
    edges[a][b] = true;
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

void solve(int* verts, int nverts, int* set, int nset, int next, int idx, bool tpre)
{
    assert(nset < COUNT(maxset));
    set[nset++] = next;
    if (prefix(next, 't')) tpre = true;
    if (tpre && nset == 3) tcount++;
    if (nset > nmaxset) {
        memcpy(maxset, set, nset * sizeof(set[0]));
        nmaxset = nset;
    }
    for (int i = idx; i < nverts; i++) {
        int a = verts[i];
        if (connected(a, set, nset)) {
            solve(verts, nverts, set, nset, a, i + 1, tpre);
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

    for (int a = 0; a < COUNT(edges); a++) {
        int verts[16];
        int nverts = 0;
        for (int b = a + 1; b < COUNT(edges[a]); b++) {
            if (edges[a][b]) {
                verts[nverts++] = b;
            }
        }
        int set[16];
        solve(verts, nverts, set, 0, a, 0, false);
    }

    printf("%d\n", tcount);

    for (int i = 0; i < nmaxset; i++) {
        if (i > 0) printf(",");
        char buf[3] = { 0 };
        printf("%s", decode(maxset[i], buf));
    }
}
