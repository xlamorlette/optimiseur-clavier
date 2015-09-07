#ifndef _STATISTIQUES_
#define _STATISTIQUES_

#include <array>
#include <iostream>
#include <map>
#include <vector>

//! Classe contenant les statistiques d'apparation des caract�res et bigrammes
class Statistiques
{
        friend std::ostream & operator << (std::ostream & ioStream,
            const Statistiques & iObjet);

    public:
        Statistiques() = delete;
        Statistiques & operator = (const Statistiques &) = delete;
        Statistiques(const Statistiques &) = delete;

        virtual ~Statistiques() = default;

        //! Constructeur qui lit les statistiques d'apparation des caract�res et bigrammes depuis des fichiers
        /*!
         * Peut importe l'ordre dans les fichiers et le nombre de fichiers.
         * � la fin de la lecture des fichiers, les fr�quences d'apparation sont normalis�es.
         * \param iFichiers liste des fichiers � lire
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
        //! Statistiques d'apparation des caract�res (normalis�es pour un total � 1000)
        std::map<char, int> _caracteres;

        //! Statistiques d'apparation des bigrammes (normalis�es pour un total � 1000)
        std::map<std::array<char, 2>, int> _bigrammes;

        //! Sommes totales des fr�quences des caract�res et des bigrammes
        int _sommeFrequencesCaracteres{0};
        int _sommeFrequencesBigrammes{0};

        //! M�thode appel�e par le constructeur pour extraire les statistiques depuis des fichiers
        void litFichiers(std::vector<std::string> & iFichiers);

        //! M�thode appel�e par le constructeur pour normaliser les fr�quences afin que leurs sommes soient 1000
        /*!
         * �tant donn� les erreurs d'arrondis, on calcule �galement les sommes exactes des fr�quences, apr�s normalisation
         */
        void normaliseFrequences();
};

#endif
