#ifndef _REPARTITION_
#define _REPARTITION_

#include <iostream>
#include <map>

class Statistiques;


//! Classe contenant une répartition des touches entre les deux mains
class Repartition
{
        friend std::ostream & operator << (std::ostream & ioStream,
            const Repartition & iObjet);

    public:
        enum class Main
        {
                GAUCHE,
                DROITE
        };

        Repartition() = delete;
        Repartition & operator = (const Repartition &) = delete;
        Repartition(const Repartition &) = delete;

        //! Constructeur faisant une répartition simple à partir des statistiques de fréquences d'apparition des caractères
        /*!
         * On prend les caractères par ordre décroissant de fréquence d'apparition et on les assigne alternativement sur les deux mains
         * \param iStatistiques statistiques à utiliser
         */
        Repartition(const Statistiques & iStatistiques);


        virtual ~Repartition() = default;

        //! Initialise la répartition à partir des statistiques
        /*!
         * \param iRepartitionEquilibree si false, répartit les caractères alternativement sur les deux mains
         * sans prendre en compte leur fréquences
         */
        void Initialise(bool iRepartitionEquilibree = true);

        //! Recalcule les scores partiels
        void calculeScore();

        //! Retourne le score total
        /*!
         * \throw Une exception std::logic_error si le score n'est pas à jour
         */
        int score() const;

        //! \throw Une exception std::logic_error si la répartition n'est pas complète
        /*!
         * Vérifie que toutes les lettres assignées, avec autant de lettres sur chaque main
         */
        void verifieRepartitionComplete() const;

        //! Retourne les statistiques
        const Statistiques & statistiques() const
        {
            return *_pStatistiques;
        }

        //! Intervertit deux caractères
        /*
         * Si les caractères sont sur la même main, ne fait rien et retourne false
         * Si les caractères sont sur des mains différentes:
         *    - intervertit les caractères entre les deux mains
         *    - attention : le score n'est pas recalculé et est donc invalide
         *    - retourne true
         */
        bool intervertitCaracteres(char caractere1,
                char caractere2);

    private:
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
