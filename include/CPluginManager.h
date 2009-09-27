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
