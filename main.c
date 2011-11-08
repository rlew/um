#include <stdio.h>
#include <stdlib.h>
#include "um.h"

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "No executables provided.\n");
        exit(1);
    }
    FILE* program = fopen(argv[2], "r");
    if(!program) {
        fprintf(stderr, "File failed to open.\n");
        exit(1);
    }
    // do more input validation
    build_and_execute_um(program);
    return 0;
}
