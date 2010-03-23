/*
 * This file is part of catalog-server.
 * Copyright (C) 2008-2010  Kevin Vicrey <kevin.vicrey@gmail.com>
 * Copyright (C) 2008-2009  Romain Giraud <giraud.romain@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

        void Add  (const CFilm & Film) const throw (CException);
        void Edit (const CFilm & Film) const throw (CException);
        void Delete(const CFilm & Film) const throw (CException);

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
