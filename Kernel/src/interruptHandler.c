/*
 * interruptionHandler.c
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

#include <IDT.h>
#include <interruptHandler.h>
#include <keyboard.h>
#include <bits.h>

#define IDT_SIZE 255
#define LOFFSET_MASK 0x000000000000FFFFLLU
#define UOFFSET_MASK 0xFFFF000000000000LLU
#define INTGATE_MASK 0x00000E0000000000LLU

#define SELECTOR_POS 16
#define LOFFSET_POS 0
#define UOFFSET_POS 48
#define PBIT_POS 47

#define SELECTOR_SIZE 16
#define LOFFSET_SIZE 16
#define UOFFSET_SIZE 16
#define PBIT_SIZE 1

static InterruptDescriptor idt[IDT_SIZE];

static void intDescSetOffset(InterruptDescriptor * descriptor, uint32 offset);

void setupInterrupts(void) {
    IDT idtr;
    int i;

    InterruptDescriptor interr;

    /*Disable interruptions, the scheduler should enable them*/
    _Cli();
    /* Enable timer tick and keyboard */
    PIC1mask(0xFD); // 0b1111_1101 IRQ1 = keyboard - is enabled
    PIC2mask(0xFF); // 0b1111_1111

    /* Create the IDT */
    idtr = CreateIDTR((uint64)idt, sizeof(idt));

    /* Clean the IDT */
    for (i=0; i<IDT_SIZE; i++)
        idt[i] = 0;

    /* Set up the keyboard interruption */
    // intDescInitInterruptDescriptor(&interr, IDT_INTERRUPTGATE);
    interr = INTGATE_MASK;
    intDescSetOffset(&interr, (uint64)int_09_hand);
    // intDescSetPresent(&interr, TRUE);
    setbits64(&interr, PBIT_POS, PBIT_SIZE, TRUE);
    //intDescSetSegmentSelector(&interr, kernelCodeSegment());
    setbits64(&interr, SELECTOR_POS, SELECTOR_SIZE, 0x08);
    IDTSetInterrupt(idtr, 9, interr);

    /* Load the new IDT */
    IDTLoad(idtr);
}


/** Sets the offset. */
void intDescSetOffset(InterruptDescriptor * descriptor, uint32 offset) {
    static int lowerOffset, upperOffset;

    if(descriptor == NULL )
        return;

    /** Split the offset in two. */
    lowerOffset = offset & 0x0FFFF;
    upperOffset = (offset & 0xFFFF0000) >> 16;

    /** Set offsets. */
    setbits64(descriptor, LOFFSET_POS, LOFFSET_SIZE, lowerOffset);
    setbits64(descriptor, UOFFSET_POS, UOFFSET_SIZE, upperOffset);

    return;
}
