#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "ArbreAbstrait.h"
#include "TableSymboles.h"
#include "SymboleValue.h"
#include <vector>

class Procedure : public Noeud {
public:
    Procedure(TableSymboles table, Noeud* sequence, std::vector<SymboleValue*> parametres);
    void accepter(Visiteur& visiteur) override;
    void empiler(std::vector<Valeur*> parametres);
    
    inline void setNom(std::string nom) { m_nom = nom; }
    
    inline std::string                getNom()        const { return m_nom;      }
    inline Noeud*                     getSequence()   const { return m_sequence; }
    inline std::vector<SymboleValue*> getParametres() const { return m_params;   }
private:
    TableSymboles              m_table;
    Noeud*                     m_sequence;
    std::vector<SymboleValue*> m_params;
    std::string                m_nom;
};

#endif /* PROCEDURE_H */

