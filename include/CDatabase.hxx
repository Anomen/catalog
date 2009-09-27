#if !defined __CDATABASE_HXX__
#define      __CDATABASE_HXX__

#define CDATABASE nsCatalog::CDatabase

inline CDATABASE::CDatabase(const std::string &db)
    : m_dbName(db), m_Db(0)
{
}

inline CDATABASE::~CDatabase()
{
    if (m_Db) sqlite3_close (m_Db);
}

#endif
