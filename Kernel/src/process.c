#include <process.h>
#include <naiveMemory.h>


static uint64 nextPID = 0;


void initProcess(void *function_ptr, Process *process_list)
{
L1:
      Process new_process = {
            .pid   = nextPID,
            .stack = null,
            // .heap  = null,
            .rsp   = null
      };
L2:
      new_process.stack = malloc(STACK_SIZE);
      new_process.rsp = (uint8 *)new_process.stack + STACK_SIZE;

      // all these will be popped by popaq (GP registers)
      // and iretq (the rest)
L8:
      new_process.rsp = push_stack(new_process.rsp, &new_process.rsp, sizeof(new_process.rsp));
L3:
      // clean flags for the process
      uint64 flags = 0x1 << 9;
      new_process.rsp = push_stack(new_process.rsp, &flags, sizeof(uint64));
L4:
      // clean segment registers for the process
      uint64 kcs64 = 0;
      __asm__ volatile ("mov %%cs, %0" : "=r"(kcs64));
      new_process.rsp = push_stack(new_process.rsp, &kcs64, sizeof(uint64));
L5:
      // instruction pointer
      new_process.rsp = push_stack(new_process.rsp, &function_ptr, sizeof(function_ptr));
L6:
      // clean registers for the process
      Registers start = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      new_process.rsp = push_stack(new_process.rsp, &start, sizeof(Registers));
L7:
      process_list[nextPID++] = new_process;
}


uint8 *push_stack(uint8 *rsp, const void *data, uint64 size)
{
      rsp -= size;
      memcpy(rsp, data, size);
      return rsp;
}
