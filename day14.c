#include <limits.h>
#include <stdio.h>
#include <string.h>

#define N 500
int pos[N][2];
int vel[N][2];
int n;

char map[103][101];
//int w = 11, h = 7;
int w = 101, h = 103;

int main()
{
    FILE* file = fopen("day14.txt", "r");
    int sum = 0, sum2 = 0;

    for (int p[2], v[2]; 4 == fscanf(file, "p=%d,%d v=%d,%d\n", &p[0], &p[1], &v[0], &v[1]); ) {
        memcpy(pos[n], p, sizeof(p));
        memcpy(vel[n], v, sizeof(v));
        n++;
    }

    FILE* log = NULL;
    //log = fopen("day14.log", "w");
    int min = INT_MAX;
    for (int t = 1; t <= 10000; t++) {
        for (int i = 0; i < n; i++) {
            pos[i][0] += vel[i][0];
            pos[i][1] += vel[i][1];
            pos[i][0] = (pos[i][0] + w) % w;
            pos[i][1] = (pos[i][1] + h) % h;
        }

        if (log) {
            memset(map, '.', sizeof(map));
            for (int i = 0; i < n; i++) {
                map[pos[i][1]][pos[i][0]] = 'X';
            }

            fprintf(log, "%d\n", t);
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    fprintf(log, "%c", map[y][x]);
                }
                fprintf(log, "\n");
            }
            fprintf(log, "\n\n");
        }

        int q[4] = { 0 };
        for (int i = 0; i < n; i++) {
            if (pos[i][0] < w / 2) {
                if (pos[i][1] < h / 2) q[0]++;
                if (pos[i][1] > h / 2) q[1]++;
            }
            if (pos[i][0] > w / 2) {
                if (pos[i][1] < h / 2) q[2]++;
                if (pos[i][1] > h / 2) q[3]++;
            }
        }
        //printf("%d %d %d %d\n", q[0], q[1], q[2], q[3]);
        int prod = q[0] * q[1] * q[2] * q[3];
        if (t == 100) sum = prod;
        if (prod < min) {
            min = prod;
            sum2 = t;
        }
    }
    if (log) fclose(log);

    printf("%d %d\n", sum, sum2);
}
