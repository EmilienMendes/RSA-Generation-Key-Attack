#include "attaque_cpa.h"
/**
 * Affichage du tableau
 * @param t tableau affiche
 */
void afficher_tableau(Tableau v)
{
    unsigned int longeur = v.x;
    unsigned int largeur = v.y;
    for (unsigned int i = 0; i < longeur; i++)
    {
        for (unsigned int j = 0; j < largeur; j++)
        printf("[%d][%d] : %d\n", i, j, v.tab[i][j]);
        printf("\n");
    }
    printf("\n");
}

/**
 * Initialisation du tableau
 * @param t tableau d'entre
 * @param longueur nombre de ligne du tableau
 * @param largeur nombre de colonne du tableau
 */
void initialisation_tableau(Tableau t, unsigned int longeur, unsigned int largeur)
{
    for (unsigned int i = 0; i < longeur; i++)
        t.tab[i] = (unsigned int *)malloc(sizeof(int) * largeur);
}

/**
 * Suppression du tableau en memoire
 * @param t tableau supprime
 */
void free_tableau(Tableau t)
{
    for (unsigned int i = 0; i < t.x; i++)
        free(t.tab[i]);
    free(t.tab);
}

// https://stackoverflow.com/questions/12733105/c-function-that-counts-lines-in-file
int countlines(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int ch = 0;
    int lines = 0;

    if (fp == NULL)
        return 0;

    while (!feof(fp))
    {
        ch = fgetc(fp);
        if (ch == '\n')
        {
            lines++;
        }
    }
    fclose(fp);
    return lines;
}

/**
 * Recuperation des mesures lie a la trace
 * @param fichier trace de l'execution du programme
 * @param lambda nombre de petits premiers utilise
 * @return tableau a 2 dimensions contenant les trace pour chacun des (i,j)
 */
Tableau recuperation_mesure(char *fichier, unsigned int lambda)
{
    // n = taille du fichier / lambda
    unsigned int n = countlines(fichier) / lambda;

    FILE *fptr = fopen(fichier, "r");

    Tableau l;
    l.tab = (unsigned int **)malloc(sizeof(unsigned int *) * n);
    initialisation_tableau(l, n, lambda);
    l.x = n;
    l.y = lambda;
    unsigned int valeur;
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < lambda; j++)
        {
            fscanf(fptr, "%u", &valeur);
            l.tab[i][j] = valeur;
        }
    }
    fclose(fptr);
    return l;
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

/**
 * @param m tableau des poids de hamming
 * @param l tableau de mesure
 * @param j index du tableau utilise
 * @param nb_trace nombre de trace utilise pour la CPA
 * @param lambda largeur du tableau
 * @return coefficient de correlation de Pearson entre m[i] et l[i] (de taille lambda)
 */
float distingueur(Tableau m, Tableau l, unsigned int j, unsigned int nb_trace, unsigned int lambda)
{
    float moyenne_x = 0;
    float moyenne_y = 0;

    for (int i = 0; i < nb_trace; i++)
    {
        moyenne_x += m.tab[i][j];
        moyenne_y += l.tab[i][j];
    }

    moyenne_x /= nb_trace;
    moyenne_y /= nb_trace;

    float somme_x_carre = 0;
    float somme_y_carre = 0;
    float numerateur = 0;

    for (int i = 0; i < nb_trace; i++)
    {
        numerateur += (m.tab[i][j] - moyenne_x) * (l.tab[i][j] - moyenne_y);
        somme_x_carre += (m.tab[i][j] - moyenne_x) * (m.tab[i][j] - moyenne_x);
        somme_y_carre += (l.tab[i][j] - moyenne_y) * (l.tab[i][j] - moyenne_y);
    }

    float denominateur = sqrt(somme_x_carre * somme_y_carre);

    float correlation = numerateur / denominateur;
    return correlation;
}

/**
 * @param liste elements a considerer
 * @param taille_liste taille de la liste a considerer
 * @return la valeur maximal du tableau
 */
unsigned int argmax(float *liste, int taille_liste)
{
    int index_max = 1;
    for (unsigned int i = 2; i < taille_liste; i++)
    {
        if (liste[i] > liste[index_max])
            index_max = i;
    }
    // for (int i = 2; i < taille_liste; i++)
    // {
    //     if(liste[i] == liste[index_max] && i != index_max){
    //         printf("Correlation indentique de %.2f\n",liste[i]);
    //     }
    // }

    return index_max;
}

/**
 * @param a liste des restes dans la formule
 * @param r liste des modulos dans la formule
 * @param lambda nombre d'elements dans le systeme d'equation
 * @param x nombre retrouve
 */
void theoreme_reste_chinois(unsigned int *a, unsigned int *r, unsigned int lambda, mpz_t x)
{
    mpz_t Mi, yi, ri, produit_modulo;
    mpz_inits(Mi, yi, ri, NULL);

    mpz_init_set_ui(produit_modulo, 1);
    for (unsigned int i = 0; i < lambda; i++)
        mpz_mul_ui(produit_modulo, produit_modulo, r[i]);

    for (unsigned int i = 0; i < lambda; i++)
    {
        mpz_set_ui(ri, r[i]);
        mpz_fdiv_q(Mi, produit_modulo, ri);
        mpz_invert(yi, Mi, ri);

        mpz_mul(yi, yi, Mi);      // yi = yi x Mi
        mpz_mul_ui(yi, yi, a[i]); // yi = yi x Mi x ai
        mpz_add(x, x, yi);        // ap += ai x yi x Mi
    }
    mpz_mod(x, x, produit_modulo);
    mpz_clears(Mi, yi, ri, NULL);
}

/**
 * Attaque pour recuperer le nombre premier lors de la generation RSA
 * @param lambda nombre de petit premiers
 * @param trace fichier contenant la trace d'execution du programme
 */
void attaque_cpa(unsigned int lambda, char *trace, char *parametres)
{
    mpz_t p;
    mpz_init(p);

    Tableau l = recuperation_mesure(trace, lambda);
    int n = l.x;
    unsigned int *s = generation_liste_nombres_premiers(lambda);

    Tableau m;
    m.tab = (unsigned int **)malloc(sizeof(unsigned int *) * n);
    m.x = n;
    m.y = lambda;
    initialisation_tableau(m, n, lambda);
    // Creation tableau de la taillle maximal (meme si tout le tableau n'est pas forcement utilise)
    float *score = (float *)malloc(sizeof(float) * s[lambda - 1]);
    unsigned int *candidat = (unsigned int *)malloc(sizeof(unsigned int) * lambda);

    for (unsigned int j = 0; j < lambda; j++)
    {
        for (unsigned int h = 1; h < s[j]; h++)
        {
            for (unsigned int i = 0; i < n; i++)
            {
                int nombre = h - (n - i) * 2; // Marche uniquement pour  n - i ????
                // On prend le nombre et on le fais devenir positif s'il ne l'etait pas deja
                while (nombre < 0)
                    nombre += s[j];
                m.tab[i][j] = poids_hamming(nombre % s[j]);
            }
            score[h] = distingueur(m, l, j, n, lambda);
        }
        candidat[j] = argmax(score, s[j]);
    }

    theoreme_reste_chinois(candidat, s, lambda, p);

    FILE *fptr = fopen(parametres, "r");
    mpz_t public_key, verification, prod_modulo;
    /*
    p % p' = s
    p = s + k x p' (avec s le produit des modulos)
    n  = p x q

    n = (s + k x p') x q
      = s x q + k x p' x q

    Donc divise n % p' = s
    */

    mpz_init_set_ui(prod_modulo, 1);
    for (int i = 0; i < lambda; i++)
        mpz_mul_ui(prod_modulo, prod_modulo, s[i]);

    mpz_inits(public_key, verification, NULL);
    gmp_fscanf(fptr, "n = %Zd", public_key);
    mpz_mod(public_key, public_key, p);

    if (mpz_cmp(public_key, prod_modulo) != 0)
    {
        printf("Recuperation partiel de la cle \n");
        gmp_printf("p' = %Zd\n", p);
    }
    else
        gmp_printf("Erreur dans la valeur n mod s = %Zd !\n", public_key);

    mpz_clears(public_key, verification, prod_modulo, p, NULL);

    free_tableau(l);
    free_tableau(m);
    free(s);
    free(candidat);
    free(score);
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage : %s <lambda> <fichier1> <fichier2> \nlambda: nombre de petits premiers utilise pour le crible optimise\n", argv[0]);
        printf("fichier1 : nom du fichier contenant la trace d'execution\nfichier2 : nom du fichier contenant les parametres RSA\n");
        return 1;
    }
    
    unsigned int lambda = atoi(argv[1]);
    char *trace = argv[2];
    char *parametres = argv[3];

    srand(time(NULL));

    attaque_cpa(lambda, trace, parametres);
    return 0;
}