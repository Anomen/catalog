/*
 * This file is part of catalog-server.
 * Copyright (C) 2008-2009  Kevin Vicrey <kevin.vicrey@gmail.com>
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

#include <stdlib.h>
#include <iconv.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "util.h"

using namespace std;

void nsCatalog::Urlencode (string & Str) 
{
    setlocale(LC_ALL,"");

    ostringstream oss;

    wstring t;
    t.resize (Str.size());
    t.resize (mbstowcs (&t[0], Str.c_str(), Str.size()));

    unsigned Taille (t.size());

    for (unsigned i (0); i < Taille; ++i)
        if ((L'a' <= t[i] && t[i] <= L'z')
            || (L'A' <= t[i] && t[i] <= L'Z')
            || (L'0' <= t[i] && t[i] <= L'9')
            || (t[i] == L'-' || t[i] == L'_' || t[i] == L'.'))
        {
            char c;
            wcstombs (&c, &t[i], 1);
            oss << c;
        }
        else 
        {
            oss << '%' << hex << int (t[i]);
        }

    Str = oss.str();
}
