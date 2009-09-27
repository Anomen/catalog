#if !defined __IMDB_COM_H__
#define      __IMDB_COM_H__

#include <vector>
#include <string>

#include "ARecherche.h"
#include "CFilm.h"

namespace nsCatalog
{
    class imdb_com : public ARecherche
    {
      public:
        imdb_com();
        ~imdb_com();

        VFilm_t Recherche (std::string MotCle) throw (CException);
        CFilm & Detail    (CFilm & Film      );
    };
}


#endif
