/*
 * semaphore.h
 *
 *  Created on: 02/10/2008
 *      Author: arkanrow
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdint.h>
#include <types.h>

enum semRet {SEM_FULL=0, SEM_OK, SEM_BLOCK};

void setupSemaphore();

void semaphoreReset(uint32 semId);

void semaphoreInc(int32 semId);

uint32 semaphoreDec(int32 semId, uint32 PID);

uint32 semaphoreNBDec(int32 semId, uint32 PID);

uint32 getSemaphoreQueueSize();

uint32 getSysSemaphoreCount();

uint32 getUsrSemaphoreCount();

uint32 implGetId(uint32 PID);

#endif /* SEMAPHORE_H_ */
