#include "Optimisation.hpp"

#include "Caracteres.hpp"
#include "Repartition.hpp"
#include "Statistiques.hpp"

using namespace std;

void Optimisation::ProcheEnProche(Repartition & ioRepartition,
        bool iDebogage)
{
    int scoreReference = ioRepartition.score();
    const auto & statistiquesCaracteres = ioRepartition.statistiques().statistiquesCaracteres();
    bool auMoinsUneInversion = true;
    int nombreBoucles = 1;
    while (auMoinsUneInversion)
    {
        auMoinsUneInversion = false;
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
                bool interversionFaite = ioRepartition.intervertitCaracteres(caractere1, caractere2);
                if (interversionFaite)
                {
                    if (iDebogage)
                    {
                        cout << "Test l'interversion des caract\u00E8res ";
                        Caracteres::AfficheCaractereDenormalise(caractere1, cout);
                        cout << " et " ;
                        Caracteres::AfficheCaractereDenormalise(caractere2, cout);
                        cout << endl;
                    }
                    ioRepartition.calculeScore();
                    int nouveauScore = ioRepartition.score();
                    if (nouveauScore > scoreReference)
                    {
                        auMoinsUneInversion = true;
                        scoreReference = nouveauScore;
                        if (iDebogage)
                        {
                            cout << "Augmentation du score : " << scoreReference << endl;
                        }
                    }
                    else
                    {
                        ioRepartition.intervertitCaracteres(caractere1, caractere2);
                        if (iDebogage)
                        {
                            cout << "Pas d'augmentation du score" << endl;
                        }
                    }
                }
            }
        }
        if (iDebogage)
        {
            cout << "\nR\u00E9partition \u00E0 l'issue de la boucle " << nombreBoucles << " :\n" << ioRepartition;
        }
        nombreBoucles ++;
    }
}
