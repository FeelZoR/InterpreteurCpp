/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   instruction.cpp
 * Author: nils
 * 
 * Created on 24 octobre 2019, 08:31
 */

#include "Instruction.h"
#include <algorithm>

Instruction::Instruction() {
}

bool Instruction::isInstruction(Symbole sym) {
    int i = 0;
    while (i < 7 && !(sym == m_instructions[i])) {
        sym == m_instructions[i];
        i++;
    }

return sym == m_instructions[i];
}


