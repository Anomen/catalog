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

#include "CException.h"
#include "CDatabase.h"
#include "constants.h"
#include "ARecherche.h"
#include "CPluginManager.h"

#include <sys/types.h>  // socket(), bind()
#include <sys/socket.h> // socket(), bind()
#include <arpa/inet.h>  // sockaddr_in
#include <signal.h>     // signal()
#include <sys/wait.h>   // wait()
#include <string.h>     // memset()
#include <stdlib.h>     // atoi()
#include <limits.h>     // MAX_INPUT

#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace nsCatalog;

namespace
{
    void SignalFils (int Num)
    {
        wait (0);
    }

    void Write (int sd, char *buf, size_t taille) throw (CException)
    {
        strcat (buf, "\nFIN");
        if (write (sd, buf, taille) < 0)
            throw CException (ERR_WRITE);

    }

    void Write (int sd, string Msg) throw (CException)
    {
        Msg += "\nFIN";
        if (write (sd, Msg.c_str(), Msg.size()) < 0)
            throw CException (ERR_WRITE);

    }

    string CmdList() throw (CException)
    {
        CDatabase db;

        try {
            db.Init();
        } catch (const CException &e) {
            throw CException (ERR_CONN_DB);
        }

        VFilm_t VFilm;
        try {
            VFilm = db.GetAllFilms();
        } catch (const CException &e) {
            throw CException (ERR_DB);
        }

        ostringstream oss;
        oss << SUCCESS << '\n';
        for (VFilm_t::iterator i = VFilm.begin(); i < VFilm.end(); i++)
            oss << (*i).GetId() << ' ' << (*i).GetTitre() << '\n';

        return oss.str();

    }

    string CmdEngines() throw (CException)
    {
        vector<string> VEngines = CPluginManager::getInstance()->GetEngines();

        ostringstream oss;
        oss << SUCCESS << '\n';

        for (vector<string>::iterator i = VEngines.begin();
                i < VEngines.end(); i++)
            oss << *i << '\n';

        return oss.str();

    }

    string CmdSearch (string Moteur, string Titre) throw (CException)
    {
        ARecherche *Net;

        try {
            Net = CPluginManager::getInstance()->Create(Moteur);
        } catch (const CException &e) {
            throw CException (ERR_NOT_ENGINE);
        }

        VFilm_t VFilm;
        
        try {
            VFilm = Net->Recherche (Titre);
        } catch (const CException &e) {
            throw CException (ERR_NOT_FND);
        }

        ostringstream oss;
        oss << SUCCESS << '\n';
        unsigned Num (0);
        for (VFilm_t::iterator i = VFilm.begin(); i < VFilm.end(); i++)
            oss << ++Num << ' ' << (*i).GetTitre() << '\n';

        return oss.str();

    }

    string CmdImage (int sd, string Titre) throw (CException)
    {
        CDatabase db;

        try {
            db.Init();
        } catch (const CException &e) {
            throw CException (ERR_CONN_DB);
        }

        VFilm_t VFilm;
        try {
            VFilm = db.GetFilm (Titre);
        } catch (const CException &e) {
            throw CException (ERR_NOT_FND);
        }

        ostringstream succ;
        succ << SUCCESS << "\n";

        ostringstream oss;
        oss << VFilm.at(0).GetIdImg();
        ifstream ifs (oss.str().c_str());
        char Buf[MAX_INPUT];
        while (1)
        {
            int NbLu = ifs.readsome (Buf, MAX_INPUT);
            if (write (sd, Buf, NbLu) <= 0)
                break;
        }

        if (write (sd, "FIN", 3) <= 0)
            throw CException (ERR_WRITE);

        return "";

    }

    string CmdShow (string Titre) throw (CException)
    {
        CDatabase db;

        try {
            db.Init();
        } catch (const CException &e) {
            throw CException (ERR_CONN_DB);
        }

        VFilm_t VFilm;
        try {
            VFilm = db.GetFilm (Titre);
        } catch (const CException &e) {
            throw CException (ERR_NOT_FND);
        }

        ostringstream oss;
        oss << SUCCESS << '\n';
        if (VFilm.size() > 1)
            for (VFilm_t::iterator i = VFilm.begin(); i < VFilm.end(); i++)
                oss << (*i).GetId() << ' ' << (*i).GetTitre() << '\n';
        else
            oss << VFilm[0].GetId()    << '\n'
                << VFilm[0].GetTitre() << '\n'
                << VFilm[0].GetDesc()  << '\n'
                << VFilm[0].GetGenre() << '\n'
                << VFilm[0].GetRealisateur() << '\n'
                << VFilm[0].GetActeurs()  << '\n'
                << VFilm[0].GetJour() << '/' << VFilm[0].GetMois() << '/' << VFilm[0].GetAnnee() << '\n'
                << VFilm[0].GetDuree() << '\n';

        return oss.str();

    }

    string CmdAdd (string Moteur, string Titre, unsigned Num)
    {
        if (!Num--)
            throw CException (ERR_CMD);

        ARecherche *Net;
        try {
            Net = CPluginManager::getInstance()->Create(Moteur);
        } catch (const CException &e) {
            throw CException (ERR_NOT_ENGINE);
        }

        VFilm_t VFilm;
        try {
            VFilm = Net->Recherche (Titre);
        } catch (const CException & e) {
            throw CException (ERR_NOT_FND);
        }

        CDatabase db;
        try {
            db.Init();
        } catch (const CException &e) {
            throw CException (ERR_CONN_DB);
        }

        try
        {
            bool Exists (false);
            try {
                db.GetFilm (VFilm.at(Num).GetTitre());
                Exists = true;
            } catch (const CException &) { }

            if (Exists) throw CException (ERR_RDY_ADD);

            Net->Detail (VFilm.at(Num));
            db.Ajouter (VFilm.at(Num));
        }
        catch (const exception & e) {
            throw CException (ERR_INDICE);
        }

        ostringstream oss;
        oss << SUCCESS << '\n';
        return oss.str();

    }

    string CmdRemove (string Titre, int Num)
    {
        if (!Num--)
            throw CException (ERR_CMD);

        CDatabase db;
        try {
            db.Init();
        } catch (const CException &e) {
            throw CException (ERR_CONN_DB);
        }

        VFilm_t VFilm;
        try {
            VFilm = db.GetFilm (Titre);
        } catch (const CException &e) {
            throw CException (ERR_NOT_FND);
        }

        unsigned img = VFilm.at(Num).GetIdImg();

        try {
            db.Supprimer (VFilm.at(Num));
        } catch (const exception & e) {
            throw CException (ERR_INDICE);
        }

        ostringstream strImg;
        strImg << img;
        unlink (strImg.str().c_str());

        ostringstream oss;
        oss << SUCCESS << '\n';
        return oss.str();

    }

}

int main (int argc, char **argv)
{
    int sd (0);

    signal (SIGCHLD, SignalFils);
    CPluginManager::getInstance();

    try
    {
        if (argc == 1)
            throw CException ("Usage: serveur <port>");
        if ((sd = socket (PF_INET, SOCK_STREAM, 0)) < 0)
            throw CException ("Impossible de créer la socket.");

        sockaddr_in Addr;
        memset (&Addr, 0, sizeof (Addr));
        Addr.sin_family = AF_INET;
        Addr.sin_port = htons (atoi (argv[1]));
        Addr.sin_addr.s_addr = INADDR_ANY;

        if (bind (sd, (sockaddr *) & Addr, sizeof (Addr)) < 0)
            throw CException ("Impossible d'attacher la socket (bind).");

        if (listen (sd, 32) < 0)
            throw CException ("Impossible d'écouter sur la socket (listen).");

        while (true)
        {
            int sdCom (accept (sd, 0, 0)), NbLu (0);

            if (fork()) continue;

            close (sd);

            try
            {
                while (true)
                {
                    string Choix;
                    while (true)
                    {
                        char Msg[MAX_INPUT];
                        if ((NbLu = read (sdCom, (void *) & Msg, MAX_INPUT)) <= 0)
                            throw CException ("Erreur de lecture."); 

                        Msg[NbLu] = '\0';
                        Choix += string (Msg);

                        if (!strcmp (& Msg[strlen(Msg)-3], "FIN"))
                            break;
                    }

                    Choix = Choix.substr(0, Choix.size()-4);

                    cout << Choix << endl;
                    string Retour;

                    try
                    {
                        // LIST
                        if (Choix == "LIST")
                            Retour = CmdList();

                        // ENGINES
                        else if (Choix == "ENGINES")
                            Retour = CmdEngines();

                        // IMAGE <film>
                        // IMAGE <idfilm>
                        else if (!Choix.find ("IMAGE"))
                        {
                            CmdImage (sdCom, Choix.substr (6, Choix.size()));
                            continue;
                        }

                        // SHOW <film>
                        // SHOW <idfilm>
                        else if (!Choix.find ("SHOW"))
                            Retour = CmdShow (Choix.substr (5, Choix.size()));

                        // SEARCH <engine> <film>
                        else if (!Choix.find ("SEARCH"))
                        {
                            int FirstSpace = Choix.find_first_of(' ', 7);
                            Retour = CmdSearch(
                                Choix.substr (7         , FirstSpace-7), // engine
                                Choix.substr (FirstSpace, Choix.size())  // film
                            );
                        }

                        // ADD <engine> <film> <num>
                        else if (!Choix.find ("ADD"))
                        {
                            int FirstSpace  = Choix.find_first_of (' ', 4);
                            int LastSpace   = Choix.find_last_of  (' '   );
                            Retour = CmdAdd (
                                Choix.substr (4, FirstSpace-4),                        // engine
                                Choix.substr (FirstSpace, LastSpace-FirstSpace),       // film
                                atoi (Choix.substr (LastSpace, Choix.size()).c_str())  // num
                            );
                        }

                        // REMOVE <film> <num>
                        // REMOVE <idfilm>
                        else if (!Choix.find ("REMOVE"))
                            Retour = CmdRemove (
                                Choix.substr (7, Choix.find_last_of (' ')-7), 
                                atoi (Choix.substr (Choix.find_last_of (' '), 
                                              Choix.size()).c_str())
                            );
                        else
                            throw CException (ERR_CMD);

                        Write (sdCom, Retour);

                    }
                    catch (const CException &e) 
                    {
                        try {
                            Write (sdCom, e.GetStr() + "\n");
                        }
                        catch (...) { }
                    }
                    catch (const exception &e)
                    {
                        ostringstream oss;
                        oss << ERR_CMD << '\n';

                        try {
                            Write (sdCom, oss.str());
                        }
                        catch (...) { }
                    }
                }
            }
            catch (const CException &e) { }

            close (sdCom);
            return EXIT_SUCCESS;
        }
    }
    catch (const CException & e)
    {
        cerr << e.GetStr() << endl;
    }

    if (sd >= 0)
        close (sd);

    return EXIT_SUCCESS;
}
