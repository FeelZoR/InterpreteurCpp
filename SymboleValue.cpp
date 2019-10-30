#include "SymboleValue.h"
#include "Exceptions.h"
#include "Visiteur.h"
#include <stdlib.h>

SymboleValue::SymboleValue(const Symbole & s) :
Symbole(s.getChaine()) {
  if (s == "<ENTIER>") {
    m_valeur = atoi(s.getChaine().c_str()); // c_str convertit une string en char*
    m_defini = true;
  } else if (s == "<CHAINE>") {
      m_valeur = -1;
      m_defini = true;
  } else {
    m_defini = false;
  }
}

void SymboleValue::compiler(ostream & out, int indentation) {
    out << getChaine();
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
