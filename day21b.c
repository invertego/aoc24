#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum {
    X = -1,
    R = 0,
    D = 1,
    L = 2,
    U = 3,
    A = 10,

    N = 3,
};

const int dx[] = { 1, 0,-1, 0 };
const int dy[] = { 0, 1, 0,-1 };

typedef struct {
    int pos[N][2];
    int num;
} state_t;

bool dirvalid(int x, int y)
{
    return (y == 0 && x >= 1 && x <= 2)
        || (y == 1 && x >= 0 && x <= 2);
}

bool numvalid(int x, int y)
{
    return (y >= 0 && y <= 2 && x >= 0 && x <= 2)
        || (y == 3 && x >= 1 && x <= 2);
}

const int dirpad[2][3] = {
    { X, U, A },
    { L, D, R },
};

const int numpad[4][3] = {
    { 7, 8, 9 },
    { 4, 5, 6 },
    { 1, 2, 3 },
    { X, 0, A },
};

bool press(state_t s, int level, int d, const int* code, state_t* tout)
{
    if (level == N) {
        assert((d >= 0 && d <= 9) || d == A);
        int target = code[s.num];
        if (d != target) return false;
        state_t t = s;
        t.num += 1;
        *tout = t;
        return true;
    }

    if (d == A) {
        int b;
        int* pos = s.pos[level];
        if (level < N - 1) {
            b = dirpad[pos[1]][pos[0]];
        } else {
            assert (level == N - 1);
            b = numpad[pos[1]][pos[0]];
        }
        return press(s, level + 1, b, code, tout);
    } else {
        assert(d >= 0 && d <= 3);
        int* pos = s.pos[level];
        int x = pos[0], y = pos[1];
        x += dx[d];
        y += dy[d];
        if (level < N - 1 && !dirvalid(x, y)) return false;
        if (level == N - 1 && !numvalid(x, y)) return false;
        state_t t = s;
        pos = t.pos[level];
        pos[0] = x;
        pos[1] = y;
        *tout = t;
        return true;
    }
}

void printnum(int rx, int ry)
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            if (x == rx && y == ry) printf("* ");
            else if (numpad[y][x] == X) printf("  ");
            else if (numpad[y][x] == A) printf("A ");
            else printf("%d ", numpad[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

void printdir(int rx, int ry)
{
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            if (x == rx && y == ry) printf("* ");
            else if (dirpad[y][x] == X) printf("  ");
            else if (dirpad[y][x] == A) printf("A ");
            else printf("%d ", dirpad[y][x]);
        }
        printf("\n");
    }
    printf("\n");
}

void print(state_t s)
{
    printnum(s.pos[N - 1][0], s.pos[N - 1][1]);
    for (int i = N - 2; i >= 0; i--) {
        printdir(s.pos[i][0], s.pos[i][1]);
    }
}

int main()
{
    FILE* file = fopen("day21.txt", "r");

    for (int x; 1 == fscanf(file, "%dA", &x); ) {
        printf("%03dA\n\n", x);
        int code[4] = {
            (x / 100) % 10,
            (x /  10) % 10,
            (x /   1) % 10,
            A,
        };

        state_t s = { 0 };
        for (int i = 0; i < N - 1; i++) {
            s.pos[i][0] = 2;
            s.pos[i][1] = 0;
        }
        s.pos[N - 1][0] = 2;
        s.pos[N - 1][1] = 3;
        s.num = 0;

        while (s.num != 4) {
            print(s);
            state_t t;
            int npress = 0;
            while (!npress) {
                printf("%X> ", code[s.num]);
                char buf[256];
                fgets(buf, sizeof(buf), stdin);
                for (char* b = buf; *b; b++) {
                    int d;
                    switch (*b) {
                        case '^':
                        case 'u': d = U; break;
                        case 'v':
                        case 'd': d = D; break;
                        case '<':
                        case 'l': d = L; break;
                        case '>':
                        case 'r': d = R; break;
                        case 'A':
                        case 'a': d = A; break;
                        case 'q': goto quit;
                        case ' ':
                        case '\n': continue;
                        default:
                            printf("invalid char %c\n", *b);
                            continue;
                    }
                    if (!press(s, 0, d, code, &t)) {
                        printf("invalid press %d\n", npress + 1);
                        break;
                    }
                    s = t;
                    npress++;
                }
                printf("\n");
            }
        }
        printf("done\n");
    quit:
        printf("\n");
    }
}
