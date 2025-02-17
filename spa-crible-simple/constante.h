#ifndef __CONSTANTE_H__
#define __CONSTANTE_H__

#define TRUE 1
#define FALSE 0

/*
 Valeur de consommation de courant pour chacune des lignes
 Valeur choisie aleatoirement apres plusieurs essais pour
 obtenir des valeurs coherentes
*/
typedef enum
{
   LIGNE1 = 5,
   LIGNE2 = 15,
   LIGNE3 = 20,
   LIGNE4 = 10,
   LIGNE5 = 7,
   LIGNE6 = 25,
   LIGNE7 = 10,
   LIGNE8 = 25
} VALEUR_COURANT;
/*
 Recuperation dans une structure l'arret du j eme tour
 de l'algorithme par la division de vj par r
*/
typedef struct
{
   unsigned int j;
   unsigned int r;
} Liste_Diviseur;

#endif