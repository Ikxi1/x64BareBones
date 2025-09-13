/*
 * scheduler.c
 *
 *  Created on: 02/10/2008
 *      Author: arkanrow
 */
#include <page.h>
#include <string.h>
#include <interruptHandler.h>
#include <types.h>
#include <scheduler.h>
#include <process.h>
#include <shield.h>


/////////////////////////
#include <vtprintf.h>

#ifndef SCHED_PRIORITY

#ifdef TESTING
	void _Sti(void) {}
#endif

static SchedProcess * activeList;
static uint32 lastAdded = -1;
static uint32 currentProcess = 0;
static uint32 iterator;

static uint32 getCurrentRunningProc( void );

void setupScheduler()
{
	int i;
	SchedProcess zeroP = {0, 0, SCHED_FREE, 0, 0};
	activeList = page();

	for (i=0; i<MEM_BLOCK/sizeof(SchedProcess); i++)
		activeList[i] = zeroP;
}

void startScheduler()
{
	_Sti();
}

uint32 schedule()
{
	uint32 running;

//	vtprintf(0,"schedule\n");

	if(activeList[currentProcess].state == SCHED_RUNNING)
		activeList[currentProcess].state = SCHED_READY;

	running = getCurrentRunningProc();

	currentProcess = activeList[currentProcess].next;

	while(currentProcess == 0 ||
			activeList[currentProcess].state != SCHED_READY)
	{
		if (activeList[currentProcess].state == SCHED_DEAD)
		{
			uint32 pid = activeList[currentProcess].PID;
			schedRemoveProcess(pid);
			endProcess(pid);
			activeList[currentProcess].state = SCHED_FREE;
		}
		currentProcess = activeList[currentProcess].next;
	}

	//vtprintf(0,"schedules: %d\n",activeList[currentProcess].PID);

	activeList[currentProcess].state = SCHED_RUNNING;
	activeList[currentProcess].usage = 100/running;

	return activeList[currentProcess].PID;
}

void schedAddProcess(uint32 PID)
{
	uint32 i;

	for (i=0; i<MEM_BLOCK/sizeof(SchedProcess); i++)
	{
		if (activeList[i].state == SCHED_FREE)
		{
			activeList[i].PID = PID;
			activeList[i].state = SCHED_READY;
			activeList[i].next = 0;
			break;
		}
	}

	if (lastAdded != -1)
		activeList[lastAdded].next = i;

	lastAdded = i;
/*	uint32 prev, next, jumps = 1, i;

	prev = next = activeList[0].next;

	while(next != 0 && jumps < MEM_BLOCK/sizeof(SchedProcess))
		jumps++, prev = next, next = activeList[next].next;

	if(jumps < MEM_BLOCK/sizeof(SchedProcess))
		for (i=1; i<MEM_BLOCK/sizeof(SchedProcess); i++)
		{
			if (activeList[i].state == SCHED_FREE)
			{
				activeList[i].PID = PID;
				activeList[i].state = SCHED_READY;
				activeList[i].next = 0;
				break;
			}
		}

	activeList[prev].next = i;

	if (lastAdded != -1)
		activeList[lastAdded].next = i;

	vtprintf(0,"Last added %d - PID %d\n",lastAdded,activeList[lastAdded].PID);
	lastAdded = i;*/
}

uint32 schedRemoveProcess(uint32 PID)
{
	uint32 i=0, j=0;

	i = j = activeList[0].next;

	while(i != 0)
	{
		if (activeList[i].PID == PID)
		{
			activeList[j].next = activeList[i].next;
			activeList[i].state = SCHED_FREE;
			if(i == lastAdded)
				lastAdded = j;
			return TRUE;
		}

		j = i;
		i = activeList[i].next;
	}

	return FALSE;
}

SchedProcess * schedGetCurrentProcess()
{
	return &activeList[currentProcess];
}

SchedProcess * schedGetProcess(uint32 PID)
{
	uint32 i=0;

	i = activeList[0].next;

	while(i != 0)
	{
		if (activeList[i].PID == PID)
			return &activeList[i];

		i = activeList[i].next;
	}

	return NULL;
}

void beginIteration (void)
{
	/* Salto el proceso 0 (Kernel) */
	iterator = activeList[0].next;
}

SchedProcess * iterateProcesses (void)
{
	SchedProcess * ans;

	if (iterator == 0)
		return NULL;

	ans = &activeList[iterator];
	iterator = activeList[iterator].next;

	return ans;
}

uint32 countProcesses (void)
{
	uint32 count = 0;

	beginIteration();

	while (iterateProcesses() != NULL)
		count++;

	return count;
}

static uint32 getCurrentRunningProc( void )
{
	uint32 current = activeList[0].next, total = 0;

	while( current != 0 )
	{
		if( activeList[current].state == SCHED_READY )
			total++;

		current = activeList[current].next;
	}

	return total;
}


#endif /** ifndef SCHED_PRIORITY */
