#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mem.h>
#include "memseg.h"
#include "uarray.h"
#include "stack.h"
#include "seq.h"
#include <assert.h>

//PROBLEM: not passing things by reference / modifying within funcitons.

static int MEM_SEG_LEN = 10;

void verifyMapped(Seq_T memorySegments, Seq_T unmappedSegments) {
    assert(Seq_length(memorySegments) == Seq_length(unmappedSegments));
    for(int i = 0; i < Seq_length(memorySegments); i++){
        if(Seq_get(memorySegments, i) == Seq_get(unmappedSegments, i)){
            fprintf(stderr, "Cannot be both mapped and unmapped: %d\n", i);
         /*   if(Seq_get(unmappedSegments, i) == NULL) {
                fprintf(stderr, "Both NULL\n");
            }*/
            exit(1);
        }
        if(Seq_get(memorySegments, i) != NULL) {
            if(Seq_get(unmappedSegments, i) != NULL){
                fprintf(stderr, "Failed to unmap index: %d\n", i);
                exit(1);
            }
            UArray_T temp = Seq_get(memorySegments, i);
            for(UM_Word j = 0; j < (UM_Word)UArray_length(temp); j++) {
                UM_Word value = *(UM_Word*)UArray_at(temp, j);
                if(value != 0){
                    printf("Segmented load at %d with value of %u\n", i, value);
                }
            }
        }
    }
    printf("Verified\n");
}


int main(int argc, char* argv[]) {
    (void) argc; (void) argv;
    Mem* mem;
    NEW(mem);
    instantiateMem(mem, MEM_SEG_LEN);
    printf("Verifying after instantiate:\n");
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    mapSegment(mem, 0, 10);
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    UM_Word value = 0;
    segmentedStore(mem, 0, 9, value);

    if(segmentedLoad(mem, 0, 9) != value) {
        fprintf(stderr, "incorrect load and store");
        exit(1);
    }
    else {
        printf("value at memseg: %u\n", segmentedLoad(mem, 0, 9));
        printf("Woohoo! Correct seg store and load!\n");
    }

    printf("resizing\n");
    mapSegment(mem, 11, 1);
    printf("verifying index 1 mapping\n");
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    unmapSegment(mem, 0);
    printf("verifying index 0 unmapping\n");
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    unmapSegment(mem, 11);
    printf("verifying index 1 unmapping\n");
    verifyMapped(mem->mappedIDs, mem->unmappedIDs);

    freeMem(mem);
    return 0;
}
