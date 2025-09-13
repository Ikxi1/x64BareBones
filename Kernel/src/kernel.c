#include <multiboot.h>
#include <types.h>
#include <stdint.h>
#include <i386flat.h>
#include <segmentSelector.h>
#include <shield.h>
#include <interruptHandler.h>
#include <process.h>
#include <scheduler.h>
#include <page.h>
#include <semaphore.h>
#include <syscallsImpl.h>
#include <vt.h>
#include <vtprintf.h>
#include <ipc.h>
#include <shell.h>
#include <main.h>

#define CHECK_FLAG(flags, n) (flags & (0x1 << n))
#define EIGHTH_MB_MARK 0x800000

/*End of kernel constant defined in the loader*/
extern uint8 * eokl;

static void setmem(unsigned long length, unsigned long addr, memory_map_t * kernel_land, memory_map_t * user_land);
static int init(int argc, char ** argv);

void SetupKernelProtection(memory_map_t kl);

int _main(multiboot_info_t* mbd, unsigned int magic)
{	/* Let's Rock */
	memory_map_t user_land;
	memory_map_t kernel_land;

	/***DEBUGGING***/
//	int p =1;
//	while(p);
	/***************/

	if (CHECK_FLAG(mbd->flags, 6))
		setmem(mbd->mmap_length, mbd->mmap_addr, &kernel_land, &user_land);
	else
		return 0;

	/*TODO which should be first? setupShield or startPaging?*/
	/*Setup the memory manager*/

	//startPaging(user_land.length_low, (void*)(user_land.base_addr_low+2*MEM_BLOCK));
	startPaging(user_land.length_low, (void*)(EIGHTH_MB_MARK));

	/*Setup the protection*/
	// setupShield();

	/*Setup the semaphores*/
	setupSemaphore();

	/*Setup the virtual terminals*/
	setupVTs();

	/*Print some information about kernel-land and user-land*/
	kprintf("Kernel-Land starting at 0x%x size: %dKiB(%dMiB)\n",
			kernel_land.base_addr_low, kernel_land.length_low/1024,
			kernel_land.length_low/(1024*1024));
	kprintf("User-Land starting at 0x%x size: %dKiB(%dMiB)\n",
			user_land.base_addr_low, user_land.length_low/1024,
			user_land.length_low/(1024*1024));

	/*Setup the processor exception and interruption handlers*/
	setupInterrupts();

	/*Setup the kernel system calls*/
	setupSyscalls();

	/*Setup the scheduler*/
	setupScheduler();

	/*Setup the interprocess communication*/
	setupIPC();

	/*Setup an empty process table*/
	setupProcessTable();

	/*Create some process*/
	startProcess((processCode)init, 0, 0, FALSE, 0, NULL);
	startProcess((processCode)shell, 0, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 1, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 2, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 3, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 4, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 5, 0, TRUE, 0, NULL);
	startProcess((processCode)shell, 6, 0, TRUE, 0, NULL);

	/*Start the scheduling*/
	startScheduler();
	while(1);
	return 0;
}

static int init(int argc, char ** argv)
{
	while(1);
}

static void setmem(unsigned long length, unsigned long addr,
		memory_map_t * kernel_land, memory_map_t * user_land)
{
	memory_map_t * ptr = (memory_map_t*)addr;
	unsigned long uleokl = (unsigned long)eokl;

	/* Set up kernel-land memory map, and align it's limits to page boundary
	 * by adding a page if necessary */
	kernel_land->base_addr_low = 0x0;
	kernel_land->base_addr_high = 0x0;
	kernel_land->length_low =
		PAGE_SIZE * (uleokl / PAGE_SIZE + (uleokl % PAGE_SIZE? 1 : 0));
	kernel_land->length_high = 0;

	/* Set up user-land memory map, start exactly after kernel-land */
	user_land->base_addr_low = kernel_land->length_low;
	user_land->base_addr_high = 0x0;
	user_land->length_low = 0x0;
	user_land->length_high = 0x0;

	/* Search through GRUB's memory map for the chunk above the first MB
	 * (0x100000) */
	while (((unsigned long)ptr < addr + length) &&
			!(ptr->type == 1 &&
			ptr->base_addr_high == 0 &&
			ptr->base_addr_low == 0x100000))
		/* Advance the pointer to the next in the list */
		ptr = (memory_map_t*)((uint8*)ptr + ptr->size + sizeof(ptr->size));

	if (ptr->type == 1 &&
			ptr->base_addr_high == 0 &&
			ptr->base_addr_low == 0x100000)
	{	/* Success, set up user-land limits */
		user_land->length_low = ptr->length_low;
		user_land->length_high = ptr->length_high;
	}
	else
	{	/* Fail */
		kernel_land->base_addr_high = kernel_land->base_addr_low = 0x0;
		kernel_land->length_high = kernel_land->length_low = 0x0;
		user_land->base_addr_high = user_land->base_addr_low = 0x0;
		user_land->length_high = user_land->length_low = 0x0;
	}

	/* Even if this kernel uses 32 bits, check lenght_high just in case we run
	 * in a PC with RAM > 4GB */
	if (user_land->length_high != 0)
		user_land->length_low = 0xFFFFFFFF - kernel_land->length_low;
	else
		/* Align user-land limits to page boundary by removing the last chunk if necessary */
		/* I think this is not necessary at all... whatever... */
		user_land->length_low -= user_land->length_low % PAGE_SIZE;
}
