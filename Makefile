# Les fichiers inclus
CONSTANTS_H  = include/constants.h
CEXCEPTION_H = include/CException.h include/CException.hxx
CCONNEXION_H = include/CConnexion.h include/CConnexion.hxx $(CEXCEPTION_H)
CFILM_H      = include/CFilm.h include/CFilm.hxx 
UTIL_H       = include/util.h
CPLUGINMANAGER = include/CPluginManager.h $(CEXCEPTION_H)
CDATABASE_H  = include/CDatabase.h include/CDatabase.hxx \
               $(CEXCEPTION_H) $(CFILM_H)

# moteurs de recherche
ARECHERCHE_H = include/ARecherche.h include/ARecherche.hxx \
			   $(CFILM_H) $(CCONNEXION_H)
#ALLOCINE_FR_H = $(ARECHERCHE_H) $(CFILM_H) $(UTIL_H)

# Commande générique pour compiler
OPT = -Wall -O3 -Iinclude/ -Iinclude/moteurs/ -ldl -rdynamic
COMPILER = g++ -c $< -o $*.o $(OPT)

# Nom du programme à compiler
nom = serveur

$(nom): lib/$(nom).o lib/CConnexion.o lib/CFilm.o lib/CDatabase.o \
		lib/sqlite3.o lib/ARecherche.o lib/util.o lib/CPluginManager.o
	g++ -s $^ -o $(nom) $(OPT) -lpthread

lib/$(nom).o: projet/$(nom).cxx $(CEXCEPTION_H) $(CONSTANTS_H)
	$(COMPILER)

lib/CConnexion.o: util/CConnexion.cxx $(CCONNEXION_H)
	$(COMPILER)

lib/util.o: util/util.cxx $(UTIL_H)
	$(COMPILER)

lib/CFilm.o: util/CFilm.cxx $(CFILM_H)
	$(COMPILER)

lib/CDatabase.o : util/CDatabase.cxx $(CDATABASE_H)
	$(COMPILER)

lib/CPluginManager.o : util/CPluginManager.cxx $(CPLUGINMANAGER)
	$(COMPILER)

lib/ARecherche.o : util/ARecherche.cxx $(ARECHERCHE_H)
	$(COMPILER)

lib/sqlite3.o : util/sqlite3.c include/sqlite3.h
	gcc -c $< -o $*.o -w

# For plugin
plugin: lib/ARecherche.o plugins/lib/$(nom).o
	g++ -o plugins/$(nom).so $^ -shared

plugins/lib/$(nom).o: plugins/src/$(nom).cxx plugins/src/$(nom).h $(ARECHERCHE_H) $(CFILM_H) $(UTIL_H)
	$(COMPILER)

clean:
	rm lib/*.o -fv; rm serveur -fv;
