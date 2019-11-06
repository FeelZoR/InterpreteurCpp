#include "TableProcedures.h"
#include "Procedure.h"
#include <algorithm>

TableProcedures* TableProcedures::table = nullptr;

TableProcedures::TableProcedures() {}

TableProcedures* TableProcedures::getTable() {
    if (table == nullptr) {
        table = new TableProcedures();
    }
    
    return table;
}

void TableProcedures::ajoutProcedure(Symbole nom, Procedure* procedure) {
    std::string nomProcedure(nom.getChaine());
    m_noms.push_back(nomProcedure);
    m_procedures.push_back(procedure);
    procedure->setNom(nomProcedure);
}

void TableProcedures::executerProcedure(Symbole nom, std::vector<Noeud*> parametres, VisiteurExecuter& v) {
    long i = std::distance(m_noms.begin(), std::find(m_noms.begin(), m_noms.end(), nom.getChaine()));
    Procedure* p = m_procedures[i];
    std::vector<Valeur*> params;
    for (Noeud* noeud : parametres) {
        noeud->accepter(v);
        params.push_back(v.getDerniereValeur());
    }
    p->empiler(params);
    p->accepter(v);
}

void TableProcedures::compilerProcedures(VisiteurCompiler& v) {
    for (int i = 0; i < m_procedures.size(); i++) {
        Procedure* procedure = m_procedures[i];
        procedure->accepter(v);
    }
}
