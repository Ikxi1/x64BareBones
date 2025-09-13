/*
 * process.c
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

//TODO EMPROLIJAR, lograr que el proceos se cierre y haga el cleanup, multiples schedulers etc.

#include <types.h>
#include <stdint.h>
#include <process.h>
#include <page.h>
#include <shield.h>
#include <string.h>
#include <interruptHandler.h>
#include <scheduler.h>
#include <vt.h>
#include <syscalls.h>
#include <vtprintf.h>
#include <ipc.h>

/*List of process*/
process * procList;

static int nextPID = 0;

/*Maximum process count*/
static int maxProcess;

typedef struct
{
	uint32 EDI, ESI;
	uint32 ESP, EBP;
	uint32 EBX, EDX, ECX , EAX;
}context;

static void * push(void * ptr, void * data, uint32 size);
static uint32 newContext(void * stack, void * heap, processCode p, int argc, char ** argv);
static uint32 getNextAvailablePID(void);

uint32 switchContext(uint32 esp)
{
	uint32 newEsp;

	/*Context is saved in the prev. process stack, so we just save ESP*/
	if (schedGetCurrentProcess()->PID != 0)
		procList[schedGetCurrentProcess()->PID].ESP = esp;

	/*Unmap old process stack and heap*/
	procList[schedGetCurrentProcess()->PID].stack;
	procList[schedGetCurrentProcess()->PID].heap;
	/*Unmap old process shared memory*/
	deactivateZones(schedGetCurrentProcess()->PID);

	newEsp = procList[schedule()].ESP;

	/*Map new process shared memory*/
	activateZones(schedGetCurrentProcess()->PID);
	/*Map new process stack and heap*/
	procList[schedGetCurrentProcess()->PID].stack;
	procList[schedGetCurrentProcess()->PID].heap;

	return newEsp;
}

uint32 getKernelContext(uint32 esp)
{
	/*if we're in kernel, just return esp (the kernel stack), this is used for
	 * the first switch*/
	if (schedGetCurrentProcess()->PID == 0)
		procList[0].ESP = esp;
	/*This returns the kernel ESP, with old the process ESP already pushed as
	 * a parameter*/
	return (uint32)push((void*)procList[0].ESP, &esp, sizeof(uint32));
}

void setupProcessTable()
{
	int i;

	maxProcess = MEM_BLOCK/sizeof(process);
	if ((procList = page()) == NULL)
		return;

	for (i=0; i<maxProcess; i++)
	{
		procList[i].stack = NULL;
		procList[i].heap = NULL;
		procList[i].ESP = 0;
		procList[i].vt = 0;
		procList[i].priority = 0;
		procList[i].fixedPriority = 0;
	}

	/*Setup the process zero, representing the kernel*/
	procList[0].heap = NULL;

	/*The NULL will be replaced with the correct stack on the first switch*/
	procList[0].stack = NULL;

	/*This in fact is useless, as kernel will always write in vt 7, doesn't
	 * read anything, and shouldn't be scheduled (the schedule should
	 * contemplate not executing process zero)*/
	procList[0].vt = 7;
	procList[0].priority = 0;
	procList[0].fixedPriority = 0;

	schedAddProcess(0);
}

int startProcess(processCode p, uint32 vt, uint32 priority, uint32 attached,
		uint32 argc, char ** argv)
{
	uint32 PID;

	if ((PID = getNextAvailablePID()) == 0)
		return 0;

	kprintf("Starting process PID: %d, vt %d, main in 0x%x... ",PID, vt, p);

	/*Get memory for the heap*/
	if ((procList[PID].heap = page()) == NULL)
		return 0;

	/*Get memory for the stack*/
	if ((procList[PID].stack = page()) == NULL)
	{
		unpage(procList[PID].heap);
		return 0;
	}

	/*Create the new context*/
	procList[PID].ESP =
		newContext(procList[PID].stack, procList[PID].heap, p, argc, argv);


	/*Set the target terminal the process will run in*/
	procList[PID].vt = vt;

	/*Set the process priority*/
	procList[PID].priority = priority;

	/*Set the parent PID*/
	procList[PID].parentPID = schedGetCurrentProcess()->PID;

	schedAddProcess(PID);

//	if (attached)
//		attach(PID);

	kprintf("OK\n");

	return PID;
}

void endProcess(uint32 PID)
{
	uint32 i;

	kprintf("Killing process %d\n", PID);
	/*Free heap memory*/
	unpage(procList[PID].heap);
	procList[PID].heap = NULL;

	/*Free stack memory*/
	unpage(procList[PID].stack);
	procList[PID].stack = NULL;

	/*If process is attached to a terminal, dettach*/
	dettach(PID);

	/*Set all the child process to die, this is horribly expensive, I know.*/
	for (i=0; i<maxProcess; i++)
		if (procList[i].stack != NULL && procList[i].parentPID == PID)
			schedGetProcess(i)->state = SCHED_DEAD;
}

process * getProcess(uint32 PID)
{
	return &(procList[PID]);
}

uint32 maxProcessCount()
{
	return maxProcess;
}

static uint32 getNextAvailablePID(void)
{
	uint32 bkpPID = nextPID;

	while (++nextPID == maxProcess ||
			procList[nextPID].stack != NULL)
	{
		if (nextPID == bkpPID)
			return 0;
		if (nextPID == maxProcess)
			nextPID = 0;
	}

	return nextPID;
}

static void * push(void * ptr, void * data, uint32 size)
{
	ptr = (uint8*)ptr - size;
	memcpy(ptr, data, size);
	return ptr;
}

static uint32 newContext(void * stack, void * heap, processCode p, int argc, char ** argv)
{
	/* Enable the interrupt flag*/
	char ** newArgv;
	uint32 i;
	uint32 flags = 0x1 << 9;
	void * ip = Exit;
	context start = {0,0,0,0,0,0,0,0};
	uint8 * esp;
	//uint8 * tmpHeap;

	/*Set the temporal user mapping to fill the pages with the context*/
	setTmpUserMapping(stack, heap);

	/*Point ESP to the end of the block (the begining of the stack)*/
	esp = (uint8*)stack + MEM_BLOCK;
	//esp = (uint8*)getTmpVirtualStack() + MEM_BLOCK;
	//tmpHeap = (uint8*)getTmpVirtualHeap();

	/*Clear the heap*/
	for (i=0; i<MEM_BLOCK; i++)
		//tmpHeap[i] = 0;
		((char*)heap)[i] = 0;

	/*Get enough space for the argv vector*/
	esp -= argc * sizeof(uint32);
	newArgv = (char**)esp;

	/*for each string, copy it and add it to the argv vector*/
	for (i=0; i<argc; i++)
	{
		/*push the entire string*/
		esp = push(esp, argv[i], strlen(argv[i])+1);
		newArgv[i] = (char*)esp;
	}

	/*Push argv and argc*/
	esp = push(esp, &newArgv, sizeof(uint32));
	esp = push(esp, &argc, sizeof(uint32));

	/*Push return address for main (endProccess)*/
	esp = push(esp, &ip, sizeof(ip));

	/*Push the starting address*/
	ip = p;
	esp = push(esp, &flags, sizeof(uint32));
	esp = push(esp, &ip, sizeof(ip));

	/*Push the starting context*/
	esp = push(esp, &start, sizeof(context));

	//((int*)getTmpVirtualStack())[0] = 100;

	/*Unset the temporal mapping*/
	//unSetTmpUserMapping();

	//return (uint32)esp - (uint32)getTmpVirtualStack() + (uint32)getVirtualStack();
	return (uint32)esp;
}
