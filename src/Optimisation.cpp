#include "Optimisation.hpp"

#include "Caracteres.hpp"
#include "Repartition.hpp"
#include "Statistiques.hpp"

using namespace std;

void Optimisation::optimise(Repartition & ioRepartition,
        bool iDebogage)
{
    const auto & statistiquesCaracteres = ioRepartition.statistiques().statistiquesCaracteres();
    for (const auto & clefValeur: statistiquesCaracteres)
    {
        char caractere1 = clefValeur.first;
        for (const auto & clefValeur2: statistiquesCaracteres)
        {
            char caractere2 = clefValeur2.first;
            if (caractere1 == caractere2)
            {
                continue;
            }
            if (iDebogage)
            {
                cout << "Test l'\u00E9change des caract\u00E8res ";
                Caracteres::AfficheCaractereDenormalise(caractere1, cout);
                cout << " et " ;
                Caracteres::AfficheCaractereDenormalise(caractere2, cout);
                cout << endl;
            }
        }
    }
}
