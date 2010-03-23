#
# This file is part of catalg-server.
# Copyright (C) 2009  Kevin Vicrey <kevin.vicrey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Les fichiers inclus
CONSTANTS_H  = include/constants.h
CEXCEPTION_H = include/CException.h include/CException.hxx
CCONNEXION_H = include/CConnection.h include/CConnection.hxx $(CEXCEPTION_H)
CFILM_H      = include/CFilm.h include/CFilm.hxx 
UTIL_H       = include/util.h
CPLUGINMANAGER = include/CPluginManager.h $(CEXCEPTION_H)
CDATABASE_H  = include/CDatabase.h include/CDatabase.hxx \
               $(CEXCEPTION_H) $(CFILM_H)

# moteurs de recherche
ASEARCH_H = include/ASearch.h include/ASearch.hxx \
			   $(CFILM_H) $(CCONNEXION_H)
#ALLOCINE_FR_H = $(ASEARCH_H) $(CFILM_H) $(UTIL_H)

# Commande générique pour compiler
OPT = -Wall -O3 -Iinclude/ -Iinclude/moteurs/ -ldl -rdynamic
COMPILER = g++ -c $< -o $*.o $(OPT)

# Nom du programme à compiler
nom = serveur

$(nom): lib/$(nom).o lib/CConnection.o lib/CFilm.o lib/CDatabase.o \
		lib/sqlite3.o lib/ASearch.o lib/util.o lib/CPluginManager.o
	g++ -s $^ -o $(nom) $(OPT) -lpthread

lib/$(nom).o: projet/$(nom).cxx $(CEXCEPTION_H) $(CONSTANTS_H)
	$(COMPILER)

lib/CConnection.o: util/CConnection.cxx $(CCONNEXION_H)
	$(COMPILER)

lib/util.o: util/util.cxx $(UTIL_H)
	$(COMPILER)

lib/CFilm.o: util/CFilm.cxx $(CFILM_H)
	$(COMPILER)

lib/CDatabase.o : util/CDatabase.cxx $(CDATABASE_H)
	$(COMPILER)

lib/CPluginManager.o : util/CPluginManager.cxx $(CPLUGINMANAGER)
	$(COMPILER)

lib/ASearch.o : util/ASearch.cxx $(ASEARCH_H)
	$(COMPILER)

lib/sqlite3.o : util/sqlite3.c include/sqlite3.h
	gcc -c $< -o $*.o -w

# For plugin
plugin: lib/ASearch.o plugins/lib/$(nom).o
	g++ -o plugins/$(nom).so $^ -shared

plugins/lib/$(nom).o: plugins/src/$(nom).cxx plugins/src/$(nom).h $(ASEARCH_H) $(CFILM_H) $(UTIL_H)
	$(COMPILER)

clean:
	rm lib/*.o -fv; rm serveur -fv;
