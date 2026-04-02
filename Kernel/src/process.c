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

      new_process.stack = malloc(16384);

      Registers start = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

      // how the frick do i continue??????
      // how do i get the instruction pointer from the stack
}
