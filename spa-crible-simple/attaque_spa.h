#ifndef __ATTAQUE_SPA_H__
#define __ATTAQUE_SPA_H__

#include "../generation-nombre-premier/generation_premier.h"
#include "constante.h"
#include <stdio.h>

#define BUFF_SIZE 512

// Operations sur des listes et fichiers
void afficher_liste(Liste_Diviseur *liste, unsigned int taille_liste);
void recuperer_parametres(char *fichier, mpz_t n, mpz_t p, mpz_t q);

// Operations sur les j et r
Liste_Diviseur *recuperer_diviseur(unsigned int N, char *fichier, unsigned int *taille_liste, unsigned int *m);
Liste_Diviseur *enlever_doublon(Liste_Diviseur *liste, unsigned int taille_liste, unsigned int *taille_sans_doublon);

// Operations pour recuperer les valeurs ap,bp,cp aq,bq,cq
void calcul_produit(mpz_t s, Liste_Diviseur *liste, unsigned int taille_liste);
void theoreme_reste_chinois(Liste_Diviseur *liste, unsigned int taille_liste, unsigned int m, mpz_t s, mpz_t x);
void theoreme_reste_chinois_simplifie(mpz_t x, mpz_t a1, mpz_t a2, mpz_t r1, mpz_t r2);

// Attaque et verification du fonctionnement
unsigned int attaque_spa(Liste_Diviseur *pliste, Liste_Diviseur *qliste, unsigned int ptaille_liste, unsigned int qtaille_liste, mpz_t s, mpz_t cp, mpz_t cq, unsigned int m1, unsigned int m2, unsigned int k ,mpz_t n);
void exhaustif(mpz_t c, mpz_t s, unsigned int k, mpz_t n);
unsigned int verification(mpz_t premier, mpz_t s, mpz_t x);



#endif