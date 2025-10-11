#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

typedef uint64 (*processCode)(uint64 * argv, char ** argc);

typedef struct {
    void *stack;
    void *heap;
    uint64 rsp;
    uint64 parentPID; // if the process is started by another process, which one to return to
} process;

uint64 startProcess(processCode p, uint64 attached, uint64 argc, char **argv);

void setupProcessTable();

void endProcess();

process *getProcess(uint64 PID);

#endif
