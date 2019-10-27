#include "Instruction.h"

Instruction::Instruction() {
}

bool Instruction::isInstruction(Symbole sym) {
    int i = 0;
    while (i < 7 && !(sym == m_instructions[i])) {
        i++;
    }
return sym == m_instructions[i];
}


