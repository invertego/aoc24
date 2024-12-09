#include <stdio.h>
#include <string.h>

char buf[20000 + 1];
int map[200000];
int n;

void printmap()
{
    for (int i = 0; i < n; i++) {
        if (map[i] >= 0) {
            printf("%d", map[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int main()
{
    FILE* file = fopen("day9.txt", "r");
    long long sum = 0, sum2 = 0;

    fscanf(file, "%s", buf);
    int buflen = strlen(buf);

    int nextid = 0;
    for (int i = 0; i < buflen; i++) {
        int l = buf[i] - '0';
        int id;
        if (i & 1) id = -1;
        else id = nextid++;
        for (int j = 0; j < l; j++) {
            map[n++] = id;
        }
    }

    //printmap();

    for (int i = 0, j = n - 1; i <= j; i++) {
        if (map[i] >= 0) {
            sum += i * map[i];
        } else {
            while (j > i && map[j] < 0) {
                j--;
            }
            if (j <= i) break;
            sum += i * map[j];
            j--;
        }
    }

    for (int sbeg = n - 1; sbeg >= 0; ) {
        while (sbeg >= 0 && map[sbeg] < 0) {
            sbeg--;
        }
        if (sbeg < 0) break;
        int id = map[sbeg];
        int send = sbeg - 1;
        while (send >= 0 && map[send] == id) {
            send--;
        }
        int slen = sbeg - send;
        int dbeg = 0, dlen = 0;
        while (dbeg <= send) {
            while (dbeg <= send && map[dbeg] >= 0) {
                dbeg++;
            }
            if (dbeg > send) break;
            int dend = dbeg + 1;
            while (dend < n && map[dend] < 0) {
                dend++;
            }
            dlen = dend - dbeg;
            if (dlen >= slen) break;
            dbeg = dend;
            dlen = 0;
        }
        if (dlen >= slen) {
            for (int i = 0; i < slen; i++) {
                map[dbeg + i] = id;
                map[send + 1 + i] = -1;
            }
        }
        sbeg = send;
    }

    for (int i = 0; i < n; i++) {
        if (map[i] >= 0) {
            sum2 += i * map[i];
        }
    }

    //printmap();

    printf("%lld %lld", sum, sum2);
}
