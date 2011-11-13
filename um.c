#include "um.h"
#include "mem.h"
#include "bitpack.h"

/* Registers and memory statically defined upon creation of the machine. */
#define numRegisters 8
static UM_Word registers[numRegisters];
static Mem* memorySegments;
static int programCounter;
static int INITIAL_SET_SIZE = 500; // Number of memory segment IDs
static int PROGRAM_HINT = 500;     // Number of program instructions

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
    initializeRegisters(registers, numRegisters);

    mapProgram(program);
    programCounter = 0;
    
    while(programCounter < 
          UArray_length((UArray_T)Seq_get(memorySegments->mappedIDs, 0)))
    {
        UM_Word instruction = *(UM_Word*)UArray_at(
            (UArray_T)Seq_get(memorySegments->mappedIDs, 0), programCounter);
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
            //printf("bits: %u\n", temp);
        }

        UM_Word* instr;
        NEW(instr);
        *instr = temp;
        Seq_addhi(words, instr);
        c = getc(program);
    }

    mapSegment(memorySegments, 0, Seq_length(words));

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
            programCounter++;
            break;
        }
        case SEGLOAD:{
            UM_Word ID = registers[instr.reg2];
            UM_Word offset = registers[instr.reg3];
            UM_Word toStore = segmentedLoad(memorySegments, ID, offset);
            registers[instr.reg1] = toStore;
            programCounter++;
            break;
        }
        case SEGSTORE:{
            UM_Word ID = registers[instr.reg1];
            UM_Word offset = registers[instr.reg2];
            UM_Word value = registers[instr.reg3];
            segmentedStore(memorySegments, ID, offset, value);
            programCounter++;
            break;
        }
        case ADD:{
            addition(registers, instr.reg1, instr.reg2, instr.reg3);
            programCounter++;
            break;
        }
        case MULTIPLY:{
            multiplication(registers, instr.reg1, instr.reg2, instr.reg3);
            programCounter++;
            break;
        }
        case DIVIDE:{
            division(registers, instr.reg1, instr.reg2, instr.reg3);
            programCounter++;
            break;
        }
        case NAND:{
            bitwiseNAND(registers, instr.reg1, instr.reg2, instr.reg3);
            programCounter++;
            break;
        }
        case HALT: {
            freeMem(memorySegments);
            exit(4);
            programCounter = 0;
            break;
        }
        case MAP:{
            UM_Word length = registers[instr.reg3];
            UM_Word ID = registers[instr.reg2];
            mapSegment(memorySegments, ID, length);
            programCounter++;
            break;
        }
        case UNMAP:{
            UM_Word ID = registers[instr.reg3];
            unmapSegment(memorySegments, ID);
            programCounter++;
            break;
        }
        case OUTPUT:{
            output(registers, instr.reg3);
            programCounter++;
            break;
        }
        case INPUT:{
            input(registers, instr.reg3);
            programCounter++;
            break;
        }
        case LOADPROG:{
            UM_Word ID = registers[instr.reg2];
            UArray_T copy = segmentCopy(memorySegments, ID);
            unmapSegment(memorySegments, 0);
            mapSegment(memorySegments, 0, UArray_length(copy));
            UArray_T toFree = Seq_put(memorySegments->mappedIDs, 0, copy);
            UArray_free(&toFree);
            programCounter = registers[instr.reg3];
            break;
        }
        case LOADVAL:{
            loadValue(registers, instr.reg1, instr.value);
            programCounter++;
            break;
        }
    }
}
