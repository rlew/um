#include "um.h"
#include "mem.h"
#include "bitpack.h"

static UM_Word registers[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static Mem* memorySegments;
static UArray_T programCounter;
static int INITIAL_SET_SIZE = 500;
static int PROGRAM_HINT = 500;

/*
 * Returns a Instruction with a opcode and filled registers.
 */
static Instruction parseInstruction(UM_Word instructionCode){
    Instruction instr = { HALT, 0, 0, 0, 0 };
    instr.op = Bitpack_getu(instructionCode, 4, 28);
    if(instr.op == LOADVAL){
        instr.reg1 = Bitpack_getu(instructionCode, 3, 25);
        instr.value = Bitpack_getu(instructionCode, 25, 0);
    }
    else{
        //instr.value = Bitpack_getu(instructionCode, 19, 9);
        instr.reg1 = Bitpack_getu(instructionCode, 3, 6);
        instr.reg2 = Bitpack_getu(instructionCode, 3, 3);
        instr.reg3 = Bitpack_getu(instructionCode, 3, 0);
    }
    return instr;
}

/*
 * Creates memeory for the program to run and gets the instruction code along
 * with the registers.
 */
void build_and_execute_um(FILE* program){
    NEW(memorySegments);
    instantiateMem(memorySegments, INITIAL_SET_SIZE);

    int numInstr = mapProgram(program);

    for(int i = 0; i < numInstr; i++){
        UM_Word instruction = *(UM_Word*)UArray_at(programCounter, i);
        Instruction instr = parseInstruction(instruction);
        execute_instruction(instr);
        if(instr.op == HALT) break;
    }
}

/*
 * Initializes the program counter and returns the number of instructions.
 */
int mapProgram(FILE* program) {
    Seq_T words = Seq_new(PROGRAM_HINT);
    int c = getc(program);
    while(c != EOF) {
        UM_Word temp = Bitpack_newu(temp, 8, 24, c);
        for(int bit = 16; bit >=0; bit -=8){
            int b = getc(program);
            temp = Bitpack_newu(temp, 8, bit, b);
        }

        UM_Word* instr;
        NEW(instr);
        *instr = temp;
        Seq_addhi(words, instr);
        c = getc(program);
    }

    programCounter = mapSegment(memorySegments, 0, Seq_length(words));

    for(UM_Word locToLoad = 0; locToLoad < (UM_Word)Seq_length(words); locToLoad++){
        UM_Word value = *(UM_Word*)Seq_get(words, locToLoad);
        segmentedStore(memorySegments, 0, locToLoad, value);
    }

    return Seq_length(words);
}

/*
 * Executes the Instruction based on the opcode.
 */
void execute_instruction(Instruction instr){
    switch(instr.op) {
        case MOVE:{
            conditionalMove(registers, instr.reg1, instr.reg2, instr.reg3);
            break;}
        case SEGLOAD:{
            UM_Word ID = registers[instr.reg2];
            UM_Word offset = registers[instr.reg3];
            UM_Word toStore = segmentedLoad(memorySegments, ID, offset);
            registers[instr.reg1] = toStore;
            break;
        }
        case SEGSTORE:{
            UM_Word ID = registers[instr.reg1];
            UM_Word offset = registers[instr.reg2];
            UM_Word value = registers[instr.reg3];
            segmentedStore(memorySegments, ID, offset, value);
            break;
        }
        case ADD:
            addition(registers, instr.reg1, instr.reg2, instr.reg3);
            break;
        case MULTIPLY:
            multiplication(registers, instr.reg1, instr.reg2, instr.reg3);
            break;
        case DIVIDE:
            division(registers, instr.reg1, instr.reg2, instr.reg3);
            break;
        case NAND:
            bitwiseNAND(registers, instr.reg1, instr.reg2, instr.reg3);
            break;
        case HALT: {
            halt(registers);
            freeMem(memorySegments);
            programCounter = NULL;
            break;
        }
        case MAP:{
            UM_Word length = registers[instr.reg3];
            UM_Word ID = registers[instr.reg2];
            mapSegment(memorySegments, ID, length);
            break;
        }
        case UNMAP:{
            UM_Word ID = registers[instr.reg3];
            unmapSegment(memorySegments, ID);
            break;
        }
        case OUTPUT:
            output(registers, instr.reg1);
            break;
        case INPUT:
            input(registers, instr.reg1);
            break;
        case LOADPROG:{
            unmapSegment(memorySegments, 0);
            UM_Word ID = registers[instr.reg2];
            UArray_T copy = segmentCopy(memorySegments, ID);
            UArray_T toSet = mapSegment(memorySegments, 0, UArray_length(copy));
            FREE(toSet);
            toSet = copy;
            break;
        }
        case LOADVAL:
            loadValue(registers, instr.reg1, instr.value);
            break;
    }
}
