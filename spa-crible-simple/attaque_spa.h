#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"

#include <stdio.h>

#define BUFF_SIZE 512

void afficher_liste(Liste_Diviseur *liste, unsigned int taille_liste);
Liste_Diviseur *recuperer_diviseur(int N, char *fichier, int *taille_liste, int *m);
int attaque_spa(Liste_Diviseur *pliste, Liste_Diviseur *qliste, int ptaille_liste, int qtaille_liste, int m1,int m2, mpz_t n,mpz_t p,mpz_t q);


