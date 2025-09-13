/*
 * LDT.h
 *
 *  Created on: 19/09/2008
 *      Author: arkanrow
 */

#ifndef LDT_H_
#define LDT_H_

#include <types.h>
#include <stdint.h>
#include <segmentDescriptor.h>
#include <segmentSelector.h>

typedef uint64 LDT;

LDT CreateLDTR(uint32 base, uint16 size);

void LDTSetEntry(LDT ldt, uint16 index, uint64 entry);

uint64 LDTGetEntry(LDT ldt, uint16 index);

 void LDTLoad(LDT ldt, uint16 newCSindex, uint16 newDSindex);

 uint32 LDTGetBase(LDT ldt);

 uint16 LDTGetSize(LDT ldt);

void LDTSave(LDT * ldt);

#endif /* LDT_H_ */
