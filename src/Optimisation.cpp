#include "Optimisation.hpp"

#include "Caracteres.hpp"
#include "Repartition.hpp"
#include "Statistiques.hpp"

using namespace std;
using namespace boost::multiprecision;


void Optimisation::ProcheEnProche(Repartition & ioRepartition,
        int iChoixRepartitionInitiale,
        bool iDebogage)
{
    // répartition initiale
    switch (iChoixRepartitionInitiale)
    {
        case 1:
            break;
        case 2:
            ioRepartition.InitialisationEquilibree();
            break;
        default:
            throw logic_error("Valeur invalide pour le choix de la r\u00E9partition initiale");
    }
    cout << "\nR\u00E9partition initiale :\n" << ioRepartition;

    // recherche de proche en proche
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


void Optimisation::ChoisitCaractere(Repartition & ioMeilleureRepartition,
        Repartition & ioRepartitionEnCours,
        int iNbCaracteresChoisis,
        int iNbCaracteresDisponibles,
        std::map<char, int>::const_iterator itStatCar,
        std::map<char, int>::const_iterator itStatCarEnd,
        boost::multiprecision::uint128_t iNbTotalCombinaisons,
        boost::multiprecision::uint128_t & ioNbCombinaisonsTestees,
        bool iDebogage)
{
    int nbCaracteresAChoisir = ioRepartitionEnCours.statistiques().statistiquesCaracteres().size() / 2;
    if (iNbCaracteresChoisis < nbCaracteresAChoisir)
    {
        iNbCaracteresChoisis++;
        iNbCaracteresDisponibles--;
        for (; (itStatCar != itStatCarEnd) && (nbCaracteresAChoisir - iNbCaracteresChoisis <= iNbCaracteresDisponibles); ++itStatCar, iNbCaracteresDisponibles--)
        {
            ioRepartitionEnCours.assigneCaractere(itStatCar->first, Repartition::Main::GAUCHE);
            Optimisation::ChoisitCaractere(ioMeilleureRepartition, ioRepartitionEnCours, iNbCaracteresChoisis, iNbCaracteresDisponibles,
                    next(itStatCar), itStatCarEnd, iNbTotalCombinaisons, ioNbCombinaisonsTestees, iDebogage);
            ioRepartitionEnCours.assigneCaractere(itStatCar->first, Repartition::Main::NON_ASSIGNE);
        }
    }
    else
    {
        ioNbCombinaisonsTestees++;
        ioRepartitionEnCours.finitAssignationCaracteres(Repartition::Main::DROITE);
        ioRepartitionEnCours.calculeScore();
        if (iDebogage)
        {
            cout << ioRepartitionEnCours;
            cout << "Nombre de combinaisons test\u00E9es : " << ioNbCombinaisonsTestees << " / " << iNbTotalCombinaisons << endl;
        }
        if ((ioRepartitionEnCours.score() > ioMeilleureRepartition.score())
                || ((ioRepartitionEnCours.score() == ioMeilleureRepartition.score())
                        && (ioRepartitionEnCours.scoreSubsidiaire() < ioMeilleureRepartition.scoreSubsidiaire())))
        {
            cout << "Meilleure r\u00E9partition trouv\u00E9e :\n" << ioRepartitionEnCours;
            ioMeilleureRepartition = ioRepartitionEnCours;
        }
        int pourcentage = static_cast<int>(ioNbCombinaisonsTestees * 100 / iNbTotalCombinaisons);
        int pourcentagePrecedent = static_cast<int>((ioNbCombinaisonsTestees - 1) * 100 / iNbTotalCombinaisons);
        if (pourcentage > pourcentagePrecedent)
        {
            cout << "Proportion des combinaisons test\u00E9es : " << pourcentage << "%" << endl;
        }
    }
}


uint128_t factorial(int n)
{
   uint128_t result = 1;
   for (int i=n; i>1; --i)
   {
       result *= i;
   }
   return result;
}

uint128_t square(uint128_t n)
{
    return n * n;
}

void Optimisation::Exhaustif(Repartition & ioRepartition,
        bool iDebogage)
{
    // pour parcourir l'ensemble des répartitions possibles :
    // on choisit la moitié des caractères à mettre dans une main
    // l'autre moitié est assignée à l'autre main
    // par symétrie, on fixe le premier caractère

    // nombre de combinaisons possible
    int nbCaracteres = ioRepartition.statistiques().statistiquesCaracteres().size();
    uint128_t nbTotalCombinaisons = factorial(nbCaracteres) / square(factorial(nbCaracteres / 2)) / 2;
    uint128_t nbCombinaisonsTestees = 0;
    cout << "Nombre de combinaisons \u00E0 parcourir : " << nbTotalCombinaisons << endl;

    Repartition repartitionEnCours(ioRepartition);
    repartitionEnCours.reinitialise();
    const std::map<char, int> & statistiquesCaracteres = ioRepartition.statistiques().statistiquesCaracteres();
    std::map<char, int>::const_iterator itStatCar = statistiquesCaracteres.begin();
    repartitionEnCours.assigneCaractere(itStatCar->first, Repartition::Main::GAUCHE);
    itStatCar++;
    Optimisation::ChoisitCaractere(ioRepartition, repartitionEnCours, 1, statistiquesCaracteres.size() - 1, itStatCar, statistiquesCaracteres.end(),
            nbTotalCombinaisons, nbCombinaisonsTestees, iDebogage);
}
