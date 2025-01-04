#ifndef __GAUSS_H__
#define __GAUSS_H
#include <math.h>
// https://stackoverflow.com/questions/26065359/m-pi-flagged-as-undeclared-identifier
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include <stdlib.h>
#include <stdio.h>


double gauss(double sigma);
#endif