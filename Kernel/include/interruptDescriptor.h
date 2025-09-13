#ifndef INTERRUPTDESCRIPTOR_H_
#define INTERRUPTDESCRIPTOR_H_

// TODO: DPL_T y RPL_T tienen las mismas constantes... no habria que cambiarlo? Si bien puedo redefinirlo aca,
// tendria que hacer lo mismo con SegmentSelector. Necesito los 2 .h y tienen conflictos si incluyo los 2.
#include "segmentSelector.h"
//#include "segmentDescriptor.h"
#include "types.h"

typedef uint64 InterruptDescriptor;
//typedef InterruptDescriptor TaskGate; /** Not used. */
typedef InterruptDescriptor InterruptGate;
typedef InterruptDescriptor TrapGate;

typedef enum InterruptDescriptorType {
//	IDT_TASKGATE,
	IDT_INTERRUPTGATE,
	IDT_TRAPGATE
}InterruptDescriptorType;


//TODO: Crear un "common" con cosas como bool y DPL_T.

/** Initializes a IDT register. */
 void intDescInitInterruptDescriptor(InterruptDescriptor * descriptor, InterruptDescriptorType type);

/** Gets the descriptor type. */
 InterruptDescriptorType intDescGetIntDescType(InterruptDescriptor descriptor);

/** Sets the Segment Selector. */
void intDescSetSegmentSelector(InterruptDescriptor * descriptor, SegmentSelector segSelect);

/** Gets the Segment Selector. */
SegmentSelector intDescGetSegmentSelector(InterruptDescriptor descriptor);

/** Sets the offset. */
 void intDescSetOffset(InterruptDescriptor * descriptor, uint32 offset);

/** Gets the offset. */
 uint32 intDescGetOffset(InterruptDescriptor descriptor);

/* Fucking functions no andan... :S */
/** Sets Present Bit. */
 void intDescSetPresent(InterruptDescriptor * descriptor, bool present);

/** Gets the Present Bit. */
 bool intDescGetPresent(InterruptDescriptor descriptor);

///** Sets the Descriptor Privilege Level. */
// void intDescSetDPL(InterruptDescriptor * descriptor, DPL_T dpl);
//
///** Gets Descriptor Privilege Level. */
// DPL_T intDescGetDPL(InterruptDescriptor dpl);

/*Prints the descriptor on the screen*/
void printIDTDesc(InterruptDescriptor id);



#endif /*INTERRUPTDESCRIPTOR_H_*/
