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

#if !defined __CONSTANTS_H__
#define      __CONSTANTS_H__

namespace nsCatalog
{
    const uint16_t CstPort = 2001;

    enum Errno {
        SUCCESS        = 200,
        ERR_WRITE      = 300,  // Erreur d'écriture avec write
        ERR_CONN_DB    = 301,  // Erreur de connexion à la BD.
        ERR_DB         = 302,  // Erreur de BD.
        ERR_NOT_FND    = 303,  // Aucun film trouvé.
        ERR_RDY_ADD    = 304,  // Film déjà ajouté.
        ERR_CMD        = 305,  // Erreur sur la commande
        ERR_INDICE     = 306,  // Erreur sur l'indice du film
        ERR_NOT_ENGINE = 307,  // Le moteur de recherche n'existe pas
    };

    const std::string CstDbFileName ("catalog.db");
}

#endif
