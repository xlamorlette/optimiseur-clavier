#include "Repartition.hpp"

#include "Caracteres.hpp"
#include "Statistiques.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace std;


Repartition::Repartition(const Statistiques & iStatistiques,
        bool iRepartitionEquilibree):
        _pStatistiques{&iStatistiques}
{
    if (iRepartitionEquilibree)
    {
        // on trie d'abord les caractères par ordre décroissant de fréquence
        vector<pair<int, char>> caracteresTries;
        for (const auto & clefValeur: _pStatistiques->statistiquesCaracteres())
        {
            caracteresTries.push_back(pair<int, char>(clefValeur.second, clefValeur.first));
        }
        sort(caracteresTries.begin(), caracteresTries.end());
        reverse(caracteresTries.begin(), caracteresTries.end());

        // puis on fait la répartition:
        // G D D G G D D G G etc.
        Main mainPrecedente = Main::GAUCHE;
        Main main = Main::GAUCHE;
        for (const auto & paire: caracteresTries)
        {
            _repartition[paire.second] = main;
            if (main == mainPrecedente)
            {
                main = (main == Main::GAUCHE) ? Main::DROITE : Main::GAUCHE;
            }
            else
            {
                mainPrecedente = main;
            }
        }
    }
    else
    {
        // répartition bête : on prend les caractères dans l'ordre qu'ils viennent
        Main main = Main::GAUCHE;
        for (const auto & clefValeur: _pStatistiques->statistiquesCaracteres())
        {
            _repartition[clefValeur.first] = main;
            main = (main == Main::GAUCHE) ? Main::DROITE : Main::GAUCHE;
        }
    }
    calculeScore();
}


ostream & operator << (ostream & ioStream,
        const Repartition & iObjet)
{
    ioStream << "Repartition :" << endl;

    // on sépare les caractères par main, et on les trie par fréquences décroissantes
    array<vector<pair<int, char>>, 2> mains;
    for (const auto & clefValeur: iObjet._repartition)
    {
        int indexMain = (clefValeur.second == Repartition::Main::GAUCHE) ? 0 : 1;
        mains[indexMain].push_back(pair<int, char>(iObjet._pStatistiques->frequenceCaractere(clefValeur.first), clefValeur.first));
    }
    for (int indexMain: {0, 1})
    {
        sort(mains[indexMain].begin(), mains[indexMain].end());
        reverse(mains[indexMain].begin(), mains[indexMain].end());
        ioStream << "    main " << indexMain << " :";
        for (const auto & paire: mains[indexMain])
        {
            ioStream << " ";
            Caracteres::AfficheCaractereDenormalise(paire.second, ioStream);
            ioStream << "(" << paire.first << ")";
        }
        ioStream << endl;
    }

    // on regarde quels sont les bigrammes sur la meme main, ou bien alternant les mains, et on les trie par fréquences décroissantes
    // 0: même main, 1: mains différentes
    array<vector<pair<array<char, 2>, int>>, 2> bigrammesParAlternance;
    for (const auto & bigramme: iObjet._pStatistiques->statistiquesBigrammes())
    {
        if (iObjet._repartition.at(bigramme.first[0]) == iObjet._repartition.at(bigramme.first[1]))
        {
            bigrammesParAlternance[0].push_back(bigramme);
        }
        else
        {
            bigrammesParAlternance[1].push_back(bigramme);
        }
    }
    for (int indexAlternance: {0, 1})
    {
        // trie par ordre décroissant sur la deuxième valeur des paires
        sort(bigrammesParAlternance[indexAlternance].begin(), bigrammesParAlternance[indexAlternance].end(),
                [](const pair<array<char, 2>, int> & iPremier, const pair<array<char, 2>, int> & iSecond) -> bool {
                    return iPremier.second > iSecond.second;});
        if (indexAlternance == 0)
        {
            ioStream << "    bigrammes sur la meme main :";
        }
        else
        {
            ioStream << "    bigrammes alternant les mains :";
        }
        for (const auto & paire: bigrammesParAlternance[indexAlternance])
        {
            ioStream << " ";
            Caracteres::AfficheBigrammeDenormalise(paire.first, ioStream);
            ioStream << "(" << paire.second << ")";
        }
        ioStream << endl;
    }

    ioStream << "    score repartition des caracteres : " << iObjet._scoreRepartitionCaracteres
            << " / " << iObjet._pStatistiques->sommeFrequencesCaracteres() << endl;
    ioStream << "    score repartition des bigrammes : " << iObjet._scoreRepartitionBigrammes
            <<  " / " << iObjet._pStatistiques->sommeFrequencesBigrammes() << endl;
    ioStream << "    score total : " << iObjet._scoreRepartitionCaracteres + iObjet._scoreRepartitionBigrammes
            <<  " / " << iObjet._pStatistiques->sommeTotale() << endl;
    return ioStream;
}


void Repartition::calculeScore()
{
    // score de répartition des caractères entre les deux mains
    // = somme totale des fréquences - | différence entre les sommes des deux mains |`
    array<int, 2> sommeFrequencesParMain{0, 0};
    for (const auto & clefValeur: _repartition)
    {
        int indexMain = (clefValeur.second == Repartition::Main::GAUCHE) ? 0 : 1;
        sommeFrequencesParMain[indexMain] += _pStatistiques->frequenceCaractere(clefValeur.first);
    }
    _scoreRepartitionCaracteres = _pStatistiques->sommeFrequencesCaracteres()
            - abs(sommeFrequencesParMain[0] - sommeFrequencesParMain[1]);


    // score de répartition des bigrammes
    // = somme des fréquences des bigrammes alternant les deux mains
    _scoreRepartitionBigrammes = 0;
    for (const auto & bigramme: _pStatistiques->statistiquesBigrammes())
    {
        if (_repartition.at(bigramme.first[0]) != _repartition.at(bigramme.first[1]))
        {
            _scoreRepartitionBigrammes += bigramme.second;
        }
    }

    _scoresAJour = true;
}


int Repartition::score() const
{
    if (! _scoresAJour)
    {
        throw logic_error("Score non a jour");
    }
    return _scoreRepartitionCaracteres + _scoreRepartitionBigrammes;
}

