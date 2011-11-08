#ifndef UM_INCLUDED
#define UM_INCLUDED
#include <stdlib.h>
#include <stdint.h>
#include "registers.h"
#include "memseg.h"
#include <stdio.h>


enum opcode {
    MOVE,
    SEGLOAD,
    SEGSTORE,
    ADD,
    MULTIPLY,
    DIVIDE,
    NAND,
    HALT,
    MAP,
    UNMAP,
    OUTPUT,
    INPUT,
    LOADPROG,
    LOADVAL
};

typedef struct{
    enum opcode op;
    UM_Word value, reg1, reg2, reg3;
}Instruction; 

int mapProgram(FILE* program);

void build_and_execute_um(FILE* program);

void execute_instruction(Instruction);
// Reads in program from file in 32bit word chunks and returns program counter

#endif
