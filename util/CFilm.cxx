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

#include "CFilm.h"

#include <iostream>
#include <ctime>

using namespace std;
using namespace nsCatalog;

namespace nsCatalog
{
    ostream & operator << (ostream &os, const CFilm &F)
    {
        return os << '"' << F.m_Title << "\" :" << endl
                  << "\t- id : " << F.m_Id << endl
                  << "\t- description : " << F.m_Desc << endl
                  << "\t- genre : " << F.m_Genre << endl
                  << "\t- réalisateur : " << F.m_Creator << endl
                  << "\t- acteurs : " << F.m_Actors << endl
                  << "\t- date : " << F.GetJour() << '/' << F.GetMois() << '/' << F.GetAnnee() << endl
                  << "\t- duree : " << F.m_Time << flush;
    }
}
