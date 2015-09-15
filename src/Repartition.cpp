#include "Repartition.hpp"

#include "Caracteres.hpp"
#include "Statistiques.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace std;


Repartition::Repartition(const Statistiques & iStatistiques):
        _pStatistiques{&iStatistiques}
{
    // répartition bête : on prend les caractères dans l'ordre qu'ils viennent
    Main main = Main::GAUCHE;
    for (const auto & clefValeur: _pStatistiques->statistiquesCaracteres())
    {
        _repartition[clefValeur.first] = main;
        main = (main == Main::GAUCHE) ? Main::DROITE : Main::GAUCHE;
    }
    calculeScore();
}

Repartition::Repartition(const Repartition & iObjet):
        _scoreRepartitionCaracteres{iObjet._scoreRepartitionCaracteres},
        _scoreRepartitionBigrammes{iObjet._scoreRepartitionBigrammes},
        _scoreSubsidiaire{iObjet._scoreSubsidiaire},
        _scoresAJour{iObjet._scoresAJour},
        _pStatistiques{iObjet._pStatistiques}
{
    _repartition.insert(iObjet._repartition.begin(), iObjet._repartition.end());
}


Repartition & Repartition::operator = (const Repartition & iObjet)

{
    _repartition.clear();
    _repartition.insert(iObjet._repartition.begin(), iObjet._repartition.end());
    _scoreRepartitionCaracteres = iObjet._scoreRepartitionCaracteres;
    _scoreRepartitionBigrammes = iObjet._scoreRepartitionBigrammes;
    _scoreSubsidiaire = iObjet._scoreSubsidiaire;
    _scoresAJour = iObjet._scoresAJour;
    _pStatistiques = iObjet._pStatistiques;
    return *this;
}


void Repartition::InitialisationEquilibree()
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
    ioStream << "    score subsidiaire : " << iObjet._scoreSubsidiaire << endl;
    return ioStream;
}


void Repartition::calculeScore()
{
    // score de répartition des caractères entre les deux mains
    // = somme totale des fréquences - | différence entre les sommes des deux mains |`
    array<int, 2> sommeFrequencesParMain{0, 0};
    array<int, 2> sommeFrequencesCarreesParMain{0, 0};
    array<int, 2> nbLettresParMain{0, 0};
    for (const auto & clefValeur: _repartition)
    {
        if (clefValeur.second == Main::NON_ASSIGNE)
        {
            throw logic_error("Lettre non assignee");
        }
        int indexMain = (clefValeur.second == Main::GAUCHE) ? 0 : 1;
        int frequence = _pStatistiques->frequenceCaractere(clefValeur.first);
        sommeFrequencesParMain[indexMain] += frequence;
        sommeFrequencesCarreesParMain[indexMain] += frequence * frequence;
        nbLettresParMain[indexMain] ++;
    }
    if (nbLettresParMain[0] != nbLettresParMain[1])
    {
        throw logic_error("Repartition desequilibree");
    }
    _scoreRepartitionCaracteres = _pStatistiques->sommeFrequencesCaracteres() - abs(sommeFrequencesParMain[0] - sommeFrequencesParMain[1]);
    _scoreSubsidiaire = abs(sommeFrequencesCarreesParMain[0] - sommeFrequencesCarreesParMain[1]);

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


int Repartition::scoreSubsidiaire() const
{
    if (! _scoresAJour)
    {
        throw logic_error("Score non a jour");
    }
    return _scoreSubsidiaire;
}


bool Repartition::intervertitCaracteres(char caractere1,
        char caractere2)
{
    if (_repartition.at(caractere1) == _repartition.at(caractere2))
    {
        return false;
    }
    Main mainInitiale = _repartition.at(caractere1);
    _repartition[caractere1] = _repartition.at(caractere2);
    _repartition[caractere2] = mainInitiale;
    _scoresAJour = false;
    return true;
}


void Repartition::assigneCaractere(char iCaractere,
        Main iMain)
{
    _repartition[iCaractere] = iMain;
    _scoresAJour = false;
}


void Repartition::reinitialise()
{
    for (auto & clefValeur: _repartition)
    {
        clefValeur.second = Main::NON_ASSIGNE;
    }
    _scoresAJour = false;
}


void Repartition::finitAssignationCaracteres(Main iMain)
{
    for (auto & clefValeur: _repartition)
    {
        if (clefValeur.second == Main::NON_ASSIGNE)
        {
            clefValeur.second = iMain;
        }
    }
    _scoresAJour = false;
}
