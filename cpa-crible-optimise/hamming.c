#include "hamming.h"

/**
 * @param nombre entier dont on souhaite determiner son poids de Hamming
 * @return nombre de 1 dans le nombre
 */
unsigned int poids_hamming(unsigned int nombre)
{
    unsigned int taille_entier = sizeof(unsigned int) * 8;
    unsigned int poids_hamming = 0;
    for (int i = 0; i < taille_entier; i++)
    {
        poids_hamming += (nombre & 1);
        nombre = nombre >> 1;
    }
    return poids_hamming;
}