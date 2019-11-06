#include "SymboleValue.h"
#include "Exceptions.h"
#include "Visiteur.h"
#include <stdlib.h>
#include "ValeurEntiere.h"
#include "ValeurReelle.h"
#include "ValeurChaine.h"

SymboleValue::SymboleValue(const Symbole & s) :
Symbole(s.getChaine()) {
  if (s == "<ENTIER>") {
    setValeur(atoi(s.getChaine().c_str())); // c_str convertit une string en char*
  } else if (s == "<CHAINE>") {
      this->m_valeur = new ValeurChaine(s.getChaine().substr(1, s.getChaine().size() - 2));
      m_defini = true;
  } else if (s == "<REEL>") {
      this->m_valeur = new ValeurReelle(stof(s.getChaine()));
      m_defini = true;
  } else {
    m_defini = false;
  }
}

void SymboleValue::accepter(Visiteur& visiteur) {
    visiteur.visiterSymboleValue(this);
}

ostream & operator<<(ostream & cout, const SymboleValue & symbole) {
  cout << (Symbole) symbole << "\t\t - Valeur=";
  if (symbole.m_defini) cout << symbole.m_valeur << " ";
  else cout << "indefinie ";
  return cout;
}


void SymboleValue::setValeur(int valeur) {
    this->m_valeur=new ValeurEntiere(valeur);
    m_defini=true;
}