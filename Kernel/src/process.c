#include <process.h>
#include <naiveMemory.h>


static uint64 nextPID = 0;

Process process_list[MAX_PROCESSES] = {0};


void initProcess(void *function_ptr)
{
      Process new_process = {
            .pid   = nextPID,
            .stack = null,
            .heap  = null,
            .rsp   = null
      };

      new_process.stack = malloc(STACK_SIZE);
      new_process.stack += STACK_SIZE;

      // all these will be popped by popaq (GP registers)
      // and iretq (the rest)

      // clean flags for the process
      uint64 flags = 0x1 << 9;
      new_process.rsp = push_stack(new_process.rsp, &flags, sizeof(uint64));

      // clean segment registers for the process
      uint64 kcs64 = 0;
      __asm__ volatile ("mov %%cs, %0" : "=r"(kcs64));
      new_process.rsp = push_stack(new_process.rsp, &kcs64, sizeof(uint64));

      // interrupt pointer
      new_process.rsp = push_stack(new_process.rsp, &function_ptr, sizeof(function_ptr));

      // clean registers for the process
      Registers start = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      new_process.rsp = push_stack(new_process.rsp, &start, sizeof(Registers));

      process_list[nextPID++] = new_process;
}


uint64 push_stack(uint64 rsp, const void *data, uint64 size)
{
      uint8 *r = (uint8 *)rsp;
      r = (uint8 *)r - size;
      memcpy(r, data, size);
      return (uint64)r;
}
