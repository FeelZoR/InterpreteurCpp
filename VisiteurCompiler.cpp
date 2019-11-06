#include "VisiteurCompiler.h"
#include "Procedure.h"

VisiteurCompiler::VisiteurCompiler(std::ostream& out, int indentation)
: m_out(out), m_indentation(indentation) {}

void VisiteurCompiler::visiterProcedure(Procedure* procedure) {
    m_out << "def " << procedure->getNom() << "(";
    bool first = true;
    for (SymboleValue* param : procedure->getParametres()) {
        if (!first) { m_out << ", "; first = false; }
        param->accepter(*this);
    }
    m_out << "):" << endl;
    m_indentation++;
    procedure->getSequence()->accepter(*this);
    m_indentation--;
}

void VisiteurCompiler::visiterNoeudSeqInst(NoeudSeqInst* noeud) {
    for (unsigned int i = 0; i < noeud->getInstructions().size(); i++) {
        m_out << std::string(m_indentation * 4, ' ');
        noeud->getInstructions()[i]->accepter(*this);
        m_out << endl;
    }
}

void VisiteurCompiler::visiterNoeudAffectation(NoeudAffectation* noeud) {
    int indent = m_indentation;
    m_indentation = -1;
    noeud->getVariable()->accepter(*this);
    m_out << "=";
    noeud->getExpression()->accepter(*this);

    m_indentation = indent;
}

void VisiteurCompiler::visiterNoeudOperateurBinaire(NoeudOperateurBinaire* noeud) {
    std::string op = noeud->getOperateur().getChaine();
    int indent = m_indentation;
    m_indentation = -1;
    
    m_out << "(";
    if (op == "non") {
        m_out << "not ";
        noeud->getOperandeGauche()->accepter(*this);
    } else {
        if (op == "et") op = " and ";
        else if (op == "ou") op = " or ";

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
        m_out << "if ";
        int indentOrigine = m_indentation;
        m_indentation = -1;
        noeud->getCondition()->accepter(*this);
        m_indentation = indentOrigine;
    } else {
        m_out << "se";
    }

    m_out << ":" << endl;
    m_indentation++;
    noeud->getSequence()->accepter(*this);
    m_indentation--;

    if (noeud->getProchaineCondition() != nullptr) {
        m_out << "el";
        noeud->getProchaineCondition()->accepter(*this);
    } else {
        m_out << endl;
    }
}

void VisiteurCompiler::visiterNoeudInstRepeter(NoeudInstRepeter* noeud) {
    std::string indent(m_indentation * 4, ' ');
    m_out << indent << "transpilation_dowhile_boolean = 0" << endl;
    m_out << "while not (";
    int indentOrigine = m_indentation;
    m_indentation = -1;
    noeud->getCondition()->accepter(*this);
    m_indentation = indentOrigine;
    m_out << ") or transpilation_dowhile_boolean == 0:" << endl;
    m_indentation++;
    m_out << indent << "    transpilation_dowhile_boolean+=1" << endl;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
}

void VisiteurCompiler::visiterNoeudInstPour(NoeudInstPour* noeud) {
    std::string indent(4 * m_indentation, ' ');
    int indentOrigine = m_indentation;
    
    if (noeud->getInit() != nullptr) noeud->getInit()->accepter(*this);
    m_out << endl;
    m_indentation = -1;
    m_out << "while";
    noeud->getCondition()->accepter(*this);
    m_out << ":" << endl;
    m_indentation = indentOrigine + 1;
    noeud->getSequence()->accepter(*this);
    m_out << indent << "    ";
    if (noeud->getAffectation() != nullptr) noeud->getAffectation()->accepter(*this);
    m_indentation--;
}

void VisiteurCompiler::visiterNoeudInstTantQue(NoeudInstTantQue* noeud) {
    std::string indent(m_indentation * 4, ' ');
    m_out << "while " ;
    noeud->getCondition()->accepter(*this);
    m_out << ":" << endl;
    m_indentation++;
    noeud->getSequence()->accepter(*this);
    m_indentation--;
}

void VisiteurCompiler::visiterNoeudInstLire(NoeudInstLire* noeud) {
    std::string indent(m_indentation * 4, ' ');
    for (Noeud* var : noeud->getVariables()) {
        m_out << indent;
        int currIndent = m_indentation;
        m_indentation = -1;
        var->accepter(*this);
        m_indentation = currIndent;
        m_out << " = input()" << endl;
    }
}

void VisiteurCompiler::visiterNoeudInstEcrire(NoeudInstEcrire* noeud) {
    std::string indent(m_indentation * 4, ' ');
    bool first = true;
    for (Noeud* inst : noeud->getEcritures()) {
        if (first) m_out << "print(";
        else m_out << indent << "print(";
        
        int currIndent = m_indentation;
        m_indentation = -1;
        inst->accepter(*this);
        m_indentation = currIndent;
        m_out << ", end = '')" << endl;
        first = false;
    }
    m_out << indent << "print('')" << endl;
}

void VisiteurCompiler::visiterNoeudInstAppel(NoeudInstAppel* noeud) {
    m_out << noeud->getNom().getChaine() << "(";
    for(int i = 0; i < noeud->getParametres().size(); i++) {
        if (i != 0) { m_out << ", "; }
        noeud->getParametres()[i]->accepter(*this);
    }
    m_out << ")" << endl;
}

void VisiteurCompiler::visiterNoeudAlea(NoeudAlea* noeud) {
    m_out << "random.randint(";
    noeud->getMin()->accepter(*this);
    m_out << ",";
    noeud->getMax()->accepter(*this);
    m_out << ")" << endl;
}

void VisiteurCompiler::visiterSymboleValue(SymboleValue* symbole) {
    m_out << symbole->getChaine();
}