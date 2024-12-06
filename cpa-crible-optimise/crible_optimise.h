#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

// Fonction pour la liste des premiers nombres premiers
mpz_t *generation_liste_nombres_premiers(int N);
void free_liste(int taille,mpz_t *liste);
int verification_entier_nul(mpz_t *liste,int taille);

// Fonction pour le crible optimise
void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *s, char *trace,char *parametres, gmp_randstate_t generator);
