#include "registers.h"
#include "memseg.h"
#include <stdio.h>
#include <stdlib.h>
#include <mem.h>

/*
 * Stores the value at $r[original] in $r[toSet] if value is not 0. Called
 * with command 0.
 */
void conditionalMove(UM_Word* registers, UM_Word toSet, UM_Word original,
                     UM_Word value){
    (void)registers;
    (void)toSet;
    (void)original;
    (void)value;
//    if(registers[value] != 0)
//        registers[toSet] = registers[original];

}

/*
 * Adds the values in $r[val1] and $r[val2] and takes the first 32 significant 
 * bits and stores the sum in $r[sum]. Called with command 3.
 */
void addition(UM_Word* registers, UM_Word sum, UM_Word val1, UM_Word val2) {
    (void)registers;
    (void)sum;
    (void)val1;
    (void)val2;
}

/*
 * Multiplies $r[val1] and $r[val2] and takes the first 32 signifcant bits
 * and stores the product in $r[product]. Called with command 4.
 */
void multiplication(UM_Word* registers, UM_Word product, UM_Word val1, 
                    UM_Word val2) {
    (void)registers;
    (void)product;
    (void)val1;
    (void)val2;
}

/*
 * Divides the value in $r[numerator] by that found in $r[denominator] and
 * stores the result in $r[quotient]. Called with command 5.
 */
void division(UM_Word* registers, UM_Word quotient, UM_Word numerator, 
              UM_Word denominator) {
    (void)registers;
    (void)quotient;
    (void)numerator;
    (void)denominator;
}

/* 
 * Performs bitwise NOT and AND operations on the values in $r[val1] and
 * $r[val2] and stores the result in $r[result]. Called with command 6.
 */
void bitwiseNAND(UM_Word* registers, UM_Word result, UM_Word val1, 
                 UM_Word val2) {
    (void)registers;
    (void)result;
    (void)val1;
    (void)val2;
}

/*
 * Load the value passed into the register $r[reg]. Called with command 13.
 */
void loadValue(UM_Word* registers, UM_Word reg, UM_Word value) {
    (void)registers;
    (void)reg;
    (void)value;
}

/*
 * Reads from std input and stores in the register $r[reg]. Called with
 * command 11.
 */
void input(UM_Word* registers, UM_Word reg) {
    (void)registers;
    (void)reg;
}

/*
 * Reads the value in $r[reg] to std output. Called with command 10.
 */
void output(UM_Word* registers, UM_Word reg) {
    (void)registers;
    (void)reg;
}

/*
 * Halts all program operations and frees all corresponding memory. Called
 * with command 7.
 */
void halt(UM_Word* registers){
    (void)registers;
}
