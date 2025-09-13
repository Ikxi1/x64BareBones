/*
 * proccess.h
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <stdint.h>

typedef int (*processCode)(int * argv, char ** argc);

typedef struct
{
	void * stack;
	void * heap;
	uint32 ESP;
	uint8 vt;
	uint8 priority;
	uint8 fixedPriority;
	uint32 parentPID;
} process;

int startProcess(processCode p, uint32 vt, uint32 priority, uint32 attached,
		uint32 argc, char ** argv);

void setupProcessTable();

void endProcess();

process * getProcess(uint32 PID);

uint32 maxProcessCount();

#endif /* PROCESS_H_ */
