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
                DROITE,
                NON_ASSIGNE
        };

        Repartition(const Repartition & iObjet);

        Repartition & operator = (const Repartition & iObjet);

        Repartition() = delete;


        //! Constructeur faisant une répartition simple à partir des statistiques de fréquences d'apparition des caractères
        /*!
         * On répartit les caractères alternativement sur les deux mains dans leur ordre d'apparition
         * \param iStatistiques statistiques à utiliser
         */
        Repartition(const Statistiques & iStatistiques);

        virtual ~Repartition() = default;

        //! Initialise la répartition à partir des statistiques
        /*!
         * On prend les caractères par ordre décroissant de fréquence d'apparition et on les assigne alternativement sur les deux mains
         */
        void InitialisationEquilibree();

        //! Recalcule les scores partiels
        /*!
         * \throw Une exception std::logic_error si une lettre n'est pas assignée à une main
         * ou s'il n'y a pas autant de lettres sur chaque main
         */
        void calculeScore();

        //! Retourne le score total
        /*!
         * \throw Une exception std::logic_error si le score n'est pas à jour
         */
        int score() const;
        int scoreSubsidiaire() const;

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

        //! Assigne un caractère à une main
        void assigneCaractere(char iCaractere,
                Main iMain);

        //! Dé-assigne tous les caractères
        void reinitialise();

        //! Assigne tous les caractères non assignés à la main donnée
        void finitAssignationCaracteres(Main iMain);


    private:
        //! Répartition des caractères entre les deux mains
        std::map<char, Main> _repartition;

        //! Scores partiels correspondant à la répartition des caractères et des bigrammes entre les mains
        int _scoreRepartitionCaracteres{0};
        int _scoreRepartitionBigrammes{0};
        //! Score subsidiaire à minimiser : différence des carrés des fréquences entre les mains
        int _scoreSubsidiaire{0};

        //! Drapeau indiquant si les scores partiels sont à jour
        bool _scoresAJour{true};

        //! Lien vers les statistiques à utiliser
        const Statistiques * _pStatistiques{nullptr};
};

#endif
