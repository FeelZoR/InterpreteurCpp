#ifndef VISITEUREXECUTER_H
#define VISITEUREXECUTER_H

#include "Visiteur.h";

class VisiteurExecuter : public Visiteur {
public:
    void visiterNoeudSeqInst(NoeudSeqInst* noeud) override;
    void visiterNoeudAffectation(NoeudAffectation* noeud) override;
    void visiterNoeudOperateurBinaire(NoeudOperateurBinaire* noeud) override;
    void visiterNoeudInstSi(NoeudInstSi* noeud) override;
    void visiterNoeudInstRepeter(NoeudInstRepeter* noeud) override;
    void visiterNoeudInstPour(NoeudInstPour* noeud) override;
    void visiterNoeudInstTantQue(NoeudInstTantQue* noeud) override;
    void visiterNoeudInstLire(NoeudInstLire* noeud) override;
    void visiterNoeudInstEcrire(NoeudInstEcrire* noeud) override;
    void visiterSymboleValue(SymboleValue* symbole) override;
private:
    int m_derniereValeur = 0;
};

#endif /* VISITEUREXECUTER_H */

