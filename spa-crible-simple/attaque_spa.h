#include "../generation-nombre-premier/generation_premier.h"

#include "constante.h"
#include <stdio.h>

#define BUFF_SIZE 512

// Operations sur des listes et fichiers
void afficher_liste(Liste_Diviseur *liste, unsigned int taille_liste);
void recuperer_parametres(char *fichier, mpz_t n, mpz_t p, mpz_t q);

// Operations sur les j et r
Liste_Diviseur *recuperer_diviseur(int N, char *fichier, int *taille_liste, int *m);
Liste_Diviseur *enlever_doublon(Liste_Diviseur *liste, int taille_liste, int *taille_sans_doublon);

// Operations pour recuperer les valeurs ap,bp,cp aq,bq,cq
void calcul_produit(mpz_t s, Liste_Diviseur *liste, int taille_liste);
void theoreme_reste_chinois(Liste_Diviseur *liste, int taille_liste, int m, mpz_t s, mpz_t x);
void theoreme_reste_chinois_simplifie(mpz_t x, mpz_t a1, mpz_t a2, mpz_t r1, mpz_t r2);

// Attaque et verification du fonctionnement
int attaque_spa(Liste_Diviseur *pliste, Liste_Diviseur *qliste, int ptaille_liste, int qtaille_liste, mpz_t s, mpz_t cp, mpz_t cq, int m1, int m2, mpz_t n);
int verification(mpz_t premier, mpz_t s, mpz_t x);
