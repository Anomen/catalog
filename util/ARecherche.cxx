#include "ARecherche.h"

#include <iconv.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace nsCatalog;

vector<string> ARecherche::Extraire (const string & Texte, string Begin, string End) const
{
    int Pos (0), Taille (Begin.size());
    vector<string> VResult;

    while (true)
    {
        if ((Pos = Texte.find (Begin, Pos+1)) == -1)
            break;
        Pos += Taille;

        try
        {
            int PosEnd = Texte.find (End, Pos);

            string Temp (Texte.substr (Pos, PosEnd - Pos));
            VResult.push_back (Temp);

            Pos = PosEnd + End.size();
        }
        catch (const exception &) { break; }
    }

    return VResult;

}

string ARecherche::GetPage (string Url) throw (CException)
{
    iconv_t cd = iconv_open ("UTF-8", "ISO-8859-1");

    string Page = m_Cnx.GetPage (Url);
    char In[Page.size()], Out[Page.size()];

    char *ptrOut = Out;
    char *ptrIn  = In ;

    strcpy (In, Page.c_str());

    size_t szIn  = Page.size();
    size_t szOut = Page.size();

    iconv (cd, &ptrIn, &szIn, &ptrOut, &szOut);
    iconv_close (cd);

    *ptrOut = '\0';

    return string (Out);
}
