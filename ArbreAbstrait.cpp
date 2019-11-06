#include <stdlib.h>
#include <typeinfo>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include "Visiteur.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

void NoeudSeqInst::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudSeqInst(this);
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

void NoeudAffectation::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudAffectation(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

void NoeudOperateurBinaire::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudOperateurBinaire(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence), m_prochaineCondition(nullptr), m_isPremiereCondition(true) {
}

void NoeudInstSi::ajoute(Noeud* condition) {
    ((NoeudInstSi*) condition)->setIsPremiereCondition(false);
    if (m_prochaineCondition == nullptr) {
        m_prochaineCondition = condition;
    } else {
        m_prochaineCondition->ajoute(condition);
    }
}

void NoeudInstSi::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstSi(this);
}

void NoeudInstSi::setIsPremiereCondition(bool value) {
    m_isPremiereCondition = value;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* instruction, Noeud* condition) : m_sequence(instruction), m_condition(condition) {

}

void NoeudInstRepeter::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstRepeter(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstPour
////////////////////////////////////////////////////////////////////////////////

NoeudInstPour::NoeudInstPour(Noeud* init, Noeud* condition, Noeud* affect, Noeud* sequence)
: m_init(init), m_condition(condition), m_affectation(affect), m_sequence(sequence) {
}

void NoeudInstPour::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstPour(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstTantQue
////////////////////////////////////////////////////////////////////////////////

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

void NoeudInstTantQue::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstTantQue(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstLire
////////////////////////////////////////////////////////////////////////////////
NoeudInstLire::NoeudInstLire() { 
    m_variables = vector<Noeud*>();
}

void NoeudInstLire::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstLire(this);
}

void NoeudInstLire::ajoute(Noeud* var){
    m_variables.push_back(var);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudInstEcrire::NoeudInstEcrire() {
}

void NoeudInstEcrire::ajoute(Noeud* instruction) {
    m_ecritures.push_back(instruction);
}

void NoeudInstEcrire::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstEcrire(this);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstAppel
////////////////////////////////////////////////////////////////////////////////

NoeudInstAppel::NoeudInstAppel(Symbole nom, vector<Noeud*> parametres)
: m_nom(nom), m_parametres(parametres) {
}

void NoeudInstAppel::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstAppel(this);
}
