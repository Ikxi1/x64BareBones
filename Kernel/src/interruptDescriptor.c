#include <interruptDescriptor.h>
#include <bits.h>
#include <types.h>
#include <stdint.h>
#include <string.h>

#ifdef TESTING
#include <stdio.h>
#else
#include <vtprintf.h>
#define printf kprintf
#endif

// TODO: NULL Check: Sacarlo?

/** Desciptor types: Masking a descriptor with this mask will return the same mask if it is correct. */
#define INTGATE_MASK 	0x00000E0000000000LLU	/**< Interrupt Gate mask. */
#define TRAPGATE_MASK	0x00000F0000000000LLU	/**< Trap Gate mask */

/** Descriptor Element Masks. */
#define SELECTOR_MASK	0x00000000FFFF0000LLU
#define LOFFSET_MASK	0x000000000000FFFFLLU
#define UOFFSET_MASK	0xFFFF000000000000LLU
#define OFFSET_MASK		LOFFSET_MASK | UOFFSET_MASK

/** Descriptor element positions. */
#define SELECTOR_POS	16
#define LOFFSET_POS		0
#define UOFFSET_POS		48
#define PBIT_POS		47
#define DPL_POS			45
#define TYPE_POS		40

/** Descriptor element sizes. */
#define SELECTOR_SIZE	16
#define LOFFSET_SIZE	16
#define UOFFSET_SIZE	16
#define PBIT_SIZE		1
#define DPL_SIZE		2
#define TYPE_SIZE		5

/** Initializes a IDT register. */
 void intDescInitInterruptDescriptor(InterruptDescriptor * descriptor, InterruptDescriptorType type)
{

    if(descriptor == NULL )
        return;

    *descriptor = 0;


    switch(type)
    {
        case IDT_INTERRUPTGATE:
            //setbits64(descriptor, 0, 64, INTGATE_MASK);
            *descriptor = INTGATE_MASK;
            break;
        case IDT_TRAPGATE:
            //setbits64(descriptor, 0, 64, TRAPGATE_MASK);
            *descriptor = TRAPGATE_MASK;
            break;
    }
}

/** Gets the descriptor type. */
 InterruptDescriptorType intDescGetIntDescType(InterruptDescriptor descriptor)
{
    static uint64 type = 0;

    type = getbits64(descriptor, TYPE_POS, TYPE_SIZE) << TYPE_POS;

    if(type == INTGATE_MASK)
        return IDT_INTERRUPTGATE;
    else if(type == TRAPGATE_MASK)
        return IDT_TRAPGATE;

    return -1;
}

/** Sets the Segment Selector. */
 void intDescSetSegmentSelector(InterruptDescriptor * descriptor, SegmentSelector segSelect)
{
    if(descriptor == NULL )
        return;

    /** Set the new selector. */
    setbits64(descriptor, SELECTOR_POS, SELECTOR_SIZE, segSelect);
}

/** Gets the Segment Selector. */
 SegmentSelector intDescGetSegmentSelector(InterruptDescriptor descriptor)
{
    return getbits64(descriptor, SELECTOR_POS, SELECTOR_SIZE);
}

/** Sets the offset. */
 void intDescSetOffset(InterruptDescriptor * descriptor, uint32 offset)
{
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

/** Gets the offset. */
 uint32 intDescGetOffset(InterruptDescriptor descriptor)
{
    return getbits64(descriptor,UOFFSET_POS,UOFFSET_SIZE)<<16|getbits64(descriptor,LOFFSET_POS,LOFFSET_SIZE);
}

/** Sets Present Bit. */
 void intDescSetPresent(InterruptDescriptor * descriptor, bool present)
{
    if(descriptor == NULL )
        return;

    setbits64(descriptor,PBIT_POS,PBIT_SIZE,present);
}

/** Gets the Present Bit. */
 bool intDescGetPresent(InterruptDescriptor descriptor)
{
    return getbits64(descriptor,PBIT_POS,PBIT_SIZE);
}

/*Prints the descriptor on the screen*/
void printIDTDesc(InterruptDescriptor id)
{
    //"TYPE: %s P: %d DPL: %d\n Selector: %X\tOffSet: %X\n"
    printf ("TYPE: %s P: %d Selector: %x\tOffSet: %x\n", intDescGetIntDescType(id) == IDT_TRAPGATE? "Trap Gate":"Interrupt Gate",
            intDescGetPresent(id), intDescGetSegmentSelector(id), intDescGetOffset(id));
    return ;
}
