#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mem.h>
#include "memseg.h"
#include "uarray.h"
#include "stack.h"
#include "seq.h"

//PROBLEM: not passing things by reference / modifying within funcitons.

static int MEM_SEG_LEN = 1;

void verifyMapped(Seq_T memorySegments, Seq_T unmappedSegments) {
    // Verifies that the available unmapped segments is updated upon mapping
    for(int i = 0; i < Seq_length(unmappedSegments); i++){
        UM_Word index = *(UM_Word*)Seq_get(unmappedSegments, i);
        if((UArray_T)Seq_get(memorySegments, index) != NULL){
            fprintf(stderr, 
                    "Unmapped segments not updated correctly for: %u\n", 
                    index);
            exit(1);
        }
    }

    for(int i = 0; i < MEM_SEG_LEN; i++) {
        UArray_T segments = (UArray_T)Seq_get(memorySegments, i);
        if (segments != NULL) {
            for(int j = 0; j < UArray_length(segments); j++) {
                // Each 32-bit word must be initialized to 0
                printf("memseg: %u\n", *(UM_Word*)UArray_at(segments, j));
                if(*(UM_Word*)UArray_at(segments, j) != 0) {
                    fprintf(stderr, "segment incorrectly initialized\n");
                    exit(1);
                }
            }
        }
    }
    printf("Verified bitch.\n");
}

int main(int argc, char* argv[]) {
    (void) argc; (void) argv;
    Mem* mem;
    NEW(mem);
    instantiateMem(mem, MEM_SEG_LEN);

    mapSegment(mem, 0, 10);
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    UM_Word value = 20;
    segmentedStore(mem, 0, 9, value);

    if(segmentedLoad(mem, 0, 9) != value) {
        fprintf(stderr, "incorrect load and store");
        exit(1);
    }
    else {
        printf("value: %u\n", segmentedLoad(mem, 0, 9));
        printf("Woohoo! Correct seg store and load!\n");
    }
   
    mapSegment(mem, 1, 1);
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    unmapSegment(mem, 0);
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    unmapSegment(mem, 1);
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    freeMem(mem);
    return 0;
}
