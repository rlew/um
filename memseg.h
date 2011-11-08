#ifndef MEMSEG_INCLUDED
#define MEMSEG_INCLUDED
#include <stdlib.h>
#include <stdint.h>
#include "seq.h"
#include "uarray.h"

typedef uint32_t UM_Word;

typedef struct Mem{
    Seq_T mappedIDs;
    Seq_T unmappedIDs;
}Mem;

void instantiateMem(Mem* memorySegments, int length);
UArray_T mapSegment(Mem* memorySegments, UM_Word ID, int length);
void unmapSegment(Mem* memorySegments, UM_Word index);
UArray_T getIndex(Mem* memorySegments, UM_Word ID);
UArray_T segmentCopy(Mem* memorySegments, UM_Word ID);
UM_Word segmentedLoad(Mem* memorySegments, UM_Word ID, int offset);
void segmentedStore(Mem* memorySegments, UM_Word ID, int offset, UM_Word
                       value);
void freeMem(Mem* memorySegments);
#endif
