#include "gauss.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"
void test_gauss(double sig)
{
    int nb_exec = 10000;
    float *x = (float *)malloc(sizeof(float) * nb_exec);

    for (int i = 0; i < nb_exec; i++)
        x[i] = gauss(sig);

    float mu = 0;

    for (int i = 0; i < nb_exec; i++)
        mu += x[i];
    mu /= nb_exec;

    float sigma = 0;
    for (int i = 0; i < nb_exec; i++)
        sigma += pow((x[i] - mu), 2);

    printf("esperance = %f\n", mu);
    printf("ecart type = %f\n", sqrt(sigma / nb_exec));

    free(x);
}

int main()
{
    srand(time(NULL));
    test_gauss(1);
}
