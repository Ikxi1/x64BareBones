#include <i386flat.h>
#include <shield.h>
#include <segmentSelector.h>
#include <segmentDescriptor.h>
#include <GDT.h>
#include <page.h>
#include <stdint.h>
#include <types.h>

//////////////////////////////////////////////////////////////////
// What one believes to be true either is true or becomes true. //
//                -- John Lilly                                 //
//////////////////////////////////////////////////////////////////

typedef uint32 PageEntry;

/*GDT and IDT with 15 entries*/
static SegmentDescriptor gdt[0xF];
static SegmentSelector nuls = 0;
static SegmentSelector kcss = 0;
static SegmentSelector kdss = 0;
static SegmentSelector ucss = 0;

static PageEntry * dirT;
static PageEntry * mapStart;
//static PageEntry * kmap;
//static PageEntry * mmap;

void write_cr3(uint32 * a);
void write_cr0(uint32 a);
uint32 read_cr0();

/*defined in shield.asm*/
#ifndef TESTING
	void enablePaging();
	void loadDirTable(PageEntry * a);
#else
	void write_cr3(uint32 * a){}
	void write_cr0(uint32 a){}
	uint32 read_cr0(){return 0;}
#endif

static void SetKernelShield();
static void SetupPagination();

#define FOURTH_MB_MARK 0x400000
#define EIGHTH_MB_MARK 0x800000

void setupShield()
{
	/*Alloc the necessary memory to manage the LDT's*/
	SetKernelShield();
	SetupPagination();
}

static void SetKernelShield()
{
	GDT gdtr;

	SegmentDescriptor nul, kcs, kds, ucs;

	/* Set up the segments we'll need in the GDT:
	 * All of them code or data 32 bit segments */
	segmentSetZero(&nul);
	segmentSetZero(&kcs);
	segmentSetAttrib(&kcs, SD_DB_32 | SD_G_YES);
	segmentSetBase(&kcs, 0x0);
	segmentSetLimit(&kcs, 0xFFFFFFFF/PAGE_SIZE);
	kds = ucs = kcs;

	/* Set each corresponding segment type */
	segmentSetType(&kcs, SD_CODE | SD_CONFORMING);
	segmentSetType(&kds, SD_DATA | SD_WRITABLE);
	segmentSetType(&ucs, SD_CODE);
	segmentSetAccess(&kcs, SD_CODE_OR_DATA | SD_SEGMENT_PRESENT | SD_LV0);
	segmentSetAccess(&kds, SD_CODE_OR_DATA | SD_SEGMENT_PRESENT | SD_LV0);
	segmentSetAccess(&ucs, SD_CODE_OR_DATA | SD_SEGMENT_PRESENT | SD_LV3);

	/* Load the segments to the new GDT */
	gdtr = CreateGDTR((uint32)gdt, sizeof(gdt));
	nuls = GDTSetSegment(gdtr, 0, nul);
	kcss = GDTSetSegment(gdtr, 1, kcs);
	kdss = GDTSetSegment(gdtr, 2, kds);
	ucss = GDTSetSegment(gdtr, 3, ucs);

	/* Load the new gdt */
	GDTLoad(gdtr, 1, 2);

	/* Pray! */
}

SegmentSelector kernelCodeSegment()
{
	return kcss;
}

SegmentSelector kernelDataSegment()
{
	return kdss;
}

SegmentSelector userCodeSegment()
{
	return ucss;
}

static void SetupPagination()
{
	unsigned long address=0; // holds the physical address of where a page is
	unsigned int i,j;
	/*Directory Table*/
	dirT = (PageEntry*)((char*)FOURTH_MB_MARK - PAGE_SIZE);
	/*Mapping start*/
	mapStart = (PageEntry*)FOURTH_MB_MARK;

	/*Map the kernel (first 4MB)*/
	dirT[0] = (PageEntry)mapStart;
	dirT[0] = dirT[0] | 3;
	for(i=0; i<1024; i++)
	{
		/*Attribute set to: supervisor level, read/write, present(011 in
		 * binary)*/
		mapStart[i] = address | 3;
		address = address + 4096; // 4096 = 4kb
	}
	mapStart += PAGE_SIZE/sizeof(PageEntry);

	/*Map the pages that map the rest of the memory (second 4MB)*/
	dirT[1] = (PageEntry)mapStart;
	dirT[1] = dirT[1] | 3;
	for(i=0; i<1024; i++)
	{
		/*Attribute set to: supervisor level, read/write, not present(010 in
		 * binary)*/
		mapStart[i] = address | 3;
		address = address + 4096; // 4096 = 4kb
	}
	mapStart += PAGE_SIZE/sizeof(PageEntry);

	/*We have mapped the first 8MB of the memory, now we'll use the pages in
	 * the second 4MB to set them in dirT to enable easy idMapping*/
	for (i=2; i<1024; i++)
	{
		/*Clean al the pages*/
		for (j=0; j<1024; j++)
			mapStart[j] = 2;

		/*Set the page in the dir table*/
		dirT[i] = ((uint32)mapStart) | 3;
		mapStart += PAGE_SIZE/sizeof(PageEntry);
	}

	write_cr3(dirT); // put that page directory address into CR3
	write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1
}

void setUserMapping(void * stack, void * heap)
{
	uint32 istack = (uint32)stack;
	uint32 iheap = (uint32)heap;

	/*Get the last Directory Table entry*/
	PageEntry * table = (PageEntry*)(dirT[1023] & 0xFFFFF000);

	/*Map stack and heap*/
	table[0] = (istack & 0xFFFFF000) | 0x3;
	table[1] = ((istack + PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[2] = ((istack + 2 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[3] = ((istack + 3 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[4] = (iheap & 0xFFFFF000) | 0x3;
	table[5] = ((iheap + PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[6] = ((iheap + 2 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[7] = ((iheap + 3 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
}

void * getVirtualStack()
{
	/*Index 1 in dir table*/
	/*Index 0 in pag table*/
	/*Directory Table entry 1024, Page Table entry 0*/
	return (void*)(1023<<22);
}

void * getVirtualHeap()
{
	/*Index 1 in dir table*/
	/*Index 4 in pag table*/
	/*Directory Table entry 1024, Page Table entry 4*/
	return (void*)((1023<<22) | (4<<12));
}

void setTmpUserMapping(void * stack, void * heap)
{
	uint32 istack = (uint32)stack;
	uint32 iheap = (uint32)heap;

	/*Get the last Directory Table entry*/
	PageEntry * table = (PageEntry*)(dirT[1022] & 0xFFFFF000);

	/*Map stack and heap*/
	table[0] = (istack & 0xFFFFF000) | 0x3;
	table[1] = ((istack + PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[2] = ((istack + 2 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[3] = ((istack + 3 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[4] = (iheap & 0xFFFFF000) | 0x3;
	table[5] = ((iheap + PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[6] = ((iheap + 2 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
	table[7] = ((iheap + 3 * PAGE_SIZE) & 0xFFFFF000) | 0x3;
}

void unSetTmpUserMapping()
{
	/*Get the last Directory Table entry*/
	PageEntry * table = (PageEntry*)(dirT[1022] & 0xFFFFF000);

	/*Map stack and heap*/
	table[0] = 0x2;
	table[1] = 0x2;
	table[2] = 0x2;
	table[3] = 0x2;
	table[4] = 0x2;
	table[5] = 0x2;
	table[6] = 0x2;
	table[7] = 0x2;
}

void * getTmpVirtualStack()
{
	/*Index 1 in dir table*/
	/*Index 0 in pag table*/
	/*Directory Table entry 1024, Page Table entry 0*/
	return (void*)(1022<<22);
}

void * getTmpVirtualHeap()
{
	/*Index 1 in dir table*/
	/*Index 4 in pag table*/
	/*Directory Table entry 1024, Page Table entry 4*/
	return (void*)((1022<<22) | (4<<12));
}

void * idMap4Pages(void * p)
{
	uint32 phys = (uint32)p;
	uint32 dirIndex = (phys >> 22) & 0x3FF;
	uint32 tabIndex = (phys >>12) & 0x3FF;
	PageEntry * table = (PageEntry*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
	table[tabIndex] = (phys & 0xFFFFF000) | 3;
	table[tabIndex+1] = ((phys & 0xFFFFF000) + PAGE_SIZE) | 3;
	table[tabIndex+2] = ((phys & 0xFFFFF000) + 2 * PAGE_SIZE) | 3;
	table[tabIndex+3] = ((phys & 0xFFFFF000) + 3 * PAGE_SIZE) | 3;

	return p;
}

void unMap4Pages(void * p)
{
	uint32 phys = (uint32)p;
	uint32 dirIndex = (phys >> 22) & 0x3FF;
	uint32 tabIndex = (phys >>12) & 0x3FF;
	PageEntry * table = (PageEntry*)(dirT[dirIndex] & 0xFFFFF000);

	dirT[dirIndex] = dirT[dirIndex] | 3;	//Set the directory as present
	table[tabIndex] = 2;
	table[tabIndex+1] = 2;
	table[tabIndex+2] = 2;
	table[tabIndex+3] = 2;
}
