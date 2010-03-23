#if !defined __ALLOCINE_FR_H__
#define      __ALLOCINE_FR_H__

#include <vector>
#include <string>

#include "ASearch.h"
#include "CFilm.h"

namespace nsCatalog
{
    class allocine_fr : public ASearch
    {
      public:
        allocine_fr();
        ~allocine_fr();

        VFilm_t Search (std::string KeyWord) throw (CException);
        CFilm & Detail    (CFilm & Film      );

        std::string GetPage (std::string Url) throw (CException);
    };
}


#endif
