#ifndef _REPARTITION_
#define _REPARTITION_

#include <iostream>
#include <map>

class Statistiques;


//! Classe contenant une r�partition des touches entre les deux mains
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


        //! Constructeur faisant une r�partition simple � partir des statistiques de fr�quences d'apparition des caract�res
        /*!
         * On r�partit les caract�res alternativement sur les deux mains dans leur ordre d'apparition
         * \param iStatistiques statistiques � utiliser
         */
        Repartition(const Statistiques & iStatistiques);

        virtual ~Repartition() = default;

        //! Initialise la r�partition � partir des statistiques
        /*!
         * On prend les caract�res par ordre d�croissant de fr�quence d'apparition et on les assigne alternativement sur les deux mains
         */
        void InitialisationEquilibree();

        //! Recalcule les scores partiels
        /*!
         * \throw Une exception std::logic_error si une lettre n'est pas assign�e � une main
         * ou s'il n'y a pas autant de lettres sur chaque main
         */
        void calculeScore();

        //! Retourne le score total
        /*!
         * \throw Une exception std::logic_error si le score n'est pas � jour
         */
        int score() const;
        int scoreSubsidiaire() const;

        //! Retourne les statistiques
        const Statistiques & statistiques() const
        {
            return *_pStatistiques;
        }

        //! Intervertit deux caract�res
        /*
         * Si les caract�res sont sur la m�me main, ne fait rien et retourne false
         * Si les caract�res sont sur des mains diff�rentes:
         *    - intervertit les caract�res entre les deux mains
         *    - attention : le score n'est pas recalcul� et est donc invalide
         *    - retourne true
         */
        bool intervertitCaracteres(char caractere1,
                char caractere2);

        //! Assigne un caract�re � une main
        void assigneCaractere(char iCaractere,
                Main iMain);

        //! D�-assigne tous les caract�res
        void reinitialise();

        //! Assigne tous les caract�res non assign�s � la main donn�e
        void finitAssignationCaracteres(Main iMain);


    private:
        //! R�partition des caract�res entre les deux mains
        std::map<char, Main> _repartition;

        //! Scores partiels correspondant � la r�partition des caract�res et des bigrammes entre les mains
        int _scoreRepartitionCaracteres{0};
        int _scoreRepartitionBigrammes{0};
        //! Score subsidiaire � minimiser : diff�rence des carr�s des fr�quences entre les mains
        int _scoreSubsidiaire{0};

        //! Drapeau indiquant si les scores partiels sont � jour
        bool _scoresAJour{true};

        //! Lien vers les statistiques � utiliser
        const Statistiques * _pStatistiques{nullptr};
};

#endif
