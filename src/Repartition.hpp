#ifndef _REPARTITION_
#define _REPARTITION_

#include <iostream>
#include <map>

class Statistiques;


//! Classe contenant une r�partition des touches entre les deux mains
class Repartition
{
        enum class Main
        {
                GAUCHE,
                DROITE
        };

        friend std::ostream & operator << (std::ostream & ioStream,
            const Repartition & iObjet);

    public:
        Repartition() = delete;
        Repartition & operator = (const Repartition &) = delete;
        Repartition(const Repartition &) = delete;

        //! Constructeur faisant une r�partition simple � partir des statistiques de fr�quences d'apparition des caract�res
        /*!
         * On prend les caract�res par ordre d�croissant de fr�quence d'apparition et on les assigne alternativement sur les deux mains
         * \param iStatistiques statistiques � utiliser
         * \param iRepartitionEquilibree si false, r�partit les caract�res alternativement sur les deux mains
         *        sans prendre en compte leur fr�quences
         */
        Repartition(const Statistiques & iStatistiques,
                bool iRepartitionEquilibree = true);

        virtual ~Repartition() = default;

        //! Retourne le score total
        /*!
         * \throw Une exception std::logic_error si le score n'est pas � jour
         */
        int score();

    private:
        //! Recalcule les scores partiels
        void calculeScore();

        //! R�partition des caract�res entre les deux mains
        std::map<char, Main> _repartition;

        //! Scores partiels correspondant � la r�partition des caract�res et des bigrammes entre les mains
        int _scoreRepartitionCaracteres{0};
        int _scoreRepartitionBigrammes{0};

        //! Drapeau indiquant si les scores partiels sont � jour
        bool _scoresAJour{true};

        //! Lien vers les statistiques � utiliser
        const Statistiques * _pStatistiques{nullptr};
};

#endif
