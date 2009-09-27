#include "CFilm.h"

#include <iostream>
#include <ctime>

using namespace std;
using namespace nsCatalog;

namespace nsCatalog
{
    ostream & operator << (ostream &os, const CFilm &F)
    {
        return os << '"' << F.m_Titre << "\" :" << endl
                  << "\t- id : " << F.m_Id << endl
                  << "\t- description : " << F.m_Desc << endl
                  << "\t- genre : " << F.m_Genre << endl
                  << "\t- réalisateur : " << F.m_Realisateur << endl
                  << "\t- acteurs : " << F.m_Acteurs << endl
                  << "\t- date : " << F.GetJour() << '/' << F.GetMois() << '/' << F.GetAnnee() << endl
                  << "\t- duree : " << F.m_Duree << flush;
    }
}
