#include <process.h>

process *procList;

static uint64 nextPID = 0;

static uint64 maxProcess;

typedef struct process {
      uint64 PID;
      
} Process;
