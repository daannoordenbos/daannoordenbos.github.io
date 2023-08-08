#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <chrono>

/** Auxiliary **/
void            setSeed         (int);
void            setSeed         (int, int);
void            randomStatus    ();
void            setRandomSeed   ();
int             getTime         ();
unsigned int    randomInteger   ();

/** Univariate Distributions **/
double  randomUniform           ();
double  randomUniform           (double l, double u);
int     randomDiscrete          (int l, int u);
bool    randomBernoulli         (double p);


#endif // RANDOM_H_INCLUDED
