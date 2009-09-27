#if !defined __CCONNEXION_H__
#define      __CCONNEXION_H__

#include <string>
#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket(), AF_INET
#include <netdb.h>      // gethostbyname()
#include <fstream>

#include "CException.h"

namespace nsCatalog
{
    class CConnexion
    {
      public:
        CConnexion (std::string Serveur = "");
        ~CConnexion();

        void Init () throw (CException);
        void Close();

        std::string GetPage (std::string Url) throw (CException);
        void        GetFich (std::string Url, std::ofstream & ofs) throw (CException);

        // Les modifieurs
        void SetServeur (std::string Serveur);

      private:
        std::string m_Serveur;
        int         m_Sd;

        void Convert (std::string & Str) const;
    };
}

#include "CConnexion.hxx"

#endif
