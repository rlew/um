#include <stdlib.h>
#include "memseg.h"
#include <stdio.h>
#include <mem.h>
#include <assert.h>

/*
 * Increase the available set of IDs (to at least the ID passed in) in 
 * unmappedIDs and sets the corresponding IDs in mappedIDs to NULL. 
 */
static void resizeMem(Mem* memorySegments, UM_Word ID) {
    UM_Word length = Seq_length(memorySegments->mappedIDs);
    UM_Word max = length * 2;
    if(ID > max){
        max = ID + 1;
    }
    for(UM_Word i = length; i < max; i++) {
        UM_Word* value;
        NEW(value);
        *value = i;
        Seq_addhi(memorySegments->unmappedIDs, value);
        Seq_addhi(memorySegments->mappedIDs, NULL);
    }

}

/*
 * Allocates memory for the mapped and unmapped IDs in memory that can be used
 * to created mapped memory segments. It is a c.r.t. to pass in a length less
 * then or equal to zero.
 */
void instantiateMem(Mem* mem, int length) {
    assert(length > 0);
    mem->mappedIDs = Seq_new(length);
    mem->unmappedIDs = Seq_new(length);
    mem->numMapped = 0;
    
    for(UM_Word i = 0; i < (UM_Word)length; i++) {
        UM_Word* value;
        NEW(value);
        *value = i;
        Seq_addhi(mem->mappedIDs, NULL);
        Seq_addhi(mem->unmappedIDs, value);
    }
}

/*
 * Maps a segment in memory by marking an ID as in use and allocating a
 * segment of memory of the specified length. Returns the index of the mapped
 * segment
 */
void mapSegment(Mem* memorySegments, UM_Word ID, int length) {
    if(Seq_length(memorySegments->mappedIDs) == memorySegments->numMapped ||
       (UM_Word)Seq_length(memorySegments->mappedIDs) <= ID){
        resizeMem(memorySegments, ID);
    }

    UArray_T segment = UArray_new(length, sizeof(UM_Word));
    
    // Initializing each UM_Word in the memory segment to 0
    for(UM_Word i = 0; i < (UM_Word)length; i++) {
        UM_Word* elem = UArray_at(segment, i);
        *elem = 0;
    }

    UM_Word* availableID = (UM_Word*)Seq_put(memorySegments->unmappedIDs, ID,
                                             NULL);
    ID = memorySegments->numMapped;
    while(availableID == NULL){
        availableID = (UM_Word*)Seq_put(memorySegments->unmappedIDs, ID,
                                        NULL);
        ID++;
    }
    UM_Word index = *availableID;
    FREE(availableID);
    Seq_put(memorySegments->mappedIDs, index, segment);
    memorySegments->numMapped++;
}

/*
 * Returns an ID to the pool of available IDs and frees all associated memory
 * with the given ID
 */
void unmapSegment(Mem* memorySegments, UM_Word index) {
    UArray_T segmentID = Seq_put(memorySegments->mappedIDs, index, NULL);
    UArray_free(&segmentID);
    UM_Word* value;
    NEW(value);
    *value = index;
    Seq_put(memorySegments->unmappedIDs, index, value);
}

/*
 * Returns the memory segment stored at the specified ID and offset. It is a
 * c.r.t. for the ID to be unmapped.
 */
UM_Word segmentedLoad(Mem* memorySegments, UM_Word ID, int offset){
  assert(Seq_get(memorySegments->mappedIDs, ID));
  return *(UM_Word*)UArray_at((UArray_T)Seq_get(memorySegments->mappedIDs,
           ID), offset);
}

/*
 * Stores the value passed at the specified index and offset in the memory
 * segments. It is a c.r.t. for the ID to be unmapped
 */
void segmentedStore(Mem* memorySegments, UM_Word ID, int offset, UM_Word
                       value){
    UArray_T temp = Seq_get(memorySegments->mappedIDs, ID);
    assert(temp);
    UM_Word* word = UArray_at(temp, offset);
    assert(word);
    *word = value;
}

UArray_T getIndex(Mem* memorySegments, UM_Word ID){
    return (UArray_T)Seq_get(memorySegments->mappedIDs, ID);
}

UArray_T segmentCopy(Mem* memorySegments, UM_Word ID){
    UArray_T segment = Seq_get(memorySegments->mappedIDs, ID);
    UArray_T copy = UArray_new(UArray_length(segment), sizeof(UM_Word));
    for(int i = 0; i < UArray_length(segment); i++){
        UM_Word* value = UArray_at(copy, i);
        *value = *(UM_Word*)UArray_at(segment, i);
    }
    return copy;
}

void freeMem(Mem* memorySegments) {
    while(Seq_length(memorySegments->mappedIDs) != 0) {
        UArray_T seg = Seq_remlo(memorySegments->mappedIDs);
        if(seg != NULL) {
            UArray_free(&seg);
        }
    }
    while(Seq_length(memorySegments->unmappedIDs) != 0) {
        UM_Word* ID = Seq_remlo(memorySegments->unmappedIDs);
        if(ID != NULL) {
            FREE(ID);
        }
    }
    Seq_free(&memorySegments->mappedIDs);
    Seq_free(&memorySegments->unmappedIDs);
    FREE(memorySegments);
}
