#include "attaque_spa.h"

Liste_Diviseur *recuperer_diviseur(int N, char *fichier, int *taille_liste)
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


void attaque_spa(Liste_Diviseur *liste){

}

int main(int argc, char **argv)
{
    int N = atoi(argv[1]);
    char *fichier = argv[2];

    int *taille_liste = (int *)malloc(sizeof(int));
    *(taille_liste) = 0;
    Liste_Diviseur *liste = recuperer_diviseur(N, fichier, taille_liste);

    for (int i = 0; i < *(taille_liste); i++)
        printf("j : %d r : %d\n", liste[i].j, liste[i].r);


    free(liste);
}