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
            printf("[%d][%d] : %f\n", i, j, v.tab[i][j]);
        printf("\n");
    }
    printf("\n");
}

void free_liste_entier(Liste *liste, unsigned int nb_element)
{
    for (int i = 1; i < nb_element; i++)
        free(liste[i].l);
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
        t.tab[i] = (float *)malloc(sizeof(int) * largeur);
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

// https://stackoverflow.com/questions/12733105/c-function-that-compteurs-lines-in-file
int compteurlines(char *filename)
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
    unsigned int n = compteurlines(fichier) / lambda;

    FILE *fptr = fopen(fichier, "r");

    Tableau l;
    l.tab = (float **)malloc(sizeof(float *) * n);
    initialisation_tableau(l, n, lambda);
    l.x = n;
    l.y = lambda;
    float valeur;
    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = 0; j < lambda; j++)
        {
            fscanf(fptr, "%f", &valeur);
            l.tab[i][j] = valeur;
        }
    }
    fclose(fptr);
    return l;
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

    float correlation;
    // Si jamais une des variables est constante, pour eviter de diviser par 0
    if (numerateur == 0.f && denominateur == 0.f)
        return 1;

    else
        correlation = numerateur / denominateur;

    return correlation;
}

/**
 * @param liste elements a considerer
 * @param taille_liste taille de la liste a considerer
 * @return la liste des elements ayant la valeur maximal
 */
Liste argmax(float *liste, int taille_liste)
{
    int index_max = 1;
    for (unsigned int i = 2; i < taille_liste; i++)
    {
        if (liste[i] > liste[index_max])
            index_max = i;
    }

    // Compte le nombre d'element avec la meme correlation
    unsigned int nb_element = 0;
    for (unsigned int i = 1; i < taille_liste; i++)
    {
        if ((liste[index_max] - liste[i]) < TRESHOLD_COMPARAISON_FLOAT)
            nb_element++;
    }
    // Cree la liste avec les elements qui ont la valeur max
    Liste liste_valeur_max;
    liste_valeur_max.l = (unsigned int *)malloc(sizeof(unsigned int) * nb_element);
    liste_valeur_max.taille = nb_element;

    unsigned j = 0;
    for (unsigned int i = 1; i < taille_liste; i++)
    {
        if ((liste[index_max] - liste[i]) < TRESHOLD_COMPARAISON_FLOAT)
        {
            liste_valeur_max.l[j] = i;
            j++;
        }
    }

    return liste_valeur_max;
}

/**
 * @param a liste des restes dans la formule
 * @param r liste des modulos dans la formule
 * @param lambda nombre d'elements dans le systeme d'equation
 * @param x nombre retrouve
 */
void theoreme_reste_chinois(unsigned int *a, unsigned int *r, unsigned int lambda, mpz_t x)
{
    mpz_set_ui(x, 0);
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
 * Verification que la cle partiel retrouve est correct
 * @param n cle publique
 * @param p cle prive
 * @param p_prime cle partiel a tester
 * @param s produit des premiers utilise dans le theoreme des restes chinois
 * @return Vrai si la cle partiel, Faux sinon
 */
unsigned int verification_cle(mpz_t n, mpz_t p, mpz_t p_prime, mpz_t s)
{

    unsigned int cle_correct = FALSE;
    mpz_t public_key, verification;
    mpz_inits(public_key, verification, NULL);

    // Recuperation de n mod s
    mpz_mod(public_key, n, s);

    // verif = p % s
    mpz_mod(verification, p, s);

    // Verification que p % s = p'
    if (mpz_cmp(verification, p_prime) == 0)
    {
        // gmp_printf("Recuperation partiel de la cle p' = %Zd\n",p_prime);
        // Verification que p' divise n mod s
        if (mpz_divisible_p(public_key, p) != 0)
            printf("Les deux methodes sont equivalentes !!\n");
        cle_correct = TRUE;
    }
    mpz_clears(public_key, verification, NULL);
    return cle_correct;
}

// A modifier
/**
 * Effectue le theoreme des restes chinois avec toutes les combinaisons possibles
 */
unsigned int generation_combinaison(Liste *candidat, unsigned int *s, unsigned int lambda, unsigned int nb_combinaisons_possible, mpz_t p, mpz_t public_key, mpz_t private_key, mpz_t prod_modulo)
{
    // Attribut pour la liste des candidats
    unsigned int valeur_retour = FALSE;
    unsigned int nb_combinaison_teste = 0;
    unsigned int *potentiel_candidat = (unsigned int *)malloc(sizeof(int *) * lambda);

    // Attributs pour le candidat courant
    // Tableau pour stocker les indices des elements dans chaque sous-ensemble
    int indices[lambda];

    // Initialisation des indices de la combinaison
    for (int j = 0; j < lambda; j++)
        indices[j] = 0;

    while (nb_combinaison_teste < nb_combinaisons_possible)
    {
        // Affectation de la combinaison courantes
        for (int j = 0; j < lambda; j++)
            potentiel_candidat[j] = candidat[j].l[indices[j]];
        // Test de la combinaison courante
        theoreme_reste_chinois(potentiel_candidat, s, lambda, p);
        valeur_retour = verification_cle(public_key, private_key, p, prod_modulo);
        nb_combinaison_teste++;

        if (valeur_retour)
            break;

        // Construction de la combinaison suivante
        int k = 0;

        while (k < lambda)
        {
            indices[k]++;

            if (indices[k] < candidat[k].taille)
                break;
            else
            {
                // Si on depasse la taille du sous-ensemble, reinitialiser l'indice et passer au sous-ensemble precedent
                indices[k] = 0;
                k++;
            }
        }
    }

    free(potentiel_candidat);
    return valeur_retour;
}

/**
 * Attaque pour recuperer le nombre premier lors de la generation RSA
 * @param lambda nombre de petit premiers
 * @param s liste des petits premiers
 * @param trace fichier contenant la trace d'execution du programme
 * @return 1 si l'attaque reussi et 0 sinon
 */
float * attaque_cpa(unsigned int lambda, unsigned int *s, char *trace, char *parametres, FILE *fichier_stats,FILE *fichier_stats2)
{
    mpz_t p;
    mpz_init(p);

    Tableau l = recuperation_mesure(trace, lambda);
    int n = l.x;

    Tableau m;
    m.tab = (float **)malloc(sizeof(float *) * n);
    m.x = n;
    m.y = lambda;
    initialisation_tableau(m, n, lambda);

    // Creation tableau de la taillle maximal (meme si tout le tableau n'est pas forcement utilise)
    float *score = (float *)malloc(sizeof(float) * s[lambda - 1]);
    Liste *candidat = (Liste *)malloc(sizeof(Liste) * lambda);

    unsigned int l0[1] = {1};
    candidat[0].l = l0;
    candidat[0].taille = 1;

    for (unsigned int j = 1; j < lambda; j++)
    {
        int correlation_max = FALSE;
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
            if (score[h] == 1.0f && !correlation_max)
                correlation_max = TRUE;
        }
        // if (!correlation_max)
        //     printf("Pas de correlation de 1 pour %d\n", j);
        candidat[j] = argmax(score, s[j]);
    }

    // Recuperation de n, p
    FILE *fptr = fopen(parametres, "r");

    mpz_t public_key, private_key;
    mpz_inits(public_key, private_key, NULL);

    gmp_fscanf(fptr, "n = %Zd\n", public_key);
    gmp_fscanf(fptr, "p = %Zd", private_key);

    fclose(fptr);

    mpz_t verification, prod_modulo;
    mpz_init(verification);

    // Calcul de s
    mpz_init_set_ui(prod_modulo, 1);

    for (int i = 0; i < lambda; i++)
        mpz_mul_ui(prod_modulo, prod_modulo, s[i]);

    // Comptage de toutes les combinaisons possibles
    mpz_t nb_combinaison_possible;
    mpz_init_set_ui(nb_combinaison_possible, 1);
    // printf("Possibilite \n");
    for (unsigned int j = 0; j < lambda; j++)
    {
        // if (candidat[j].taille == 1)
        // printf("%d ", candidat[j].l[0]);

        if (candidat[j].taille > 1)
        {
            // printf("X%d ", j);
            mpz_mul_ui(nb_combinaison_possible, nb_combinaison_possible, candidat[j].taille);
        }
    }

    /*
    printf("\n");

    for (int i = 0; i < lambda; i++)
    {
        if (candidat[i].taille > 1)
        {
            printf("X%d : ", i);
            for (int j = 0; j < candidat[i].taille; j++)
                printf("%d ", candidat[i].l[j]);
            printf("\n");
        }
    }
    // */
    unsigned int valeur_retour = FALSE;


    if (mpz_cmp_ui(nb_combinaison_possible, MAX_COMBINAISON) != 1)
        valeur_retour = generation_combinaison(candidat, s, lambda, mpz_get_ui(nb_combinaison_possible), p, public_key, private_key, prod_modulo);
    fprintf(fichier_stats, "%d %d\n", n, valeur_retour);
    float nb_bits_recupere = stats_cpa(lambda,s,n,candidat,prod_modulo,public_key,private_key,fichier_stats2);

    mpz_clears(nb_combinaison_possible, public_key, private_key, verification, prod_modulo, p, NULL);

    free_tableau(l);
    free_tableau(m);
    free_liste_entier(candidat, lambda);
    free(score);

    float *retour = (float *) malloc(sizeof(float) * 2);
    retour[0] = (float) valeur_retour;
    retour[1] = nb_bits_recupere;
    return retour;
}

/*
On regarde combien de bits on recupere (pas reelement une attaque)
*/
float stats_cpa(unsigned int lambda, unsigned int *s, unsigned int n, Liste *candidat, mpz_t prod_modulo, mpz_t public_key, mpz_t private_key, FILE *fichier_stats)
{

    mpz_t modulo;
    mpz_init(modulo);

    float nb_bits_recupere = 0;
    unsigned int est_dans_liste;

    // Pour chaque reste
    for (unsigned int j = 0; j < lambda; j++)
    {
        est_dans_liste = FALSE;
        mpz_mod_ui(modulo, private_key, s[j]);
        for (unsigned int i = 0; i < candidat[j].taille; i++)
        {
            if (mpz_cmp_ui(modulo, candidat[j].l[i]) == 0)
                est_dans_liste = TRUE;
        }
        if (est_dans_liste && candidat[j].taille == 1)
            nb_bits_recupere += log2(s[j]);
            // nb_bits_recupere += log2(s[j]) - log2(candidat[j].taille);
    }
    mpz_clear(modulo);

    fprintf(fichier_stats, "%d %2.f\n", n, nb_bits_recupere);

    return nb_bits_recupere;
}
