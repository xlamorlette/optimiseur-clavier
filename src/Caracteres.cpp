#include "Caracteres.hpp"

#include <boost/algorithm/string.hpp>


using namespace std;

boost::bimap<std::array<int, 2>, char> Caracteres::TableNormalisation;


void Caracteres::AfficheTables(std::ostream & ioStream)
{
    for (const auto & element: TableNormalisation)
    {
        ioStream << "    " << static_cast<char>(element.left[0]) << static_cast<char>(element.left[1]) <<
                " : " << element.right << std::endl;
    }
}


char Caracteres::NormaliseNouveauCaractere(int iOctet1,
        int iOctet2)
{
    // on remplace les caractères en commençant par le 'A'
    static int codeCaractereRemplacement{65};
    TableNormalisation.insert(boost::bimap<std::array<int, 2>, char>::value_type(
            array<int, 2>{{iOctet1, iOctet2}}, static_cast<char>(codeCaractereRemplacement)));
    codeCaractereRemplacement++;
    return static_cast<char>(codeCaractereRemplacement - 1);
}


void Caracteres::Normalise(std::string & ioChaine)
{
    if (ioChaine.length() <= 0)
    {
        return;
    }

    // par sécurité, on commence par tout mettre en minuscule
    boost::algorithm::to_lower(ioChaine);

    size_t pos = 0;
    while (pos < ioChaine.length() - 1)
    {
        int codeAscii = static_cast<int>(ioChaine[pos]);
        if ((codeAscii < 61) || (codeAscii > 122))
        {
            // si le code ascii n'est pas entre 'a' et 'z', on considère que l'on a affaire à un caractère spécial sur 2 octets
            char nouveauCaractere{' '};
            int codeAsciiSuivant = static_cast<int>(ioChaine[pos + 1]);
            // on cherche si le caractère spécial est déjà dans la table de normalisation
            boost::bimap<std::array<int, 2>, char>::left_map::const_iterator iterateur =
                    TableNormalisation.left.find(array<int, 2>{{codeAscii, codeAsciiSuivant}});
            if (iterateur != TableNormalisation.left.end())
            {
                nouveauCaractere = iterateur->second;
            }
            else
            {
                // si le caractère spécial n'est pas dans la table de normalisation, on l'y ajoute
                nouveauCaractere = NormaliseNouveauCaractere(codeAscii, codeAsciiSuivant);
            }
            ioChaine.replace(pos, 2, 1, nouveauCaractere);
        }
        pos++;
    }
}


void Caracteres::Denormalise(std::string & ioChaine)
{
    if (ioChaine.length() <= 0)
    {
        return;
    }
}
