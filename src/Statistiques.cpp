#include "Statistiques.hpp"

#include "Caracteres.hpp"

#include <fstream>
#include <sstream>

using namespace std;


Statistiques::Statistiques(vector<string> & iFichiers)
{
    for (const auto & nomFichier: iFichiers)
    {
        ifstream fichier(nomFichier);
        if (! fichier.is_open())
        {
            cerr << "Erreur lors de l'ouverture du fichier " << nomFichier << endl;
            exit(-1);
        }
        while (! fichier.eof())
        {
            string ligne;
            getline(fichier, ligne);

            // ignore les lignes de commentaires
            if ((ligne.length() <= 0) || (ligne[0] == '#'))
            {
                continue;
            }

            // analyse la ligne
            istringstream iss(ligne);
            string element1;
            string element2;
            string element3;
            iss >> element1 >> element2 >> element3;
            Caracteres::Normalise(element1);
            if ((element1.length() > 2) || (element3 != ""))
            {
                cerr << "Ligne invalide dans le fichier " << nomFichier << " : " << endl;
                cerr << ligne << endl;
                exit(-1);
            }
            if (element1.length() == 1)
            {
                //cout << "'" << element1[0] << "' : " << (int)element1[0] << endl;
                _caracteres[element1[0]] = 1;
            }
            else
            {
                //cout << "'" << element1 << "' : " << (int)element1[0] << " - " << (int)element1[1] << endl;
            }
        }
        fichier.close();
    }

    //std::map<char, int> _caracteres;
    //std::map<std::array<char, 2>, int> _bigrammes;
    _bigrammes[array<char, 2> {'a', 'b'}] = 10;
    _bigrammes[array<char, 2> {'a', 'c'}] = 20;
}


ostream & operator << (ostream & ioStream,
        const Statistiques & iObjet)
{
    ioStream << "caracteres :" << endl;
    ioStream << iObjet._caracteres;
    ioStream << "bigrammes :" << endl;
    ioStream << iObjet._bigrammes;
    return ioStream;
}

