#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define A(i, j) a[i][j]
char a[64][128], b[64][128];
int w, h;

char path[1 << 15];

const char dstr[] = "^v<>";
const int di[] = {-1, 1, 0, 0 };
const int dj[] = { 0, 0,-1, 1 };

bool push(int i, int j, int d, bool apply)
{
    char c = A(i, j), ac;
    if (c == '.') return true;
    if (c == '#') return false;

    int adj = 0;
    if (c != 'O' && d < 2) {
        if (c == '[') adj = 1;
        if (c == ']') adj = -1;
        ac = A(i, j + adj);
    }

    int ni = i + di[d], nj = j + dj[d];
    if (!push(ni, nj, d, apply)) return false;
    if (adj && !push(ni, nj + adj, d, apply)) return false;
    if (apply) {
        A(i, j) = '.';
        A(ni, nj) = c;
        if (adj) {
            A(i, j + adj) = '.';
            A(ni, nj + adj) = ac;
        }
    }
    return true;
}

int run()
{
    memcpy(b, a, sizeof(a));

    int si, sj;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (A(i, j) == '@') {
                A(i, j) = '.';
                si = i, sj = j;
                break;
            }
        }
    }

    int ci = si, cj = sj;
    int npath = strlen(path);
    for (int p = 0; p < npath; p++) {
        int d = strchr(dstr, path[p]) - dstr;
        int ni = ci + di[d];
        int nj = cj + dj[d];
        char nc = A(ni, nj);
        if (nc == '#') {
        } else if (nc == '.') {
            ci = ni, cj = nj;
        } else if (nc == 'O' || nc == '[' || nc == ']') {
            if (push(ni, nj, d, false)) {
                push(ni, nj, d, true);
                ci = ni, cj = nj;
            }
        } else assert(false);
    }

    int sum = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (A(i, j) == 'O' || A(i, j) == '[') sum += 100 * i + j;
            //printf("%c", (i == ci && j == cj) ? '@' : A(i, j));
        }
        //printf("\n");
    }

    memcpy(a, b, sizeof(a));

    return sum;
}

void widen()
{
    for (int i = 0; i < h; i++) {
        for (int j = w - 1; j >= 0; j--) {
            const char* wide = "";
            char c = A(i, j);
            if (c == '@') {
                wide = "@.";
            } else if (c == '.') {
                wide = "..";
            } else if (c == '#') {
                wide = "##";
            } else if (c == 'O') {
                wide = "[]";
            } else assert(false);
            A(i, j * 2 + 0) = wide[0];
            A(i, j * 2 + 1) = wide[1];
        }
    }
    w *= 2;
}

int main()
{
    FILE* file = fopen("day15.txt", "r");
    int sum = 0, sum2 = 0;

    h = 0;
    while (1 == fscanf(file, "%[#.O@]\n", &A(h, 0))) h++;
    w = h;

    while (1 == fscanf(file, "%[<>^v]\n", path + strlen(path))) ;

    sum = run();
    widen();
    sum2 = run();

    printf("%d %d\n", sum, sum2);
}
