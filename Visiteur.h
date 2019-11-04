/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Visiteur.h
 * Author: feelzor
 *
 * Created on October 30, 2019, 3:01 PM
 */

#ifndef VISITEUR_H
#define VISITEUR_H

#include "ArbreAbstrait.h"
#include "SymboleValue.h"

class Visiteur {
public:
    virtual void visiterNoeudSeqInst(NoeudSeqInst* noeud) = 0;
    virtual void visiterNoeudAffectation(NoeudAffectation* noeud) = 0;
    virtual void visiterNoeudOperateurBinaire(NoeudOperateurBinaire* noeud) = 0;
    virtual void visiterNoeudInstSi(NoeudInstSi* noeud) = 0;
    virtual void visiterNoeudInstRepeter(NoeudInstRepeter* noeud) = 0;
    virtual void visiterNoeudInstPour(NoeudInstPour* noeud) = 0;
    virtual void visiterNoeudInstTantQue(NoeudInstTantQue* noeud) = 0;
    virtual void visiterNoeudInstLire(NoeudInstLire* noeud) = 0;
    virtual void visiterNoeudInstEcrire(NoeudInstEcrire* noeud) = 0;
    virtual void visiterNoeudInstAppel(NoeudInstAppel* noeud) = 0;
    virtual void visiterSymboleValue(SymboleValue* symbole) = 0;
};

#endif /* VISITEUR_H */

