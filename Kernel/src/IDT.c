/*
 * IDT.c
 *
 *  Created on: 19/09/2008
 *      Author: arkanrow
 */

#include <IDT.h>
#include <bits.h>

void loadIDT(uint64 reg);
void saveIDT(uint64 * reg);


IDT CreateIDTR(uint32 base, uint16 size) {
    IDT r = size;
    setbits64(&r, 16, 32, base);
    return r;
}

void IDTSetInterrupt(IDT idt, uint16 index, InterruptDescriptor id) {
    uint32 i = getbits64(idt, 16, 32);
    InterruptDescriptor * base = (InterruptDescriptor*)i;
    base[index] = id;
}

InterruptDescriptor IDTGetInterrupt(IDT idt, uint16 index) {
    uint32 i = getbits64(idt, 16, 32);
    InterruptDescriptor * base = (InterruptDescriptor*)i;
    return base[index];
}

uint32 IDTGetBase(IDT idt) {
    return getbits64(idt, 16, 32);
}

uint16 IDTGetSize(IDT idt) {
    return getbits64(idt, 0, 16);
}

void IDTSetSize(IDT * idt, uint16 newsize) {
    setbits64(idt, 0, 16, newsize);
}

void IDTLoad(IDT idt) {
    loadIDT(idt);
}

void IDTSave(IDT * idt) {
    saveIDT(idt);
}
