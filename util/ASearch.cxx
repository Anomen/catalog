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

#include "ASearch.h"

#include <iconv.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace nsCatalog;

vector<string> ASearch::Extract (const string & Text, string Begin, string End) const
{
    int Pos (0), Taille (Begin.size());
    vector<string> VResult;

    while (true)
    {
        if ((Pos = Text.find (Begin, Pos+1)) == -1)
            break;
        Pos += Taille;

        try
        {
            int PosEnd = Text.find (End, Pos);

            string Temp (Text.substr (Pos, PosEnd - Pos));
            VResult.push_back (Temp);

            Pos = PosEnd + End.size();
        }
        catch (const exception &) { break; }
    }

    return VResult;

}

string ASearch::GetPage (string Url) throw (CException)
{
    iconv_t cd = iconv_open ("UTF-8", "ISO-8859-1");

    string Page = m_Cnx.GetPage (Url);
    char In[Page.size()], Out[Page.size()];

    char *ptrOut = Out;
    char *ptrIn  = In ;

    strcpy (In, Page.c_str());

    size_t szIn  = Page.size();
    size_t szOut = Page.size();

    iconv (cd, &ptrIn, &szIn, &ptrOut, &szOut);
    iconv_close (cd);

    *ptrOut = '\0';

    return string (Out);
}
