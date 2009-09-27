#include <stdlib.h>
#include <iconv.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "util.h"

using namespace std;

void nsCatalog::Urlencode (string & Str) 
{
    setlocale(LC_ALL,"");

    ostringstream oss;

    wstring t;
    t.resize (Str.size());
    t.resize (mbstowcs (&t[0], Str.c_str(), Str.size()));

    unsigned Taille (t.size());

    for (unsigned i (0); i < Taille; ++i)
        if ((L'a' <= t[i] && t[i] <= L'z')
            || (L'A' <= t[i] && t[i] <= L'Z')
            || (L'0' <= t[i] && t[i] <= L'9')
            || (t[i] == L'-' || t[i] == L'_' || t[i] == L'.'))
        {
            char c;
            wcstombs (&c, &t[i], 1);
            oss << c;
        }
        else 
        {
            oss << '%' << hex << int (t[i]);
        }

    Str = oss.str();
}
