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

#if !defined __CFILM_HXX__
#define      __CFILM_HXX__

#include <stdlib.h>

inline nsCatalog::CFilm::CFilm()
    : m_Title (""), m_Desc (""), m_Genre (""), m_Creator (""), m_Actors (""),
      m_Url (""), m_J (0), m_M (0), m_A (0), m_Time (0), m_Id (0)
{
    srand (time(NULL));
    m_IdImg = rand();
}

inline std::string nsCatalog::CFilm::GetTitle() const { return m_Title; }
inline std::string nsCatalog::CFilm::GetDesc () const { return m_Desc;  }
inline std::string nsCatalog::CFilm::GetGenre() const { return m_Genre; }
inline std::string nsCatalog::CFilm::GetCreator() const { return m_Creator;}
inline std::string nsCatalog::CFilm::GetActors    () const { return m_Actors;    }
inline std::string nsCatalog::CFilm::GetUrl        () const { return m_Url;        }
inline unsigned    nsCatalog::CFilm::GetTime() const { return m_Time; }
inline unsigned    nsCatalog::CFilm::GetId   () const { return m_Id; }
inline unsigned    nsCatalog::CFilm::GetIdImg() const { return m_IdImg; }
inline unsigned    nsCatalog::CFilm::GetJour () const { return m_J; }
inline unsigned    nsCatalog::CFilm::GetMois () const { return m_M; }
inline unsigned    nsCatalog::CFilm::GetAnnee() const { return m_A; }

inline void nsCatalog::CFilm::SetTitle (std::string Title) { m_Title = Title; }
inline void nsCatalog::CFilm::SetDesc  (std::string Desc ) { m_Desc = Desc; }
inline void nsCatalog::CFilm::SetGenre (std::string Genre) { m_Genre = Genre; }
inline void nsCatalog::CFilm::SetCreator (std::string Creator) { m_Creator = Creator; }
inline void nsCatalog::CFilm::SetActors(std::string Actors) { m_Actors = Actors; }
inline void nsCatalog::CFilm::SetUrl    (std::string Url    ) { m_Url = Url; }
inline void nsCatalog::CFilm::SetTime  (unsigned Time) { m_Time = Time; }
inline void nsCatalog::CFilm::SetId     (unsigned Id) { m_Id = Id; }
inline void nsCatalog::CFilm::SetIdImg  (unsigned IdImg) { m_IdImg = IdImg; }
inline void nsCatalog::CFilm::SetJour   (unsigned J) { m_J = J; }
inline void nsCatalog::CFilm::SetMois   (unsigned M) { m_M = M; }
inline void nsCatalog::CFilm::SetAnnee  (unsigned A) { m_A = A; }

#endif
