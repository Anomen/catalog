#include <sys/types.h>  // socket(), connect()
#include <sys/socket.h> // socket(), AF_INET, connect()
#include <netdb.h>      // gethostbyname()
#include <string.h>     // memset()
#include <stdlib.h>     // atoi()
#include <limits.h>     // MAX_INPUT
#include <string>
#include <iostream>
#include <sstream>

#include "CConnexion.h"
#include "CException.h"

using namespace nsCatalog;
using namespace std;

void CConnexion::Init() throw (CException)
{
    if (m_Serveur == "")
        throw CException ("Aucun serveur précisé.");

    if ((m_Sd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
        throw CException ("Erreur à l'ouverture de la socket.");

    hostent * Hostent = gethostbyname (m_Serveur.c_str());
    if (!Hostent)
        throw CException ("Erreur gethostbyname()");

    sockaddr_in Adr;
    memset (&Adr, 0, sizeof (Adr));
    Adr.sin_family      = AF_INET;
    Adr.sin_port        = htons (80);
    Adr.sin_addr.s_addr = ((struct in_addr *) (Hostent->h_addr))->s_addr;

    if (connect (m_Sd, reinterpret_cast<sockaddr* > (&Adr), sizeof (Adr)) < 0)
        throw CException ("Erreur connect()");
}

string CConnexion::GetPage (string Url) throw (CException)
{
    Init();

    int NbEnv (0); 
    string Str = "GET /" + Url + " HTTP/1.0\r\nHost: " + m_Serveur + "\r\n\r\n";
    if ((NbEnv = write (m_Sd, Str.c_str(), Str.size())) <= 0)
    {
        Close();
        throw CException ("Erreur send()");
    }

    int NbLu (0);
    string Page;
    char Buf[MAX_INPUT];

    while (1) {
        if (!(NbLu = read (m_Sd, Buf, MAX_INPUT))) break;
        Buf[NbLu] = '\0';
        Page += string (Buf);
    }
    Page[Page.size()-1] = '\0';

    if (atoi (Page.substr (8, 12).c_str()) != 200)
        throw CException ("Erreur de requête : " + Page.substr (9, Page.find_first_of ('\n')));

    Close();
    return Page;
}

void CConnexion::GetFich (string Url, ofstream &ofs) throw (CException)
{
    Init();

    string Serveur (Url.substr (7, Url.find_first_of ("/", 7)-7));
    Url = Url.substr (Url.find_first_of ("/", 7), Url.size() - Url.find_first_of ("/", 7));

    int NbEnv (0); 
    string Str = "GET " + Url + " HTTP/1.0\r\nHost: " + Serveur + "\r\n\r\n";
    if ((NbEnv = write (m_Sd, Str.c_str(), Str.size())) <= 0)
    {
        Close();
        throw CException ("Erreur send()");
    }

    int NbLu (0);
    char Buf[MAX_INPUT];
    bool enteteTrouvee (false);

    while (1) {
        if (!(NbLu = read (m_Sd, Buf, MAX_INPUT))) break;

        if (!enteteTrouvee)
        {
            char *Search;
            if ((Search = strstr (Buf, "\r\n\r\n")))
            {
                ofs.write (Search+4, NbLu - 4 - (Search-Buf));
                enteteTrouvee = true;
                continue;
            }
        }
        else
            ofs.write (Buf, NbLu);
    }

    Close();
}
