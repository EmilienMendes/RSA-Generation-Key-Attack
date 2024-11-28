#include "../utility/utility.h"
#include "constante.h"
#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

void generation_entier_crible_simple(unsigned int k, unsigned int N, unsigned int t, mpz_t p, mpz_t *r, gmp_randstate_t generator, char *fichier);
