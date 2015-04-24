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

    private:
        //! Statistiques d'apparation des caractères (normalisées pour un total à 1000)
        std::map<char, int> _caracteres;

        //! Statistiques d'apparation des bigrammes (normalisées pour un total à 1000)
        std::map<std::array<char, 2>, int> _bigrammes;
};

#endif
