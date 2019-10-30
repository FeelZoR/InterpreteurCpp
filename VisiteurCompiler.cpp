#include "VisiteurCompiler.h"

VisiteurCompiler::VisiteurCompiler(std::ostream& out, int indentation)
: m_out(out), m_indentation(indentation) {}

void VisiteurCompiler::visiterNoeudSeqInst(NoeudSeqInst* noeud) {
    for (unsigned int i = 0; i < noeud->getInstructions().size(); i++) {
        m_out << std::string(m_indentation * 4, ' ');
        noeud->getInstructions()[i]->accepter(*this);
        m_out << endl;
    }
}

void VisiteurCompiler::visiterNoeudAffectation(NoeudAffectation* noeud) {
    bool instructionSeule = true;
    if (m_indentation < 0) {
        instructionSeule = false;
    }

    int indent = m_indentation;
    m_indentation = -1;
    noeud->getVariable()->accepter(*this);
    m_out << "=";
    noeud->getExpression()->accepter(*this);

    m_indentation = indent;
    if (instructionSeule) m_out << ";";
}

void VisiteurCompiler::visiterNoeudOperateurBinaire(NoeudOperateurBinaire* noeud) {
    std::string op = noeud->getOperateur().getChaine();
    int indent = m_indentation;
    m_indentation = -1;
    
    m_out << "(";
    if (op == "non") {
        m_out << "!";
        noeud->getOperandeGauche()->accepter(*this);
    } else {
        if (op == "et") op = "&&";
        else if (op == "ou") op = "||";

        noeud->getOperandeGauche()->accepter(*this);
        m_out << op;
        noeud->getOperandeDroit()->accepter(*this);
    }
    m_out << ")";
    
    m_indentation = indent;
}

void VisiteurCompiler::visiterNoeudInstSi(NoeudInstSi* noeud) {
    std::string indent(m_indentation * 4, ' ');
    if (noeud->getCondition() != nullptr) {
        if (!noeud->isPremiereCondition()) { m_out << ' '; }
        m_out << "if (";
        int indentOrigine = m_indentation;
        m_indentation = -1;
        noeud->getCondition()->accepter(*this);
        m_indentation = indentOrigine;
        m_out << ")";
    }

    m_out << " {" << endl;
    m_indentation++;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
    m_out << indent << "}";

    if (noeud->getProchaineCondition() != nullptr) {
        m_out << " else";
        noeud->getProchaineCondition()->accepter(*this);
    } else {
        m_out << endl;
    }
}

void VisiteurCompiler::visiterNoeudInstRepeter(NoeudInstRepeter* noeud) {
    std::string indent(m_indentation * 4, ' ');
    m_out << "do {" << endl;
    m_indentation++;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
    m_out << indent << "} while (!(";
    int indentOrigine = m_indentation;
    m_indentation = -1;
    noeud->getCondition()->accepter(*this);
    m_indentation = indentOrigine;
    m_out << "));" << endl;
}

void VisiteurCompiler::visiterNoeudInstPour(NoeudInstPour* noeud) {
    std::string indent(4 * m_indentation, ' ');
    int indentOrigine = m_indentation;
    m_indentation = -1;
    m_out << "for (";
    if (noeud->getInit() != nullptr) noeud->getInit()->accepter(*this);
    m_out << ";";
    noeud->getCondition()->accepter(*this);
    m_out << ";";
    if (noeud->getAffectation() != nullptr) noeud->getAffectation()->accepter(*this);
    m_out << ") {" << endl;
    m_indentation = indentOrigine + 1;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
    m_out << indent << "}" << endl;
}

void VisiteurCompiler::visiterNoeudInstTantQue(NoeudInstTantQue* noeud) {
    std::string indent(m_indentation * 4, ' ');
    m_out << "while " ;
    noeud->getCondition()->accepter(*this);
    m_out << " {" << endl;
    m_indentation++;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
    m_out << indent << "}" << endl;
}

void VisiteurCompiler::visiterNoeudInstLire(NoeudInstLire* noeud) {
    m_out << "std::cin";
    for (Noeud* var : noeud->getVariables()) {
        m_out << " >> ";
        int indent = m_indentation;
        m_indentation = -1;
        var->accepter(*this);
        m_indentation = indent;
    }

    m_out << ";";
}

void VisiteurCompiler::visiterNoeudInstEcrire(NoeudInstEcrire* noeud) {
    m_out << "std::cout";
    for (Noeud* inst : noeud->getEcritures()) {
        m_out << " << ";
        if ((typeid (*inst) == typeid (SymboleValue) && *((SymboleValue*) inst) == "<CHAINE>")) {
            m_out << ((SymboleValue*) inst)->getChaine();
        } else {
            int indent = m_indentation;
            m_indentation = -1;
            inst->accepter(*this);
            m_indentation = indent;
        }
    }
    m_out << ";";
}

void VisiteurCompiler::visiterSymboleValue(SymboleValue* symbole) {
    m_out << symbole->getChaine();
}