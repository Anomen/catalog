/*
 * This file is part of catalg-server.
 * Copyright (C) 2009  Kevin Vicrey <kevin.vicrey@gmail.com>
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

#if !defined __CCONNEXION_H__
#define      __CCONNEXION_H__

#include <string>
#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket(), AF_INET
#include <netdb.h>      // gethostbyname()
#include <fstream>

#include "CException.h"

namespace nsCatalog
{
    class CConnexion
    {
      public:
        CConnexion (std::string Serveur = "");
        ~CConnexion();

        void Init () throw (CException);
        void Close();

        std::string GetPage (std::string Url) throw (CException);
        void        GetFich (std::string Url, std::ofstream & ofs) throw (CException);

        // Les modifieurs
        void SetServeur (std::string Serveur);

      private:
        std::string m_Serveur;
        int         m_Sd;

        void Convert (std::string & Str) const;
    };
}

#include "CConnexion.hxx"

#endif
