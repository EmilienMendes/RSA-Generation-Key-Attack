#include "crible_simple.h"

void ecriture_parametres(mpz_t p,mpz_t q,mpz_t n,char *fichier){
    FILE * file = fopen(fichier,"w");
    gmp_fprintf(file,"n = %Zd\n",n);
    gmp_fprintf(file,"p = %Zd\n",p);
    gmp_fprintf(file,"q = %Zd\n",q);
    fclose(file);
}



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
            fprintf(file, "%d\n", LIGNE1);
            // Verification de la non divisiblite du nombre par les N petits premiers
            while (i < N)
            {
                fprintf(file, "%d\n", LIGNE2);

                divisible = mpz_divisible_p(v, r[i]);
                fprintf(file, "%d\n", LIGNE3);
                if (divisible != 0)
                {
                    // gmp_printf("j : %d divisible par %Zd\n",cpt, r[i]);
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
            cpt++;
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
    char *ptrace = argv[4];
    char *qtrace = argv[5];
    char *parametres = argv[6];
    unsigned int seed = time(NULL);
    // Initialisation des valeurs pour le crible
    mpz_t p,q,n;
    mpz_inits(p,q,n,NULL);

    mpz_t *r;
    r = generation_liste_nombres_premiers(N);
    // Generateur pseudo aleatoires
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, seed);
    generation_entier_crible_simple(k, N, t, p, r, generator, ptrace);
    generation_entier_crible_simple(k, N, t, q, r, generator, qtrace);

    mpz_mul(n,p,q);
    // if(mpz_probab_prime_p(p,10) && mpz_probab_prime_p(q,10) )
    ecriture_parametres(p,q,n,parametres);


    gmp_randclear(generator);
    mpz_clears(p,q,n,NULL);
    return 0;
}