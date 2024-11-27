#include "utility.h"


mpz_t *generation_liste_nombres_premiers(int N)
{
    mpz_t *liste = (mpz_t *)malloc(N * sizeof(mpz_t));
    mpz_init_set_ui(liste[0], 2);
    unsigned int element = 1;
    while (element < N)
    {
        mpz_init(liste[element]);
        mpz_nextprime(liste[element], liste[element - 1]);
        element++;
    }
    return liste;
}

void free_liste(int taille, mpz_t *liste)
{
     for (int i = 0; i < taille; i++)
        mpz_clear(liste[i]);
    free(liste);
}
