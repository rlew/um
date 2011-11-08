#include "registers.h"
#include "memseg.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

/*
 * Stores the value at $r[original] in $r[toSet] if value is not 0. Called
 * with command 0.
 */
void conditionalMove(UM_Word* registers, UM_Word toSet, UM_Word original,
                     UM_Word value){
    if(registers[value] != 0)
        registers[toSet] = registers[original];
}

/*
 * Adds the values in $r[val1] and $r[val2] and takes the first 32 significant 
 * bits and stores the sum in $r[sum]. Called with command 3.
 */
void addition(UM_Word* registers, UM_Word sum, UM_Word val1, UM_Word val2) {
    registers[sum] = registers[val1] + registers[val2];
}

/*
 * Multiplies $r[val1] and $r[val2] and takes the first 32 signifcant bits
 * and stores the product in $r[product]. Called with command 4.
 */
void multiplication(UM_Word* registers, UM_Word product, UM_Word val1, 
                    UM_Word val2) {
    registers[product] = registers[val1] * registers[val2];
}

/*
 * Divides the value in $r[numerator] by that found in $r[denominator] and
 * stores the result in $r[quotient]. It is a c.r.t. for denominator to be 0
 * Called with command 5.
 */
void division(UM_Word* registers, UM_Word quotient, UM_Word numerator, 
              UM_Word denominator) {
    assert(denominator);
    registers[quotient] = registers[numerator] / registers[denominator];
}

/* 
 * Performs bitwise NOT and AND operations on the values in $r[val1] and
 * $r[val2] and stores the result in $r[result]. Called with command 6.
 */
void bitwiseNAND(UM_Word* registers, UM_Word result, UM_Word val1, 
                 UM_Word val2) {
    registers[result] = ~(registers[val1] & registers[val2]);
}

/*
 * Load the value passed into the register $r[reg]. Called with command 13.
 */
void loadValue(UM_Word* registers, UM_Word reg, UM_Word value) {
    registers[reg] = value;
}

/*
 * Reads from std input and stores in the register $r[reg]. Called with
 * command 11.
 */
void input(UM_Word* registers, UM_Word reg) {
    int c = getc(stdin);
    if(c == '\n'){
        UM_Word one = 0;
        one = ~one;
        registers[reg] = one;
    }
    else {
        if(c > 255 && c < 0) 
            exit(1);
        else
            registers[reg] = (UM_Word)c;
    }
}

/*
 * Reads the value in $r[reg] to std output. Called with command 10.
 */
void output(UM_Word* registers, UM_Word reg) {
    putchar((char)registers[reg]);
}

/*
 * Halts all program operations and frees all corresponding memory. Called
 * with command 7.
 */
void halt(UM_Word* registers){
    FREE(registers);
}
