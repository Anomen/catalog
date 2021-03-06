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

#if !defined __ARECHERCHE_H__
#define      __ARECHERCHE_H__

#include <vector>
#include <string>
#include <fstream>
#include "CFilm.h"
#include "CConnection.h"

namespace nsCatalog
{
    class ASearch
    {
      public:
        ASearch (std::string Server);
        virtual ~ASearch();

        virtual VFilm_t Search (std::string KeyWord) throw (CException) = 0;
        virtual CFilm & Detail    (CFilm & Film      ) = 0;

        std::vector<std::string> Extract (const std::string & Text,
                                           std::string Begin, std::string End) const;

        bool Delete (std::string & Text, int PosBeg, int PosEnd) const;
        bool Delete (std::string & Text, std::string Str)        const;
      
      protected:
        CConnection m_Cnx;

        std::string GetPage (std::string Url) throw (CException);
        void        GetFile (std::string Url, std::ofstream & ofs) 
                                              throw (CException);
        
    };
}

#include "ASearch.hxx"

#endif
