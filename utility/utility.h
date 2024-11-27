#include "gmp.h"
#include "stdlib.h"




// Fonction pour la liste des premiers nombres premiers
mpz_t *generation_liste_nombres_premiers(int N);
void free_liste(int taille,mpz_t *liste);