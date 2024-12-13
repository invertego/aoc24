#include <stdio.h>
#include <stdlib.h>

int solve(long long (*game)[2])
{
    int min = 0, mini, minj;
    for (int i = 0; i <= 100; i++)
    for (int j = 0; j <= 100; j++) {
        int x = game[0][0] * i + game[1][0] * j;
        int y = game[0][1] * i + game[1][1] * j;
        if (x == game[2][0] && y == game[2][1]) {
            int s = 3 * i + j;
            if (!min || s < min) {
                min = s;
                mini = i;
                minj = j;
            }
        }
    }
    if (!min) return 0;
    return 3 * mini + minj;
}

long long solve2(long long (*game)[2])
{
    long long result = 0;

    FILE* in = fopen("input.txt", "w");
    fprintf(in, "(declare-const x Int)\n");
    fprintf(in, "(declare-const y Int)\n");
    fprintf(in, "(assert (= (+ (* x %lld) (* y %lld)) %lld))\n", game[0][0], game[1][0], game[2][0]);
    fprintf(in, "(assert (= (+ (* x %lld) (* y %lld)) %lld))\n", game[0][1], game[1][1], game[2][1]);
    fprintf(in, "(minimize (+ (* x 3) y))\n");
    fprintf(in, "(check-sat)\n");
    fprintf(in, "(get-objectives)\n");
    fclose(in);

    system("z3 input.txt > output.txt");

    FILE* out = fopen("output.txt", "r");
    for (char line[256]; fgets(line, sizeof(line), out);) {
        if (1 == sscanf(line, " ((+ (* x 3) y) %lld)", &result)) {
            break;
        }
    }
    fclose(out);

    return result;
}

int main()
{
    FILE* file = fopen("day13.txt", "r");
    long long sum = 0, sum2 = 0;
    long long game[3][2];

    while (2 == fscanf(file, "Button A: X+%lld, Y+%lld\n", &game[0][0], &game[0][1])) {
        fscanf(file, "Button B: X+%lld, Y+%lld\n", &game[1][0], &game[1][1]);
        fscanf(file, "Prize: X=%lld, Y=%lld\n", &game[2][0], &game[2][1]);

        //for (int i = 0; i < 3; i++)
        //for (int j = 0; j < 2; j++)
            //printf("%lld ", game[i][j]);
        //printf("\n");

        sum += solve(game);
        game[2][0] += 10000000000000;
        game[2][1] += 10000000000000;
        sum2 += solve2(game);
    }

    printf("%lld %lld", sum, sum2);
}
