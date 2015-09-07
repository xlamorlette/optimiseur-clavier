#ifndef _STATISTIQUES_
#define _STATISTIQUES_

#include <array>
#include <iostream>
#include <map>
#include <vector>

//! Classe contenant les statistiques d'apparation des caractères et bigrammes
class Statistiques
{
        friend std::ostream & operator << (std::ostream & ioStream,
            const Statistiques & iObjet);

    public:
        Statistiques() = delete;
        Statistiques & operator = (const Statistiques &) = delete;
        Statistiques(const Statistiques &) = delete;

        virtual ~Statistiques() = default;

        //! Constructeur qui lit les statistiques d'apparation des caractères et bigrammes depuis des fichiers
        /*!
         * Peut importe l'ordre dans les fichiers et le nombre de fichiers.
         * À la fin de la lecture des fichiers, les fréquences d'apparation sont normalisées.
         * \param iFichiers liste des fichiers à lire
         */
        Statistiques(std::vector<std::string> & iFichiers);

        const std::map<char, int> & statistiquesCaracteres() const
        {
            return _caracteres;
        }

        int frequenceCaractere(char iCaractere) const
        {
            return _caracteres.at(iCaractere);
        }

        const std::map<std::array<char, 2>, int> & statistiquesBigrammes() const
        {
            return _bigrammes;
        }

        int sommeFrequencesCaracteres() const
        {
            return _sommeFrequencesCaracteres;
        }

        int sommeFrequencesBigrammes() const
        {
            return _sommeFrequencesBigrammes;
        }

        int sommeTotale() const
        {
            return _sommeFrequencesCaracteres + _sommeFrequencesBigrammes;
        }


    private:
        //! Statistiques d'apparation des caractères (normalisées pour un total à 1000)
        std::map<char, int> _caracteres;

        //! Statistiques d'apparation des bigrammes (normalisées pour un total à 1000)
        std::map<std::array<char, 2>, int> _bigrammes;

        //! Sommes totales des fréquences des caractères et des bigrammes
        int _sommeFrequencesCaracteres{0};
        int _sommeFrequencesBigrammes{0};

        //! Méthode appelée par le constructeur pour extraire les statistiques depuis des fichiers
        void litFichiers(std::vector<std::string> & iFichiers);

        //! Méthode appelée par le constructeur pour normaliser les fréquences afin que leurs sommes soient 1000
        /*!
         * Étant donné les erreurs d'arrondis, on calcule également les sommes exactes des fréquences, après normalisation
         */
        void normaliseFrequences();
};

#endif
