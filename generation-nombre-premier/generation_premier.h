#include "gmp.h"
#include "stdlib.h"

unsigned int *generation_liste_nombres_premiers(unsigned int N);
void free_mpz_liste(unsigned int taille, mpz_t *liste);