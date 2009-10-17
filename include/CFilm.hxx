/*
 * This file is part of catalg-server.
 * Copyright (C) 2009  Kevin Vicrey <kevin.vicrey@gmail.com>
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

#if !defined __CFILM_HXX__
#define      __CFILM_HXX__

#include <stdlib.h>

inline nsCatalog::CFilm::CFilm()
    : m_Titre (""), m_Desc (""), m_Genre (""), m_Realisateur (""), m_Acteurs (""),
      m_Url (""), m_J (0), m_M (0), m_A (0), m_Duree (0), m_Id (0)
{
    srand (time(NULL));
    m_IdImg = rand();
}

inline std::string nsCatalog::CFilm::GetTitre() const { return m_Titre; }
inline std::string nsCatalog::CFilm::GetDesc () const { return m_Desc;  }
inline std::string nsCatalog::CFilm::GetGenre() const { return m_Genre; }
inline std::string nsCatalog::CFilm::GetRealisateur() const { return m_Realisateur;}
inline std::string nsCatalog::CFilm::GetActeurs    () const { return m_Acteurs;    }
inline std::string nsCatalog::CFilm::GetUrl        () const { return m_Url;        }
inline unsigned    nsCatalog::CFilm::GetDuree() const { return m_Duree; }
inline unsigned    nsCatalog::CFilm::GetId   () const { return m_Id; }
inline unsigned    nsCatalog::CFilm::GetIdImg() const { return m_IdImg; }
inline unsigned    nsCatalog::CFilm::GetJour () const { return m_J; }
inline unsigned    nsCatalog::CFilm::GetMois () const { return m_M; }
inline unsigned    nsCatalog::CFilm::GetAnnee() const { return m_A; }

inline void nsCatalog::CFilm::SetTitre (std::string Titre) { m_Titre = Titre; }
inline void nsCatalog::CFilm::SetDesc  (std::string Desc ) { m_Desc = Desc; }
inline void nsCatalog::CFilm::SetGenre (std::string Genre) { m_Genre = Genre; }
inline void nsCatalog::CFilm::SetRealisateur (std::string Realisateur) { m_Realisateur = Realisateur; }
inline void nsCatalog::CFilm::SetActeurs(std::string Acteurs) { m_Acteurs = Acteurs; }
inline void nsCatalog::CFilm::SetUrl    (std::string Url    ) { m_Url = Url; }
inline void nsCatalog::CFilm::SetDuree  (unsigned Duree) { m_Duree = Duree; }
inline void nsCatalog::CFilm::SetId     (unsigned Id) { m_Id = Id; }
inline void nsCatalog::CFilm::SetIdImg  (unsigned IdImg) { m_IdImg = IdImg; }
inline void nsCatalog::CFilm::SetJour   (unsigned J) { m_J = J; }
inline void nsCatalog::CFilm::SetMois   (unsigned M) { m_M = M; }
inline void nsCatalog::CFilm::SetAnnee  (unsigned A) { m_A = A; }

#endif
