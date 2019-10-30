#include <iostream>
#include <fstream>
using namespace std;
#include "Interpreteur.h"
#include "Exceptions.h"
#include "VisiteurExecuter.h"

void compiler(ostream& out, const TableSymboles& symboles, Noeud* arbre);

int main(int argc, char* argv[]) {
    bool compile = false;
    string nomFich;
    ofstream o;
    ostream* out;
    if (argc < 3) {
        cout << "Usage : " << argv[0] << "<interpreter|compiler> <nom_fichier_source> [fichierDestination]" << endl << endl;
        cout << "Entrez le nom du fichier que voulez-vous interpréter : ";
        getline(cin, nomFich);
    } else {
        compile = strcmp(argv[1], "compiler") == 0;
        nomFich = argv[2];
        if (argc == 4) {
            o.open(argv[3]);
            if (o) {
                out = &o;
            } else {
                out = &cout;
            }
        } else {
            out = &cout;
        }
    }

    ifstream fichier(nomFich.c_str());
    try {
        if (fichier.fail()) throw FichierException();
        Interpreteur interpreteur(fichier);
        interpreteur.analyse();
        // Si pas d'exception levée, l'analyse syntaxique a réussi
        if(interpreteur.getArbre() != nullptr){
            cout << endl << "================ Syntaxe Correcte" << endl;
        } else {
            cout << endl << "================ Syntaxe Incorrecte" << endl;
        }
        if (compile) {
            compiler(*out, interpreteur.getTable(), interpreteur.getArbre());
        } else {
            // On affiche le contenu de la table des symboles avant d'exécuter le programme
            cout << endl << "================ Table des symboles avant exécution : " << interpreteur.getTable();
            cout << endl << "================ Execution de l'arbre" << endl;
            // On exécute le programme si l'arbre n'est pas vide
            if (interpreteur.getArbre() != nullptr) {
                VisiteurExecuter visiteur;
                interpreteur.getArbre()->accepter(visiteur);
            }
            // Et on vérifie qu'il a fonctionné en regardant comment il a modifié la table des symboles
            cout << endl << "================ Table des symboles apres exécution : " << interpreteur.getTable();
        }
    } catch (InterpreteurException & e) {
        cout << e.what() << endl;
    }

    if (o) {
        o.close();
    }

    return 0;
}

void compiler(ostream& out, const TableSymboles& symboles, Noeud* arbre) {
    std::string indentationBasique(4, ' ');
    out << "#include <iostream>" << endl << endl;
    out << "int main() {" << endl;
    for (int i = 0; i < symboles.getTaille(); i++) {
        const SymboleValue s = symboles[i];
        if (s == "<VARIABLE>") {
            out << indentationBasique << "int " << s.getChaine() << ";" << endl;
        }
    }
    arbre->compiler(out, 1);
    out << "}" << endl;
}