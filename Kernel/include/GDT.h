/*
 * GDT.h
 *
 *  Created on: 19/09/2008
 *      Author: arkanrow
 */

#ifndef GDT_H_
#define GDT_H_

#include <types.h>
#include <stdint.h>
#include <segmentDescriptor.h>
#include <segmentSelector.h>

/* If i used a struct I would have had to override 32bit packing and meh... */
typedef uint64 GDT;

 GDT CreateGDTR(uint32 base, uint16 size);

 void GDTSetEntry(GDT gdt, uint16 index, uint64 entry);

 uint64 GDTGetEntry(GDT gdt, uint16 index);

 SegmentSelector GDTSetSegment(GDT gdt, uint16 index, SegmentDescriptor sd);

 uint16 GDTGetSize(GDT gdt);

 void GDTSetSize(GDT * gdt, uint16 newsize);

 void GDTLoad(GDT gdt, uint16 newCSindex, uint16 newDSindex);

 void GDTSave(GDT * gdt);

 uint32 GDTGetBase(GDT gdt);

 uint16 GDTGetSize(GDT gdt);

void GDTPrint(GDT gdt);

#endif /* GDT_H_ */
