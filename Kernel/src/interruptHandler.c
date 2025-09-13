/*
 * interruptionHandler.c
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

#include <shield.h>
#include <IDT.h>
#include <interruptHandler.h>
#include <interruptDescriptor.h>
#include <keyboard.h>
#include <vtprintf.h>
#include <scheduler.h>
#include <process.h>
#include <syscalls.h>

#define IDT_SIZE 255

static InterruptDescriptor idt[IDT_SIZE];

#ifdef TESTING
void int_08_hand(void) {}
void int_09_hand(void) {}
void PIC1mask(uint8 mask) {}
void PIC2mask(uint8 mask) {}
void _Cli(void) {}
#else

#endif

static void pagefault()
{
	uint32 pid = schedGetCurrentProcess()->PID;
	vtprintf(getProcess(pid)->vt,
			"Process %PID: Page fault! Process killed\n", pid);
	kprintf("Process %PID: Page fault! Process killed\n", pid);
	kill(pid);
	yield();
}

void setupInterrupts(void)
{
	IDT idtr;
	int i;

	InterruptDescriptor interr;

	/*Disable interruptions, the scheduler should enable them*/
	_Cli();
	/* Enable timer tick and keyboard */
	PIC1mask(0xFC);
	PIC2mask(0xFF);

	/* Create the IDT */
	idtr = CreateIDTR((uint32)idt, sizeof(idt));

	/* Clean the IDT */
	for (i=0; i<IDT_SIZE; i++)
		idt[i] = 0;

	/* Set up the timer tick interruption */
	intDescInitInterruptDescriptor(&interr, IDT_INTERRUPTGATE);
	intDescSetOffset(&interr, (uint32)int_08_hand);
	intDescSetPresent(&interr, TRUE);
	intDescSetSegmentSelector(&interr, kernelCodeSegment());
	IDTSetInterrupt(idtr, 8, interr);

	/* Set up the keyboard interruption */
	intDescInitInterruptDescriptor(&interr, IDT_INTERRUPTGATE);
	intDescSetOffset(&interr, (uint32)int_09_hand);
	intDescSetPresent(&interr, TRUE);
	intDescSetSegmentSelector(&interr, kernelCodeSegment());
	IDTSetInterrupt(idtr, 9, interr);

	/* Set up the keyboard interruption */
	intDescInitInterruptDescriptor(&interr, IDT_TRAPGATE);
	intDescSetOffset(&interr, (uint32)pagefault);
	intDescSetPresent(&interr, TRUE);
	intDescSetSegmentSelector(&interr, kernelCodeSegment());
	IDTSetInterrupt(idtr, 14, interr);

	/* Load the new IDT */
	IDTLoad(idtr);
}
