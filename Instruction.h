/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   instruction.h
 * Author: nils
 *
 * Created on 24 octobre 2019, 08:31
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Symbole.h"


class Instruction {
public:
    Instruction();
    bool isInstruction(Symbole sym);
private:
    string m_instructions[8] = {"<VARIABLE>","ecrire","si","pour","tantque","repeter","lire","appel"};

};

#endif /* INSTRUCTION_H */

