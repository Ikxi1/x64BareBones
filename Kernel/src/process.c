#include <process.h>
#include <naiveMemory.h>


static uint64 nextPID = 0;

static uint64 maxProcess;


uint64 startProcess(void *function_ptr)
{
      uint64 new_pid = nextPID + 1;
      Process new_process = {
            .stack = null,
            .heap  = null,
            .rsp   = null
      };

      new_process.stack = malloc(STACK_SIZE);
      new_process.stack *= STACK_SIZE;
      // clean registers for the process
      Registers start = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

      // clean flags for the process

      // clean segment registers for the process

      // interrupt pointer

      // all these will be popped by popaq (GP registers)
      // and iretq (the rest)


}


void *push_stack(void *rsp, void *data, uint64 size)
{
      ptr = (uint8 *)ptr - size;
      memcpy(ptr, data, size);
      return ptr;
}