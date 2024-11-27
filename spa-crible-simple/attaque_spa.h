#include "../utility/utility.h"
#include "constante.h"

#include <stdio.h>

#define BUFF_SIZE 512

Liste_Diviseur *recuperer_diviseur(int N, char *fichier, int *taille_liste, int *m);
int attaque_spa(Liste_Diviseur *liste, int taille_liste, int m);
