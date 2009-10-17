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

#include "CDatabase.h"
#include <stdlib.h>     // atoi()

#include <iostream>
#include <sstream>

using namespace std;
using namespace nsCatalog;

VFilm_t CDatabase::s_films;

void CDatabase::Init() throw (CException)
{
    if (sqlite3_open (m_dbName.c_str(), &m_Db))
        throw CException (sqlite3_errmsg(m_Db));

    Exec("CREATE TABLE IF NOT EXISTS films ("
         "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
         "  titre TEXT NOT NULL,"
         "  description TEXT NOT NULL,"
         "  genre TEXT NOT NULL,"
         "  realisateur TEXT NOT NULL,"
         "  acteurs TEXT NOT NULL,"
         "  jour_sortie INTEGER NOT NULL,"
         "  mois_sortie INTEGER NOT NULL,"
         "  annee_sortie INTEGER NOT NULL,"
         "  duree INTEGER NOT NULL,"
         "  idimg INTEGER)");
}

void CDatabase::Delete() const throw (CException)
{
    Exec("DROP TABLE films");
}

void CDatabase::Exec (string query) const throw (CException)
{
    char *errMsg = 0;
    if (sqlite3_exec(m_Db, query.c_str(), CDatabase::recupFilm, 0, &errMsg)
            != SQLITE_OK)
    {
        string str (errMsg);
        sqlite3_free(errMsg);
        throw CException (str);
    }
    sqlite3_free(errMsg);
}

int CDatabase::recupFilm(void *NotUsed, int argc, char **argv, char **azColName) throw(CException)
{
    CFilm f;
    f.SetId(atoi(argv[0]));
    f.SetTitre(argv[1]);
    f.SetDesc(argv[2]);
    f.SetGenre(argv[3]);
    f.SetRealisateur(argv[4]);
    f.SetActeurs(argv[5]);

    f.SetJour(atoi(argv[6]));
    f.SetMois(atoi(argv[7]));
    f.SetAnnee(atoi(argv[8]));
    f.SetDuree(atoi(argv[9]));
    f.SetIdImg(atoi(argv[10]));

    s_films.push_back(f);
    return 0;
}

VFilm_t CDatabase::GetFilm(string titre) const throw(CException)
{
    s_films.erase(s_films.begin(),s_films.end());
    Exec("SELECT * FROM films WHERE titre LIKE \""+titre+"\"");

    if (!s_films.size())
        throw CException ("Aucun film trouvé.");

    return s_films;
}

VFilm_t CDatabase::GetAllFilms() const throw(CException)
{
    s_films.erase(s_films.begin(),s_films.end());
    Exec("SELECT * FROM films");
    return s_films;
}

inline string CDatabase::Escape (string Str) const
{
    istringstream iss (Str);
    ostringstream oss;

    string Temp;
    while (getline (iss, Temp, '"'))
        oss << Temp << "\"\"";

    return oss.str().substr (0, oss.str().size()-2);
}

void CDatabase::Ajouter (const CFilm & Film) const throw (CException)
{
    bool Exists (false);
    try {
        GetFilm(Film.GetTitre());
        Exists = true;
    } catch (const CException &) { }

    if (Exists) throw CException ("Le film est déjà enregistré.");

    stringstream jour, mois, annee, duree, idImg;
    jour  << dec << Film.GetJour();
    mois  << Film.GetMois();
    annee << Film.GetAnnee();
    duree << Film.GetDuree();
    idImg << Film.GetIdImg();

    Exec("INSERT INTO films VALUES ("
         "NULL,"
         "\"" + Escape (Film.GetTitre()) + "\","
         "\"" + Escape (Film.GetDesc()) + "\","
         "\"" + Escape (Film.GetGenre()) + "\","
         "\"" + Escape (Film.GetRealisateur()) + "\","
         "\"" + Escape (Film.GetActeurs()) + "\","
         "" + jour.str() + ","
         "" + mois.str() + ","
         "" + annee.str() + ","
         "" + duree.str() + ","
         "" + idImg.str() + ")");
}

void CDatabase::Modifier (const CFilm & Film) const throw (CException)
{
    stringstream id, jour, mois, annee, duree;
    id    << Film.GetId();
    jour  << Film.GetJour();
    mois  << Film.GetMois();
    annee << Film.GetAnnee();
    duree << Film.GetDuree();

    Exec("UPDATE films SET "
         "titre=\"" + Film.GetTitre() + "\","
         "description=\"" + Film.GetDesc() + "\","
         "genre=\"" + Film.GetGenre() + "\","
         "realisateur=\"" + Film.GetRealisateur() + "\","
         "acteurs=\"" + Film.GetActeurs() + "\","
         "jour_sortie=" + jour.str() + ","
         "mois_sortie=" + mois.str() + ","
         "annee_sortie=" + annee.str() + ","
         "duree=" + duree.str() + " "
         "WHERE id="+id.str());
}

void CDatabase::Supprimer(const CFilm & Film) const throw (CException)
{
    stringstream id;
    id << Film.GetId();

    Exec("DELETE FROM films WHERE id="+id.str());
}
