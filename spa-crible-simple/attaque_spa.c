#include "attaque_spa.h"

void afficher_liste(Liste_Diviseur *liste, unsigned int taille_liste)
{
    for (int i = 0; i < taille_liste; i++)
        printf("j : %d r : %d\n", liste[i].j, liste[i].r);
}

void recuperer_parametres(char *fichier,mpz_t n,mpz_t p,mpz_t q){
    FILE *file = fopen(fichier,"r");
    gmp_fscanf(file,"n = %Zd\n",n);
    gmp_fscanf(file,"p = %Zd\n",p);
    gmp_fscanf(file,"q = %Zd\n",q);
    fclose(file);
    // printf("--------------------------\n");
    // gmp_printf("n = %Zd\n",n);
    // gmp_printf("p = %Zd\n",p);
    // gmp_printf("q = %Zd\n",q);
    // printf("--------------------------\n");


}


Liste_Diviseur *recuperer_diviseur(int N, char *fichier, int *taille_liste, int *m)
{
    FILE *file = fopen(fichier, "r");
    char buffer[BUFF_SIZE];
    unsigned int j = 0;
    unsigned int r, valeur_courant_actuel, valeur_courant_precedent;
    mpz_t *liste_premiers = generation_liste_nombres_premiers(N);

    /*
    On passe une premiere fois dans le fichier pour recuperer le nombre de fois
    ou l'algorithme est execute
    */
    while (fgets(buffer, BUFF_SIZE, file) != NULL)
    {
        valeur_courant_precedent = valeur_courant_actuel;
        valeur_courant_actuel = atoi(buffer);
        // Ligne 1 => j = 2 donc on vj est incremente
        if (valeur_courant_actuel == LIGNE1)
            j++;

        if (valeur_courant_precedent == LIGNE3 && valeur_courant_actuel == LIGNE4)
            *(taille_liste) = *(taille_liste) + 1;

        if (valeur_courant_actuel == LIGNE4 || valeur_courant_actuel == LIGNE7)
            *(m) = *(m) + 1;
    }

    Liste_Diviseur *liste = (Liste_Diviseur *)malloc(sizeof(Liste_Diviseur) * (*taille_liste));
    // On revient au debut du fichier pour recuper les diviseurs
    fseek(file, 0, SEEK_SET);
    unsigned int cpt = 0;
    j = 0;

    while (fgets(buffer, BUFF_SIZE, file) != NULL)
    {
        valeur_courant_precedent = valeur_courant_actuel;
        valeur_courant_actuel = atoi(buffer);
        // Ligne 1 => j = 2 donc on vj est incremente
        if (valeur_courant_actuel == LIGNE1)
        {
            r = 1;
            j++;
        }
        // Ligne 7 => On passe au prochain pj
        if (valeur_courant_actuel == LIGNE5)
            r++;

        // Enchainement de ligne 3 & ligne 4 => signifie que le nombre a ete divise par pj
        if (valeur_courant_precedent == LIGNE3 && valeur_courant_actuel == LIGNE4)
        {
            liste[cpt].r = mpz_get_ui(liste_premiers[r]);
            liste[cpt].j = j;
            cpt++;
        }
    }

    free_liste(N, liste_premiers);
    return liste;
}

Liste_Diviseur *enlever_doublon(Liste_Diviseur *liste, int taille_liste, int *taille_sans_doublon)
{
    unsigned int doublon = 0;

    /*
    On recupere le nombre de doublon et on categorise les doublons
    en modifiant leur valeur par -1
    */
    for (int i = 0; i < taille_liste; i++)
    {
        // L'element est deja un doublon, on ne l'analyse pas
        if (liste[i].r == -1)
            continue;

        // On regarde dans le reste de la liste si le nombre est present
        for (int j = i + 1; j < taille_liste; j++)
        {
            if (liste[i].r == liste[j].r)
            {
                doublon++;
                liste[j].r = -1;
            }
        }
    }
    *(taille_sans_doublon) = taille_liste - doublon;

    Liste_Diviseur *liste_sans_doublon = (Liste_Diviseur *)malloc(sizeof(Liste_Diviseur) * *(taille_sans_doublon));

    unsigned int cpt = 0;
    for (int i = 0; i < taille_liste; i++)
    {
        // Si l'element est un doublon on ne l'ajoute pas a la liste
        if (liste[i].r == -1)
            continue;

        liste_sans_doublon[cpt].r = liste[i].r;
        liste_sans_doublon[cpt].j = liste[i].j;
        cpt++;
    }

    return liste_sans_doublon;
}

// Calcule sp et sq
unsigned int calcul_produit(mpz_t s, Liste_Diviseur *liste, int taille_liste)
{
    mpz_init_set_ui(s, 1);
    for (int i = 0; i < taille_liste; i++)
        mpz_mul_ui(s, s, liste[i].r);
    return mpz_sizeinbase(s, 2);
}

/*
    p = 2 * (m - j1) [r1]
    p = 2 * (m - j2) [r2]
            ...
    p = 2 * (m - jn) [rn]

    https://www.bibmath.net/dico/index.php?action=affiche&quoi=./c/chinois.html
    Theoreme des restes chinois
    p = ap [sp] = 2 x (m - j1) x (sp/r1) x ((sp/r1) ^ -1 mod r1) + 2 x (m - j2) x .... x (2 * m - jn) x (sp/rn) x ((sp/rn) ^ -1 mod rn) [sp]
                = a1 x M1 x y1 + ... + an x Mn x yn
    avec        ai = 2 x (m - ji)   Mi = sp / ri yi = (sp/ri) ^ -1 mod ri
*/

void theoreme_reste_chinois(Liste_Diviseur *liste, int taille_liste, int m, mpz_t s, mpz_t x)
{
    unsigned int ai;
    mpz_t Mi, yi, ri;
    mpz_inits(Mi, yi, ri, NULL);
    for (int i = 0; i < taille_liste; i++)
    {

        ai = 2 * (m - liste[i].j);
        mpz_set_ui(ri, liste[i].r);
        mpz_fdiv_q(Mi, s, ri);
        mpz_invert(yi, Mi, ri);

        mpz_mul(yi, yi, Mi);    // yi = yi x Mi
        mpz_mul_ui(yi, yi, ai); // yi = yi x Mi x ai
        mpz_add(x, x, yi);      // ap += ai x yi x Mi
    }
    mpz_clears(Mi, yi, ri, NULL);
}

int attaque_spa(Liste_Diviseur *pliste, Liste_Diviseur *qliste, int ptaille_liste, int qtaille_liste, int m, mpz_t n,mpz_t p,mpz_t q)
{
    mpz_t sp, sq;
    unsigned int nb_bits_sp, nb_bits_sq;
    nb_bits_sp = calcul_produit(sp, pliste, ptaille_liste);
    nb_bits_sq = calcul_produit(sq, qliste, qtaille_liste);
    if (nb_bits_sp < TRESHOLD_S && nb_bits_sq < TRESHOLD_S)
    {
        printf("Pas assez de bits exploitables pour l'attaque\n");
        return 0;
    }

    // gmp_printf("sp : %Zd\n", sp);
    // gmp_printf("sq : %Zd\n", sq);

    mpz_t ap, aq, bp, bq;
    mpz_inits(ap, aq, bp, bq, NULL);

    theoreme_reste_chinois(pliste, ptaille_liste, m, sp, ap);
    theoreme_reste_chinois(pliste, ptaille_liste, m, sq, bq);

    gmp_printf("ap (%d bits) = %Zd [sp]\n", mpz_sizeinbase(ap, 2), ap);
    gmp_printf("bq (%d bits) = %Zd [sq]\n", mpz_sizeinbase(bq, 2), bq);

    mpz_t reste;
    mpz_init(reste);
    mpz_mod(reste,p,sp);
    gmp_printf("p = %Zd [sp]\n",reste);
    mpz_clear(reste);

    if (mpz_invert(aq, ap, sp) == 0)
        printf("Pas d'inverse de ap mod sp\n");
    else
    {
        mpz_mul(aq, aq, n);
        mpz_mod(aq, aq, sp);
        gmp_printf("aq = %Zd [sp] \n", aq);
    }

    if (mpz_invert(bp, bq, sq) == 0)
        printf("Pas d'inverse de bq mod sq\n");
    else
    {
        mpz_mul(bp, bp, n);
        mpz_mod(bp, bp, sq);
        gmp_printf("bq = %Zd [sq] \n", aq);
    }


    mpz_clears(ap, aq, bp, bq, sp, sq, NULL);
    return 1;
}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    char *ptrace = argv[2];
    char *qtrace = argv[3];
    char *fichier_parametres = argv[4];

    int m = 0;

    mpz_t n, p, q;
    mpz_inits(n, p, q,NULL);
    recuperer_parametres(fichier_parametres, n, p, q);
    
    int ptaille_liste = 0;
    int qtaille_liste = 0;

    Liste_Diviseur *pliste = recuperer_diviseur(N, ptrace, &ptaille_liste, &m);
    Liste_Diviseur *qliste = recuperer_diviseur(N, qtrace, &qtaille_liste, &m);

    //afficher_liste(pliste,ptaille_liste);
    // afficher_liste(qliste,qtaille_liste);
    
    int ptaille_liste_sans_doublon = 0;
    int qtaille_liste_sans_doublon = 0;
    
    Liste_Diviseur *pliste_sans_doublon = enlever_doublon(pliste, ptaille_liste, &ptaille_liste_sans_doublon);
    Liste_Diviseur *qliste_sans_doublon = enlever_doublon(qliste, qtaille_liste, &qtaille_liste_sans_doublon);
    
    // afficher_liste(pliste_sans_doublon, ptaille_liste_sans_doublon);
    // afficher_liste(qliste_sans_doublon, qtaille_liste_sans_doublon);


    attaque_spa(pliste_sans_doublon, qliste_sans_doublon, ptaille_liste_sans_doublon, qtaille_liste_sans_doublon, m, n,p,q);

    mpz_clears(n, p, q, NULL);
    free(pliste);
    free(qliste);
    free(pliste_sans_doublon);
    free(qliste_sans_doublon);
}