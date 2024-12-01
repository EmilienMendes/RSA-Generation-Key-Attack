#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"
#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

void ecriture_parametres(mpz_t p, mpz_t q, mpz_t n, char *fichier);
void generation_entier_crible_simple(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *r, gmp_randstate_t generator, char *fichier);
