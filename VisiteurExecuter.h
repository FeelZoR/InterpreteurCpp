#ifndef VISITEUREXECUTER_H
#define VISITEUREXECUTER_H

#include "Visiteur.h"
#include "Valeur.h"

class VisiteurExecuter : public Visiteur {
public:
    void visiterProcedure(Procedure* procedure) override;
    void visiterNoeudSeqInst(NoeudSeqInst* noeud) override;
    void visiterNoeudAffectation(NoeudAffectation* noeud) override;
    void visiterNoeudOperateurBinaire(NoeudOperateurBinaire* noeud) override;
    void visiterNoeudInstSi(NoeudInstSi* noeud) override;
    void visiterNoeudInstRepeter(NoeudInstRepeter* noeud) override;
    void visiterNoeudInstPour(NoeudInstPour* noeud) override;
    void visiterNoeudInstTantQue(NoeudInstTantQue* noeud) override;
    void visiterNoeudInstLire(NoeudInstLire* noeud) override;
    void visiterNoeudInstEcrire(NoeudInstEcrire* noeud) override;
    void visiterNoeudInstAppel(NoeudInstAppel* noeud) override;
    void visiterSymboleValue(SymboleValue* symbole) override;
    
    inline Valeur* getDerniereValeur() { return m_derniereValeur; }
private:
    Valeur* m_derniereValeur;
};

#endif /* VISITEUREXECUTER_H */

