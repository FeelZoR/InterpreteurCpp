#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "Symbole.h"
#include "Lecteur.h"
#include "Exceptions.h"
#include "TableSymboles.h"
#include "ArbreAbstrait.h"

class Interpreteur {
public:
	Interpreteur(ifstream & fichier);   // Construit un interpréteur pour interpreter
	                                    //  le programme dans  fichier 
                                      
	void analyse();                     // Si le contenu du fichier est conforme à la grammaire,
	                                    //   cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                                            //   la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
	                                    // Sinon, une exception sera levée

	inline const TableSymboles & getTable () const  { return m_table;    } // accesseur	
	inline Noeud* getArbre () const { return m_arbre; }                    // accesseur
	
private:
    Lecteur        m_lecteur;  // Le lecteur de symboles utilisé pour analyser le fichier
    TableSymboles  m_table;    // La table des symboles valués
    Noeud*         m_arbre;    // L'arbre abstrait
    bool           m_erreur;   // Etat de l'interpreteur

    // Implémentation de la grammaire
    Noeud*  programme();   //   <programme> ::= {procedure <procedure>} procedure principale() <seqInst> finproc FIN_FICHIER
    void    procedure();   //   <procedure> ::= <variable>([<variable> {, <variable> }]) <seqInst> finproc

    Noeud*  seqInst();	   //     <seqInst> ::= <inst> { <inst> }
    Noeud*  inst();	   //        <inst> ::= <affectation> ; | <instSi>
    Noeud*  affectation(); // <affectation> ::= <variable> = <expression> 
    Noeud*  expression();  //  <expression> ::= <expEt> { ou <expEt> }
    Noeud*  expEt();       //       <expEt> ::= <expComp> { et <expComp> }
    Noeud*  expComp();     //     <expComp> ::= <expAdd> { == | != | < | <= | > | >= <expAdd> }
    Noeud*  expAdd();      //      <expAdd> ::= <expMult> { + | - <expMult> }
    Noeud*  expMult();     //     <expMult> ::= <facteur> { * | / <facteur> }
    Noeud*  facteur();     //     <facteur> ::= <entier>  |  <reel>  |  <chaine>  |  <variable>  |  - <facteur>  | non <facteur> | ( <expression> ) | <alea>
                           //   <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
    Noeud*  instSi();      //      <instSi> ::= si ( <expression> ) <seqInst> { sinonsi (<expression>) <seqInst> } [ sinon <seqInst> ] finsi
    Noeud*  instPour();    //    <insPour> ::= pour ( [ <affectation> ] ; <expression> ; [ <affection> ]) <seqInst> finpour
    Noeud*  instTantQue(); // <instTantQue> ::= tantque ( <expression> ) <seqInst> fintantque
    Noeud*  instLire();    //   <instLire>  ::= lire (<variable> {, <variable>});
    Noeud*  instEcrire();  //  <instEcrire> ::= ecrire ( <expression> | <chaine> {, <expression> | <chaine> } );
    Noeud*  instRepeter(); // <instRepeter> ::= repeter <seqInst> jusqua ( <expression> )
    Noeud*  instAppel();   //       <appel> ::= appel <variable>([<expression> {, <expression>}]);
    Noeud*  alea();        //        <alea> ::= alea(<expression>, <expression>)

    // outils pour simplifier l'analyse syntaxique
    void tester (const string & symboleAttendu) const;   // Si symbole courant != symboleAttendu, on lève une exception
    void testerEtAvancer(const string & symboleAttendu); // Si symbole courant != symboleAttendu, on lève une exception, sinon on avance
    void erreur (const string & mess) const;             // Lève une exception "contenant" le message mess
};

#endif /* INTERPRETEUR_H */
