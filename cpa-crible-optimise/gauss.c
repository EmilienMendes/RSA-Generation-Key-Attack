#include "gauss.h"


/**
 * @param sigma ecart type
 * @return Nombre suivant la loi de gauss avec une esperance de 0
 */
double gauss(double sigma)
{
  static double x0, x1;
  static unsigned int nb_ready;

  double u, v, w, z;

  if (nb_ready == 0)
  {
    u = ((double) rand()) / RAND_MAX;
    v = ((double) rand()) / RAND_MAX;
    w = sqrt(-2.0 * log(u));
    z = 2 * M_PI * v;
    x0 = w * cos(z);
    x1 = w * sin(z);
    nb_ready = 1;
    return x0*sigma;
  }
  else
  {
    nb_ready = 0;
    return x1*sigma;
  }
}

