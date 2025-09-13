/*
 * GDT.c
 *
 *  Created on: 19/09/2008
 *      Author: arkanrow
 */

#include <GDT.h>
#include <bits.h>

void loadGDT(uint64 reg, uint16 CS, uint16 DS);
void saveGDT(uint64 * reg);
#include <vtprintf.h>
#define printf kprintf


 GDT CreateGDTR(uint32 base, uint16 size)
{
    GDT r = size;
    setbits64(&r, 16, 32, base);
    return r;
}

 void GDTSetEntry(GDT gdt, uint16 index, uint64 entry)
{
    uint32 i = getbits64(gdt, 16, 32);
    uint64 * base = (uint64*)i;
    base[index] = entry;
}

 SegmentSelector GDTSetSegment(GDT gdt, uint16 index, SegmentDescriptor sd)
{
    SegmentSelector ret;

    uint32 i = getbits64(gdt, 16, 32);
    SegmentDescriptor * base = (SegmentDescriptor*)i;
    base[index] = sd;

    /*Build a selector wich has enough privilege level to access the segment*/
    segSelSet(&ret, segmentGetAccess(sd, SD_GET_LV), SS_GDT, index);
    return ret;
}

 uint64 GDTGetEntry(GDT gdt, uint16 index)
{
    uint32 i = getbits64(gdt, 16, 32);
    uint64 * base = (uint64*)i;
    return base[index];
}

 uint32 GDTGetBase(GDT gdt)
{
    return getbits64(gdt, 16, 32);
}

 uint16 GDTGetSize(GDT gdt)
{
    return getbits64(gdt, 0, 16);
}

 void GDTSetSize(GDT * gdt, uint16 newsize)
{
    setbits64(gdt, 0, 16, newsize);
}

void GDTLoad(GDT gdt, uint16 newCSindex, uint16 newDSindex)
{
    SegmentSelector cs, ds;

    /*Create a selector for the new code segment we'll use*/
    segSelZero(&cs);
    segSelSetRPL(&cs, SS_LV0);
    segSelSetTI(&cs, SS_GDT);
    segSelSetIndex(&cs, newCSindex);
    /*Create a selector for the new data segment we'll use*/
    segSelZero(&ds);
    segSelSetRPL(&ds, SS_LV0);
    segSelSetTI(&ds, SS_GDT);
    segSelSetIndex(&ds, newDSindex);

    loadGDT(gdt, cs, ds);
}

 void GDTSave(GDT * gdt)
{
    saveGDT(gdt);
}

void GDTPrint(GDT gdt)
{
    uint32 i;
    uint64 entry;

    printf("GDT Location: 0x%x GDT Size: %db\n", GDTGetBase(gdt), GDTGetSize(gdt));
    for(i=1; i<GDTGetSize(gdt)/sizeof(uint64); i++)
    {
        entry = GDTGetEntry(gdt, i);

        if (gdt != 0)
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
