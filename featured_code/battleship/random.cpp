#include <iostream>

#include "random.h"

/** Pseudo Random integer generator by George Marsaglia**/

int m_w = 521288629;
int m_z = 362436069;

unsigned int randomInteger()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;
}

/** Seeds **/

void setSeed (int seed)
{
    // deterministic starting point
    m_w = 521288629 + seed;
    m_z = 362436069;
}

void setSeed (int a, int b)
{
    // deterministic starting point
    m_w = a;
    m_z = b;
}

void randomStatus ()
{
    std::cout << "m_w: " << m_w << "\nm_z: " << m_z << "\n";
}

void setRandomSeed () {
    m_w = getTime();
}

int getTime () {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


/** Derived random variables **/

double randomUniform ()
{
    return (((double) randomInteger()) + 1.0) * 2.328306435454494e-10;
}

double randomUniform (double u, double l)
{
    return u + (l - u) * randomUniform();
}

int randomDiscrete (int l, int u)
{
    return l + randomInteger() % (u - l);
}

bool randomBernoulli (double p)
{
    return (randomUniform() <= p);
}
