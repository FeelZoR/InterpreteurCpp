/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VisiteurCompiler.h
 * Author: feelzor
 *
 * Created on October 30, 2019, 3:10 PM
 */

#ifndef VISITEURCOMPILER_H
#define VISITEURCOMPILER_H

#include "Visiteur.h"

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

