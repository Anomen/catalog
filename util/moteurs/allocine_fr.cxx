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

using namespace std;
using namespace nsCatalog;

allocine_fr::allocine_fr()
    : ARecherche ("www.allocine.fr")
{ }

allocine_fr::~allocine_fr() { }

string allocine_fr::GetPage (string Url) throw (CException)
{
    iconv_t cd = iconv_open ("UTF-8", "ISO-8859-1");

    string Page = ARecherche::GetPage(Url);
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

VFilm_t allocine_fr::Recherche (string MotCle) throw (CException)
{
    int NumPage (0);
    VFilm_t VFilms;
    Urlencode (MotCle);

    while (true)
    {
        ostringstream oss;
        oss << "recherche/?motcle=" << MotCle << "&rub=1&page=" << ++NumPage;

        string Page = GetPage (oss.str());

        vector<string> VStrTitre = Extraire (
            Page,
            "<td valign=\"top\"><h4><a href=\"/film/fichefilm_gen_cfilm=",
            "</a>"
        );

        vector<string> VStrAnnee = Extraire (
            Page,
            "<div style=\"padding-top: 2px;\"><h4 style=\"color: #808080\">",
            "</h4>"
        );

        if (!VStrTitre.size()) break;

        unsigned Taille (VStrTitre.size());
        for (unsigned i (0); i < Taille; ++i)
        {
            string Temp (VStrTitre[i]);
            Supprimer (Temp, 0, Temp.find_first_of ('>', 0)+1);
            Supprimer (Temp, "<b>");
            Supprimer (Temp, "</b>");

            CFilm F;
            F.SetTitre (Temp);
            F.SetUrl ("film/fichefilm_gen_cfilm=" 
                      + VStrTitre[i].substr (0, 
                            VStrTitre[i].find_first_of ('.')) 
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
        Temp = Extraire (Page, "Date de sortie : <b>", "<").at(0);

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
        Temp = Extraire (Page, "par <a class=\"link1\" href=\"/personne/fichepersonne_gen_cpersonne", "</a>").at (0);
        Supprimer (Temp, 0, Temp.find_first_of ('>')+1);

        Film.SetRealisateur (Temp);
    }
    catch (const exception &) { }

    // Description
    try
    {
        Temp = Extraire (Page, "<td valign=\"top\" style=\"padding:10 0 0 0\"><div align=\"justify\"><h4>", "</h4>").at (0);
        Supprimer (Temp, "<b>");
        Supprimer (Temp, "</b>");
        Supprimer (Temp, "<br");
        Supprimer (Temp, "/>");
        Supprimer (Temp, ">");
        Supprimer (Temp, "\n");

        Film.SetDesc (Temp);
    }
    catch (const exception &) { }

    // Acteurs
    try
    {
        Temp = Extraire (Page, "<div style=\"padding: 2 0 2 0;\"><h4>Avec ", " &nbsp;&nbsp;<img").at (0);
        while (Supprimer (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));

        Film.SetActeurs (Temp);
    }
    catch (const exception &) { }

    // Genre
    try
    {
        Temp = Extraire (Page, "Genre : ", "</h4>").at (0);
        while (Supprimer (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));

        Film.SetGenre (Temp);
    }
    catch (const exception &) { }

    // Durée
    try
    {
        Temp = Extraire (Page, "<h4>Dur", "&nbsp;</h4>").at (0);
        Supprimer (Temp, 0, Temp.find_first_of (':')+2);

        unsigned i, Duree (0);
        istringstream iss2 (Temp);
        iss2 >> i >> Temp;

        if (Temp == "min.")
            Duree += i;
        else
        {
            Duree += i * 60;
            iss2 >> i;
            Duree += i;
        }

        Film.SetDuree (Duree);
    }
    catch (const exception &) { }

    // Image
    try
    {
        Temp = Extraire (Page, "<td valign=\"top\" width=\"120\">\r\n<img src=\"", "\"").at (0);

        ostringstream oss;
        oss << Film.GetIdImg();

        ofstream ofs (oss.str().c_str(), fstream::binary);
        GetFich (Temp, ofs);
    }
    catch (const exception &) { }

    return Film;
}
