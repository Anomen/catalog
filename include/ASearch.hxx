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

#if !defined __ARECHERCHE_HXX__
#define      __ARECHERCHE_HXX__

#include <iostream>

#define ARECHERCHE nsCatalog::ASearch

inline ARECHERCHE::~ASearch() { }

inline ARECHERCHE::ASearch (std::string Server)
{
    try
    {
        m_Cnx.SetServer (Server);
        //m_Cnx.Init();
    }
    catch (const CException & E)
    {
        std::cerr << "Impossible de créer une connexion.\n";
    }
}

inline bool ARECHERCHE::Delete (std::string & Text, int PosBeg, int PosEnd) const
{
    if (PosBeg == -1 || PosEnd == -1) return false;

    bool Rempl (false);
    std::string Temp;

    try
    {
        Temp  = Text.substr (0, PosBeg);
        Temp += Text.substr (PosEnd, Text.size() - PosEnd);
        Text = Temp;
        Rempl = true;
    }
    catch (const std::exception &) { }

    return Rempl;
}

inline bool ARECHERCHE::Delete (std::string & Text, std::string Str) const
{
    bool Rempl (false);

    while (true)
    {
        int i (Text.find (Str));

        if (i != -1) 
        {
            Rempl = true;
            Delete (Text, i, i+Str.size());
        }
        else break;
    }

    return Rempl;
}

inline void ARECHERCHE::GetFile (std::string Url, std::ofstream & ofs) throw (CException)
{
    m_Cnx.GetFile (Url, ofs);
}

#endif
