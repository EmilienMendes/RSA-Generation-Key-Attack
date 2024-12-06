#include "crible_optimise.h"

int verification_entier_nul(mpz_t *liste, int taille)
{
    for (unsigned int i = 0; i < taille; i++)
    {
        if (mpz_cmp_ui(liste[i], 0) == 0)
            return TRUE;
    }
    return FALSE;
}

/* @TODO Ajouter l'utilisation de la trace lors de la generation
 */
/**
 * Genere un entier avec la methode du crible optimise
 * @param k taille de l'entier souhaite
 * @param N nombre de petits premiers utilises pour le crible
 * @param t nombre de tours de Miller Rabin
 * @param p entier premier
 * @param s listes des N peits premiers
 * @param trace fichier qui contiendra la trace de l'execution du programme
 * @param parametres fichier qui contiendra les valeurs des entiers RSA (p,n)
 * @param generator generateur de nombre aleatoire
 */
void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *s, char *trace,char *parametres, gmp_randstate_t generator)
{
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
        mpz_mod(r[j], v, s[j]);
    }

    unsigned int prime = mpz_probab_prime_p(v, t);
    while (!prime)
    {
        // Verification avec les reste modulaires
        while (verification_entier_nul(r, N))
        {
            mpz_add_ui(v, v, 2);
            for (unsigned int j = 0; j < N; j++)
                mpz_add_ui(r[j], r[j], 2);
        }
        // Test Miller Rabin
        if ((prime = mpz_probab_prime_p(v, t)) == 0)
            mpz_add_ui(v, v, 2);
    }

    mpz_set(p, v);
    mpz_clear(v);
    free_liste(N, r);
}

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        printf("Usage : %s <k> <N> <t> <fichier1> <fichier2> \n", argv[0]);
        printf("k : nombre de bits de l'entier premier\nN: nombre de petits premiers utilise pour le crible\nt: nombre de tours de Miller-Rabin\n");
        printf("fichier1 : nom de fichier contenant la trace d'execution du programme\nfichier2: nom de fichier contenant les parametres n et p\n");
        return 1;
    }

    // Variable utilisateur
    int k = atoi(argv[1]);
    int N = atoi(argv[2]);
    int t = atoi(argv[3]);
    char *ptrace = argv[4];
    char *parametres = argv[4];
    // Initialisation des valeurs pour le crible
    mpz_t p;
    mpz_init(p);

    mpz_t *r;
    r = generation_liste_nombres_premiers(N);
    // Generateur pseudo aleatoires
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, time(NULL));

    generation_entier_crible_optimise(k, N, t, p, r, ptrace, parametres, generator);
    
    mpz_clear(p);
    return 0;
}