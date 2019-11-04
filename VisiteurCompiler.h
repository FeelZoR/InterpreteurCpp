#ifndef VISITEURCOMPILER_H
#define VISITEURCOMPILER_H

#include "Visiteur.h"

// Cette classe permet de transpiler le code en Python3
class VisiteurCompiler : public Visiteur {
public:
    VisiteurCompiler(std::ostream& out, int indentation);
    
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
    std::ostream& m_out;
    int m_indentation;
};

#endif /* VISITEURCOMPILER_H */

