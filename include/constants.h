#if !defined __CONSTANTS_H__
#define      __CONSTANTS_H__

namespace nsCatalog
{
    const uint16_t CstPort = 2001;

    enum Errno {
        SUCCESS        = 200,
        ERR_WRITE      = 300,  // Erreur d'écriture avec write
        ERR_CONN_DB    = 301,  // Erreur de connexion à la BD.
        ERR_DB         = 302,  // Erreur de BD.
        ERR_NOT_FND    = 303,  // Aucun film trouvé.
        ERR_RDY_ADD    = 304,  // Film déjà ajouté.
        ERR_CMD        = 305,  // Erreur sur la commande
        ERR_INDICE     = 306,  // Erreur sur l'indice du film
        ERR_NOT_ENGINE = 307,  // Le moteur de recherche n'existe pas
    };

    const std::string CstDbFileName ("catalog.db");
}

#endif
