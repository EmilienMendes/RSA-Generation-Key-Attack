#ifndef __CRIBLE_OPTIMISE_H__
#define __CRIBLE_OPTIMISE_H__

#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"
#include <stdio.h>
#include <time.h>

unsigned int verification_entier_nul(mpz_t *liste, unsigned int taille);
void generation_entier_crible_optimise(unsigned int k, unsigned int N, unsigned int t, mpz_t p, unsigned int *s, char *trace, gmp_randstate_t generator);

#endif