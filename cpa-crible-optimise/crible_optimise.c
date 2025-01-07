#include "crible_optimise.h"

unsigned int verification_entier_nul(mpz_t *liste, unsigned int taille)
{
    for (unsigned int i = 0; i < taille; i++)
    {
        if (mpz_cmp_ui(liste[i], 0) == 0)
            return TRUE;
    }
    return FALSE;
}

// TODO Verifier que l'entier est bien de k bits et l'ouverture reglementaire des fichiers

/**
 * Genere un entier avec la methode du crible optimise
 * @param k taille de l'entier souhaite
 * @param N nombre de petits premiers utilises pour le crible
 * @param t nombre de tours de Miller Rabin
 * @param p entier premier
 * @param s listes des N petits premiers
 * @param sigma ecart type du bruit lors de l'acquisition de trace
 * @param trace fichier qui contiendra la trace de l'execution du programme (NULL pour ne pas stocker la trace)
 * @param generator generateur de nombre aleatoire
 */
void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, unsigned int *s, int sigma, char *trace, gmp_randstate_t generator)
{
    FILE *fptr;
    if (trace != NULL)
        fptr = fopen(trace, "w");

    mpz_t v;
    mpz_init(v);
    /*
    Creation d'un entier v d'exactement k bits
    Le dernier bit vaut 1 pour avoir un entier impair
    */
    mpz_urandomb(v, generator, k);
    mpz_setbit(v, k - 1);
    mpz_setbit(v, 0);
    mpz_t *r = (mpz_t *)malloc(N * sizeof(mpz_t));
    // R[j] <- v0 mod sj
    for (unsigned int j = 0; j < N; j++)
    {
        mpz_init(r[j]);
        mpz_mod_ui(r[j], v, s[j]);
    }
    unsigned int prime = mpz_probab_prime_p(v, t);
    while (!prime)
    {
        // Verification avec les reste modulaires
        while (verification_entier_nul(r, N))
        {
            mpz_add_ui(v, v, 2);
            for (unsigned int j = 0; j < N; j++)
            {
                if (trace != NULL)
                {
                    // int bruit = (int) gauss(sigma);
                    fprintf(fptr, "%d\n", poids_hamming(mpz_get_ui(r[j])));
                }
                mpz_add_ui(r[j], r[j], 2);
                mpz_mod_ui(r[j], r[j], s[j]);
            }
        }
        // Test Miller Rabin
        if ((prime = mpz_probab_prime_p(v, t)) == 0)
        {
            mpz_add_ui(v, v, 2);
            for (unsigned int j = 0; j < N; j++)
            {
                if (trace != NULL)
                {
                    // int bruit = (int) gauss(sigma);
                    fprintf(fptr, "%d\n", poids_hamming(mpz_get_ui(r[j])));
                }

                mpz_add_ui(r[j], r[j], 2);
                mpz_mod_ui(r[j], r[j], s[j]);
            }
        }
    }
    /*
    if (trace != NULL)
    {
        printf("\nReste reel :\n");
        mpz_t tmp;
        mpz_init(tmp);
        for (int i = 0; i < N; i++)
        {
            mpz_mod_ui(tmp, v, s[i]);
            gmp_printf("%Zd ", tmp);
        }

        printf("\n");
        mpz_clear(tmp);
    }
    // */
    mpz_set(p, v);
    mpz_clear(v);
    free_mpz_liste(N, r);
    if (trace != NULL)

        fclose(fptr);
}

void ecriture_parametre(char *fichier, mpz_t n, mpz_t p)
{
    FILE *fptr = fopen(fichier, "w");
    gmp_fprintf(fptr, "n = %Zd\n", n);
    gmp_fprintf(fptr, "p = %Zd\n", p);
    fclose(fptr);
}
