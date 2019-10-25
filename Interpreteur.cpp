#include "Interpreteur.h"
#include "Instruction.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

Interpreteur::Interpreteur(ifstream & fichier) :
m_lecteur(fichier), m_table(), m_arbre(nullptr),m_erreur(false) {
}

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
  tester(symboleAttendu);
  m_lecteur.avancer();
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
  // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
  testerEtAvancer("procedure");
  testerEtAvancer("principale");
  testerEtAvancer("(");
  testerEtAvancer(")");
  Noeud* sequence = seqInst();
  testerEtAvancer("finproc");
  tester("<FINDEFICHIER>");
  return sequence;
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
            while (!instructions->isInstruction(m_lecteur.getSymbole()) && m_lecteur.getSymbole() != "finproc" && m_lecteur.getSymbole() != "EOF") {
                m_erreur = true;
                m_lecteur.avancer();
            }
        }
    } while (instructions->isInstruction(m_lecteur.getSymbole()) && m_lecteur.getSymbole() != "finproc" && m_lecteur.getSymbole() != "EOF");
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
  // <expression> ::= <facteur> { <opBinaire> <facteur> }
  //  <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
  Noeud* fact = facteur();
  while ( m_lecteur.getSymbole() == "+"  || m_lecteur.getSymbole() == "-"  ||
          m_lecteur.getSymbole() == "*"  || m_lecteur.getSymbole() == "/"  ||
          m_lecteur.getSymbole() == "<"  || m_lecteur.getSymbole() == "<=" ||
          m_lecteur.getSymbole() == ">"  || m_lecteur.getSymbole() == ">=" ||
          m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!=" ||
          m_lecteur.getSymbole() == "et" || m_lecteur.getSymbole() == "ou"   ) {
    Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
    m_lecteur.avancer();
    Noeud* factDroit = facteur(); // On mémorise l'opérande droit
    fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // Et on construuit un noeud opérateur binaire
  }
  return fact; // On renvoie fact qui pointe sur la racine de l'expression
}

Noeud* Interpreteur::facteur() {
  // <facteur> ::= <entier> | <variable> | - <facteur> | non <facteur> | ( <expression> )
  Noeud* fact = nullptr;
  if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>") {
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