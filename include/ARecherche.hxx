#if !defined __ARECHERCHE_HXX__
#define      __ARECHERCHE_HXX__

#include <iostream>

#define ARECHERCHE nsCatalog::ARecherche

inline ARECHERCHE::~ARecherche() { }

inline ARECHERCHE::ARecherche (std::string Serveur)
{
    try
    {
        m_Cnx.SetServeur (Serveur);
        //m_Cnx.Init();
    }
    catch (const CException & E)
    {
        std::cerr << "Impossible de créer une connexion.\n";
    }
}

inline bool ARECHERCHE::Supprimer (std::string & Texte, int PosBeg, int PosEnd) const
{
    if (PosBeg == -1 || PosEnd == -1) return false;

    bool Rempl (false);
    std::string Temp;

    try
    {
        Temp  = Texte.substr (0, PosBeg);
        Temp += Texte.substr (PosEnd, Texte.size() - PosEnd);
        Texte = Temp;
        Rempl = true;
    }
    catch (const std::exception &) { }

    return Rempl;
}

inline bool ARECHERCHE::Supprimer (std::string & Texte, std::string Str) const
{
    bool Rempl (false);

    while (true)
    {
        int i (Texte.find (Str));

        if (i != -1) 
        {
            Rempl = true;
            Supprimer (Texte, i, i+Str.size());
        }
        else break;
    }

    return Rempl;
}

inline void ARECHERCHE::GetFich (std::string Url, std::ofstream & ofs) throw (CException)
{
    m_Cnx.GetFich (Url, ofs);
}

#endif
