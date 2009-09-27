#if !defined __CCONNEXION_HXX__
#define      __CCONNEXION_HXX__

#define CCONNEXION nsCatalog::CConnexion

inline CCONNEXION::CConnexion (std::string Serveur /* = "" */)
    : m_Serveur (Serveur)
{ }

inline CCONNEXION::~CConnexion()
{
    Close();
}

inline void CCONNEXION::Close()
{
    close (m_Sd);
    m_Sd = 0;
}

inline void CCONNEXION::SetServeur (std::string Serveur)
{
    m_Serveur = Serveur;
}

#endif
