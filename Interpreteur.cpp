#include "Interpreteur.h"
#include "Instruction.h"
#include "TableProcedures.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

Interpreteur::Interpreteur(ifstream & fichier)
: m_lecteur(fichier), m_table(), m_arbre(nullptr),m_erreur(false), m_nbErreur(0) {}

void Interpreteur::analyse() {
  m_arbre = programme(); // on lance l'analyse de la première règle
  if(m_erreur){
      m_arbre = nullptr; // Si on a trouvé une erreur de syntaxe on vide l'arbre
  }
}

void Interpreteur::tester(const string & symboleAttendu) const {
  // Teste si le symbole courant est égal au symboleAttendu... Si non, lève une exception
  static char messageWhat[256];
  if (m_lecteur.getSymbole() != symboleAttendu) {
    sprintf(messageWhat,
            "Ligne %d, Colonne %d - Erreur de syntaxe - Symbole attendu : %s - Symbole trouvé : %s",
            m_lecteur.getLigne(), m_lecteur.getColonne(),
            symboleAttendu.c_str(), m_lecteur.getSymbole().getChaine().c_str());
    throw SyntaxeException(messageWhat);
  }
}

void Interpreteur::testerEtAvancer(const string & symboleAttendu) {
  // Teste si le symbole courant est égal au symboleAttendu... Si oui, avance, Sinon, lève une exception
    try {
        tester(symboleAttendu);
        m_lecteur.avancer();
        m_nbErreur = 0;
    } catch (SyntaxeException& e) {
        m_erreur = true;
        m_nbErreur++;
        if (m_nbErreur >= 3) {
            throw e; // Si le grain fin échoue, on repasse au gros grain
        } else {
            cerr << e.what() << endl;
        }
    }
}

void Interpreteur::erreur(const string & message) const {
  // Lève une exception contenant le message et le symbole courant trouvé
  // Utilisé lorsqu'il y a plusieurs symboles attendus possibles...
  static char messageWhat[256];
  sprintf(messageWhat,
          "Ligne %d, Colonne %d - Erreur de syntaxe - %s - Symbole trouvé : %s",
          m_lecteur.getLigne(), m_lecteur.getColonne(), message.c_str(), m_lecteur.getSymbole().getChaine().c_str());
  throw SyntaxeException(messageWhat);
}

Noeud* Interpreteur::programme() {
    // <programme> ::= {procedure <procedure>} procedure principale() <seqInst> finproc FIN_FICHIER
    Noeud* sequence = nullptr;
    while (sequence == nullptr) {
        testerEtAvancer("procedure");
        if (m_lecteur.getSymbole() == "<VARIABLE>") {
            procedure();
        } else {
            testerEtAvancer("principale");
            testerEtAvancer("(");
            testerEtAvancer(")");
            sequence = seqInst();
            testerEtAvancer("finproc");
            tester("<FINDEFICHIER>");
        }
    }
    return sequence;
}

void Interpreteur::procedure() {
    // <procedure> ::= <variable>([<variable> {, <variable> }]) <seqInst> finproc
    TableSymboles table = m_table;
    m_table = TableSymboles();
    std::vector<SymboleValue*> parametres;
    
    tester("<VARIABLE>");
    Symbole nom = m_lecteur.getSymbole();
    m_lecteur.avancer();
    testerEtAvancer("(");
    if (m_lecteur.getSymbole() == "<VARIABLE>") {
        parametres.push_back(m_table.chercheAjoute(m_lecteur.getSymbole()));
        m_lecteur.avancer();
        while (m_lecteur.verifierPourAvancer(",")) {
            tester("<VARIABLE>");
            parametres.push_back(m_table.chercheAjoute(m_lecteur.getSymbole()));
        }
    }
    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    testerEtAvancer("finproc");
    TableProcedures::getTable()->ajoutProcedure(nom, new Procedure(m_table, sequence, parametres));
    m_table = table;
}   

Noeud* Interpreteur::seqInst() {
    // <seqInst> ::= <inst> { <inst> }
    Instruction* instructions = new Instruction();
    NoeudSeqInst* sequence = new NoeudSeqInst();

    do {
        try {
            sequence->ajoute(inst());
        } catch (SyntaxeException e) {
            cerr << e.what() << endl;
            m_erreur = true;
            while (!instructions->isInstruction(m_lecteur.getSymbole()) && m_lecteur.getSymbole() != "finproc" && m_lecteur.getSymbole() != "<FINDEFICHIER>") {
                m_lecteur.avancer();
            }
        }
    } while (instructions->isInstruction(m_lecteur.getSymbole()) && m_lecteur.getSymbole() != "finproc" && m_lecteur.getSymbole() != "<FINDEFICHIER>");
    // Tant que le symbole courant est un début possible d'instruction...
    // Il faut compléter cette condition chaque fois qu'on rajoute une nouvelle instruction
    return sequence;
}


Noeud* Interpreteur::inst() {
  // <inst> ::= <affectation>  ; | <instSi>
  if (m_lecteur.getSymbole() == "<VARIABLE>") {
    Noeud *affect = affectation();
    testerEtAvancer(";");
    return affect;
  }
  else if (m_lecteur.getSymbole() == "ecrire") {
      return instEcrire();
  }
  else if (m_lecteur.getSymbole() == "si")
    return instSi();
  else if (m_lecteur.getSymbole() == "pour")
    return instPour();
  else if (m_lecteur.getSymbole() == "tantque") {
      return instTantQue();
  }
  // Compléter les alternatives chaque fois qu'on rajoute une nouvelle instruction
  else if(m_lecteur.getSymbole() == "repeter"){
      return instRepeter();  
  } 
  else if(m_lecteur.getSymbole() == "lire"){
      return instLire();
  }
  else if(m_lecteur.getSymbole() == "appel"){
      return instAppel();
  }
  else {
      erreur("Instruction incorrecte");
      return nullptr;
  }
}

Noeud* Interpreteur::affectation() {
  // <affectation> ::= <variable> = <expression> 
  tester("<VARIABLE>");
  Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole()); // La variable est ajoutée à la table et on la mémorise
  m_lecteur.avancer();
  testerEtAvancer("=");
  Noeud* exp = expression();             // On mémorise l'expression trouvée
  return new NoeudAffectation(var, exp); // On renvoie un noeud affectation
}

Noeud* Interpreteur::expression() {
  // <expression> ::= <expEt> { ou <expEt> } 
    Noeud* exprOu = expEt();
    while ( m_lecteur.getSymbole() == "ou" ) {
      Symbole operateur = m_lecteur.getSymbole();
      m_lecteur.avancer();
      Noeud* facteurd = expEt(); 
      exprOu = new NoeudOperateurBinaire(operateur, exprOu, facteurd);
    }
    return exprOu; 
}

Noeud* Interpreteur::expEt(){
  // <expEt>    ::= <expComp> { et <expComp> }
    Noeud* exprEt = expComp();
    while ( m_lecteur.getSymbole() == "et" ) {
      Symbole operateur = m_lecteur.getSymbole();
      m_lecteur.avancer();
      Noeud* facteurd = expComp(); 
      exprEt = new NoeudOperateurBinaire(operateur, exprEt, facteurd);
    }
    return exprEt; 
}

Noeud* Interpreteur::expComp(){
  // <expComp>  ::= <expAdd> { == | != | < | <= | > | >= <expAdd> }
     Noeud* exprComp = expAdd();
  
    while ( m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!=" || m_lecteur.getSymbole() == "<" || m_lecteur.getSymbole() == "<=" ||m_lecteur.getSymbole() == ">" ||m_lecteur.getSymbole() == ">=" ) {
      Symbole operateur = m_lecteur.getSymbole();
      m_lecteur.avancer();
      Noeud* facteurd = expAdd();
      exprComp = new NoeudOperateurBinaire(operateur, exprComp, facteurd);
    }
    return exprComp;    
}

Noeud* Interpreteur::expAdd(){
  // <expAdd>   ::= <expMult> { + | - <expMult> }
    Noeud* exprAdd = expMult();
  
    while ( m_lecteur.getSymbole() == "+" || m_lecteur.getSymbole() == "-" ) {
      Symbole operateur = m_lecteur.getSymbole();
      m_lecteur.avancer();
      Noeud* facteurd = expMult();
      
      exprAdd = new NoeudOperateurBinaire(operateur, exprAdd, facteurd);
    }
    return exprAdd; 
}

Noeud* Interpreteur::expMult(){
  // <expMult>  ::= <facteur> { * | / <facteur> }
    Noeud* exprMult = facteur();
  
    while ( m_lecteur.getSymbole() == "/" || m_lecteur.getSymbole() == "*" ) {
      Symbole operateur = m_lecteur.getSymbole();
      m_lecteur.avancer();
      Noeud* facteurd = facteur();    
      exprMult = new NoeudOperateurBinaire(operateur, exprMult, facteurd);
    }
    return exprMult;
}

Noeud* Interpreteur::facteur() {
  // <facteur> ::= <entier> | <reel> | <chaine> | <variable> | - <facteur> | non <facteur> | ( <expression> ) | <alea>
  Noeud* fact = nullptr;
  if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>" || m_lecteur.getSymbole() == "<REEL>" || m_lecteur.getSymbole() == "<CHAINE>") {
    fact = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
    m_lecteur.avancer();
  } else if (m_lecteur.getSymbole() == "-") { // - <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustraction binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("-"), m_table.chercheAjoute(Symbole("0")), facteur());
  } else if (m_lecteur.getSymbole() == "non") { // non <facteur>
    m_lecteur.avancer();
    // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
    fact = new NoeudOperateurBinaire(Symbole("non"), facteur(), nullptr);
  } else if (m_lecteur.getSymbole() == "(") { // expression parenthésée
    m_lecteur.avancer();
    fact = expression();
    testerEtAvancer(")");
  } else if (m_lecteur.getSymbole() == "alea") {
    fact = alea();
  } else
    erreur("Facteur incorrect");
  return fact;
}

Noeud* Interpreteur::instSi() {
  // <instSi> ::= si ( <expression> ) <seqInst> { sinonsi (<expression>) <seqInst> } [ sinon <seqInst> ] finsi
  testerEtAvancer("si");
  testerEtAvancer("(");
  Noeud* condition = expression(); // On mémorise la condition
  testerEtAvancer(")");
  Noeud* sequence = seqInst();     // On mémorise la séquence d'instruction
  
  Noeud* result = new NoeudInstSi(condition, sequence); 

  while (m_lecteur.getSymbole() == "sinonsi") {
      m_lecteur.avancer();
      testerEtAvancer("(");
      condition = expression();
      testerEtAvancer(")");
      sequence = seqInst();
      result->ajoute(new NoeudInstSi(condition, sequence));
  }

  if (m_lecteur.getSymbole() == "sinon") {
      m_lecteur.avancer();
      sequence = seqInst();
      result->ajoute(new NoeudInstSi(nullptr, sequence));
  }

  testerEtAvancer("finsi");
  return result; // Et on renvoie un noeud Instruction Si
}

Noeud* Interpreteur::instRepeter() {
    // <instRepeter> ::=repeter <seqInst> jusqua( <expression> )
    testerEtAvancer("repeter");
    Noeud* sequence = seqInst(); // Stokage instruction
    testerEtAvancer("jusqua");
    testerEtAvancer("(");
    Noeud* condition = expression(); // stockage condtion
    testerEtAvancer(")");
   return new NoeudInstRepeter(sequence,condition);
}   

Noeud* Interpreteur::instPour() {
    // <instPour> ::= pour ( [ <affectation> ] ; <expression> ; [ <affection> ]) <seqInst> finpour
    testerEtAvancer("pour");
    testerEtAvancer("(");

    Noeud* init = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>") {
        init = affectation();
    }

    testerEtAvancer(";");
    Noeud* condition = expression();
    testerEtAvancer(";");

    Noeud* affect = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>") {
        affect = affectation();
    }

    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    testerEtAvancer("finpour");

    return new NoeudInstPour(init, condition, affect, sequence);
}

Noeud* Interpreteur::instEcrire() {
    //  <instEcrire> ::= ecrire ( <expression> | <chaine> {, <expression> | <chaine> } );
    //      <chaine> ::= " { <lettre> } "
    testerEtAvancer("ecrire");
    testerEtAvancer("(");
    Noeud* ecrire = new NoeudInstEcrire();
    do {
        if (m_lecteur.getSymbole() == "<CHAINE>") {
            ecrire->ajoute(m_table.chercheAjoute(m_lecteur.getSymbole()));
            m_lecteur.avancer();
        } else {
            ecrire->ajoute(expression());
        }
    } while (m_lecteur.verifierPourAvancer(","));
    testerEtAvancer(")");
    testerEtAvancer(";");

    return ecrire;
}

Noeud* Interpreteur::instTantQue() {
    // <instTantQue> ::= tantque ( <expression> ) <seqInst> fintantque
    testerEtAvancer("tantque");
    testerEtAvancer("(");
    Noeud* condition = expression();
    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    testerEtAvancer("fintantque");
    return new NoeudInstTantQue(condition, sequence);
}


Noeud* Interpreteur::instLire(){
    //<instLire>  ::= lire (variable {, variable});
    Noeud* var = new NoeudInstLire();
    testerEtAvancer("lire");
    testerEtAvancer("(");
    tester("<VARIABLE>");
    var->ajoute(m_table.chercheAjoute(m_lecteur.getSymbole()));
    m_lecteur.avancer();
 
    while(m_lecteur.getSymbole() == ","){
        m_lecteur.avancer();
        tester("<VARIABLE>");
        var->ajoute(m_table.chercheAjoute(m_lecteur.getSymbole()));
        m_lecteur.avancer();     
    }
    testerEtAvancer(")");
    testerEtAvancer(";");
    return var;
}

Noeud* Interpreteur::instAppel() {
    // <appel> ::= appel <variable>([<expression> {, <expression>}]);
    testerEtAvancer("appel");
    tester("<VARIABLE>");
    Symbole nom = m_lecteur.getSymbole();
    m_lecteur.avancer();
    testerEtAvancer("(");
    
    std::vector<Noeud*> parametres;
    if (m_lecteur.getSymbole() != ")") {
        parametres.push_back(expression());
        while (m_lecteur.verifierPourAvancer(",")) {
            parametres.push_back(expression());
        }
    }
    testerEtAvancer(")");
    testerEtAvancer(";");
    
    return new NoeudInstAppel(nom, parametres);
} 

Noeud* Interpreteur::alea() {
    // <alea> ::= alea(<expression>, <expression>)
    testerEtAvancer("alea");
    testerEtAvancer("(");
    Noeud* min = expression();
    testerEtAvancer(",");
    Noeud* max = expression();
    testerEtAvancer(")");
    
    return new NoeudAlea(min, max);
}
