#ifndef _OPTIMISATION_
#define _OPTIMISATION_

class Repartition;

//! Classe contenant les algorithmes d'optimisation de la répartition
class Optimisation
{
    public:
        static void optimise(Repartition & ioRepartition,
                bool iDebogage = false);
};

#endif
