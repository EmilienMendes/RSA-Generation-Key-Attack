#include "generation_premier.h"

/**
 * Generations des premiers nombres premier avec nextprime
 * @param N quantite de nombres premiers a generer
 * @return la liste ordonne des N plus petits nombres premiers
 */
unsigned int *generation_liste_nombres_premiers(unsigned int N)
{
    unsigned int *liste = (unsigned int *)malloc(N * sizeof(unsigned int));
    mpz_t premier;
    mpz_init_set_ui(premier, 2);
    for (unsigned int i = 0; i < N; i++)
    {
        liste[i] = mpz_get_ui(premier);
        mpz_nextprime(premier, premier);
    }
    return liste;
}

/**
 * Suppression de tous les N mpz_t entiers premiers de la liste
 * @param taille taille de la liste
 * @param liste liste a supprimer
 */
void free_liste(unsigned int taille, mpz_t *liste)
{
    for (unsigned int i = 0; i < taille; i++)
        mpz_clear(liste[i]);
    free(liste);
}