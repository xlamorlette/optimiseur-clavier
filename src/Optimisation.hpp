#ifndef _OPTIMISATION_
#define _OPTIMISATION_

class Repartition;

//! Classe contenant les algorithmes d'optimisation de la r�partition
class Optimisation
{
    public:
        static void ProcheEnProche(Repartition & ioRepartition,
                bool iDebogage = false);
};

#endif