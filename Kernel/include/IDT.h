/*
 * IDT.h
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

#ifndef IDT_H_
#define IDT_H_

#include <types.h>
#include <stdint.h>
#include <interruptDescriptor.h>

/* If i used a struct I would have had to override 32bit packing and meh... */
typedef uint64 IDT;

 IDT CreateIDTR(uint32 base, uint16 size);

 void IDTSetInterrupt(IDT idt, uint16 index, InterruptDescriptor entry);

 InterruptDescriptor IDTGetInterrupt(IDT idt, uint16 index);

 uint16 IDTGetSize(IDT idt);

 void IDTSetSize(IDT * idt, uint16 newsize);

 void IDTLoad(IDT idt);

 void IDTSave(IDT * idt);

 uint32 IDTGetBase(IDT idt);

 uint16 IDTGetSize(IDT idt);

void IDTPrint(IDT idt);

#endif /* IDT_H_ */
