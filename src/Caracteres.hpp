#ifndef _CARACTERES_
#define _CARACTERES_

#include <iostream>
#include <string>

#include <boost/bimap.hpp>

//! Classe contenant des fonctions utilitaires pour tranformer les caractères
/*!
 * Il est nécessaire de transformer bijectivement les caractères unicode utilisés en caractère ANSI afin de simplifier les algorithmes
 * et de permettre l'utilisation de maps indexées par des char.
 * Pour cela on choisit des équivalents parmis les lettres ANSI majuscules.
 * Cela implique donc de ne travailler qu'avec des lettres minuscules.
 */
class Caracteres
{
    public:
        //! Transforme une chaine en remplaçant les caractères unicode par leurs équivalents ANSI choisis
        /*!
         * Note: les caractères non unicode sont mis en minuscules
         * \param ioChaine chaine à transformer
         */
        static void Normalise(std::string & ioChaine);

        //! Transforme une chaine en rétablissant les caractères unicode à partir de leurs équivalents ANSI choisis
        /*!
         * \param ioChaine chaine à transformer
         */
        static void Denormalise(std::string & ioChaine);

        //! Affiche le contenu des tables de normalisation et de dénormalisation
        static void AfficheTables(std::ostream & ioStream);

    private:
        //! Ajoute dans les tables de normalisation / dénormalisation un nouveau caractère
        /*
         * Le caractère de substition est choisi automatiquement
         * \param iOctet1 code du premier octet du caractère
         * \param iOctet2 code du deuxième octet du caractère
         * \return le caractère de substitution
         */
        static char NormaliseNouveauCaractere(int iOctet1,
                int iOctet2);

        static boost::bimap<std::array<int, 2>, char> TableNormalisation;
};


template<class TYPE, std::size_t TAILLE>
std::ostream & operator << (std::ostream & ioStream,
        const std::array<TYPE, TAILLE> & iObjet)
{
    for (TYPE element: iObjet)
    {
        ioStream << element;
    }
    return ioStream;
}


template<class CLEF, class VALEUR>
std::ostream & operator << (std::ostream & ioStream,
        const std::map<CLEF, VALEUR> & iObjet)
{
    for (const auto & clefValeur: iObjet)
    {
        ioStream << "    " << clefValeur.first << " : " << clefValeur.second << std::endl;
    }
    return ioStream;
}

#endif
