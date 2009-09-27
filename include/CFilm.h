#if !defined __CFILM_H__
#define      __CFILM_H__

#include <string>
#include <vector>

namespace nsCatalog
{
    class CFilm
    {
      public:
        CFilm();

        // Modifieurs
        std::string GetRealisateur () const;
        std::string GetTitre  () const;
        std::string GetDesc   () const;
        std::string GetGenre  () const;
        std::string GetActeurs() const;
        std::string GetUrl    () const;
        unsigned    GetJour   () const;
        unsigned    GetMois   () const;
        unsigned    GetAnnee  () const;
        unsigned    GetDuree  () const;
        unsigned    GetId     () const;
        unsigned    GetIdImg  () const;

        friend std::ostream & operator << (std::ostream &os, const CFilm &F);

        // Modifieurs
        void SetTitre (std::string Titre);
        void SetDesc  (std::string Desc );
        void SetGenre (std::string Genre);
        void SetRealisateur (std::string Realisateur);
        void SetActeurs (std::string Acteurs);
        void SetUrl     (std::string Url);
        void SetJour  (unsigned J);
        void SetMois  (unsigned M);
        void SetAnnee (unsigned A);
        void SetDuree (unsigned Duree);
        void SetId    (unsigned Id);
        void SetIdImg (unsigned IdImg);

      private:
        std::string m_Titre, m_Desc, m_Genre, m_Realisateur, m_Acteurs, m_Url;
        unsigned    m_J, m_M, m_A, m_Duree, m_Id, m_IdImg;
    };

    typedef std::vector <CFilm> VFilm_t;
}

#include "CFilm.hxx"

#endif
