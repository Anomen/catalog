#if !defined __CDATABASE_H__
#define      __CDATABASE_H__

#include <string>
#include <sstream>

#include "CException.h"
#include "CFilm.h"
#include "constants.h"
#include "sqlite3.h"

namespace nsCatalog
{
    class CDatabase
    {
      public:
        CDatabase(const std::string &db = CstDbFileName);
        ~CDatabase();

        void Init() throw (CException);
        void Delete() const throw (CException);

        void Ajouter  (const CFilm & Film) const throw (CException);
        void Modifier (const CFilm & Film) const throw (CException);
        void Supprimer(const CFilm & Film) const throw (CException);

        VFilm_t GetFilm(std::string titre) const throw (CException);
        VFilm_t GetAllFilms()              const throw (CException);

      private:
        const std::string m_dbName;
        sqlite3 * m_Db;
        static VFilm_t s_films;

        void Exec (std::string query) const throw (CException);
        static int recupFilm(void *NotUsed, int argc, char **argv, char **azColName) throw(CException);

        std::string Escape (std::string Str) const;
    };
}

#include "CDatabase.hxx"

#endif
