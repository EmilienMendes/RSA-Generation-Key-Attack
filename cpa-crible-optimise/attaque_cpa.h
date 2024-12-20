#ifndef __ATTAQUE_CPA_H__
#define __ATTAQUE_CPA_H__

#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void afficher_tableau(Tableau v);
void initialisation_tableau(Tableau t, unsigned int longeur, unsigned int largeur);
void free_tableau(Tableau t);

Tableau recuperation_mesure(char *fichier, unsigned int lambda);
unsigned int poids_hamming(unsigned int nombre);
float distingueur(Tableau m, Tableau l, int i, int lambda);
unsigned int argmax(float *liste, int taille_liste);
void theoreme_reste_chinois(unsigned int *a, unsigned int *r,unsigned int lambda, mpz_t x);

void attaque_cpa(unsigned int lambda, char *trace, char *parametres, mpz_t p);

#endif