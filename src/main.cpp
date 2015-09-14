#include "Caracteres.hpp"
#include "Optimisation.hpp"
#include "Repartition.hpp"
#include "Statistiques.hpp"

#include <boost/program_options.hpp>
#include <fcntl.h>
#include <stdexcept>

using namespace std;
namespace po = boost::program_options;


int main(int argc,
        char **argv)
{
    try
    {
        vector<string> fichiersStatistiques;
        int choixRepartitionInitiale;
        int choixAlgorithme;
        po::options_description optionsDescription("Options autorisees");
        optionsDescription.add_options()
                ("aide,a", "aide sur les options")
                ("debogage,d", "active le mode d\u00E9bogage")
                ("statistiques,s", po::value<vector<string>>(&fichiersStatistiques)->multitoken()->required(),
                        "fichiers contenant les statistiques d'apparition des caract\u00E8res et des bigrammes " \
                        "(peut importe le nombre de fichiers, leur ordre et si les contenus sont melang\u00E9s)")
                ("algorithme,g", po::value<int>(&choixAlgorithme)->default_value(1), "algorithme d'optimisation :\n" \
                        "1 : recherche de proche en proche \u00E0 partir d'une r\u00E9partition initiale\n"
                        "2 : recherche exhaustive")
                ("initiale,i", po::value<int>(&choixRepartitionInitiale)->default_value(1), "r\u00E9partition initiale :\n" \
                        "1 : dans l'ordre d'apparition des caract\u00E8res dans les statistiques, alternativement dans chaque main\n" \
                        "2 : \u00E9quilibrage simple selon les statistiques d'apparition des caract\u00E8res");

        po::variables_map variablesMap;
        po::store(po::parse_command_line(argc, argv, optionsDescription), variablesMap);

        if (variablesMap.count("aide"))
        {
            cout << optionsDescription << endl;
            return 0;
        }

        po::notify(variablesMap);
        bool debogage = variablesMap.count("debogage") ? true : false;

        // --- analyse les fichiers de statistiques ---
        Statistiques stats(fichiersStatistiques);
        if (debogage)
        {
            cout << "Table de normalisation des caract\u00E8res :" << endl;
            Caracteres::AfficheTables(cout);
            cout << "Statistiques : " << endl;
            cout << stats;
        }

        Repartition repartition(stats);

        switch (choixAlgorithme)
        {
            case 1:
                Optimisation::Exhaustif(repartition, debogage);
                break;
            case 2:
                Optimisation::ProcheEnProche(repartition, choixRepartitionInitiale, debogage);
                break;
            default:
                throw logic_error("Valeur invalide pour le choix de l'algorithme");
        }


        cout << "\nR\u00E9sultat de l'optimisation :\n" << repartition;
    }
    catch (exception & exception)
    {
        cerr << "Erreur : " << exception.what() << endl;
        return -1;
    }
    catch (...)
    {
        cerr << "Erreur inconnue !" << endl;
        return -1;
    }
    return 0;
}
