#if !defined __ARECHERCHE_H__
#define      __ARECHERCHE_H__

#include <vector>
#include <string>
#include <fstream>
#include "CFilm.h"
#include "CConnexion.h"

namespace nsCatalog
{
    class ARecherche
    {
      public:
        ARecherche (std::string Serveur);
        virtual ~ARecherche();

        virtual VFilm_t Recherche (std::string MotCle) throw (CException) = 0;
        virtual CFilm & Detail    (CFilm & Film      ) = 0;

        std::vector<std::string> Extraire (const std::string & Texte,
                                           std::string Begin, std::string End) const;

        bool Supprimer (std::string & Texte, int PosBeg, int PosEnd) const;
        bool Supprimer (std::string & Texte, std::string Str)        const;
      
      protected:
        CConnexion m_Cnx;

        std::string GetPage (std::string Url) throw (CException);
        void        GetFich (std::string Url, std::ofstream & ofs) 
                                              throw (CException);
        
    };
}

#include "ARecherche.hxx"

#endif
