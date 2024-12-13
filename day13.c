#include <stdio.h>

long long solve(long long (*game)[2])
{
    long long a = game[0][0], b = game[1][0], x = game[2][0];
    long long c = game[0][1], d = game[1][1], y = game[2][1];
    long long z = d * x - b * y;
    long long w = a * y - c * x;
    long long det = a * d - b * c;
    if (z % det || w % det) return 0;
    return (z * 3 + w) / det;
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
        sum2 += solve(game);
    }

    printf("%lld %lld", sum, sum2);
}
