#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

class Visiteur;

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual void accepter(Visiteur& visiteur) = 0;
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();         // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {}       // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;
    void ajoute(Noeud* instruction) override;  // Ajoute une instruction à la séquence
    inline vector<Noeud *> getInstructions() const { return m_instructions; }

  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {}   // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    inline Noeud* getVariable() const { return m_variable; }
    inline Noeud* getExpression() const { return m_expression; }

  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    inline Symbole getOperateur() const { return m_operateur; }
    inline Noeud*  getOperandeGauche() const { return m_operandeGauche; }
    inline Noeud*  getOperandeDroit() const { return m_operandeDroit; }
  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSi(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSi() {}         // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    void ajoute(Noeud* condition) override;
    void setIsPremiereCondition(bool value);

    inline Noeud* getCondition() const { return m_condition; }
    inline Noeud* getSequence() const { return m_sequence; }
    inline Noeud* getProchaineCondition() const { return m_prochaineCondition; }
    inline bool   isPremiereCondition() const { return m_isPremiereCondition; }

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
    Noeud*  m_prochaineCondition;
    bool    m_isPremiereCondition;
};
////////////////////////////////////////////////////////////////////////////////
class NoeudInstRepeter : public Noeud {
        // Boucle do while
        // et ses 2 fils : le jeu d'instrcution et la condition 
public :
    NoeudInstRepeter(Noeud* instruction, Noeud* condition);

    ~NoeudInstRepeter() {}
    void accepter(Visiteur& visiteur) override;

    inline Noeud* getCondition() const { return m_condition; }
    inline Noeud* getSequence() const { return m_sequence; }
private:
    Noeud* m_sequence;
    Noeud* m_condition;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstPour : public Noeud {
    // Classe pour représenter un noeud "instruction pour"
    //  et ses 2 fils : la condition du pour et la séquence d'instruction associée
public:
    NoeudInstPour(Noeud* init, Noeud* condition, Noeud* affectation, Noeud* sequence);
    // Construit une "instruction si" avec sa condition et sa séquence d'instruction

    ~NoeudInstPour() {
    } // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    inline Noeud* getInit() const { return m_init; }
    inline Noeud* getCondition() const { return m_condition; }
    inline Noeud* getAffectation() const { return m_affectation; }
    inline Noeud* getSequence() const { return m_sequence; }

private:
    Noeud* m_init;
    Noeud* m_condition;
    Noeud* m_affectation;
    Noeud* m_sequence;
};

//////////////////////////////////////////////////////////////////////////////// 
class NoeudInstTantQue : public Noeud {
    // Classe pour représenter un noeud "instruction tant que"
    //  et ses 2 fils : la condition du tant que et la séquence d'instruction associée
public:
    NoeudInstTantQue(Noeud* condition, Noeud* sequence);
    // Construit une "instruction tant que" avec sa condition et sa séquence d'instruction

    ~NoeudInstTantQue() {
    } // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    inline Noeud* getCondition() const { return m_condition; }
    inline Noeud* getSequence() const { return m_sequence; }

private:
    Noeud* m_condition;
    Noeud* m_sequence;
};


class NoeudInstLire : public Noeud {
    // Classe pour représanter un noeud "Instruction Lire"
public:
    NoeudInstLire();
    //construit une instruction lire avec ces variables
    ~NoeudInstLire(){
        
    }
    
    void accepter(Visiteur& visiteur) override;
    void ajoute(Noeud* var) override;

    inline vector<Noeud*> getVariables() const { return m_variables; }

private:
    vector<Noeud*> m_variables;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstEcrire : public Noeud {
    // Classe pour représenter un noeud "instruction ecrire"
    //  et ses 2 fils : la condition du ecrire et la séquence d'instruction associée
public:
    NoeudInstEcrire();
    // Construit une "instruction ecrire" avec sa condition et sa séquence d'instruction

    ~NoeudInstEcrire() {
    } // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;
    virtual void ajoute(Noeud* instruction) override;

    inline vector<Noeud*> getEcritures() const { return m_ecritures; }

private:
    vector<Noeud*> m_ecritures;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstAppel : public Noeud {
    // Classe pour représenter un noeud "instruction appel"
public:
    NoeudInstAppel(Symbole nom, vector<Noeud*> parametres);
    // Construit une "instruction appel" avec sa condition et sa séquence d'instruction

    ~NoeudInstAppel() {
    } // A cause du destructeur virtuel de la classe Noeud
    void accepter(Visiteur& visiteur) override;

    inline Symbole        getNom() const { return m_nom; }
    inline vector<Noeud*> getParametres() const { return m_parametres; }

private:
    Symbole        m_nom;
    vector<Noeud*> m_parametres;
};

#endif /* ARBREABSTRAIT_H */
