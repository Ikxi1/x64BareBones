/*
 * scheduler.h
 *
 *  Created on: 02/10/2008
 *      Author: arkanrow
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <stdint.h>

//#define SCHED_PRIORITY

typedef enum {SCHED_READY, SCHED_WAITING, SCHED_RUNNING, SCHED_FREE, SCHED_DEAD} State;

typedef struct {
	uint32 PID;			/**< Process ID. */
	uint32 next;		/**< Next in queue. */
	State state;		/**< Current Process Scheduler state. */
	uint32 runs;		/**< How many times it consumed the full processor time in a row.  */
	uint8 usage;		/**< Processor usage (in %). */
} SchedProcess;

void setupScheduler();

void startScheduler();

uint32 schedule();

void schedAddProcess(uint32 PID);

uint32 schedRemoveProcess(uint32 PID);

SchedProcess * schedGetCurrentProcess();

SchedProcess * schedGetProcess(uint32 PID);

void beginIteration (void);

SchedProcess * iterateProcesses (void);

uint32 countProcesses (void);

#endif /*SCHEDULER_H_*/
