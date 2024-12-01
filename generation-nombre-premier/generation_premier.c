#include "generation_premier.h"

/**
 * Generations des premiers nombres premier avec nextprime
 * @param N quantite de nombres premiers a generer
 * @return la liste ordonne des N plus petits nombres premiers
 */
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

/**
 * Suppression de tous les N mpz_t entiers premiers de la liste
 * @param taille taille de la liste
 * @param liste liste a supprimer
 */
void free_liste(int taille, mpz_t *liste)
{
    for (int i = 0; i < taille; i++)
        mpz_clear(liste[i]);
    free(liste);
}
