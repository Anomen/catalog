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

#if !defined __CFILM_H__
#define      __CFILM_H__

#include <string>
#include <vector>

namespace nsCatalog
{
    class CFilm
    {
      public:
        CFilm();

        // Modifieurs
        std::string GetRealisateur () const;
        std::string GetTitre  () const;
        std::string GetDesc   () const;
        std::string GetGenre  () const;
        std::string GetActeurs() const;
        std::string GetUrl    () const;
        unsigned    GetJour   () const;
        unsigned    GetMois   () const;
        unsigned    GetAnnee  () const;
        unsigned    GetDuree  () const;
        unsigned    GetId     () const;
        unsigned    GetIdImg  () const;

        friend std::ostream & operator << (std::ostream &os, const CFilm &F);

        // Modifieurs
        void SetTitre (std::string Titre);
        void SetDesc  (std::string Desc );
        void SetGenre (std::string Genre);
        void SetRealisateur (std::string Realisateur);
        void SetActeurs (std::string Acteurs);
        void SetUrl     (std::string Url);
        void SetJour  (unsigned J);
        void SetMois  (unsigned M);
        void SetAnnee (unsigned A);
        void SetDuree (unsigned Duree);
        void SetId    (unsigned Id);
        void SetIdImg (unsigned IdImg);

      private:
        std::string m_Titre, m_Desc, m_Genre, m_Realisateur, m_Acteurs, m_Url;
        unsigned    m_J, m_M, m_A, m_Duree, m_Id, m_IdImg;
    };

    typedef std::vector <CFilm> VFilm_t;
}

#include "CFilm.hxx"

#endif
