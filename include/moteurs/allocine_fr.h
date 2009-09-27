#if !defined __ALLOCINE_FR_H__
#define      __ALLOCINE_FR_H__

#include <vector>
#include <string>

#include "ARecherche.h"
#include "CFilm.h"

namespace nsCatalog
{
    class allocine_fr : public ARecherche
    {
      public:
        allocine_fr();
        ~allocine_fr();

        VFilm_t Recherche (std::string MotCle) throw (CException);
        CFilm & Detail    (CFilm & Film      );

        std::string GetPage (std::string Url) throw (CException);
    };
}


#endif
