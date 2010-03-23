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

#include <map>
#include <string.h>
#include <iconv.h>
#include <stdlib.h>     // atoi()
#include <exception>
#include <string>
#include <iostream>
#include <sstream>
#include <errno.h>

#include "util.h"
#include "allocine_fr.h"
#include "CFilm.h"
#include "CPluginManager.h"

using namespace std;
using namespace nsCatalog;

allocine_fr::allocine_fr()
    : ASearch ("www.allocine.fr")
{ }

allocine_fr::~allocine_fr() { }

VFilm_t allocine_fr::Search (string KeyWord) throw (CException)
{
    int NumPage (0);
    VFilm_t VFilms;
    Urlencode (KeyWord);

    while (true)
    {
        ostringstream oss;
        oss << "recherche/?motcle=" << KeyWord << "&rub=1&page=" << ++NumPage;

        string Page = GetPage (oss.str());

        vector<string> VStrTitle = Extract (
            Page,
            "<td valign=\"top\"><h4><a href=\"/film/fichefilm_gen_cfilm=",
            "</a>"
        );

        vector<string> VStrAnnee = Extract (
            Page,
            "<div style=\"padding-top: 2px;\"><h4 style=\"color: #808080\">",
            "</h4>"
        );

        if (!VStrTitle.size()) break;

        unsigned Taille (VStrTitle.size());
        for (unsigned i (0); i < Taille; ++i)
        {
            string Temp (VStrTitle[i]);
            Delete (Temp, 0, Temp.find_first_of ('>', 0)+1);
            Delete (Temp, "<b>");
            Delete (Temp, "</b>");

            CFilm F;
            F.SetTitle (Temp);
            F.SetUrl ("film/fichefilm_gen_cfilm=" 
                      + VStrTitle[i].substr (0, 
                            VStrTitle[i].find_first_of ('.')) 
                      + ".html");

            try {
                F.SetAnnee (atoi (VStrAnnee.at(i).c_str()));
            }
            catch (exception &) { }

            VFilms.push_back (F);
        }
    }

    if (!VFilms.size())
        throw CException ("Aucun film trouvé sur internet.");

    return VFilms;
}

CFilm & allocine_fr::Detail (CFilm & Film)
{
    string Temp;
    string Page (GetPage (Film.GetUrl()));

    // Date de sortie
    map<string, unsigned> CstMois;
    CstMois["Janvier"  ] = 1;
    CstMois["Février"  ] = 2;
    CstMois["Mars"     ] = 3;
    CstMois["Avril"    ] = 4;
    CstMois["Mai"      ] = 5;
    CstMois["Juin"     ] = 6;
    CstMois["Juillet"  ] = 7;
    CstMois["Aout"     ] = 8;
    CstMois["Septembre"] = 9;
    CstMois["Octobre"  ] = 10;
    CstMois["Novembre" ] = 11;
    CstMois["Décembre" ] = 12;

    try
    {
        Temp = Extract (Page, "Date de sortie : <b><a", "<").at(0);
        Delete (Temp, 0, Temp.find_first_of('>')+1);

        unsigned J, M, A;
        istringstream iss (Temp);
        iss >> J >> Temp >> A;
        M = CstMois[Temp];

        Film.SetJour  (J);
        Film.SetMois  (M);
        Film.SetAnnee (A);
    }
    catch (const exception &) { }

    // Réalisateur
    try
    {
        Temp = Extract (Page, "par <a class=\"link1\" href=\"/personne/fichepersonne_gen_cpersonne", "</a>").at (0);
        Delete (Temp, 0, Temp.find_first_of ('>')+1);

        Film.SetCreator (Temp);
    }
    catch (const exception &) { }

    // Description
    try
    {
        Temp = Extract (Page, "<td valign=\"top\" style=\"padding:10 0 0 0\"><div align=\"justify\"><h4>", "</h4>").at (0);
        Delete (Temp, "<b>");
        Delete (Temp, "</b>");
        Delete (Temp, "<br");
        Delete (Temp, "/>");
        Delete (Temp, ">");
        Delete (Temp, "\n");

        Film.SetDesc (Temp);
    }
    catch (const exception &) { }

    // Actors
    try
    {
        Temp = Extract (Page, "<h3 class=\"SpProse\">Avec ", " &nbsp;&nbsp;<img").at (0);
        while (Delete (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));

        Film.SetActors (Temp);
    }
    catch (const exception &) { }

    // Genre
    try
    {
        Temp = Extract (Page, "Genre : ", "</h3>").at (0);
        while (Delete (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));

        Film.SetGenre (Temp);
    }
    catch (const exception &) { }

    // Durée
    try
    {
        Temp = Extract (Page, "<h3 class=\"SpProse\">Dur", "&nbsp;</h3>").at (0);
        Delete (Temp, 0, Temp.find_first_of (':')+2);

        unsigned i, Time (0);
        istringstream iss2 (Temp);
        iss2 >> i >> Temp;

        if (Temp == "min.")
            Time += i;
        else
        {
            Time += i * 60;
            iss2 >> i;
            Time += i;
        }

        Film.SetTime (Time);
    }
    catch (const exception &) { }

    // Image
    try
    {
        Temp = Extract (Page, "<td valign=\"top\" width=\"120\">\r\n<img src=\"", "\"").at (0);

        ostringstream oss;
        oss << Film.GetIdImg();

        ofstream ofs (oss.str().c_str(), fstream::binary);
        GetFile (Temp, ofs);
    }
    catch (const exception &) { }

    return Film;
}

extern "C" ASearch * create() {
    return new allocine_fr();
}
