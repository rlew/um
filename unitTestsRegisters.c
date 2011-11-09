#include "registers.h"
#include "memseg.h"
#include <stdio.h>
#include <stdlib.h>

#define numReg 8
static UM_Word registers[numReg];

void testConditionalMove(){
    registers[0] = 1;
    registers[1] = 10;
    registers[2] = 11;

    conditionalMove(registers, 0, 1, 2);

    registers[3] = 2000;
    registers[4] = 5555;
    registers[5] = 0;

    conditionalMove(registers, 3, 4, 5);

    printf("Output Move: \n");
    printf("Expected in %d: %d ", 0, 10);
    printf("Expected in %d: %d\n", 3, 2000);
    for(int i = 0; i < numReg; i++){
        printf("Index %d: %u\n", i, registers[i]);
    }
}

void testMultiplication(){
    registers[0] = 0;
    registers[1] = 4;
    registers[2] = 2;

    multiplication(registers, 0, 1, 2);

    registers[3] = 2000;
    registers[4] = 0;
    registers[5] = 1;

    multiplication(registers, 3, 4, 5);
    
    printf("Multiply Verify: ");
    if(registers[0] != 8){
        printf("Expected 8 and found: ");
        output(registers, 0);
        printf("\n");
    }
    else printf("Verified!\n");
    if(registers[3] != 0) {
        printf("Expected 0 and found: ");
        output(registers, 3);
        printf("\n");
    }
    else printf("Verified!\n");
}

void testAddition(){
    for(UM_Word i = 0; i < numReg; i++){
        registers[i] = i;
    }
    UM_Word sum = 0;
    for(UM_Word i = 0; i < numReg; i++){
        addition(registers, 0, 0, i);
        sum += i;
        if(sum != registers[0]){
            printf("Addition test failed at iteration: %u\n", i);
        }
    }
    printf("Addition test passed.\n");
}

void testDivision(){
    for(UM_Word i = 0; i < numReg; i++){
        registers[i] = i;
    }
    //Divinding by Zero should raise assertion
    //division(registers, 0, registers[1], registers[0]);

    UM_Word quotient = 0;
    for(UM_Word i = 1; i < numReg; i++){
        division(registers, 0, 0, i);
        quotient /= i;
        if(quotient != registers[0]){
            printf("Division test failed at iteration: %u\n", i);
        }
    }
    printf("Division test passed\n");
}

void testBitwiseNAND(){
    for(UM_Word i = 0; i < numReg; i++){
        if(i%2) registers[i] = 0;
        else registers[i] = ~0;
    }
    UM_Word expected = 0;
    for(UM_Word i = 1; i < numReg; i++){
        bitwiseNAND(registers, 0, i, i-1);
        if(registers[0] != expected){
           printf("testBitWiseNAND failed at iteration: %u\n", i);
        }
    }
    printf("BitwiseNAND test passed.\n");
}

void testLoadVal(){
    UM_Word normal = 200;
    loadValue(registers, 0, normal);

    printf("Load Verify: ");
    if(registers[0] != normal){
        printf("Expected %u and got: ", normal);
        output(registers, 0);
    }
    else printf("Verified!\n");
}
/*
void testHalt(){
    halt(registers);
}
*/
void testIO(){
    for(int i = 0; i < numReg; i++){
        input(registers, i);
    }
    for(int i = 0; i < numReg; i++){
        output(registers, i);
    }
    printf("\n");
}

int main (int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    initializeRegisters(registers, numReg);
    testConditionalMove();
    testAddition();
    testMultiplication();
    testDivision();
    testBitwiseNAND();
    testLoadVal();
    testIO();
    //testHalt();
    //printf("Failure! Did not halt.\n");
    return 0;
}
