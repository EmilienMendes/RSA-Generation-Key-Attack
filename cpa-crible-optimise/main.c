#include "crible_optimise.h"
#include "attaque_cpa.h"

void afficher_liste(float *s, unsigned int N)
{
    printf("s = [");
    for (int i = 0; i < N - 1; i++)
        printf("%f,", s[i]);
    printf("%f]\n", s[N - 1]);
}

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

unsigned int exemple_attaque_cpa_horizontal(unsigned int k, unsigned int lambda, unsigned int t, unsigned int *s, float bruit, char *fichier_trace, char *fichier_parametre, gmp_randstate_t generator)
{

    acquisition_trace_cpa(k, lambda, t, s, bruit, fichier_trace, fichier_parametre, generator);
    unsigned int succes_attaque = attaque_cpa(lambda, s, fichier_trace, fichier_parametre);
    // unsigned int succes_attaque2 = attaque_cpa2(lambda, s, fichier_trace, fichier_parametre);
    return succes_attaque;
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
    // Generateurs aleatoires
    long int seed = time(NULL);
    srand(seed);
    // printf("Seed %ld\n", seed);
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, seed);

    unsigned int k = atoi(argv[1]);
    unsigned int lambda = atoi(argv[2]);
    unsigned int t = atoi(argv[3]);
    char *fichier_trace = argv[4];
    char *fichier_cle = argv[5];

    // Generation des petits premiers
    unsigned int *s = generation_liste_nombres_premiers(lambda);

    unsigned int nb_attaque_reussi;
    unsigned int nb_attaque = 1000;

    FILE *fptr = fopen("stats", "a");
    for (float bruit = 0; bruit <= 0; bruit += 1)
    {
        nb_attaque_reussi = 0;
        for (unsigned int i = 0; i < nb_attaque; i++)
        {
            // if (!(i % 100))
            //     printf("Attaque %d \n", i);
            if(exemple_attaque_cpa_horizontal(k, lambda, t, s, bruit, fichier_trace, fichier_cle, generator)> 0)
                nb_attaque_reussi++;
        }
        float pourcentage_reussite = ((float)nb_attaque_reussi / (float)nb_attaque) * 100.0f;
        printf("Succes de l'attaque avec un bruit de %.1f : %.1f %% \n", bruit, pourcentage_reussite);
        fprintf(fptr, "%.1f %.1f \n", bruit, pourcentage_reussite);
    }
    fclose(fptr);

    return 0;
}