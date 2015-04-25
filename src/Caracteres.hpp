#ifndef _CARACTERES_
#define _CARACTERES_

#include <iostream>
#include <string>

#include <boost/bimap.hpp>

//! Classe contenant des fonctions utilitaires pour tranformer les caract�res
/*!
 * Il est n�cessaire de transformer bijectivement les caract�res unicode utilis�s en caract�re ANSI afin de simplifier les algorithmes
 * et de permettre l'utilisation de maps index�es par des char.
 * Pour cela on choisit des �quivalents parmis les lettres ANSI majuscules.
 * Cela implique donc de ne travailler qu'avec des lettres minuscules.
 */
class Caracteres
{
    public:
        //! Transforme une chaine en rempla�ant les caract�res unicode par leurs �quivalents ANSI choisis
        /*!
         * Note: les caract�res non unicode sont mis en minuscules
         * \param ioChaine chaine � transformer
         */
        static void Normalise(std::string & ioChaine);

        //! Transforme une chaine en r�tablissant les caract�res unicode � partir de leurs �quivalents ANSI choisis
        /*!
         * \param ioChaine chaine � transformer
         */
        static void Denormalise(std::string & ioChaine);

        //! Affiche le contenu des tables de normalisation et de d�normalisation
        static void AfficheTables(std::ostream & ioStream);

    private:
        //! Ajoute dans les tables de normalisation / d�normalisation un nouveau caract�re
        /*
         * Le caract�re de substition est choisi automatiquement
         * \param iOctet1 code du premier octet du caract�re
         * \param iOctet2 code du deuxi�me octet du caract�re
         * \return le caract�re de substitution
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
