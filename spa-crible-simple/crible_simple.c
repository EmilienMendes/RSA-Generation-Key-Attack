#include "crible_simple.h"
#include "constante.h"
/*
Parametres :
N -> Nombre d'entiers dans le crible simple
k -> Taille en bit du nombre voulu
t -> Nombre de tours de Miller-Rabin
*/

void generation_entier_crible_simple(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *r, gmp_randstate_t generator, char *fichier)
{
    FILE *file = fopen(fichier, "w");
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
    int cpt = 0;
    if (mpz_probab_prime_p(v, t) == 0)
    {
        while (TRUE)
        {
            i = 1;
            cpt++;
            fprintf(file, "%d\n", LIGNE1);
            // Verification de la non divisiblite du nombre par les N petits premiers
            while (i < N)
            {
                fprintf(file, "%d\n", LIGNE2);

                divisible = mpz_divisible_p(v, r[i]);
                fprintf(file, "%d\n", LIGNE3);
                if (divisible != 0)
                {
                    // gmp_printf("%Zd -> j : %d divisible par %Zd\n", v, cpt, r[i]);
                    fprintf(file, "%d\n", LIGNE4);

                    mpz_add_ui(v, v, 2);
                    break;
                }
                fprintf(file, "%d\n", LIGNE5);

                i++;
            }
            /*
            Verification si le nombre est divisible par l'un des k elements.
            Si c'est le cas, passe a la condition suivante.
            Sinon, on verifie si le nombre est premier, et on s'arrete si la condition est verifie
            Si le nombre n'est pas premier, on ajoute 2 et  on recommence.
            */
            if (!divisible)
            {
                fprintf(file, "%d\n", LIGNE6);

                if (mpz_probab_prime_p(v, t))
                    break;
                else
                {
                    fprintf(file, "%d\n", LIGNE7);

                    mpz_add_ui(v, v, 2);
                }
            }
        }
    }
    mpz_set(p, v);
    mpz_clear(v);
    fprintf(file, "%d\n", LIGNE1);

    fclose(file);
}

/*
@TODO
Ajouter des conditions pour l'utilisation du programme argc,argv
*/
int main(int argc, char **argv)
{
    // Variable utilisateur
    int k = atoi(argv[1]);
    int N = atoi(argv[2]);
    int t = atoi(argv[3]);
    char *trace1 = argv[4];
    // char *trace2 = argv[5];

    // Initialisation des valeurs pour le crible
    mpz_t p;
    mpz_init(p);

    mpz_t *r;
    r = generation_liste_nombres_premiers(N);
    // Generateur pseudo aleatoires
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, time(NULL));

    generation_entier_crible_simple(k, N, t, p, r, generator, trace1);

    // gmp_printf("Generation d'un nombre premier de (crible simple) %d bits : %Zd\n", k, p);

    // Verification du nombre premier genere au cas ou (optionnel)
    // printf("Taille de p : %ld\np premier : %d\n",mpz_sizeinbase(p,2),mpz_probab_prime_p(p,10));

    mpz_clear(p);
    return 0;
}