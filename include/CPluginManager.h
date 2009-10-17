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

#if !defined __CPLUGINMANAGER_H__
#define      __CPLUGINMANAGER_H__

#include <map>
#include <vector>

namespace nsCatalog
{
    class CException;
    class ARecherche;
    class CPluginManager
    {
      public:
        typedef ARecherche * make_t();

        static CPluginManager * getInstance();

        ARecherche * Create(std::string name) throw (CException);

        std::vector<std::string> GetEngines();

      private:
        typedef std::map<std::string, make_t*> MPlugins_t;
        static CPluginManager * s_Instance;
        MPlugins_t               _MPlugins;

        CPluginManager();

        void LoadDyLibrary (std::string name) throw (CException);
    };
}

#endif
