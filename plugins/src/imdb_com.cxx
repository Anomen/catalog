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
#include "imdb_com.h"
#include "CFilm.h"
#include "CPluginManager.h"

using namespace std;
using namespace nsCatalog;

imdb_com::imdb_com()
    : ASearch ("www.imdb.com")
{ }

imdb_com::~imdb_com() { }

VFilm_t imdb_com::Search (string KeyWord) throw (CException)
{
    VFilm_t VFilms;
    Urlencode (KeyWord);

    ostringstream oss;
    oss << "find?s=tt&q=" << KeyWord << "&x=26&y=12";

    string Page = Extract (
        GetPage (oss.str()),
        "<p><b>Popular Titles</b>",
        "</td></tr></table> </p>"
    ).at(0);

    vector<string> VStrTitle = Extract (
        Page,
        ".</td><td valign=\"top\"><img src=\"/images/b.gif\" width=\"1\" height=\"6\"><br><a href=\"/",
        "</a>"
    );

    //vector<string> VStrAnnee = Extract (
    //    Page,
    //    "<div style=\"padding-top: 2px;\"><h4 style=\"color: #808080\">",
    //    "</h4>"
    //);

    unsigned Taille (VStrTitle.size());
    for (unsigned i (0); i < Taille; ++i)
    {
        string Temp (VStrTitle[i]);
        Delete (Temp, 0, Temp.find_first_of ('>', 0)+1);

        CFilm F;
        F.SetTitle (Temp);
        F.SetUrl (VStrTitle[i].substr (0, VStrTitle[i].find_first_of ('"')));

        //try {
        //    F.SetAnnee (atoi (VStrAnnee.at(i).c_str()));
        //}
        //catch (exception &) { }

        VFilms.push_back (F);
    }

    if (!VFilms.size())
        throw CException ("Aucun film trouvé sur internet.");

    return VFilms;
}

CFilm & imdb_com::Detail (CFilm & Film)
{
    string Temp;
    string Page (GetPage (Film.GetUrl()));

    // Date de sortie
    map<string, unsigned> CstMois;
    CstMois["January"   ] = 1;
    CstMois["February"  ] = 2;
    CstMois["March"     ] = 3;
    CstMois["April"     ] = 4;
    CstMois["May"       ] = 5;
    CstMois["June"      ] = 6;
    CstMois["July"      ] = 7;
    CstMois["August"    ] = 8;
    CstMois["September" ] = 9;
    CstMois["October"   ] = 10;
    CstMois["November"  ] = 11;
    CstMois["December"  ] = 12;

    try
    {
        Temp = Extract (Page, "<h5>Release Date:</h5>", "<").at(0);

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
        Temp = Extract (Page, "<h5>Director:</h5>", "</a>").at (0);
        Delete (Temp, 0, Temp.find_first_of ('>')+1);
        Delete (Temp, "\n");

        Film.SetCreator (Temp);
    }
    catch (const exception &) { }

    // Description
    try
    {
        string PageSynopsis (GetPage (Film.GetUrl() + "plotsummary"));
        Temp = Extract (PageSynopsis, "<p class=\"plotpar\">", "</p>").at (0);
        while (Delete (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));
        Delete (Temp, "\n");

        Film.SetDesc (Temp);
    }
    catch (const exception &) { }

    // Actors
    try
    {
        Temp = Extract (Page, "<table class=\"cast\">", "</table>").at (0);
        vector<string> StrAct = Extract (Temp, "<td class=\"nm\">", "</a>");
        ostringstream oss;
        for (vector<string>::iterator i = StrAct.begin();
                i != StrAct.end(); i++)
        {
            Delete (*i, 0, (*i).find_first_of('>')+1);
            oss << *i << ", ";
        }

        Film.SetActors (oss.str());
    }
    catch (const exception &) { }

    // Genre
    try
    {
        Temp = Extract (Page, "<h5>Genre:</h5>", "</div>").at (0);
        while (Delete (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));
        Delete (Temp, "\n");

        Film.SetGenre (Temp);
    }
    catch (const exception &) { }

    // Durée
    try
    {
        Temp = Extract (Page, "<h5>Runtime:</h5>", "min").at (0);

        unsigned Time (0);
        istringstream iss2 (Temp);
        iss2 >> Time;
        Film.SetTime (Time);
    }
    catch (const exception &) { }

    // Image
    try
    {
        Temp = Extract (Page, "<a name=\"poster\"", "/>").at (0);
        string Temp2 = Extract (Temp, "src=\"", "\"").at(0);

        ostringstream oss;
        oss << Film.GetIdImg();

        ofstream ofs (oss.str().c_str(), fstream::binary);
        CConnection cnx (Temp2.substr(7, Temp2.find_first_of('/', 7)-7));
        cnx.GetFile (Temp2, ofs);
    }
    catch (const exception &) { }

    return Film;
}

extern "C" ASearch * create() {
    return new imdb_com();
}
