#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 16

#define STACK_SIZE 16384

typedef struct process {
      uint64 pid;
      void *stack;
      // void *heap;
      uint8 *rsp;
      // uint64 parentPID; // if the process is started by another process, which one to return to
} Process;

extern Process *process_list;

// typedef struct circ_buf_proc{
//       uint32 buffer[MAX_PROCESSES];
//       Process head;
//       Process tail;
//       uint8 length;
// } CIRC_BUF_PROC;

typedef struct registers{
      uint64 r15,r14,r13,r12,r11,r10,r9,r8,
             rdi,rsi,rpb,rdx,rcx,rbx,rax;
} Registers;

void initProcess(void *function_ptr, Process *process_list);

uint8 *push_stack(uint8 *rsp, const void *data, uint64 size);

void kernel_process();

#endif // PROCESS_H
