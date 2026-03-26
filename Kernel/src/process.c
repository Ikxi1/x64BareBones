#include <process.h>

process *procList;

static uint64 nextPID = 0;

static uint64 maxProcess;

typedef struct {
    uint64
        RDI, RSI,
        RSP, RBP,
        RAX, RBX, RCX, RDX,
        R8, R9, R10, R11, R12, R13, R14, R15;
} context;
