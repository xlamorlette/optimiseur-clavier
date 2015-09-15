#ifndef _OPTIMISATION_
#define _OPTIMISATION_

#include <map>
#include <boost/multiprecision/cpp_int.hpp>

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

        static void ChoisitCaractere(Repartition & ioMeilleureRepartition,
                Repartition & ioRepartitionEnCours,
                int iNbCaracteresChoisis,
                int iNbCaracteresDisponibles,
                std::map<char, int>::const_iterator itStatCar,
                std::map<char, int>::const_iterator itStatCarEnd,
                boost::multiprecision::uint128_t iNbTotalCombinaisons,
                boost::multiprecision::uint128_t & ioNbCombinaisonsTestees,
                bool iDebogage = false);

};

#endif
