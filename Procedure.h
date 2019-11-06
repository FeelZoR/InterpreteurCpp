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
private:
    TableSymboles              m_table;
    Noeud*                     m_sequence;
    std::vector<SymboleValue*> m_params;
};

#endif /* PROCEDURE_H */

