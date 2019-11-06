#ifndef TABLEPROCEDURES_H
#define TABLEPROCEDURES_H

#include <map>
#include <vector>
#include <string>
#include "Symbole.h"
#include "Procedure.h"
#include "VisiteurExecuter.h"

class TableProcedures {
public:
    static TableProcedures* getTable();
    void ajoutProcedure(Symbole nom, Procedure* procedure);
    void executerProcedure(Symbole nom, std::vector<Noeud*> parametres, VisiteurExecuter& v);
private:
    TableProcedures();
    static TableProcedures* table;

    std::vector<Procedure*> m_procedures;
    std::vector<std::string> m_noms;
};

#endif /* TABLEPROCEDURES_H */

