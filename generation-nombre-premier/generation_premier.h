#include "gmp.h"
#include "stdlib.h"

mpz_t *generation_liste_nombres_premiers(int N);
void free_liste(int taille, mpz_t *liste);