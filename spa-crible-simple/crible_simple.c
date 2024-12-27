#include "crible_simple.h"

/**
 * Ecriture des parametres RSA dans un fichier
 * @param p entier de la cle privee
 * @param q entier de la cle privee
 * @param fichier fichier d'ecriture cle publique / cle privee
 */
void ecriture_parametres(mpz_t p, mpz_t q, mpz_t n, char *fichier)
{
    FILE *fptr = fopen(fichier, "w");
    gmp_fprintf(fptr, "n = %Zd\n", n);
    gmp_fprintf(fptr, "p = %Zd\n", p);
    gmp_fprintf(fptr, "q = %Zd\n", q);
    fclose(fptr);
}

/**
 * Generation d'un entier premier par la methode du crible simple
 * @param k Taille en bit du nombre voulu
 * @param N Nombre d'entiers dans le crible simple
 * @param t Nombre de tours de Miller-Rabin
 * @param p entier premier genere
 * @param r liste des premiers nombres premiers par lequels on va tester la division
 * @param generator generateur de valeur aleatoire
 * @param fichier fichier ou on stocke la trace simule par l'attaque spa
 */

void generation_entier_crible_simple(unsigned int k, unsigned int N, unsigned int t, mpz_t p, unsigned int *r, gmp_randstate_t generator, char *fichier)
{
    FILE *fptr = fopen(fichier, "w");
    mpz_t v;
    mpz_init(v);

    unsigned int i, divisible;

    /*
    Creation d'un entier v d'exactement k bits
    Le dernier bit vaut 1 pour avoir un entier impair
    */
    mpz_urandomb(v, generator, k);
    mpz_setbit(v, k - 1);
    mpz_setbit(v, 0);
    if (mpz_probab_prime_p(v, t) == 0)
    {
        while (TRUE)
        {
            i = 1;
            fprintf(fptr, "%d\n", LIGNE1);
            // Verification de la non divisiblite du nombre par les N petits premiers
            while (i < N)
            {
                fprintf(fptr, "%d\n", LIGNE2);

                divisible = mpz_divisible_ui_p(v, r[i]);
                fprintf(fptr, "%d\n", LIGNE3);
                if (divisible != 0)
                {
                    fprintf(fptr, "%d\n", LIGNE4);
                    mpz_add_ui(v, v, 2);
                    break;
                }
                fprintf(fptr, "%d\n", LIGNE5);

                i++;
            }
            /*
            Verification si le nombre a ete divise l'un des N premiers.
            Si c'est verifie on repasse au debut de la boucle avec un nouvel entier premier
            Sinon, on fais le test de miller rabin, et on s'arrete si la condition est verifie
            */

            if (!divisible)
            {
                fprintf(fptr, "%d\n", LIGNE6);

                if (mpz_probab_prime_p(v, t))
                    break;
                else
                {
                    fprintf(fptr, "%d\n", LIGNE7);
                    mpz_add_ui(v, v, 2);
                }
            }
        }
    }
    mpz_set(p, v);
    mpz_clear(v);
    fprintf(fptr, "%d\n", LIGNE1);
    fclose(fptr);
}

