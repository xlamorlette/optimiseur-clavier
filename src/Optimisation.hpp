#ifndef _OPTIMISATION_
#define _OPTIMISATION_

class Repartition;

//! Classe contenant les algorithmes d'optimisation de la répartition
class Optimisation
{
    public:
        static void ProcheEnProche(Repartition & ioRepartition,
                int iChoixRepartitionInitiale,
                bool iDebogage = false);

        static void Exhaustif(Repartition & ioRepartition,
                bool iDebogage = false);
};

#endif
