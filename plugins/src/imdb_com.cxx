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
    : ARecherche ("www.imdb.com")
{ }

imdb_com::~imdb_com() { }

VFilm_t imdb_com::Recherche (string MotCle) throw (CException)
{
    VFilm_t VFilms;
    Urlencode (MotCle);

    ostringstream oss;
    oss << "find?s=tt&q=" << MotCle << "&x=26&y=12";

    string Page = Extraire (
        GetPage (oss.str()),
        "<p><b>Popular Titles</b>",
        "</td></tr></table> </p>"
    ).at(0);

    vector<string> VStrTitre = Extraire (
        Page,
        ".</td><td valign=\"top\"><img src=\"/images/b.gif\" width=\"1\" height=\"6\"><br><a href=\"/",
        "</a>"
    );

    //vector<string> VStrAnnee = Extraire (
    //    Page,
    //    "<div style=\"padding-top: 2px;\"><h4 style=\"color: #808080\">",
    //    "</h4>"
    //);

    unsigned Taille (VStrTitre.size());
    for (unsigned i (0); i < Taille; ++i)
    {
        string Temp (VStrTitre[i]);
        Supprimer (Temp, 0, Temp.find_first_of ('>', 0)+1);

        CFilm F;
        F.SetTitre (Temp);
        F.SetUrl (VStrTitre[i].substr (0, VStrTitre[i].find_first_of ('"')));

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
        Temp = Extraire (Page, "<h5>Release Date:</h5>", "<").at(0);

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
        Temp = Extraire (Page, "<h5>Director:</h5>", "</a>").at (0);
        Supprimer (Temp, 0, Temp.find_first_of ('>')+1);
        Supprimer (Temp, "\n");

        Film.SetRealisateur (Temp);
    }
    catch (const exception &) { }

    // Description
    try
    {
        string PageSynopsis (GetPage (Film.GetUrl() + "plotsummary"));
        Temp = Extraire (PageSynopsis, "<p class=\"plotpar\">", "</p>").at (0);
        while (Supprimer (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));
        Supprimer (Temp, "\n");

        Film.SetDesc (Temp);
    }
    catch (const exception &) { }

    // Acteurs
    try
    {
        Temp = Extraire (Page, "<table class=\"cast\">", "</table>").at (0);
        vector<string> StrAct = Extraire (Temp, "<td class=\"nm\">", "</a>");
        ostringstream oss;
        for (vector<string>::iterator i = StrAct.begin();
                i != StrAct.end(); i++)
        {
            Supprimer (*i, 0, (*i).find_first_of('>')+1);
            oss << *i << ", ";
        }

        Film.SetActeurs (oss.str());
    }
    catch (const exception &) { }

    // Genre
    try
    {
        Temp = Extraire (Page, "<h5>Genre:</h5>", "</div>").at (0);
        while (Supprimer (Temp, Temp.find_first_of ('<'), 
                                Temp.find_first_of ('>')+1));
        Supprimer (Temp, "\n");

        Film.SetGenre (Temp);
    }
    catch (const exception &) { }

    // Durée
    try
    {
        Temp = Extraire (Page, "<h5>Runtime:</h5>", "min").at (0);

        unsigned Duree (0);
        istringstream iss2 (Temp);
        iss2 >> Duree;
        Film.SetDuree (Duree);
    }
    catch (const exception &) { }

    // Image
    try
    {
        Temp = Extraire (Page, "<a name=\"poster\"", "/>").at (0);
        string Temp2 = Extraire (Temp, "src=\"", "\"").at(0);

        ostringstream oss;
        oss << Film.GetIdImg();

        ofstream ofs (oss.str().c_str(), fstream::binary);
        CConnexion cnx (Temp2.substr(7, Temp2.find_first_of('/', 7)-7));
        cnx.GetFich (Temp2, ofs);
    }
    catch (const exception &) { }

    return Film;
}

extern "C" ARecherche * create() {
    return new imdb_com();
}
