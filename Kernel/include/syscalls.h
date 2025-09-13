/*
 * syscalls.h
 *
 *  Created on: 30/09/2008
 *      Author: arkanrow
 */

#ifndef SYSCALLS_H_
#define SYSCALLS_H_
#include <stdint.h>
#include <top.h>
#include <types.h>

enum syscallId {SCHED_NEW_PROC=0, SCHED_EXIT, SCHED_KILL, SCHED_GETPID, TOP, NPROCESSES,
	IO_VTWRITE, IO_VTREAD,
	SEM_RES, SEM_INC, SEM_DEC, SEM_NBDEC, SEM_QSIZE, SEM_COUNT, GHEAP,
	IPC_GSHMEM , IPC_USHMEM, IPC_GETID
	};

int syscall(uint32 syscallId, uint32 p1, uint32 p2, uint32 p3, char * src,
		char * dst);

/*Scheduling system calls*/
uint32 start(int (*p)(int argc, char ** argv), uint32 attached, uint8 priority, uint32 argc,
		char ** argv);
void yield(void);
void Exit();
bool kill(uint32 pid);

/*IO system calls*/
int vtWrite(char * buffer, uint32 len);
int vtRead(char * buffer, uint32 len);
uint32 getpid();

/*Semaphore system calls*/
void semReset(int32 semId);
void semInc(int32 semId);
uint32 semDec(int32 semId);
uint32 semNBDec(int32 semId);
uint32 semCount(void);
uint32 semQSize(void);
uint32 getInfoProcesses (vecProcessInfo processes, int len);
uint32 numberOfProcesses (void);
uint32 getHeap (void);

/*IPC system calls*/
void * getShmem(uint32 id);
void ungetShmem(uint32 id);
uint32 getId(uint32 PID);

#endif /* SYSCALLS_H_ */
