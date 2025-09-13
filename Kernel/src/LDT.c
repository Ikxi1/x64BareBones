/*
 * LDT.c
 *
 *  Created on: 19/09/2008
 *      Author: arkanrow
 */

#include <LDT.h>
#include <bits.h>

#ifdef TESTING
 static void loadLDT(uint64 reg, uint16 CS, uint16 DS) {}
 static void saveLDT(uint64 * reg) {}
#else
void loadLDT(uint64 reg, uint16 CS, uint16 DS);
void saveLDT(uint64 * reg);
#endif
#ifdef TESTING
#include <stdio.h>
#else
#include <vtprintf.h>
#define printf kprintf
#endif


 LDT CreateLDTR(uint32 base, uint16 size)
{
	LDT r = size;
	setbits64(&r, 16, 32, base);
	return r;
}

 void LDTSetEntry(LDT ldt, uint16 index, uint64 entry)
{
	uint32 i = getbits64(ldt, 16, 32);
	uint64 * base = (uint64*)i;
	base[index] = entry;
}

 SegmentSelector LDTSetSegment(LDT ldt, uint16 index, SegmentDescriptor sd)
{
	SegmentSelector ret;

	uint32 i = getbits64(ldt, 16, 32);
	SegmentDescriptor * base = (SegmentDescriptor*)i;
	base[index] = sd;

	/*Build a selector wich has enough privilege level to access the segment*/
	segSelSet(&ret, segmentGetAccess(sd, SD_GET_LV), SS_LDT, index);
	return ret;
}

 uint64 LDTGetEntry(LDT ldt, uint16 index)
{
	uint32 i = getbits64(ldt, 16, 32);
	uint64 * base = (uint64*)i;
	return base[index];
}

 uint32 LDTGetBase(LDT ldt)
{
	return getbits64(ldt, 16, 32);
}

 uint16 LDTGetSize(LDT ldt)
{
	return getbits64(ldt, 0, 16);
}

 void LDTSetSize(LDT * ldt, uint16 newsize)
{
	setbits64(ldt, 0, 16, newsize);
}

 void LDTLoad(LDT ldt, uint16 newCSindex, uint16 newDSindex)
{
	SegmentSelector cs, ds;

	/*Create a selector for the new code segment we'll use*/
	segSelZero(&cs);
	segSelSetRPL(&cs, SS_LV0);
	segSelSetTI(&cs, SS_LDT);
	segSelSetIndex(&cs, newCSindex);
	/*Create a selector for the new data segment we'll use*/
	segSelZero(&ds);
	segSelSetRPL(&ds, SS_LV0);
	segSelSetTI(&ds, SS_LDT);
	segSelSetIndex(&ds, newDSindex);

	loadLDT(ldt, cs, ds);
}

 void LDTSave(LDT * ldt)
{
	saveLDT(ldt);
}

void LDTPrint(LDT ldt)
{
	uint32 i;
	uint64 entry;

	printf("LDT Location: 0x%x LDT Size: %db\n", LDTGetBase(ldt), LDTGetSize(ldt));
	for(i=1; i<LDTGetSize(ldt)/sizeof(uint64); i++)
	{
		entry = LDTGetEntry(ldt, i);

		if (ldt != 0)
		{
			if (segmentGetAccess(*((SegmentDescriptor*)&entry), SD_CODE_OR_DATA) != 0)
			{
				/*Print a segment descriptor (horrible cast)*/
				printf("%d:", i);
				printSegment(*((SegmentDescriptor*)&entry));
			}
			else
			{
				/*Print another thing*/
			}
		}
	}
}
