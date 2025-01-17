#include "attaque_spa.h"

/**
 * Affichage des j et r pour lesquelles vj = 0 modulo r
 * @param liste liste des j et r
 * @param taille_liste taile de la liste
 */
void afficher_liste(Liste_Diviseur *liste, unsigned int taille_liste)
{
    for (unsigned int i = 0; i < taille_liste; i++)
        printf("j : %d r : %d\n", liste[i].j, liste[i].r);
}

/**
 * Recuperation des entiers RSA (n,p,q) a partir du fichier de parametres
 * @param fichier fichier contenant la cle publique et la cle privee
 * @param n entier de la cle publique
 * @param p entier de la cle privee
 * @param q entier de la cle privee
 */
void recuperer_parametres(char *fichier, mpz_t n, mpz_t p, mpz_t q)
{
    FILE *fptr = fopen(fichier, "r");
    gmp_fscanf(fptr, "n = %Zd\n", n);
    gmp_fscanf(fptr, "p = %Zd\n", p);
    gmp_fscanf(fptr, "q = %Zd\n", q);
    fclose(fptr);
}

/**
 * Recuperation de tous les j a partir de la trace obtenue
 * @param N nombres de petits premiers
 * @param fichier trace que l'on va analyser
 * @param taille_liste pointeur pour stocker le nombre de diviseurs de vj1,vj2,...
 * @param m entier qui verifie la relation p = v0 + 2 x m (ou q)
 * @return Liste de tous les diviseurs de vj modulo r
 */
Liste_Diviseur *recuperer_diviseur(unsigned int N, char *fichier, unsigned int *taille_liste, unsigned int *m)
{
    FILE *fptr = fopen(fichier, "r");
    char buffer[BUFF_SIZE];
    unsigned int r, valeur_courant_actuel, valeur_courant_precedent;
    unsigned int *liste_premiers = generation_liste_nombres_premiers(N);

    /*
    On passe une premiere fois dans le fichier pour recuperer le nombre de fois
    ou l'algorithme est execute
    */
    while (fgets(buffer, BUFF_SIZE, fptr) != NULL)
    {
        valeur_courant_precedent = valeur_courant_actuel;
        valeur_courant_actuel = atoi(buffer);

        if (valeur_courant_precedent == LIGNE3 && valeur_courant_actuel == LIGNE4)
            *(taille_liste) = *(taille_liste) + 1;

        if (valeur_courant_actuel == LIGNE4 || valeur_courant_actuel == LIGNE7)
            *(m) = *(m) + 1;
    }

    Liste_Diviseur *liste = (Liste_Diviseur *)malloc(sizeof(Liste_Diviseur) * (*taille_liste));

    // On revient au debut du fichier pour recuper les diviseurs
    fseek(fptr, 0, SEEK_SET);

    unsigned int nb_diviseur = 0;
    /*
        On commence a j = -1 car on va incrementer a chaque debut de boucle c'est a dire
        quand on commence a chercher les diviseurs pour le vj courant
    */
    unsigned int j = -1;

    while (fgets(buffer, BUFF_SIZE, fptr) != NULL)
    {
        valeur_courant_precedent = valeur_courant_actuel;
        valeur_courant_actuel = atoi(buffer);

        // Ligne 7 => On passe au prochain pj
        if (valeur_courant_actuel == LIGNE5)
            r++;

        // Enchainement de ligne 3 & ligne 4 => signifie que le nombre a ete divise par pj
        if (valeur_courant_precedent == LIGNE3 && valeur_courant_actuel == LIGNE4)
        {
            liste[nb_diviseur].r = liste_premiers[r];
            liste[nb_diviseur].j = j;
            nb_diviseur++;
        }

        // Ligne 1 => j = 2 donc on vj est incremente
        if (valeur_courant_actuel == LIGNE1)
        {
            r = 1;
            j++;
        }
    }

    free(liste_premiers);
    return liste;
}




/**
 * Suppression de tous les elements en double dans la liste des diviseurs pour pouvoir faire le theoreme des restes chinois
 * @param liste listes de tous les diviseurs avec des doublons
 * @param taille_liste nombre d'element de la liste original
 * @param taille_sans_doublon pointeur pour stocker la taille de la liste sans doublon
 * @return liste des diviseurs uniques avec leur congruences et leur modulos

*/
Liste_Diviseur *enlever_doublon(Liste_Diviseur *liste, unsigned int taille_liste, unsigned int *taille_sans_doublon)
{
    unsigned int doublon = 0;

    /*
    On recupere le nombre de doublon et on categorise les doublons
    en modifiant leur valeur par -1
    */
    for (unsigned int i = 0; i < taille_liste; i++)
    {
        // L'element est deja un doublon, on ne l'analyse pas
        if (liste[i].r == -1)
            continue;

        // On regarde dans le reste de la liste si le nombre est present
        for (unsigned int j = i + 1; j < taille_liste; j++)
        {
            if (liste[i].r == liste[j].r)
            {
                doublon++;
                liste[j].r = -1;
            }
        }
    }
    // Stockage de la taille de la nouvelle liste
    *(taille_sans_doublon) = taille_liste - doublon;

    Liste_Diviseur *liste_sans_doublon = (Liste_Diviseur *)malloc(sizeof(Liste_Diviseur) * *(taille_sans_doublon));

    // Ajout des valeurs de tous les diviseurs uniques, de leur congruence et de leur modulo
    unsigned int nb_diviseur_unique = 0;
    for (unsigned int i = 0; i < taille_liste; i++)
    {
        // Si l'element est un doublon on ne l'ajoute pas a la liste
        if (liste[i].r == -1)
            continue;

        liste_sans_doublon[nb_diviseur_unique].r = liste[i].r;
        liste_sans_doublon[nb_diviseur_unique].j = liste[i].j;
        nb_diviseur_unique++;
    }

    return liste_sans_doublon;
}

/**
 TODO Ajouter description
 */
Liste_Diviseur *recuperer_diviseur_unique(unsigned int N,char *fichier,unsigned int *taille_liste,unsigned int *m){
    unsigned int taille_liste_doublon = 0;
    Liste_Diviseur *liste  = recuperer_diviseur(N,fichier,&taille_liste_doublon,m);
    Liste_Diviseur *liste_sans_doublon = enlever_doublon(liste,taille_liste_doublon,taille_liste);
    free(liste);
    return liste_sans_doublon;
}

/**
 * Cacul des produits des diviseurs de vj pour obtenir respectivement sp et sq
 * @param s parametre (sp ou sq) pour recevoir la valeur attendu
 * @param liste liste de tous les diviseurs unique des vj
 * @param taille_liste taille de la liste sans les doublons
 */
void calcul_produit(mpz_t s, Liste_Diviseur *liste, unsigned int taille_liste)
{
    mpz_init_set_ui(s, 1);
    for (unsigned int i = 0; i < taille_liste; i++)
        mpz_mul_ui(s, s, liste[i].r);
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

/**
 * Calcul des valeurs (ap,bq) a l'aide du theoreme des restes chinois
 * @param liste liste de tous les diviseurs sans doublons
 * @param taille_liste taille de la liste sans les doublons
 * @param m nombre de fois ou on a ajoute 2 pour trouver les nombres premiers (p,q)
 * @param s respectivement (sp et sq) calcule avec calcul_produit
 * @param x valeur de retour qui stockera l'entier resultant du theoreme
 */
void theoreme_reste_chinois(Liste_Diviseur *liste, unsigned int taille_liste, unsigned int m, mpz_t s, mpz_t x)
{
    unsigned int ai;
    mpz_t Mi, yi, ri;
    mpz_inits(Mi, yi, ri, NULL);

    for (unsigned int i = 0; i < taille_liste; i++)
    {
        ai = (2 * (m - liste[i].j)) % liste[i].r;
        mpz_set_ui(ri, liste[i].r);
        mpz_fdiv_q(Mi, s, ri);
        mpz_invert(yi, Mi, ri);

        mpz_mul(yi, yi, Mi);    // yi = yi x Mi
        mpz_mul_ui(yi, yi, ai); // yi = yi x Mi x ai
        mpz_add(x, x, yi);      // ap += ai x yi x Mi
    }
    mpz_mod(x, x, s);
    mpz_clears(Mi, yi, ri, NULL);
}

/**
 * Version du theoreme des restes chinois avec seulement deux elements
 * x= a1 ​+ m1​ x ( a2​−a1​) x( m1 ^−1​(mod m2​))) (mod m1 x ​m2​)
 * @param x resultat du theoreme
 * @param a1 reste de la premiere equation du theoreme
 * @param a2 reste de la deuxieme equation du theoreme
 * @param r1 congruence de la premiere equation du theoreme
 * @param r2 congruence de la deuxieme equation du theoreme
 */
void theoreme_reste_chinois_simplifie(mpz_t x, mpz_t a1, mpz_t a2, mpz_t r1, mpz_t r2)
{
    mpz_t inverse, module_mult;
    mpz_inits(inverse, module_mult, NULL);

    mpz_invert(inverse, r1, r2); // inverse = r1 ^-1 mod r2
    mpz_mul(module_mult, r1, r2);

    mpz_sub(x, a2, a1);
    mpz_mul(x, x, inverse);
    mpz_mul(x, x, r1);
    mpz_add(x, x, a1);
    mpz_mod(x, x, module_mult);

    mpz_clears(inverse, module_mult, NULL);
}

/**
 * @param premier entier a verifier
 * @param s congruence a l'entier premier
 * @param x valeur obtenu avec le theoreme des restes chinois
 * @return 1 si  x = premier mod s et 0 sinon
 */
unsigned int verification(mpz_t premier, mpz_t s, mpz_t x)
{
    mpz_t tmp;
    mpz_init(tmp);
    mpz_mod(tmp, premier, s);
    unsigned int comparaison = (mpz_cmp(tmp, x) == 0);
    mpz_clear(tmp);
    return comparaison;
}

/**
 * Attaque spa avec les traces de p et de q lors de la generation RSA
 * @param pliste liste de tout les diviseurs sans doublons a partir de la trace de p
 * @param qliste liste de tout les diviseurs sans doublons a partir de la trace de q
 * @param ptaille_liste nombre de diviseurs uniques des vj par rapport a p
 * @param ptaille_liste nombre de diviseurs uniques des vj par rapport a q
 * @param s plus petit diviseur de sp et sq (defini plus bas dans la fonction)
 * @param cp valeur congru a p modulo s
 * @param cq valeur congru a q modulo s
 * @param m1 entier qui verifie la relation p = v0 + 2 x m1
 * @param m2 entier qui verifie la relation q = v0 + 2 x m2
 * @param k taille en bits des entiers p et q
 * @param n entier publique de RSA tel que n = p x q
 * @return 2 (attaque par cp) 1 ( attaque par cq) 0 (pas assez de bits d'information pour attaquer)
 */
unsigned int attaque_spa(Liste_Diviseur *pliste, Liste_Diviseur *qliste, unsigned int ptaille_liste, unsigned int qtaille_liste, mpz_t s, mpz_t cp, mpz_t cq, unsigned int m1, unsigned int m2, unsigned int k, mpz_t n)
{
    mpz_t sp, sq;
    /*
    Calcul de sp = produit des r pour la generation de p
    Calcul de sq = produit des r pour la generation de q
    */
    calcul_produit(sp, pliste, ptaille_liste);
    calcul_produit(sq, qliste, qtaille_liste);

    /*
    Recuperation de ap, bq avec le theoreme des restes chinois
    */
    mpz_t ap, aq, bp, bq;
    mpz_inits(ap, aq, bp, bq, NULL);

    theoreme_reste_chinois(pliste, ptaille_liste, m1, sp, ap);
    theoreme_reste_chinois(qliste, qtaille_liste, m2, sq, bq);

    /*
    Calcul de aq et bp a partir de ap et bq et de n
    aq = (ap ^ -1 mod sp x n ) mod sp
    bp = (bq ^ -1 mod sq x n ) mod sq
    */
    // Calcul de aq
    mpz_invert(aq, ap, sp);
    mpz_mul(aq, aq, n);
    mpz_mod(aq, aq, sp);

    // Calcul de bp
    mpz_invert(bp, bq, sq);
    mpz_mul(bp, bp, n);
    mpz_mod(bp, bp, sq);

    // gmp_printf("ap (%d bits) = %Zd  \n", mpz_sizeinbase(ap, 2), ap);
    // gmp_printf("aq (%d bits) = %Zd  \n", mpz_sizeinbase(aq, 2), aq);
    // gmp_printf("bp (%d bits) = %Zd  \n", mpz_sizeinbase(bp, 2), bp);
    // gmp_printf("bq (%d bits) = %Zd  \n", mpz_sizeinbase(bq, 2), bq);

    /*
    On va appliquer le theoreme des restes chinois sur (ap,bp) et (aq,bq)
    Les modulos doivent etre premier entre eux
    On cherche donc le pgcd et on fais
    pgcd_s = pgcd(sp,sq)
    sp_bis = sp/pgcd_s
    sq et sp_bis sont donc premier entre eux
    */

    mpz_t pgcd_s, sp_bis, sq_bis;
    mpz_inits(pgcd_s, sp_bis, sq_bis, s, NULL);
    mpz_gcd(pgcd_s, sp, sq);
    mpz_lcm(s, sp, sq);

    mpz_fdiv_q(sp_bis, sp, pgcd_s);
    mpz_mod(ap, ap, sp_bis);
    mpz_mod(aq, aq, sp_bis);

    // Resultat cp et cq modulo sp_bis x sq =  s = lcm(sp,sq)
    theoreme_reste_chinois_simplifie(cp, ap, bp, sp_bis, sq);
    theoreme_reste_chinois_simplifie(cq, aq, bq, sp_bis, sq);

    // gmp_printf("cp = %Zd\n", cp);
    // gmp_printf("cq = %Zd\n", cq);

    unsigned int nb_bits_cp = mpz_sizeinbase(cp, 2);
    unsigned int nb_bits_cq = mpz_sizeinbase(cq, 2);

    unsigned int attaque_possible = FALSE;
    if (nb_bits_cp < k / 2 && nb_bits_cq < k / 2)
        printf(" ");
        // printf("Pas assez de bits pour faire l'attaque \ncp : (%d bits reel < %d bits requis )\ncq : (%d bits reel < %d bits requis ) \n", nb_bits_cp, k / 2, nb_bits_cq, k / 2);
    else
    {
        // printf("Attaque possible ");
        if (nb_bits_cp > nb_bits_cq)
        {
            attaque_possible = 2;
            // printf("avec cp : %d bits \n", nb_bits_cp);
        }
        else if (nb_bits_cp < nb_bits_cq)
        {
            attaque_possible = 1;
            // printf("avec cq : %d bits \n", nb_bits_cq);
        }
        else
        {
            attaque_possible = 1;
            // printf("avec cp ou cq : %d bits tous les deux\n", nb_bits_cp);
        }
    }

    mpz_clears(ap, aq, bp, bq, sp, sq, sp_bis, sq_bis, pgcd_s, NULL);
    return attaque_possible;
}

void exhaustif(mpz_t c, mpz_t s, unsigned int k, mpz_t n)
{
    unsigned int ancienne_taille_c = 0;
    unsigned int taille_c;
    while ((taille_c = mpz_sizeinbase(c, 2)) < k)
    {
        if (ancienne_taille_c != taille_c)
        {
            ancienne_taille_c = taille_c;
            printf("nb bits : %d \n", taille_c);
        }
        mpz_add(c, c, s);
    }
    while (!mpz_divisible_p(n, c))
    {
        mpz_add(c, c, s);
    }
    gmp_printf("p : %Zd\n", c);
}
