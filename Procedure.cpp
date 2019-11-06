#include "Procedure.h"
#include "Visiteur.h"

Procedure::Procedure(TableSymboles table, Noeud* sequence, std::vector<SymboleValue*> parametres) 
: m_table(table), m_sequence(sequence), m_params(parametres) {}

void Procedure::accepter(Visiteur& visiteur) {
    visiteur.visiterProcedure(this);
}

void Procedure::empiler(std::vector<Valeur*> parametres) {
    for (int i = 0; i < parametres.size() && i < m_params.size(); i++) {
        m_params[i]->setValeur(parametres[i]);
    }
}
