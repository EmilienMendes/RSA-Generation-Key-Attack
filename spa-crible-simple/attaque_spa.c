#include "attaque_spa.h"

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

int attaque_spa(Liste_Diviseur *liste, int taille_liste, int m)
{
    mpz_t sp;
    mpz_init_set_ui(sp, 1);
    for (int i = 0; i < taille_liste; i++)
        mpz_mul_ui(sp, sp, liste[i].r);

    int nb_bits_sp = mpz_sizeinbase(sp, 2);
    if (nb_bits_sp < 1)
    {
        printf("Attaque impossible sp est trop petit (%d bits) \n", nb_bits_sp);
        return 0;
    }
    // gmp_printf("sp : %Zd\n", sp);

    mpz_t ap, aq;
    mpz_inits(ap, aq, NULL);

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
    unsigned int ai;
    mpz_t Mi, yi, ri;
    mpz_inits(Mi, yi, ri, NULL);
    for (int i = 0; i < taille_liste; i++)
    {

        ai = 2 * (m - liste[i].j);
        mpz_set_ui(ri, liste[i].r);
        mpz_fdiv_q(Mi, sp, ri);
        mpz_invert(yi, Mi, ri);

        mpz_mul(yi, yi, Mi);    // yi = yi x Mi
        mpz_mul_ui(yi, yi, ai); // yi = yi x Mi x ai
        mpz_add(ap, ap, yi);    // ap += ai x yi x Mi
    }
    gmp_printf("ap (%d bits) = %Zd [sp]\n", mpz_sizeinbase(ap, 2), ap);

    if (mpz_invert(aq, ap, sp) == 0)
        printf("Pas d'inverse de ap mod sp\n");
    else
        gmp_printf("[INCOMPLET] aq = %Zd [sp] \n", aq);
    /*
    mpz_mul(aq,aq,n);
    mpz_mod(aq,aq,sp);
    */

    mpz_clears(ap, aq, sp, Mi, yi, NULL);
    return 1;
}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    char *fichier = argv[2];

    int m = 0;
    int taille_liste = 0;
    Liste_Diviseur *liste = recuperer_diviseur(N, fichier, &taille_liste, &m);
    //  for (int i = 0; i < taille_liste; i++)
    //         printf("j : %d r : %d\n", liste[i].j, liste[i].r);

    int taille_liste_sans_doublon = 0;
    Liste_Diviseur *liste_sans_doublon = enlever_doublon(liste, taille_liste, &taille_liste_sans_doublon);
    // for (int i = 0; i < taille_liste_sans_doublon; i++)
    //     printf("j : %d r : %d\n", liste_sans_doublon[i].j, liste_sans_doublon[i].r);
    printf("Nombre de j (sans doublon) trouve : %d\n", taille_liste_sans_doublon);
    attaque_spa(liste_sans_doublon, taille_liste_sans_doublon, m);
    free(liste);
}