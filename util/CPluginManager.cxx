#include <iostream> 
#include <dirent.h> // opendir(), readdir(), closedir()
#include <string.h> // strlen()
#include <dlfcn.h>  // dl functions

#include "CException.h"
#include "CPluginManager.h"

#define CPLUGINMANAGER nsCatalog::CPluginManager

CPLUGINMANAGER * CPLUGINMANAGER::getInstance()
{
    return s_Instance ? s_Instance : s_Instance = new CPluginManager();
}

CPLUGINMANAGER * CPLUGINMANAGER::s_Instance = 0;

CPLUGINMANAGER::CPluginManager()
{
    // Loading of every plugins in the plugins directory 
    DIR * dir = opendir("plugins");
    
    if (dir)
    {
        struct dirent * ent;
        while ((ent = readdir(dir)) != NULL)
        {
            // Look at the extension
            if (ent->d_name[strlen(ent->d_name)-1] == 'o'
                    && ent->d_name[strlen(ent->d_name)-2] == 's'
                    && ent->d_name[strlen(ent->d_name)-3] == '.')
            {
                try
                {
                    LoadDyLibrary (ent->d_name);
                }
                catch (CException &e)
                {
                    std::cout << e.GetStr() << std::endl;
                }
            }
        }

        closedir (dir);
    }
}

void CPLUGINMANAGER::LoadDyLibrary (std::string name) throw (CException)
{
    // load the library
    void* lib = dlopen(("./plugins/" + name).c_str(), RTLD_NOW);
    if (!lib) 
        throw CException ("Cannot load library: " + std::string(dlerror()));
 
    // reset errors
    dlerror();
 
    // load the symbols
    make_t* create_plugin = (make_t*) dlsym(lib, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) 
        throw CException ("Cannot load symbol create: " + std::string(dlsym_error));
 
    // unload the library
    //dlclose(lib);

    // register the library
    _MPlugins[name] = create_plugin;
}

nsCatalog::ARecherche * CPLUGINMANAGER::Create (std::string name) throw (CException)
{
    if (_MPlugins.find(name + ".so") == _MPlugins.end())
        throw CException ("Le plugin " + name + " n'existe pas.");

    return _MPlugins[name + ".so"]();
}

std::vector<std::string> CPLUGINMANAGER::GetEngines()
{
    std::vector<std::string> VEngines;
    for (MPlugins_t::iterator i = _MPlugins.begin();
            i != _MPlugins.end() ; i++)
        VEngines.push_back (i->first.substr(0, i->first.size()-3));

    return VEngines;
}
