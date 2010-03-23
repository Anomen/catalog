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

#if !defined __CCONNECTION_H__
#define      __CCONNECTION_H__

#include <string>
#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket(), AF_INET
#include <netdb.h>      // gethostbyname()
#include <fstream>

#include "CException.h"

namespace nsCatalog
{
    class CConnection
    {
      public:
        CConnection (std::string Server = "");
        ~CConnection();

        void Init () throw (CException);
        void Close();

        std::string GetPage (std::string Url) throw (CException);
        void        GetFile (std::string Url, std::ofstream & ofs) throw (CException);

        // Les modifieurs
        void SetServer (std::string Server);

      private:
        std::string m_Server;
        int         m_Sd;

        void Convert (std::string & Str) const;
    };
}

#include "CConnection.hxx"

#endif
