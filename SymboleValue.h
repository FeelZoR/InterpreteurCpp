#ifndef SYMBOLEVALUE_H
#define SYMBOLEVALUE_H

#include <string.h>
#include <iostream>
using namespace std;

#include "Symbole.h"
#include "ArbreAbstrait.h"
#include "Valeur.h"

class Visiteur;

class SymboleValue : public Symbole,  // Un symbole valué est un symbole qui a une valeur (définie ou pas)
                     public Noeud  {  //  et c'est aussi une feuille de l'arbre abstrait
public:
	  SymboleValue(const Symbole & s); // Construit un symbole valué à partir d'un symbole existant s
	  ~SymboleValue( ) {}
          void accepter(Visiteur& visiteur) override;
	  void setValeur(int valeur); 
          inline void     setValeur(Valeur* v)     { m_valeur = v; m_defini = true; }
          inline Valeur*  getValeur() const        { return this->m_valeur;         } // accesseur
	  inline bool     estDefini()              { return m_defini;               } // accesseur

	  friend ostream & operator << (ostream & cout, const SymboleValue & symbole); // affiche un symbole value sur cout

private:
	  bool     m_defini;	// indique si la valeur du symbole est définie
	  Valeur*  m_valeur;	// valeur du symbole si elle est définie, zéro sinon

};

#endif /* SYMBOLEVALUE_H */
