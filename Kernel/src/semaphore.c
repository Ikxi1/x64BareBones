/*
 * semaphore.c
 *
 *  Created on: 02/10/2008
 *      Author: arkanrow
 */
#include <semaphore.h>
#include <scheduler.h>
#include <page.h>
#include <interruptHandler.h>
#include <scheduler.h>
#include <shield.h>

#include <vtprintf.h>
#include <process.h>
#include <syscalls.h>

#define QSIZE 32

typedef uint32 processQueue[QSIZE];

typedef struct
{
    processQueue queue;
    uint32 qstart;
    uint32 qend;
    uint32 val;
    uint32 empty;
}semaphore;

static uint32 sysSemCount;
static uint32 usrSemCount;
static semaphore * sysSems;
static semaphore * usrSems;

static uint32 enqueue(int32 semId, uint32 PID);
static uint32 dequeue(int32 semId);

void setupSemaphore()
{
    uint32 i,j;

    sysSemCount = (MEM_BLOCK/2)/sizeof(semaphore);
    usrSemCount = (MEM_BLOCK/2)/sizeof(semaphore);

    sysSems = page();
    usrSems = sysSems + usrSemCount;

    /*Set all semaphores to zero*/
    for (i=0; i<sysSemCount; i++)
    {
        sysSems[i].val = 0;
        usrSems[i].val = 0;
        sysSems[i].empty = 1;
        usrSems[i].empty = 1;
        sysSems[i].qend = 0;
        usrSems[i].qend = 0;
        sysSems[i].qstart = 0;
        usrSems[i].qstart = 0;
        /*Emtpy the queue*/
        for (j=0; j<QSIZE; j++)
        {
            sysSems[i].queue[j] = 0;
            usrSems[i].queue[j] = 0;
        }
    }
}

void semaphoreReset(uint32 semId)
{
    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    if(semId < 9)
        return;

    int PID;

    while ((PID = dequeue(semId)) != 0)
        schedGetProcess(PID)->state = SCHED_READY;

    sems[semId].val = 0;
}

void semaphoreInc(int32 semId)
{
    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    int PID;
    SchedProcess * p;

    //kprintf("Sem %d val %d\n", semId, sems[semId].val);

    if (sems[semId].val == 0)
    {
        /*Remove a process from the queue, if there is none, increment the
         * semaphore*/
        while((PID = dequeue(semId)) != 0)
        {
            p = schedGetProcess(PID);
            if (p != NULL)
            {
                p->state = SCHED_READY;
                return;
            }
        }

        sems[semId].val++;
    }
    else
        sems[semId].val++;
}

uint32 semaphoreDec(int32 semId, uint32 PID)
{
    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    //vtprintf(0,"sem %d, pid %d, val %d\n",semId, PID, sems[semId].val);

    if (sems[semId].val == 0 && PID != 0)
    {
        /*Add to the semaphore queue*/
        if (enqueue(semId, PID) == FALSE)
            return SEM_FULL;

        schedGetProcess(PID)->state = SCHED_WAITING;

        yield();

        return SEM_BLOCK;
    }
    else if (sems[semId].val != 0)
        sems[semId].val--;

    return SEM_OK;
}

uint32 semaphoreNBDec(int32 semId, uint32 PID)
{
    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    if (sems[semId].val == 0)
        return FALSE;
    else
        sems[semId].val--;

    return TRUE;
}

uint32 getSemaphoreQueueSize()
{
    return QSIZE;
}

uint32 getSysSemaphoreCount()
{
    return sysSemCount;
}

uint32 getUsrSemaphoreCount()
{
    return usrSemCount;
}

static uint32 enqueue(int32 semId, uint32 PID)
{
    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    /*Check if queue is full*/
    if (sems[semId].qend == sems[semId].qstart && !sems[semId].empty)
        return FALSE;

    /*Enqueue the PID*/
    sems[semId].queue[sems[semId].qend] = PID;
    sems[semId].empty = 0;

    /*Increment qend*/
    sems[semId].qend = (sems[semId].qend + 1) % QSIZE;

    return TRUE;
}

static uint32 dequeue(int32 semId)
{
    int ret;

    /*Select the correct semaphore array*/
    semaphore * sems = semId < 0? sysSems : usrSems;
    semId = semId < 0? - semId : semId;

    /*Check if queue is empty*/
    if (sems[semId].empty)
        return 0;

    ret = sems[semId].queue[sems[semId].qstart];

    /*Increment qstart*/
    sems[semId].qstart = (sems[semId].qstart + 1) % QSIZE;

    if (sems[semId].qstart == sems[semId].qend)
        sems[semId].empty = 1;

    return ret;
}

uint32 implGetId(uint32 PID)
{
    return usrSemCount - PID;
}
