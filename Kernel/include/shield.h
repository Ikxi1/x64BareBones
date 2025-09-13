#ifndef SHIELD_H_
#define SHIELD_H_

#include <types.h>
#include <segmentDescriptor.h>
#include <segmentSelector.h>
#include <multiboot.h>

// void * shield(void * physMem, bool code, bool privileged);

// void setupShield();

SegmentSelector kernelCodeSegment();

SegmentSelector kernelDataSegment();

SegmentSelector userCodeSegment();

void * idMap4Pages(void * p);

void unMap4Pages(void * p);

void setUserMapping(void * stack, void * heap);

void * getVirtualStack();

void * getVirtualHeap();

void setTmpUserMapping(void * stack, void * heap);

void unSetTmpUserMapping();

void * getTmpVirtualStack();

void * getTmpVirtualHeap();

#endif /*SHIELD_H_*/
