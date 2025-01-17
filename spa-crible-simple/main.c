#include "crible_simple.h"
#include "attaque_spa.h"

/**
 * Generation de la trace SPA
 * @param k nombre de bits de l'entier
 * @param N nombre d'elements du crible
 * @param t nombre de tours de Miller Rabin
 * @param r liste des petits premiers
 * @param fichier_trace_p fichier stockant la trace de courant de p
 * @param fichier_trace_q fichier stockant la trace de courant de q
 * @param fichier_parametres fichier stockant les elements constituant la cle
 * @param generator generateur aleatoire
 */
void acquisition_trace_spa(unsigned int k, unsigned int N, unsigned int *r, unsigned int t, char *fichier_trace_p, char *fichier_trace_q, char *fichier_parametres, gmp_randstate_t generator)
{
    // Initialisation des valeurs pour le crible
    mpz_t p, q, n;
    mpz_inits(p, q, n, NULL);

    // Generation de p et q et de leurs traces respectives
    generation_entier_crible_simple(k, N, t, p, r, generator, fichier_trace_p);
    generation_entier_crible_simple(k, N, t, q, r, generator, fichier_trace_q);

    // Creation de la cle publique a partir de p et q et ecriture
    mpz_mul(n, p, q);
    ecriture_parametres(p, q, n, fichier_parametres);

    mpz_clears(p, q, n, NULL);
}

/**
 * Exemple generique de l'attaque avec les etapes d'acquistion et d'attaque des traces
 * @param k nombre de bits de l'entier
 * @param N nombre d'elements du crible
 * @param t nombre de tours de Miller Rabin
 * @param r liste des petits premiers
 * @param fichier_trace_p fichier stockant la trace de courant de p
 * @param fichier_trace_q fichier stockant la trace de courant de q
 * @param fichier_parametres fichier stockant les elements constituant la cle
 * @param generator generateur aleatoire
 */
unsigned int exemple_attaque_spa(unsigned int k, unsigned int N, unsigned int *r, unsigned int t, char *fichier_trace_p, char *fichier_trace_q, char *fichier_parametres, gmp_randstate_t generator)
{

    // Acquisition de la trace pour faire l'attaque
    acquisition_trace_spa(k, N, r, t, fichier_trace_p, fichier_trace_q, fichier_parametres, generator);

    // Attaque spa
    unsigned int m1 = 0;
    unsigned int m2 = 0;

    mpz_t n, p, q, s, cp, cq;
    mpz_inits(n, p, q, s, cp, cq, NULL);
    recuperer_parametres(fichier_parametres, n, p, q);

    unsigned int taille_liste_p = 0;
    unsigned int taille_liste_q = 0;

    Liste_Diviseur *liste_p = recuperer_diviseur_unique(N, fichier_trace_p, &taille_liste_p, &m1);
    Liste_Diviseur *liste_q = recuperer_diviseur_unique(N, fichier_trace_q, &taille_liste_q, &m2);

    attaque_spa(liste_p, liste_q, taille_liste_p, taille_liste_q, s, cp, cq, m1, m2, k, n);

    unsigned int taille_x = 1;
    // TODO Faire une verification sans "tricher" en utilisant les valeurs de p et q (non obtenables normalement)
    // Verification avec les vrais valeurs de p et de q
    if (!verification(p, s, cp))
    {
        printf("Erreur dans la valeur de cp\n");
        taille_x = 0;
    }

    if (!verification(q, s, cq))
    {
        printf("Erreur dans la valeur de cq\n");
        taille_x = 0;
    }

    /*
    if (attaque_possible == 1 || attaque_possible == 2)
    {
        char reponse;
        printf("Effectuer l'attaque [o/n] : ");
        scanf(" %c", &reponse);
        if (reponse == 'o' && attaque_possible == 1)
            exhaustif(cp, s, k, n);
        if (reponse == 'o' && attaque_possible == 2)
            exhaustif(cq, s, k, n);
    }*/
    if (taille_x != 0)
        taille_x = mpz_sizeinbase(s, 2);

    mpz_clears(n, p, q, s, cp, cq, NULL);
    free(liste_p);
    free(liste_q);
    return taille_x;
}

int main(int argc, char **argv)
{

    if (argc != 7)
    {
        printf("Usage : %s <k> <N> <t> <nom fichier 1> <nom fichier 2> <nom fichier 3>\n", argv[0]);
        printf(" k : nombre de bit du nombres \nN : nombre de petits premier\nt : nombre de tours de Miller Rabin\n");
        printf("nom fichier 1 : fichier avec la trace de p \nnom fichier 2 : fichier avec la trace de q\nnom fichier 3 : fichier contenant les parametres n,p,q\n");
        return 1;
    }

    // TODO Remettre la seed "aleatoire"
    long int seed = time(NULL);
    // seed = 1733573807;
    // Generateurs aleatoires
    srand(seed);
    gmp_randstate_t generator;
    gmp_randinit_default(generator);
    gmp_randseed_ui(generator, seed);

    // Variable utilisateur
    unsigned int k = atoi(argv[1]);
    // unsigned int N = atoi(argv[2]);
    unsigned int t = atoi(argv[3]);
    char *fichier_trace_p = argv[4];
    char *fichier_trace_q = argv[5];
    char *fichier_parametres = argv[6];

    // Variable utilitaire
    // Generation des petits premiers
    unsigned int *r ;

    unsigned int nb_attaque = 10000;
    FILE *fptr = fopen("stats", "w");
    // k = 512 n = 54,60,70
    // k = 1024 n = 100,110,120
    unsigned listeN1[3] = {54, 60, 70};
    unsigned listeN2[3] = {100, 110, 120};
    for (int j = 0; j < 6; j++)
    {
        if (j < 3)
        {
            k = 512;
            r = generation_liste_nombres_premiers(listeN1[j]);
            for (int i = 0; i < nb_attaque; i++)
                fprintf(fptr, "%d\n", exemple_attaque_spa(k, listeN1[j], r, t, fichier_trace_p, fichier_trace_q, fichier_parametres, generator));
        }
        else
        {
            k = 1024;
            r = generation_liste_nombres_premiers(listeN2[j-3]);
            for (int i = 0; i < nb_attaque; i++)
                fprintf(fptr, "%d\n", exemple_attaque_spa(k, listeN2[j-3], r, t, fichier_trace_p, fichier_trace_q, fichier_parametres, generator));
        }
    }

    fclose(fptr);
    free(r);
    gmp_randclear(generator);

    return 0;
}
