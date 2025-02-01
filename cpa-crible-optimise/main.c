#include "crible_optimise.h"
#include "attaque_cpa.h"

/**
 * Generation des entiers premiers pour la trace cpa
 * @param k taille de l'entier genere
 * @param lambda nombre de petits premiers
 * @param s listes des petits premiers
 * @param bruit ecart type de bruit lors de l'acquisition de la trace
 * @param fichier_trace fichier qui stockera la trace acquise
 * @param generator generateur de valeur aleatoire
 */

void acquisition_trace_cpa(unsigned int k, unsigned int lambda, unsigned int t, unsigned int *s, float bruit, char *fichier_trace, char *fichier_parametre, gmp_randstate_t generator)
{
    mpz_t p, q, n;
    mpz_inits(p, q, n, NULL);
    generation_entier_crible_optimise(k, lambda, t, p, s, bruit, fichier_trace, generator);
    generation_entier_crible_optimise(k, lambda, t, q, s, bruit, NULL, generator);
    mpz_mul(n, p, q);
    ecriture_parametre(fichier_parametre, n, p);
    mpz_clears(p, q, n, NULL);
}

/**
 * Attaque CPA 
 * @param k taille de l'entier genere
 * @param lambda nombre de petits premiers
 * @param t tour de Miller Rabin
 * @param s listes des petits premiers
 * @param bruit ecart type de bruit lors de l'acquisition de la trace
 * @param fichier_trace fichier qui stockera la trace acquise
 * @param generator generateur de valeur aleatoire
 */
unsigned int exemple_attaque_cpa_horizontal(unsigned int k, unsigned int lambda, unsigned int t, unsigned int *s, float bruit, char *fichier_trace, char *fichier_parametre, gmp_randstate_t generator)
{
    acquisition_trace_cpa(k, lambda, t, s, bruit, fichier_trace, fichier_parametre, generator);
    int succes_attaque = attaque_cpa(lambda, s, fichier_trace, fichier_parametre);
    return succes_attaque;
}

int main(int argc, char **argv)
{

    if (argc != 8)
    {
        printf("Usage : %s <k> <N> <t> <b> <e> <fichier1> <fichier2> \n", argv[0]);
        printf("k : nombre de bits de l'entier premier\nN: nombre de petits premiers utilise pour le crible\nt: nombre de tours de Miller-Rabin\nb : bruit lors de l'acquisition de la trace\ne : nombre d'execution du programme\n");
        printf("fichier1 : nom de fichier contenant la trace d'execution du programme\nfichier2: nom de fichier contenant n\n");
        return 1;
    }
    // Generateurs aleatoires
    long int seed = time(NULL);
    srand(seed);
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, seed);
    
    // Initialisation des parametres
    unsigned int k = atoi(argv[1]);
    unsigned int lambda = atoi(argv[2]);
    unsigned int t = atoi(argv[3]);
    unsigned int bruit = atoi(argv[4]);
    unsigned int nb_attaque = atoi(argv[5]);
    char *fichier_trace = argv[6];
    char *fichier_cle = argv[7];

    // Generation des petits premiers
    unsigned int *s = generation_liste_nombres_premiers(lambda);

    // Attaque CPA
    unsigned int nb_attaque_reussi = 0;
    for (unsigned int i = 0; i < nb_attaque; i++)
        nb_attaque_reussi+=exemple_attaque_cpa_horizontal(k, lambda, t, s, bruit, fichier_trace, fichier_cle, generator);

    float pourcentage_reussite = ((float)nb_attaque_reussi / (float)nb_attaque) * 100.0f;
    printf("Succes de l'attaque avec un bruit de %d : %.1f %% \n", bruit, pourcentage_reussite);

    return 0;
}