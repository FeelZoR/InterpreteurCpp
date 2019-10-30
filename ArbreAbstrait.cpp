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

void NoeudSeqInst::compiler(ostream & out, int indentation) {
    for (unsigned int i = 0; i < m_instructions.size(); i++) {
        cout << std::string(indentation * 4, ' ');
        m_instructions[i]->compiler(out, indentation);
        cout << endl;
    }
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

void NoeudAffectation::compiler(ostream & out, int indentation) {
    bool instructionSeule = true;
    if (indentation < 0) {
        indentation = 0;
        instructionSeule = false;
    }

    m_variable->compiler(out, -1);
    out << "=";
    m_expression->compiler(out, -1);
    if (instructionSeule) out << ";";
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

void NoeudOperateurBinaire::compiler(ostream & out, int indentation) {
    std::string op = m_operateur.getChaine();

    out << "(";
    if (op == "non") {
        out << "!";
        m_operandeGauche->compiler(out, -1);
    } else {
        if (op == "et") op = "&&";
        else if (op == "ou") op = "||";

        m_operandeGauche->compiler(out, -1);
        out << op;
        m_operandeDroit->compiler(out, -1);
    }
    out << ")";
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

void NoeudInstSi::compiler(ostream & out, int indentation) {
    std::string indent(indentation * 4, ' ');
    if (m_condition != nullptr) {
        if (!m_isPremiereCondition) { out << ' '; }
        out << "if (";
        m_condition->compiler(out, -1);
        out << ")";
    }

    out << " {" << endl;
    m_sequence->compiler(out, indentation + 1);
    out << indent << "}";

    if (m_prochaineCondition != nullptr) {
        out << " else";
        m_prochaineCondition->compiler(out, indentation);
    } else {
        out << endl;
    }
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

void NoeudInstRepeter::compiler(ostream & out, int indentation) {
    std::string indent(indentation * 4, ' ');
    out << "do {" << endl;
    m_sequence->compiler(out, indentation + 1);
    out << indent << "} while (!(";
    m_condition->compiler(out, -1);
    out << "));" << endl;
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

void NoeudInstPour::compiler(ostream & out, int indentation) {
    std::string indent(4 * indentation, ' ');
    out << "for (";
    if (m_init != nullptr) m_init->compiler(out, -1);
    out << ";";
    m_condition->compiler(out, -1);
    out << ";";
    if (m_affectation != nullptr) m_affectation->compiler(out, -1);
    out << ") {" << endl;
    m_sequence->compiler(out, indentation + 1);
    out << indent << "}" << endl;
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

void NoeudInstTantQue::compiler(ostream & out, int indentation) {
    std::string indent(indentation * 4, ' ');
    out << "while " ;
    m_condition->compiler(out,indentation);
    out << " {" << endl;
    m_sequence->compiler(out,indentation+1);
    out << indent << "}" << endl;
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

void NoeudInstLire::compiler(ostream & out, int indentation) {
    out << "std::cin";
    for (Noeud* var : m_variables) {
        out << " >> ";
        var->compiler(out, -1);
    }

    out << ";";
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

void NoeudInstEcrire::compiler(ostream & out, int indentation) {
    out << "std::cout";
    for (Noeud* inst : m_ecritures) {
        out << " << ";
        if ((typeid (*inst) == typeid (SymboleValue) && *((SymboleValue*) inst) == "<CHAINE>")) {
            out << ((SymboleValue*) inst)->getChaine();
        } else {
            inst->compiler(out, -1);
        }
    }
    out << ";";
}

void NoeudInstEcrire::accepter(Visiteur& visiteur) {
    visiteur.visiterNoeudInstEcrire(this);
}