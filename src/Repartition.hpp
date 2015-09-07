#ifndef _REPARTITION_
#define _REPARTITION_

#include <iostream>
#include <map>

class Statistiques;


//! Classe contenant une répartition des touches entre les deux mains
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

        //! Constructeur faisant une répartition simple à partir des statistiques de fréquences d'apparition des caractères
        /*!
         * On prend les caractères par ordre décroissant de fréquence d'apparition et on les assigne alternativement sur les deux mains
         * \param iStatistiques statistiques à utiliser
         * \param iRepartitionEquilibree si false, répartit les caractères alternativement sur les deux mains
         *        sans prendre en compte leur fréquences
         */
        Repartition(const Statistiques & iStatistiques,
                bool iRepartitionEquilibree = true);

        virtual ~Repartition() = default;

        //! Retourne le score total
        /*!
         * \throw Une exception std::logic_error si le score n'est pas à jour
         */
        int score();

    private:
        //! Recalcule les scores partiels
        void calculeScore();

        //! Répartition des caractères entre les deux mains
        std::map<char, Main> _repartition;

        //! Scores partiels correspondant à la répartition des caractères et des bigrammes entre les mains
        int _scoreRepartitionCaracteres{0};
        int _scoreRepartitionBigrammes{0};

        //! Drapeau indiquant si les scores partiels sont à jour
        bool _scoresAJour{true};

        //! Lien vers les statistiques à utiliser
        const Statistiques * _pStatistiques{nullptr};
};

#endif
