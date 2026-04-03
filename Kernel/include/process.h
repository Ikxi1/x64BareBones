#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PROCESSES 16

#define STACK_SIZE 16384

typedef struct process {
      void *stack;
      void *heap;
      uint64 rsp;
      // uint64 parentPID; // if the process is started by another process, which one to return to
} Process;

typedef struct circ_buf_proc{
      uint32 buffer[MAX_PROCESSES];
      Process head;
      Process tail;
      uint8 length;
} CIRC_BUF_PROC;

typedef struct registers{
      uint64 r15,r14,r13,r12,r11,r10,r9,r8,
             rdi,rsi,rpb,rdx,rcx,rbx,rax
} Registers;

uint64 startProcess(void *function_ptr);

void *push_stack(void *rsp, void *data, uint64 size);

#endif // PROCESS_H
