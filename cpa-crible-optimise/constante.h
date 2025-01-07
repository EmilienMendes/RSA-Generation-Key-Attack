#ifndef __CONSTANTE_H__
#define __CONSTANTE_H__

#define TRUE 1
#define FALSE 0
#define MAX_COMBINAISON 1000000
#define TRESHOLD_COMPARAISON_FLOAT 0.001
/*
  Mesure de la trace obtenu
*/
typedef struct
{
  float **tab;
  unsigned int x;
  unsigned int y;
} Tableau;

typedef struct{
  unsigned int *l;
  unsigned int taille;
}Liste;

#endif