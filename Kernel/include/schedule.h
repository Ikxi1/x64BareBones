#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdint.h>


uint64 currPID;

/* schedule

 Changes from one process to another.
 Saves current stack pointer. // not right now
 Returns stack pointer for next process.
*/
uint64 schedule(uint8 *rsp);

#endif // SCHEDULE_H
