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
        if ((liste[index_max] -liste[i]) < TRESHOLD_COMPARAISON_FLOAT)
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
        // Verification que p' divise n mod s
        if (mpz_divisible_p(public_key, p) != 0)
            printf("Les deux methodes sont equivalentes !!\n");
        cle_correct = TRUE;
    }
    mpz_clears(public_key, verification, NULL);
    return cle_correct;
}

// A modifier
unsigned int *generation_combinaison(Liste *lists, int n, int i)
{
    unsigned int *combinaison = (unsigned int *)malloc(sizeof(int *) * n);
    int indices[n]; // Tableau pour stocker les indices des éléments dans chaque sous-ensemble
    int count = 0;  // Compteur pour suivre le nombre de combinaisons générées
    // Initialisation des indices
    for (int j = 0; j < n; j++)
        indices[j] = 0; // Chaque indice commence à 0

    // Boucle pour générer la i-ème combinaison
    while (1)
    {
        // Si le compteur correspond à la i-ème combinaison
        if (count == i)
        {
            for (int j = 0; j < n; j++)
            {
                combinaison[j] = lists[j].l[indices[j]];
            }
            break;
        }

        // Incrémenter l'indice du dernier sous-ensemble
        int j = n - 1;
        while (j >= 0)
        {
            indices[j]++; // Passer à l'élément suivant dans le sous-ensemble
            if (indices[j] < lists[j].taille)
            {
                break; // Si l'indice est valide, on arrête
            }
            else
            {
                // Si on dépasse la taille du sous-ensemble, réinitialiser l'indice et passer au sous-ensemble précédent
                indices[j] = 0;
                j--;
            }
        }

        // Si tous les indices ont été réinitialisés à 0, cela signifie qu'on a parcouru toutes les combinaisons
        if (j < 0)
        {
            printf("La %d-ième combinaison n'existe pas.\n", i); // Si on a dépassé la taille des combinaisons possibles
            return NULL;
        }

        count++; // Incrémenter le compteur de combinaisons générées
    }
    return combinaison;
}

/**
 * Attaque pour recuperer le nombre premier lors de la generation RSA
 * @param lambda nombre de petit premiers
 * @param s liste des petits premiers
 * @param trace fichier contenant la trace d'execution du programme
 * @return 1 si l'attaque reussi et 0 sinon
 */
unsigned int attaque_cpa(unsigned int lambda, unsigned int *s, char *trace, char *parametres)
{
    mpz_t p;
    mpz_init(p);

    Tableau l = recuperation_mesure(trace, lambda);
    int n = l.x;

    Tableau m;
    m.tab = (unsigned int **)malloc(sizeof(unsigned int *) * n);
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
        int a = FALSE;
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
            if (score[h] == 1.0f && correlation_max && !a)
            {
                a = TRUE;
                // printf("Plusieurs valeurs max pour %d \n", j);
            }
            if (score[h] == 1.0f && !correlation_max && !a)
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
    unsigned int nb_combinaison_possible = 1;

    // printf("Possibilite \n");
    for (unsigned int j = 0; j < lambda; j++)
    {
        // if (candidat[j].taille == 1)
        // printf("%d ", candidat[j].l[0]);

        if (candidat[j].taille > 1)
        {
            // printf("X%d ", j);
            if (nb_combinaison_possible < MAX_COMBINAISON)
                nb_combinaison_possible *= candidat[j].taille;
            else
                nb_combinaison_possible = MAX_COMBINAISON;
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
    */
    unsigned int valeur_retour = 0;

    if (nb_combinaison_possible < MAX_COMBINAISON)
    {
        unsigned int *potentiel_candidat;

        unsigned int nb_combinaison_teste = 0;
        // printf("Combinaison possible : %d \n", nb_combinaison_possible);

        while (nb_combinaison_teste < nb_combinaison_possible && !valeur_retour)
        {
            potentiel_candidat = generation_combinaison(candidat, lambda, nb_combinaison_teste);

            // printf("%d : ", nb_combinaison_teste);
            // for (int i = 0; i < lambda; i++)
            //     printf("%d ", potentiel_candidat[i]);
            // printf("\n");

            theoreme_reste_chinois(potentiel_candidat, s, lambda, p);

            valeur_retour = verification_cle(public_key, private_key, p, prod_modulo);
            nb_combinaison_teste++;
        }
        free(potentiel_candidat);
    }

    else
        printf("Trop de combinaison possible !!! (> 25 000)\n");
    mpz_clears(public_key, private_key, verification, prod_modulo, p, NULL);

    free_tableau(l);
    free_tableau(m);
    free(candidat);
    free(score);
    // TODO free liste de candidat
    return valeur_retour;
}