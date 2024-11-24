#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

// Fonction pour la liste des premiers nombres premiers
mpz_t *generation_liste_nombres_premiers(int N);
void free_liste(int taille,mpz_t *liste);

// Fonction pour le crible simple
void generation_entier_crible_simple(unsigned int k, unsigned int t, unsigned int N, mpz_t p, mpz_t *r, gmp_randstate_t generator);
