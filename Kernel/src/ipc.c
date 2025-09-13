/*
 * ipc.c
 *
 *  Created on: 17/10/2008
 *      Author: arkanrow
 */
#include <ipc.h>

static shMem * zones;
static ownage * zoneOwners;

void setupIPC()
{
	uint32 i,j;

	zones = page();
	zoneOwners = page();

	for (i=0; i< MAX_ZONES; i++)
	{
		zones[i].refCount = 0;
		zones[i].mem = NULL;
	}

	for (i=0; i<MAX_ZONEOWN; i++)
	{
		zoneOwners[i].last = 0;
		for(j=0; j<MAX_ATTACHED; j++)
			zoneOwners[i].zones[j] = 0;
	}
}

void * implGetShmem(uint32 PID, uint32 id)
{
	uint32 j;

	if (id >= MAX_ZONES)
		return NULL;

	if (zoneOwners[PID].last == -1)
		return NULL;

	/*If zone is free, reserve some memory*/
	if (zones[id].refCount == 0)
		zones[id].mem = page();

	zones[id].refCount++;

	zoneOwners[PID].zones[zoneOwners[PID].last] = id;

	for (j=zoneOwners[PID].last; j<MAX_ATTACHED; j++)
		if (zoneOwners[PID].zones[j] == 0)
		{
			zoneOwners[PID].last = j;
			break;
		}


	if (j == MAX_ATTACHED)
		zoneOwners[PID].last = -1;

	return zones[id].mem;
}

void implUngetShmem(uint32 PID, uint32 id)
{
	uint32 i;

	for (i=0; i<MAX_ATTACHED; i++)
		if (zoneOwners[PID].zones[i] == id)
		{
			zoneOwners[PID].zones[i] = 0;
			if (i < zoneOwners[PID].last)
				zoneOwners[PID].last = i;

			if (--zones[id].refCount == 0)
			{
				unpage(zones[id].mem);
				zones[id].mem = NULL;
			}
		}

	/*Zone didn't exist*/
	return;
}

void activateZones(uint32 PID)
{
	uint32 i;

	for (i=0; i<MAX_ATTACHED; i++)
		if (zoneOwners[PID].zones[i] != 0)
			zones[zoneOwners[PID].zones[i]].mem;
}

void deactivateZones(uint32 PID)
{
	uint32 i;

	for (i=0; i<MAX_ATTACHED; i++)
		if (zoneOwners[PID].zones[i] != 0)
			zones[zoneOwners[PID].zones[i]].mem;
}

