#include "Statistiques.hpp"
#include "Caracteres.hpp"

#include <boost/program_options.hpp>
#include <fcntl.h>

using namespace std;
namespace po = boost::program_options;

int main(int argc,
        char **argv)
{
    try
    {
        vector<string> fichiersStatistiques;
        po::options_description optionsDescription("Options autorisees");
        optionsDescription.add_options()
                ("aide,a", "aide sur les options")
                ("debogage,d", "active le mode d\u00E9bogage")
                ("statistiques,s", po::value<vector<string>>(&fichiersStatistiques)->multitoken()->required(),
                        "fichiers contenant les statistiques d'apparition des lettres et des bigrammes " \
                        "(peut importe le nombre de fichiers, leur ordre et si les contenus sont melanges)");

        po::variables_map variablesMap;
        po::store(po::parse_command_line(argc, argv, optionsDescription), variablesMap);

        if (variablesMap.count("aide"))
        {
            cout << optionsDescription << endl;
            return 0;
        }

        po::notify(variablesMap);
        bool debogage = variablesMap.count("debogage") ? true : false;
        Statistiques stats(fichiersStatistiques);
        if (debogage)
        {
            cout << "Table de normalisation des caract\u00E8res :" << endl;
            Caracteres::AfficheTables(cout);
            cout << "Statistiques : " << endl;
            cout << stats;
        }
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
