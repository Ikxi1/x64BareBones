#include <syscalls.h>
#include <vtprintf.h>
#include <types.h>
#include <top.h>

#ifdef TESTING
	void scyield() {}
#else
	/*defined in syscalls.asm*/
	void scyield();
#endif

void switchToCurrent(uint32 PID);
void scSwitchToCurrent(uint32 PID);

int vtWrite(char * buffer, uint32 len)
{
	return syscall(IO_VTWRITE, len, 0, 0, buffer, NULL);
}

int vtRead(char * buffer, uint32 len)
{
	return syscall(IO_VTREAD, len, 0, 0, buffer, NULL);
}

void semReset(int32 semId)
{
	syscall(SEM_RES, semId, 0, 0, NULL, NULL);
}

void semInc(int32 semId)
{
	syscall(SEM_INC, semId, 0, 0, NULL, NULL);
}

uint32 semDec(int32 semId)
{
	return syscall(SEM_DEC, semId, 0, 0, NULL, NULL);
}

uint32 semNBDec(int32 semId)
{
	return syscall(SEM_NBDEC, semId, 0, 0, NULL, NULL);
}

uint32 semQSize(void)
{
	return syscall(SEM_QSIZE, 0, 0, 0, NULL, NULL);
}

uint32 semCount(void)
{
	return syscall(SEM_COUNT, 0, 0, 0, NULL, NULL);
}

uint32 getInfoProcesses (vecProcessInfo processes, int len)
{
	return syscall(TOP, len, 0, 0, (char*)processes, NULL);
}

uint32 numberOfProcesses (void)
{
	return syscall (NPROCESSES, 0, 0, 0, NULL, NULL);
}

uint32 getHeap (void)
{
	return syscall (GHEAP, 0, 0, 0, NULL, NULL);
}

void yield(void)
{
	scyield();
}

void Exit()
{
	syscall(SCHED_EXIT, 0, 0, 0, NULL, NULL);
}

bool kill(uint32 r)
{
	return syscall(SCHED_KILL, r, 0, 0, NULL, NULL);
}

uint32 start(int (*p)(int argc, char ** argv), uint32 attached, uint8 priority, uint32 argc,
		char ** argv)
{
	return syscall(SCHED_NEW_PROC, attached, priority, argc, (char*)argv,
		(void*)p);
}

void * getShmem(uint32 id)
{
	return (void*)syscall(IPC_GSHMEM, id, 0, 0, NULL, NULL);
}

void ungetShmem(uint32 id)
{
	syscall(IPC_USHMEM, id, 0, 0,NULL, NULL);
}

uint32 getId(uint32 PID)
{
	return syscall(IPC_GETID, PID, 0, 0, NULL, NULL);
}

uint32 getpid()
{
	return syscall(SCHED_GETPID, 0, 0, 0, NULL, NULL);
}
