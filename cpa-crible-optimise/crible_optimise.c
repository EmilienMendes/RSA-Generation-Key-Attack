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

/**
 * @param nombre entier dont on souhaite determiner son poids de Hamming
 * @return nombre de 1 dans le nombre
 */
unsigned int poids_hamming(unsigned int nombre)
{
    unsigned int taille_entier = sizeof(unsigned int) * 8;
    unsigned int poids_hamming = 0;
    for (int i = 0; i < taille_entier; i++)
    {
        poids_hamming += (nombre & 1);
        nombre = nombre >> 1;
    }
    return poids_hamming;
}

/* @TODO Ajouter l'utilisation de la trace lors de la generation
 */
/**
 * Genere un entier avec la methode du crible optimise
 * @param k taille de l'entier souhaite
 * @param N nombre de petits premiers utilises pour le crible
 * @param t nombre de tours de Miller Rabin
 * @param p entier premier
 * @param s listes des N petits premiers
 * @param trace fichier qui contiendra la trace de l'execution du programme (laisser NULL pour ne pas stocker la trace)
 * @param generator generateur de nombre aleatoire
 */
void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, unsigned int *s, char *trace, gmp_randstate_t generator)
{
    FILE *fptr;
    if( trace != NULL)
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
                if( trace != NULL)
                    fprintf(fptr, "%d\n", poids_hamming(mpz_get_ui(r[j])));
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
                if(trace != NULL)
                    fprintf(fptr, "%d\n", poids_hamming(mpz_get_ui(r[j])));
                mpz_add_ui(r[j], r[j], 2);
                mpz_mod_ui(r[j], r[j], s[j]);
            }
        }
    }
    mpz_set(p, v);
    mpz_clear(v);
   
    free_liste(N, r);
    if(trace != NULL)
        fclose(fptr);
}

void ecriture_parametre(char *fichier, mpz_t n)
{
    FILE *fptr = fopen(fichier, "w");
    gmp_fprintf(fptr, "n = %Zd\n", n);
    fclose(fptr);
}

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        printf("Usage : %s <k> <N> <t> <fichier1> <fichier2> \n", argv[0]);
        printf("k : nombre de bits de l'entier premier\nN: nombre de petits premiers utilise pour le crible\nt: nombre de tours de Miller-Rabin\n");
        printf("fichier1 : nom de fichier contenant la trace d'execution du programme\nfichier2: nom de fichier contenant n\n");
        return 1;
    }

    // Variable utilisateur
    unsigned int k = atoi(argv[1]);
    unsigned int N = atoi(argv[2]);
    unsigned int t = atoi(argv[3]);
    char *ptrace = argv[4];
    char *parametre = argv[5];
    // Initialisation des valeurs pour le crible
    mpz_t p, q, n;
    mpz_inits(p, q, n, NULL);

    unsigned int *r;
    r = generation_liste_nombres_premiers(N);
    // Generateur pseudo aleatoires
    gmp_randstate_t generator;
    long int seed = time(NULL);
    gmp_randinit_default(generator);
    // printf("Seed : %ld\n",seed);
    gmp_randseed_ui(generator, seed);

    generation_entier_crible_optimise(k, N, t, p, r, ptrace, generator);
    generation_entier_crible_optimise(k, N, t, q, r, NULL, generator);
    ecriture_parametre(parametre, n);
    mpz_clears(p, q, n, NULL);

    return 0;
}