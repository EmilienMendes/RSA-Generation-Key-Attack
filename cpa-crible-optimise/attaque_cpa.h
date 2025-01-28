#ifndef __ATTAQUE_CPA_H__
#define __ATTAQUE_CPA_H__

#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"
#include "hamming.h"
#include "gauss.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"

void afficher_tableau(Tableau v);
void initialisation_tableau(Tableau t, unsigned int longeur, unsigned int largeur);
void free_tableau(Tableau t);

Tableau recuperation_mesure(char *fichier, unsigned int lambda);
float distingueur(Tableau m, Tableau l, unsigned int i, unsigned int nb_trace, unsigned int lambda);
Liste argmax(float *liste, int taille_liste);
void theoreme_reste_chinois(unsigned int *a, unsigned int *r, unsigned int lambda, mpz_t x);

float * attaque_cpa(unsigned int lambda, unsigned int *s, char *trace, char *parametres, FILE *fichier_stats,FILE *fichier_stats2);
float stats_cpa(unsigned int lambda, unsigned int *s, unsigned int n, Liste *candidat, mpz_t prod_modulo, mpz_t public_key, mpz_t private_key, FILE *fichier_stats);

#endif