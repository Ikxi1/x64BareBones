/*
 * ipc.h
 *
 *  Created on: 17/10/2008
 *      Author: arkanrow
 */

#ifndef IPC_H_
#define IPC_H_

/*
 * ipc.c
 *
 *  Created on: 17/10/2008
 *      Author: arkanrow
 */
/*.h*/
#include <stdint.h>
#include <types.h>
#include <page.h>
#include <shield.h>

#define MAX_ATTACHED 20
#define MAX_ZONES (MEM_BLOCK/sizeof(shMem))
#define MAX_ZONEOWN (MEM_BLOCK/sizeof(ownage))

typedef struct {
	uint32 refCount;
	void * mem;
}shMem;

typedef struct {
	uint32 zones[MAX_ATTACHED];
	int32 last;
}ownage;

void setupIPC();
void * implGetShmem(uint32 PID, uint32 id);
void implUngetShmem(uint32 PID, uint32 id);
void activateZones(uint32 PID);
void deactivateZones(uint32 PID);
#endif /* IPC_H_ */
