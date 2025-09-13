/*
 * syscallsImpl.c
 *
 *  Created on: 29/09/2008
 *      Author: row
 */

#include <syscallsImpl.h>
#include <syscalls.h>
#include <shield.h>
#include <string.h>
#include <IDT.h>
#include <interruptHandler.h>
#include <process.h>
#include <semaphore.h>
#include <scheduler.h>
#include <vt.h>
#include <vtprintf.h>
#include <top.h>
#include <ipc.h>

#ifdef TESTING
    uint32 int80(void) { return 0; }
    uint32 int81(void) { return 0; }
#else
    uint32 int80(void);
    uint32 int81(void);
#endif

typedef int (*syscallT)(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);

/*Implementations of scheduler system calls*/
static int scStart(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scExit(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scKill(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scGetpid(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);

/*Implementations of IO the system calls*/
static int scScrWrite(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scScrRead(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);

/*Implementations of the semaphore system calls*/
static int scSemReset(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scSemInc(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scSemDec(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scSemNBDec(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scSemQSize(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scSemCount(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scTop(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);
static int scNumberOfProcesses(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);


static int scGetHeap(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst);

/*Implementations of the IPC system calls*/
static int scGetShmem(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scUngetShmem(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);
static int scGetId(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst);

syscallT syscallList[] = {
        /*Scheduling system calls*/
         scStart, scExit, scKill, scGetpid, scTop, scNumberOfProcesses,
        /*I O Syscalls*/
        scScrWrite, scScrRead,
        /*Semaphore Syscalls*/
        scSemReset, scSemInc, scSemDec, scSemNBDec, scSemQSize, scSemCount,
        scGetHeap,
        scGetShmem, scUngetShmem, scGetId
        };

void setupSyscalls(void)
{
    /* Get the interrupt table */
    IDT idtr;
    IDTSave(&idtr);
    InterruptDescriptor interr;

    /* Set up the interruptions we'll need */
    intDescInitInterruptDescriptor(&interr, IDT_INTERRUPTGATE);
    intDescSetOffset(&interr, (uint32)int80);
    intDescSetPresent(&interr, TRUE);
    intDescSetSegmentSelector(&interr, kernelCodeSegment());

    IDTSetInterrupt(idtr, 0x80, interr);

    /* Set up the interruptions we'll need */
    intDescInitInterruptDescriptor(&interr, IDT_INTERRUPTGATE);
    intDescSetOffset(&interr, (uint32)int81);
    intDescSetPresent(&interr, TRUE);
    intDescSetSegmentSelector(&interr, kernelCodeSegment());

    IDTSetInterrupt(idtr, 0x81, interr);
}

int syscallsImpl(uint32 syscallId, uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    if (syscallId < sizeof(syscallList)/sizeof(syscall))
        return syscallList[syscallId](p1, p2, p3, strSrc, strDst);

    return 0;
}

static int scSemReset(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    semaphoreReset(p1);
    return 0;
}

static int scStart(uint32 attached, uint32 priority, uint32 argc, char * argv,
        char * p)
{
    return startProcess((processCode)p,
            getProcess(schedGetCurrentProcess()->PID)->vt, priority, attached,
            argc, (char**)argv);
}

static int scExit(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    dettach(schedGetCurrentProcess()->PID);
    schedGetCurrentProcess()->state = SCHED_DEAD;
    yield();
    while(1);
}

static int scKill(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    SchedProcess * p;
    if ((p = schedGetProcess(p1)) == NULL)
        return FALSE;

    p->state = SCHED_DEAD;
    return TRUE;
}

static int scScrWrite(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    char buff[p1+1];

    strcpy(buff, strSrc);
    buff[p1] = '\0';
    vtprintf(getProcess(schedGetCurrentProcess()->PID)->vt, buff);

    return 0;
}

static int scScrRead(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    return vtReadImpl(strSrc,p1);
}

static int scSemInc(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    semaphoreInc(p1);
    return 0;
}

static int scSemDec(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return semaphoreDec(p1, schedGetCurrentProcess()->PID);
}

static int scSemNBDec(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return semaphoreNBDec(p1, schedGetCurrentProcess()->PID);
}

static int scSemQSize(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return getSemaphoreQueueSize();
}

static int scSemCount(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return getUsrSemaphoreCount();
}

static int scTop(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return pfTop ((vecProcessInfo)strSrc, p1);
}

static int scNumberOfProcesses(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    return countProcesses ();
}

static int scGetHeap(uint32 p1, uint32 p2, uint32 p3,
        char * strSrc, char * strDst)
{
    SchedProcess * currentSProcess;
    process * currentProcess;

    currentSProcess = schedGetCurrentProcess();
    currentProcess = getProcess(currentSProcess->PID);
    return (uint32)currentProcess->heap;
}

static int scGetShmem(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    void * mem = implGetShmem(schedGetCurrentProcess()->PID, p1);
    activateZones(schedGetCurrentProcess()->PID);
    return (uint32)mem;
}

static int scUngetShmem(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    implUngetShmem(schedGetCurrentProcess()->PID, p1);
    return 0;
}

static int scGetId(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    return implGetId(p1);
}

static int scGetpid(uint32 p1, uint32 p2, uint32 p3, char * strSrc,
        char * strDst)
{
    return schedGetCurrentProcess()->PID;
}
