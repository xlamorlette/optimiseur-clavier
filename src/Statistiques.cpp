#include "Statistiques.hpp"

#include "Caracteres.hpp"

#include <fstream>
#include <sstream>

using namespace std;


Statistiques::Statistiques(vector<string> & iFichiers)
{
    litFichiers(iFichiers);
    normaliseFrequences();
    // on ne gere qu'un nombre pair de caracteres a repartir entre les deux mains
    if (_caracteres.size() % 2)
    {
        cerr << "Ce programme ne supporte pas un nombre impair de caracteres (" << _caracteres.size() << ")" << endl;
        exit(-1);
    }
}

void Statistiques::litFichiers(vector<string> & iFichiers)
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
            int frequence = std::stoi(element2);
            if ((element1.length() > 2) || (frequence < 0) || (element3 != ""))
            {
                cerr << "Ligne invalide dans le fichier " << nomFichier << " : " << endl;
                cerr << ligne << endl;
                exit(-1);
            }
            if (element1.length() == 1)
            {
                _caracteres[element1[0]] = frequence;
            }
            else
            {
                _bigrammes[array<char, 2>{element1[0], element1[1]}] = frequence;
            }
        }
        fichier.close();
    }
}

void Statistiques::normaliseFrequences()
{
    // normalise les frequences d'apparition des caractères
    int sommeAvantNormalisation = 0;
    for (const auto & clefValeur: _caracteres)
    {
        sommeAvantNormalisation += clefValeur.second;
    }
    _sommeFrequencesCaracteres = 0;
    for (auto & clefValeur: _caracteres)
    {
        clefValeur.second = static_cast<int>(static_cast<float>(clefValeur.second) * 1000 / sommeAvantNormalisation + 0.5);
        _sommeFrequencesCaracteres += clefValeur.second;
    }

    // normalise les frequences d'apparition des bigrammes
    sommeAvantNormalisation = 0;
    for (const auto & clefValeur: _bigrammes)
    {
        sommeAvantNormalisation += clefValeur.second;
    }
    _sommeFrequencesBigrammes = 0;
    for (auto & clefValeur: _bigrammes)
    {
        clefValeur.second = static_cast<int>(static_cast<float>(clefValeur.second) * 1000 / sommeAvantNormalisation + 0.5);
        _sommeFrequencesBigrammes += clefValeur.second;
    }
}


ostream & operator << (ostream & ioStream,
        const Statistiques & iObjet)
{
    ioStream << "caracteres :" << endl;
    for (const auto & clefValeur: iObjet._caracteres)
    {
        ioStream << "    ";
        Caracteres::AfficheCaractereDenormalise(clefValeur.first, ioStream);
        ioStream << " : " << clefValeur.second << endl;
    }
    ioStream << "    total : " << iObjet._sommeFrequencesCaracteres << endl;

    ioStream << "bigrammes :" << endl;
    for (const auto & clefValeur: iObjet._bigrammes)
    {
        ioStream << "    ";
        Caracteres::AfficheBigrammeDenormalise(clefValeur.first, ioStream);
        ioStream << " : " << clefValeur.second << std::endl;
    }
    ioStream << "    total : " << iObjet._sommeFrequencesBigrammes << endl;
    return ioStream;
}

