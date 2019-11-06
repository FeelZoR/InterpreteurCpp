/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TestInstPour.cpp
 * Author: feelzor
 *
 * Created on Oct 28, 2019, 3:51:08 PM
 */

#include "TestInstPour.h"
#include "../Interpreteur.h"
#include "../TableSymboles.h"
#include "../Exceptions.h"
#include "../VisiteurExecuter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestInstPour);

TestInstPour::TestInstPour() {
}

TestInstPour::~TestInstPour() {
}

void TestInstPour::setUp() {
}

void TestInstPour::tearDown() {
}

void TestInstPour::testPour() {
    std::ifstream fichier("test04-Pour.txt");
    if (!fichier) {
        CPPUNIT_ASSERT_MESSAGE("Fichier impossible à ouvrir", false);
    }
    
    Interpreteur interpreteur(fichier);
    interpreteur.analyse();
    VisiteurExecuter visiteur;
    interpreteur.getArbre()->accepter(visiteur);
    
    const TableSymboles& table = interpreteur.getTable();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test valeur i", 11, table[3].getValeur()->getEntier());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test valeur j", 385, table[4].getValeur()->getEntier());
}

void TestInstPour::testPourErreur() {
    std::ifstream fichier("test07-PourErreurSyntaxe.txt");
    if (!fichier) {
        CPPUNIT_ASSERT_MESSAGE("Fichier impossible à ouvrir", false);
    }
    
    Interpreteur interpreteur(fichier);
    interpreteur.analyse();
    Noeud* vide = nullptr;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Test arbre vide", vide, interpreteur.getArbre());
}