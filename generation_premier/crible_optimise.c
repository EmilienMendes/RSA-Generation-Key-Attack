#include "crible_optimise.h"

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

void free_liste(int taille, mpz_t *liste)
{
     for (int i = 0; i < taille; i++)
        mpz_clear(liste[i]);
    free(liste);
}


int verification_entier_nul(mpz_t *liste,int taille){
    for (unsigned int i = 0; i < taille; i++)
    {
        if(mpz_cmp_ui(liste[i],0) == 0)
            return TRUE;
    }
    return FALSE;
}

void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *s, gmp_randstate_t generator)
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
        mpz_mod(r[j],v,s[j]);
    }

    unsigned int prime = mpz_probab_prime_p(v, t);
    while(!prime){
        // Verification avec les reste modulaires
        while(verification_entier_nul(r,N)){
            mpz_add_ui(v,v,2);
            for (unsigned int j = 0; j < N; j++)
                    mpz_add_ui(r[j],r[j],2);

        }
        // Test Miller Rabin
        if((prime = mpz_probab_prime_p(v, t)) == 0)
            mpz_add_ui(v,v,2);
    }
    
   
    mpz_set(p, v);
    mpz_clear(v);
    free(r);
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
    // Initialisation des valeurs pour le crible
    mpz_t p;
    mpz_init(p);

    mpz_t *r;
    r = generation_liste_nombres_premiers(N);
    // Generateur pseudo aleatoires
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, time(NULL));

    generation_entier_crible_optimise(k, N, t, p, r, generator);

    gmp_printf("Generation d'un nombre premier (crible optimise) de %d bits : %Zd\n", k, p);
    
    // Verification du nombre premier genere au cas ou (optionnel)
    // printf("Taille de p : %ld\np premier : %d\n",mpz_sizeinbase(p,2),mpz_probab_prime_p(p,10));

    mpz_clear(p);
    return 0;
}